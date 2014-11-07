#include "base.h"

HotRod::Base::Base() { }

HotRod::Base::~Base() {
    if (_cache != nullptr) {
        delete _cache;
        _cache = nullptr;
    }
    if (_cm != nullptr) {
        delete _cm;
        _cm = nullptr;
    }
    if (_name != nullptr) {
        delete _name;
        _name = nullptr;
    }
}

void HotRod::Base::initialize() {
    try {
        initializeCacheManager();
        initializeCache();
    }
    catch (...) {
    }
}

void HotRod::Base::initializeCacheManager() {
    infinispan::hotrod::ConfigurationBuilder builder;
    builder.connectionTimeout(Php::ini_get(HOTROD_INI_CONNECTION_TIMEOUT));
    builder.forceReturnValues(Php::ini_get(HOTROD_INI_FORCE_RETURN_VALUES));
    builder.keySizeEstimate(Php::ini_get(HOTROD_INI_KEY_SIZE_ESTIMATE));
    builder.pingOnStartup(Php::ini_get(HOTROD_INI_PING_ON_STARTUP));
    builder.protocolVersion(Php::ini_get(HOTROD_INI_PROTOCOL_VERSION));
    builder.socketTimeout(Php::ini_get(HOTROD_INI_SOCKET_TIMEOUT));
    builder.tcpNoDelay(Php::ini_get(HOTROD_INI_TCP_NO_DELAY));
    builder.valueSizeEstimate(Php::ini_get(HOTROD_INI_VALUE_SIZE_ESTIMATE));
    builder.addServer().host(_hostname).port(_port);

    _cm = new infinispan::hotrod::RemoteCacheManager(builder.build(), false);
    _cm->start();
}

void HotRod::Base::initializeCache() {
    if (_name == nullptr) {
        _cache = new infinispan::hotrod::RemoteCache<std::string, std::string>(_cm->getCache<std::string, std::string>());
    }
    else {
        _cache = new infinispan::hotrod::RemoteCache<std::string, std::string>(_cm->getCache<std::string, std::string>(_name));
    }
}

std::string *HotRod::Base::doGet(const std::string &key) {
    std::string *pvalue = nullptr;
    if (_cache != nullptr) {
        pvalue = _cache->get(key);
    }
    return pvalue;
}

void HotRod::Base::doPut(const std::string &key, const std::string &value, uint64_t life, uint64_t idle) {
    if (_cache != nullptr) {
        _cache->put(key, value, life, idle);
    }
}

void HotRod::Base::doRemove(const std::string &key) {
    if (_cache != nullptr) {
      _cache->remove(key);
    }
}


void HotRod::Base::doClear() {
    if (_cache != nullptr) {
        _cache->clear();
    }
}

std::map<std::string, std::string> HotRod::Base::getStats() {
   if (_cache != nullptr) {
        return _cache->stats();
   }
   return std::map<std::string, std::string>();
}

uint64_t HotRod::Base::getSize() {
    if (_cache != nullptr) {
       return _cache->size();
    }
    return 0;
}

