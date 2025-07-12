#include "shardkv_client.hpp"

std::optional<std::string> ShardKvClient::Get(const std::string& key) {
  // Query shardcontroller for config
  auto config = this->Query();
  if (!config) return std::nullopt;

  // find responsible server in config
  std::optional<std::string> server = config->get_server(key);
  if (!server) return std::nullopt;

  return SimpleClient{*server}.Get(key);
}

bool ShardKvClient::Put(const std::string& key, const std::string& value) {
  // Query shardcontroller for config
  auto config = this->Query();
  if (!config) return false;

  // find responsible server in config, then make Put request
  std::optional<std::string> server = config->get_server(key);
  if (!server) return false;
  return SimpleClient{*server}.Put(key, value);
}

bool ShardKvClient::Append(const std::string& key, const std::string& value) {
  // Query shardcontroller for config
  auto config = this->Query();
  if (!config) return false;

  // find responsible server in config, then make Append request
  std::optional<std::string> server = config->get_server(key);
  if (!server) return false;
  return SimpleClient{*server}.Append(key, value);
}

std::optional<std::string> ShardKvClient::Delete(const std::string& key) {
  // Query shardcontroller for config
  auto config = this->Query();
  if (!config) return std::nullopt;

  // find responsible server in config, then make Delete request
  std::optional<std::string> server = config->get_server(key);
  if (!server) return std::nullopt;
  return SimpleClient{*server}.Delete(key);
}

std::optional<std::vector<std::string>> ShardKvClient::MultiGet(
    const std::vector<std::string>& keys) {
  auto config = this->Query();
  std::map<std::string, std::vector<std::string>> keys_by_server;
  for (const auto& key : keys) {
    std::optional<std::string> server = config->get_server(key);
    if (!server) return std::nullopt;
    keys_by_server[*server].push_back(key);
  }

  std::vector<std::string> values;
  values.reserve(keys.size());
  // TODO (Part B, Step 3): Implement!
  for (const auto& [server, server_keys] : keys_by_server) {
    auto server_values = SimpleClient{server}.MultiGet(server_keys);
    if (!server_values) return std::nullopt; 
    values.insert(values.end(), server_values->begin(), server_values->end());
  }
  
  std::map<std::string, std::string> key_to_value;
  size_t index = 0;
  for (const auto& [server, server_keys] : keys_by_server) 
    for (size_t i = 0; i < server_keys.size(); i++) key_to_value[server_keys[i]] = values[index++];
  
  std::vector<std::string> ordered_values;
  ordered_values.reserve(keys.size());
  for (const auto& key : keys) ordered_values.push_back(key_to_value[key]);
  return ordered_values;
}

bool ShardKvClient::MultiPut(const std::vector<std::string>& keys,
                             const std::vector<std::string>& values) {
  // TODO (Part B, Step 3): Implement!
  if (keys.size() != values.size()) return false;

  auto config = this->Query();
  if (!config) return false;

  std::map<std::string, std::array<std::vector<std::string>, 2>> kv_by_server;
  for (size_t i = 0; i < keys.size(); i++) {
    std::optional<std::string> server = config->get_server(keys[i]);
    if (!server) return false;
    kv_by_server[*server][0].push_back(keys[i]);
    kv_by_server[*server][1].push_back(values[i]);
  }

  for (const auto& [server, kv_pairs] : kv_by_server) {
    SimpleClient client{server};
    if (!client.MultiPut(kv_pairs[0], kv_pairs[1])) return false;
  }

  return true;
}

// Shardcontroller functions
std::optional<ShardControllerConfig> ShardKvClient::Query() {
  QueryRequest req;
  if (!this->shardcontroller_conn->send_request(req)) return std::nullopt;

  std::optional<Response> res = this->shardcontroller_conn->recv_response();
  if (!res) return std::nullopt;
  if (auto* query_res = std::get_if<QueryResponse>(&*res)) {
    return query_res->config;
  }

  return std::nullopt;
}

bool ShardKvClient::Move(const std::string& dest_server,
                         const std::vector<Shard>& shards) {
  MoveRequest req{dest_server, shards};
  if (!this->shardcontroller_conn->send_request(req)) return false;

  std::optional<Response> res = this->shardcontroller_conn->recv_response();
  if (!res) return false;
  if (auto* move_res = std::get_if<MoveResponse>(&*res)) {
    return true;
  }

  return false;
}
