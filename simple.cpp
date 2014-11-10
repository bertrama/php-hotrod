#include "simple.h"

Php::Value HotRod::Simple::doGetPhp(const Php::Value &key) {
    std::string *pvalue = nullptr;
    try {
        pvalue = doGet(key);
    }
    catch (...) {
        tell("HotRod::get(): failed");
    }

    if (pvalue == nullptr) {
        return nullptr;
    }
    else {
        return Php::call("unserialize", *pvalue);
    }
}

void HotRod::Simple::doPutPhp(const Php::Value &key, const Php::Value &value, uint64_t life, uint64_t idle) {
    try {
        std::string serialized = Php::call("serialize", value);
        doPut(key, serialized, life, idle);
    }
    catch (...) {
        tell("HotRod::put(): failed");
    }
}
    
void HotRod::Simple::doRemovePhp(const Php::Value &key) {
    try {
        doRemove(key);
    }
    catch (...) {
        tell("HotRod::remove(): failed");
    }
}

void HotRod::Simple::tell(std::string message) {
    switch (_verbosity) {
    case HOTROD_VERBOSITY_SILENT:
        break;

    case HOTROD_VERBOSITY_EXCEPTION:
        throw std::runtime_error(message);
        break;

    case HOTROD_VERBOSITY_NOTICE:
        Php::notice << message << std::flush;
        break;

    case HOTROD_VERBOSITY_WARNING:
        Php::warning << message << std::flush;
        break;

    case HOTROD_VERBOSITY_ERROR:
        Php::error << message << std::flush;
        break;
    }
}

// The constructor doesn't get passed arguments from the new statement in php.
HotRod::Simple::Simple() { }

// Is this best to do here, or in an __destruct() function?
HotRod::Simple::~Simple() {
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

void HotRod::Simple::__construct(Php::Parameters &params) {

    if (params.size() > 0) {
        _hostname = params[0].stringValue();
    }

    if (params.size() > 1) {
        _port = params[1];
    }
    
    if (params.size() > 2) {
        _name = new std::string(params[2].stringValue());
    }

    if (params.size() > 3) {
        _verbosity = params[3];
    }
    
    try {
        initialize();
    }
    catch (...) {
        tell("HotRod::constructor(): failed");
    }
}

bool HotRod::Simple::offsetExists(const Php::Value &key) {
    if (_cache != nullptr) {
        try {
            return _cache->containsKey(key);
        }
        catch (...) {
            tell("HotRod::offsetExists(): failed");
        }
    }
    return false;
}

Php::Value HotRod::Simple::offsetGet(const Php::Value &key) {
    return doGetPhp(key);
}

void HotRod::Simple::offsetSet(const Php::Value &key, const Php::Value &value) {
    doPutPhp(key, value, 0, 0);
}

void HotRod::Simple::offsetUnset(const Php::Value &key) {
    doRemovePhp(key);
}

Php::Value HotRod::Simple::get(Php::Parameters &params) {
    return doGetPhp(params[0]);
}

void HotRod::Simple::put(Php::Parameters &params) {
    int64_t life = 0;
    int64_t idle = 0;
    if (params.size() > 2) {
        life = params[2];
    }
    if (params.size() > 3) {
        idle = params[3];
    }
    doPutPhp(params[0], params[1], uint64_t(life), uint64_t(idle));
}

void HotRod::Simple::remove(Php::Parameters &params) {
    doRemovePhp(params[0]);
}

void HotRod::Simple::clear() {
    try {
        doClear();
    }
    catch (...) {
        tell("HotRod::clear(): failed");
    }
}

Php::Value HotRod::Simple::stats() {
    try {
        return getStats();
    }
    catch (...) {
        tell("HotRod::stats(): failed");
    }
    return nullptr;
}

Php::Value HotRod::Simple::size() {
    try {
        return int64_t(getSize());
    }
    catch (...) {
        tell("HotRod::size(): failed");
    }
    return nullptr;
}

Php::Value HotRod::Simple::__invoke(Php::Parameters &params) {
    if (_cache != nullptr) {
        return true;
    }
    return false;
}
