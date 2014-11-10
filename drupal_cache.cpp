#include "hotrod.h"
#include "drupal_cache.h"

#define CATCH_ALL(expr) \
    try {               \
        expr;           \
    }                   \
    catch (...) {       \
    }

/**
 * HotRod::DrupalCache::__construct($bin).
 */

void HotRod::DrupalCache::__construct(Php::Parameters &params) {
    std::string prefix = "";
    if (bool(Php::GLOBALS["conf"]["hotrod"]["prefix"])) {
      prefix = Php::GLOBALS["conf"]["hotrod"]["prefix"].value().stringValue();
    }
    if (bool(Php::GLOBALS["conf"]["hotrod"]["hostname"])) {
      _hostname = Php::GLOBALS["conf"]["hotrod"]["hostname"].value().stringValue();
    }
    if (bool(Php::GLOBALS["conf"]["hotrod"]["port"])) {
      _port = Php::GLOBALS["conf"]["hotrod"]["port"];
    }

    _name = new std::string(prefix + params[0].stringValue());
    CATCH_ALL(initialize());
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
    std::string * pvalue = nullptr;
    CATCH_ALL(pvalue = doGet(params[0]));
    if (pvalue == nullptr) {
        return nullptr;
    }
    else {
        return Php::call("unserialize", *pvalue);
    }
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
    if (params.size() == 0 || params[0].isNull()) {
        // Condition 1: $cid is null.
        std::set<std::tr1::shared_ptr<std::string>> keys = getKeys();
        time_t t = time(nullptr);
        int64_t time_int = int64_t(t);
        for (auto key : keys) {
            std::string * pvalue = nullptr;
            CATCH_ALL(pvalue = doGet(*key));
            if (pvalue != nullptr) {
                Php::Value record = Php::call("unserialize", *pvalue);
                if (bool(record["temporary"]) || (record["expire"] > 0 && time_int > int64_t(record["expire"]))) {
                    CATCH_ALL(doRemove(*key));
                }
            }
        }
    }
    else if (params[0].isString()) {
        if (params.size() == 1 || !bool(params[1])) {
            // Condition 2: $cid is a string and wildcard is false.
            CATCH_ALL(doRemove(params[0].stringValue()));
        }
        else {
            // Condition 3: $cid is a string and wildcard is true.
            if (params[0] == "*" || params[0] == "") {
                CATCH_ALL(doClear());
            }
            else {
                std::string cid = params[0].stringValue();
                std::set<std::tr1::shared_ptr<std::string>> keys = getKeys();
                for (auto key : keys) {
                    if (key->compare(0, cid.size(), cid) == 0) {
                        CATCH_ALL(doRemove(*key));
                    }
                }
            }
        }
    }
    else if (params[0].isArray()) {
        // Condition 4: $cid is an array of cache ids.
        std::vector<std::string> cids = params[0].vectorValue<std::string>();
        for (auto cid : cids) {
            CATCH_ALL(doRemove(cid));
        }
    }
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
    std::map<std::string, Php::Value> ret;
    int size = params[0].size();
    for (int i = 0 ; i < size; ++i) {
        std::string *pvalue = nullptr;
        CATCH_ALL(pvalue = doGet(params[0][i]));
        if (pvalue != nullptr) {
            ret[params[0][i]] = Php::call("unserialize", *pvalue);
            params[0].unset(i);
        }
    }
    return ret;
}

/**
 * HotRod::DrupalCache::isEmpty()
 *
 * return: true if cache has no keys stored.
 */
Php::Value HotRod::DrupalCache::isEmpty(Php::Parameters &params) {
    return (getSize() == 0);
}

/**
 * HotRod::DrupalCache::set($cid, $value, $expire)
 *
 * $cid: the key.
 * $value: the value.
 * $expire:  either CACHE_PERMANENT (0), CACHE_TEMPORARY (-1), or a unix timestamp.
 */
void HotRod::DrupalCache::set(Php::Parameters &params) {
    int64_t request_time = Php::call("constant", "REQUEST_TIME");
    int64_t expire_in = DRUPAL_CACHE_PERMANENT;
    uint64_t expire = 0;
    Php::Object value;

    if (params.size() > 2) {
      expire_in = params[2];
    }
    value["cid"] = params[0];
    value["data"] = params[1];
    value["created"] = request_time;
    if (expire_in == DRUPAL_CACHE_TEMPORARY) {
      value["expire"] = request_time + DRUPAL_CACHE_TEMPORARY_OFFSET;
      value["temporary"] = true;
    }
    else if (expire_in != DRUPAL_CACHE_PERMANENT && expire_in < DRUPAL_CACHE_RELATIVE_OFFSET) {
      value["expire"] = request_time + expire_in;
      value["temporary"] = false;
    }
    else {
      value["expire"] = expire_in;
      value["temporary"] = false;
    }
    if (value["expire"] == DRUPAL_CACHE_PERMANENT) {
        expire = DRUPAL_CACHE_PERMANENT;
    }
    else {
        expire = int64_t(value["expire"] + ((value["expire"] - request_time) * 2));
    }
    CATCH_ALL(doPut(params[0], Php::call("serialize", value), expire, 0));
}
