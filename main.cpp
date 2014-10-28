#include <phpcpp.h>

#include <infinispan/hotrod/ConfigurationBuilder.h>
#include <infinispan/hotrod/RemoteCacheManager.h>
#include <infinispan/hotrod/RemoteCache.h>

#include <stdlib.h>

using namespace infinispan::hotrod;

#ifndef DEFAULT_HOTROD_HOST
#define DEFAULT_HOTROD_HOST "127.0.0.1"
#endif

#ifndef DEFAULT_HOTROD_PORT
#define DEFAULT_HOTROD_PORT 11222
#endif

class Hotrod : public Php::Base, public Php::ArrayAccess {
private:
    RemoteCacheManager *_cm = nullptr;
    RemoteCache<std::string, std::string> *_cache = nullptr;

    Php::Value _get(const Php::Value &key) {
        std::string *pvalue = nullptr;
        if (_cache != nullptr) {
            try {
                pvalue = _cache->get(std::string((const char *) key));
            }
            catch (...) {
            }
        }
        if (pvalue == nullptr) {
            return Php::Value(nullptr);
        }
        else {
            return Php::call("unserialize", Php::Value(*pvalue));
        }
    }

    void _put(const Php::Value &key, const Php::Value &value, int64_t life, int64_t idle) {
        if (_cache != nullptr) {
            try {
                std::string k = std::string((const char *) key);
                std::string v = std::string((const char *) Php::call("serialize", value));
                _cache->put(k, v, (uint64_t) life, (uint64_t) idle);
            }
            catch (...) {
            }
        }
    }

    void _remove(const Php::Value &key) {
        if (_cache != nullptr) {
            try {
                _cache->remove(std::string((const char *) key));
            }
            catch (...) {
            }
        }
    }

public:
    // The constructor doesn't get passed arguments from the new statement in php.
    Hotrod() {}

    // Is this best to do here, or in an __destruct() function?
    virtual ~Hotrod() {
        if (_cache != nullptr) {
            delete _cache;
            _cache = nullptr;
        }
        if (_cm != nullptr) {
            delete _cm;
            _cm = nullptr;
        }
    }

    void __construct(Php::Parameters &params) {
        std::string hostname;
        int port;

        if (params.size() > 0) {
            hostname = std::string((const char *) params[0]);
        }
        else {
            hostname = DEFAULT_HOTROD_HOST;
        }

        if (params.size() > 1) {
            port = params[1];
        }
        else {
            port = DEFAULT_HOTROD_PORT;
        }
    
        try {
            ConfigurationBuilder b;
            b.addServer().host(hostname).port(port);
            _cm = new RemoteCacheManager(b.build(), false);
            _cm->start();
            _cache = new RemoteCache<std::string, std::string>(_cm->getCache<std::string, std::string>());
        }
        catch (...) {
        }
    }

    virtual bool offsetExists(const Php::Value &key) override {
        if (_cache != nullptr) {
            try {
                return _cache->containsKey(std::string((const char *) key));
            }
            catch (...) {
            }
        }
        return false;
    }

    virtual Php::Value offsetGet(const Php::Value &key) override {
        return _get(key);
    }

    virtual void offsetSet(const Php::Value &key, const Php::Value &value) override {
        _put(key, value, 0, 0);
    }

    virtual void offsetUnset(const Php::Value &key) override {
        _remove(key);
    }

    Php::Value get(Php::Parameters &params) {
        return _get(params[0]);
    }

    void put(Php::Parameters &params) {
        int64_t life = 0;
        int64_t idle = 0;
        if (params.size() > 2) {
            life = params[2];
        }
        if (params.size() > 3) {
            idle = params[3];
        }
        _put(params[0], params[1], (uint64_t) life, (uint64_t) idle);
    }

    void remove(Php::Parameters &params) {
        if (_cache != nullptr) {
            try {
                _cache->remove(params[0]);
            }
            catch (...) {
            }
        }
    }

    void clear() {
        if (_cache != nullptr) {
            try {
                _cache->clear();
            }
            catch (...) {
            }
        }
    }

    Php::Value stats() {
        if (_cache != nullptr) {
            try {
                return Php::Value(_cache->stats());
            }
            catch (...) {
            }
       }
       return Php::Value(nullptr);
    }

    Php::Value size() {
        if (_cache != nullptr) {
            try {
                return Php::Value((int64_t) _cache->size());
            }
            catch (...) {
            }
        }
        return Php::Value(nullptr);
    }

    Php::Value __invoke(Php::Parameters &params) {
        if (_cache != nullptr) {
          return true;
        }
        return false;
    }
};

/**
 *  tell the compiler that the get_module is a pure C function
 */
extern "C" {
    
    /**
     *  Function that is called by PHP right after the PHP process
     *  has started, and that returns an address of an internal PHP
     *  strucure with all the details and features of your extension
     *
     *  @return void*   a pointer to an address that is understood by PHP
     */
    PHPCPP_EXPORT void *get_module() 
    {
        // static(!) Php::Extension object that should stay in memory
        // for the entire duration of the process (that's why it's static)
        static Php::Extension extension("hotrod", "1.0");

        Php::Class<Hotrod> hotrod("Hotrod");

        hotrod.method("__construct", &Hotrod::__construct, {
            Php::ByVal("hostname", Php::Type::String, false),
            Php::ByVal("port", Php::Type::Numeric, false)
        });

        hotrod.method("put", &Hotrod::put, {
            Php::ByVal("key", Php::Type::String),
            Php::ByVal("value", Php::Type::String),
            Php::ByVal("life", Php::Type::Numeric, false),
            Php::ByVal("idle", Php::Type::Numeric, false)
        });

        hotrod.method("get", &Hotrod::get, {
            Php::ByVal("key", Php::Type::String)
        });

        hotrod.method("remove", &Hotrod::remove, {
            Php::ByVal("key", Php::Type::String)
        });

        hotrod.method("clear", &Hotrod::clear);
        hotrod.method("stats", &Hotrod::stats);
        hotrod.method("size",  &Hotrod::size);

        extension.add(std::move(hotrod));
        
        // return the extension
        return extension;
    }
}
