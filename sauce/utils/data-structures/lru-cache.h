#ifndef LRU_CACHE_H_
#define LRU_CACHE_H_

#include "core.h"

#include <list>
#include <unordered_map>

template <typename Key, typename Value>
class LRUcache {
public:
  LRUcache(size_t max_size) : max_size(max_size) {}

  void put(const Key& key, const Value& value);
  bool get(const Key& key, Value& value);
private:
    size_t max_size;
    std::list<Key> usage_list;
    std::unordered_map<Key, std::pair<Value, typename std::list<Key>::iterator>> cache_map;
};

#endif // LRU_CACHE_H_
