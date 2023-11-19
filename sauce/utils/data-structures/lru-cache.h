#ifndef LRU_CACHE_H_
#define LRU_CACHE_H_

#include "core.h"

#include <list>
#include <unordered_map>

template <typename Key, typename Value>
class LRUcache {
public:
  LRUcache(size_t max_size) : max_size(max_size) {}

  void put(const Key& key, const Value& value) {
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

  bool get(const Key& key, Value& value) {
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
private:
  size_t max_size;
  std::list<Key> usage_list;
  std::unordered_map<Key, std::pair<Value, typename std::list<Key>::iterator>> cache_map;
};

#endif // LRU_CACHE_H_
