#include "hotrod.h"
#include "base.h"

#ifndef HOTROD_DRUPAL_CACHE_H
#define HOTROD_DRUPAL_CACHE_H

namespace HotRod {

    class DrupalCache : public Base, public Php::Base {
    private:
        Php::Value doGet(Php::Value cid);
        void doClear(Php::Value cid, Php::Value wildcard);
        Php::Value doGetMultiple(Php::Value &cids);
        Php::Value doIsEmpty();
        void doSet(Php::Value cid, Php::Value data, Php::Value expire);

    public:
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
        Php::Value get(Php::Parameters &params);

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
        void clear(Php::Parameters &params);
    
        /**
         * HotRod::DrupalCache::getMultiple(&$cids)
         *
         * $cids: list of cids to retrieve.  cids successfully retrieved are removed from $cids.
         *
         * return: associative array of cids and retrieved results.
         *   Retrieved results look as decribed in HotRod::DrupalCache::get($cid);
         */
        Php::Value getMultiple(Php::Parameters &params);

        /**
         * HotRod::DrupalCache::isEmpty()
         *
         * return: true if cache has no keys stored.
         */
        Php::Value isEmpty(Php::Parameters &params);

        /**
         * HotRod::DrupalCache::set($cid, $value, $expire)
         *
         * $cid: the key.
         * $value: the value.
         * $expire:  either CACHE_PERMANENT (0), CACHE_TEMPORARY (-1), or a unix timestamp.
         */
        void set(Php::Parameters &params);
    };
}

#endif
