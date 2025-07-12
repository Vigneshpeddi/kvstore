#include "simple_kvstore.hpp"

bool SimpleKvStore::Get(const GetRequest* req, GetResponse* res) {
  std::lock_guard<std::mutex> lock(mutex);
  auto it = store.find(req->key);
  if (it == store.end()) return false;
  res->value = it->second;
  return true;  
}

bool SimpleKvStore::Put(const PutRequest* req, PutResponse*) {
  std::lock_guard<std::mutex> lock(mutex);
  store[req->key] = req->value;
  return true;
}

bool SimpleKvStore::Append(const AppendRequest* req, AppendResponse*) {
  std::lock_guard<std::mutex> lock(mutex);
  store[req->key] += req->value;
  return true;
}

bool SimpleKvStore::Delete(const DeleteRequest* req, DeleteResponse* res) {
  std::lock_guard<std::mutex> lock(mutex);
  auto it = store.find(req->key);
  if (it == store.end()) return false;
  res->value = it->second;
  store.erase(it);
  return true;
}

bool SimpleKvStore::MultiGet(const MultiGetRequest* req,
                             MultiGetResponse* res) {
  std::lock_guard<std::mutex> lock(mutex);
  res->values.clear();
  for (const auto& key : req->keys) {
    auto it = store.find(key);
    if (it == store.end()) return false;
    res->values.push_back(it->second);
  }
  return true;
}

bool SimpleKvStore::MultiPut(const MultiPutRequest* req, MultiPutResponse*) {
  std::lock_guard<std::mutex> lock(mutex);
  if (req->keys.size() != req->values.size()) return false;
  for (size_t i = 0; i < req->keys.size(); i++) store[req->keys[i]] = req->values[i];
  return true;
}

std::vector<std::string> SimpleKvStore::AllKeys() {
  std::lock_guard<std::mutex> lock(mutex);
  std::vector<std::string> keys;
  keys.reserve(store.size());  
  for (const auto& pair : store) keys.push_back(pair.first);
  return keys;
}
