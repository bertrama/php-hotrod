#include "hotrod.h"
#include "drupal_cache.h"

Php::Value HotRod::DrupalCache::doGet(Php::Value cid) {
    return nullptr;
}
void HotRod::DrupalCache::doClear(Php::Value cid, Php::Value wildcard) {
}
Php::Value HotRod::DrupalCache::doGetMultiple(Php::Value &cids) {
    return nullptr;
}
Php::Value HotRod::DrupalCache::doIsEmpty() {
    return nullptr;
}
void HotRod::DrupalCache::doSet(Php::Value cid, Php::Value data, Php::Value expire) {
}


/**
 * HotRod::DrupalCache::get($cid).
 *
 * $cid: cache id (presumed to be a string);
 * 
 * return: a PHP stdClass with the following properties:
 *   cid:     the cid for the item
 *   data:    the data
 *   expires: the timestamp when the record expires.
 *   created: the timestamp when the record was created.
 *   serialized: whether the data was serialized.
 */
Php::Value HotRod::DrupalCache::get(Php::Parameters &params) {
  return doGet(params[0]);
}

/**
 * HotRod::DrupalCache::clear($cid = NULL, $wildcard = FALSE).
 *
 * Clear is messy.
 * Condition 1: $cid is null:
 *   evict any expired cache entries.
 * Condition 2: $cid is a string, $wildcard is FALSE:
 *   remove $cid from the cache
 * Condition 3: $cid is a string, $wildcard is TRUE:
 *   remove all cids from cache matching $cid.
 * Condition 4: $cid is an array of strings:
 *   remove all cids included in $cid from the cache.
 */
void HotRod::DrupalCache::clear(Php::Parameters &params) {
}
    
/**
 * HotRod::DrupalCache::getMultiple(&$cids)
 *
 * $cids: list of cids to retrieve.  cids successfully retrieved are removed from $cids.
 *
 * return: associative array of cids and retrieved results.
 *   Retrieved results look as decribed in HotRod::DrupalCache::get($cid);
 */
Php::Value HotRod::DrupalCache::getMultiple(Php::Parameters &params) {
    return doGetMultiple(params[0]);
}

/**
 * HotRod::DrupalCache::isEmpty()
 *
 * return: true if cache has no keys stored.
 */
Php::Value HotRod::DrupalCache::isEmpty(Php::Parameters &params) {
    return doIsEmpty();
}

/**
 * HotRod::DrupalCache::set($cid, $value, $expire)
 *
 * $cid: the key.
 * $value: the value.
 * $expire:  either CACHE_PERMANENT (0), CACHE_TEMPORARY (-1), or a unix timestamp.
 */
void HotRod::DrupalCache::set(Php::Parameters &params) {
}
