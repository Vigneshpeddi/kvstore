#include "common/config.hpp"

#include <sstream>

#include "common/color.hpp"
#include "common/utils.hpp"

std::string ShardControllerConfig::print() {
  std::stringstream ss;
  ss << "Shardcontroller configuration: \n";
  for (auto&& [server, shards] : this->server_to_shards) {
    ss << "- " << server << ": ";
    for (auto&& s : shards) {
      ss << s;
      if (s != shards.back()) ss << ", ";
    }
    ss << '\n';
  }
  return ss.str();
}

std::optional<std::string> ShardControllerConfig::get_server(
    const std::string& key) {
  std::string key_uppercase = to_upper(key);
  // TODO (Part B, Step 2): Implement!
  // You should use key_uppercase (instead of key) in your implementation
  for (const auto& [server, shards] : this->server_to_shards)
    for (const Shard& shard : shards)
      if (shard.contains(key_uppercase)) return server;

  cerr_color(
      RED,
      "Shardcontroller config does not contain any server responsible for "
      "the key ",
      key);
  return std::nullopt;
}
