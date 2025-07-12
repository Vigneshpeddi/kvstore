#include "concurrent_kvstore.hpp"

#include <mutex>
#include <optional>

bool ConcurrentKvStore::Get(const GetRequest* req, GetResponse* res) {
  size_t bIdx = store.bucket(req->key);
  std::shared_lock<std::shared_mutex> lock(store.bucket_mutexes[bIdx]);
  auto item = store.getIfExists(bIdx, req->key);
  if (!item.has_value()) return false;
  res->value = item.value().value;
  return true;
}

bool ConcurrentKvStore::Put(const PutRequest* req, PutResponse*) {
  size_t bIdx = store.bucket(req->key);
  std::unique_lock<std::shared_mutex> lock(store.bucket_mutexes[bIdx]);
  store.insertItem(bIdx, req->key, req->value);
  return true;
}

bool ConcurrentKvStore::Append(const AppendRequest* req, AppendResponse*) {
  size_t bIdx = store.bucket(req->key);
  std::unique_lock<std::shared_mutex> lock(store.bucket_mutexes[bIdx]);
  auto item = store.getIfExists(bIdx, req->key);
  std::string new_value;
  if (item.has_value()) new_value = item.value().value + req->value; 
  else new_value = req->value;
  store.insertItem(bIdx, req->key, new_value);
  return true;
}

bool ConcurrentKvStore::Delete(const DeleteRequest* req, DeleteResponse* res) {
  size_t bIdx = store.bucket(req->key);
  std::unique_lock<std::shared_mutex> lock(store.bucket_mutexes[bIdx]);
  auto item = store.getIfExists(bIdx, req->key);
  if (!item.has_value()) return false;
  res->value = item.value().value;
  store.removeItem(bIdx, req->key);
  return true;
}

bool ConcurrentKvStore::MultiGet(const MultiGetRequest* req,
                                 MultiGetResponse* res) {
  std::unordered_map<std::string, std::string> results;
  struct BucketInfo {
    size_t bucket_idx;
    std::string key;
  };
  std::vector<BucketInfo> bucket_infos;
  for (const auto& key : req->keys) {
    size_t bIdx = store.bucket(key);
    bucket_infos.push_back({bIdx, key});
  }
  std::sort(bucket_infos.begin(), bucket_infos.end(), 
  [](const BucketInfo& a, const BucketInfo& b) 
    {return a.bucket_idx < b.bucket_idx;});
  std::vector<std::shared_lock<std::shared_mutex>> locks;
  for (size_t i = 0; i < bucket_infos.size(); i++) {
    if (i > 0 && bucket_infos[i].bucket_idx == bucket_infos[i-1].bucket_idx) continue;
    locks.emplace_back(store.bucket_mutexes[bucket_infos[i].bucket_idx]);
  }
  bool all_found = true;
  for (const auto& info : bucket_infos) {
    auto item = store.getIfExists(info.bucket_idx, info.key);
    if (item.has_value()) results[info.key] = item.value().value;
    else all_found = false;
  }
  res->values.clear();
  for (const auto& key : req->keys) {
    auto it = results.find(key);
    if (it != results.end()) res->values.push_back(it->second);
    else res->values.push_back("");
  }
  return all_found;
}

bool ConcurrentKvStore::MultiPut(const MultiPutRequest* req,
                                 MultiPutResponse*) {
  if (req->keys.size() != req->values.size()) return false;
  struct BucketInfo {
    size_t bucket_idx;
    std::string key;
    std::string value;
  };
  std::vector<BucketInfo> bucket_infos;
  for (size_t i = 0; i < req->keys.size(); i++) {
    size_t bIdx = store.bucket(req->keys[i]);
    bucket_infos.push_back({bIdx, req->keys[i], req->values[i]});
  }
  std::sort(bucket_infos.begin(), bucket_infos.end(), 
  [](const BucketInfo& a, const BucketInfo& b) 
    {return a.bucket_idx < b.bucket_idx;});
  std::vector<std::unique_lock<std::shared_mutex>> locks;
  for (size_t i = 0; i < bucket_infos.size(); i++) {
    if (i > 0 && bucket_infos[i].bucket_idx == bucket_infos[i-1].bucket_idx) continue;
    locks.emplace_back(store.bucket_mutexes[bucket_infos[i].bucket_idx]);
  }
  for (const auto& info : bucket_infos) store.insertItem(info.bucket_idx, info.key, info.value);
  return true;
}

std::vector<std::string> ConcurrentKvStore::AllKeys() {
  std::vector<std::string> keys;
  std::vector<std::shared_lock<std::shared_mutex>> locks;
  for (size_t b = 0; b < DbMap::BUCKET_COUNT; b++) locks.emplace_back(store.bucket_mutexes[b]);
  for (size_t b = 0; b < DbMap::BUCKET_COUNT; b++)
    for (const auto& item : store.buckets[b]) keys.push_back(item.key);
  return keys;
}
