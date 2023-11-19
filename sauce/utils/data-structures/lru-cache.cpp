#include "utils/data-structures/lru-cache.h"
#include "core.h"

#include <list>
#include <unordered_map>
#include <utility>

template <typename Key, typename Value>
void LRUcache<Key, Value>::put(const Key& key, const Value& value) {
  auto it = cache_map.find(key);
  if (it != cache_map.end()) {
    usage_list.erase(it->second.second);
  } else if (cache_map.size() == max_size) {
    cache_map.erase(usage_list.back());
    usage_list.pop_back();
  }

  usage_list.push_front(key);
  cache_map[key] = {value, usage_list.begin()};
}

template <typename Key, typename Value>
bool LRUcache<Key, Value>::get(const Key& key, Value& value) {
  auto it = cache_map.find(key);
  if (it == cache_map.end()) {
    return false;
  }

  usage_list.erase(it->second.second);
  usage_list.push_front(key);
  it->second.second = usage_list.begin();
  value = it->second.first;
  return true;
}
