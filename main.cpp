#include <phpcpp.h>

#include <infinispan/hotrod/ConfigurationBuilder.h>
#include <infinispan/hotrod/RemoteCacheManager.h>
#include <infinispan/hotrod/RemoteCache.h>

#include <stdlib.h>

using namespace infinispan::hotrod;

class Hotrod : public Php::Base, public Php::ArrayAccess {
private:
    RemoteCacheManager *_cm;
    RemoteCache<std::string, std::string> *_cache;

    Php::Value _get(const Php::Value &key) {
        std::string *pvalue = _cache->get(std::string((const char *) key));
        if (pvalue == nullptr) {
            return Php::Value(pvalue);
        }
        else {
            return Php::Value(*pvalue);
        }
    }

public:
    Hotrod() {}
    virtual ~Hotrod() {}

    void __construct(Php::Parameters &params) {
        std::string hostname;
        int port;

        if (params.size() > 0) {
            hostname = std::string((const char *) params[0]);
        }
        else {
            hostname = "127.0.0.1";
        }

        if (params.size() > 1) {
            port = params[1];
        }
        else {
            port = 11222;
        }
    
        ConfigurationBuilder b;
        b.addServer().host(hostname).port(port);
        _cm = new RemoteCacheManager(b.build(), false);
        _cache = new RemoteCache<std::string, std::string>(_cm->getCache<std::string, std::string>());
        _cm->start();
    }

    virtual bool offsetExists(const Php::Value &key) override {
        return _cache->containsKey(std::string((const char *) key));
    }

    virtual Php::Value offsetGet(const Php::Value &key) override {
        return _get(key);
    }

    virtual void offsetSet(const Php::Value &key, const Php::Value &value) override {
        _cache->put(std::string((const char *) key), std::string((const char *) value));
    }

    virtual void offsetUnset(const Php::Value &key) override {
        _cache->remove(std::string((const char *) key));
    }

    Php::Value get(Php::Parameters &params) {
        return _get(params[0]);
    }

    void put(Php::Parameters &params) {
        std::string key = std::string((const char *) params[0]);
        std::string value = std::string((const char *) params[1]);
        int64_t life = 0;
        int64_t idle = 0;
        if (params.size() > 2) {
            life = params[2];
        }
        if (params.size() > 3) {
            idle = params[3];
        }
        _cache->put(key, value, (uint64_t) life, (uint64_t) idle);
    }

    void remove(Php::Parameters &params) {
        _cache->remove(std::string((const char *) params[0]));
    }

    void clear() {
        _cache->clear();
    }

    Php::Value stats() {
        return Php::Value(_cache->stats());
    }

    Php::Value size() {
        return Php::Value((int64_t) _cache->size());
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
            Php::ByVal("hostname", Php::Type::String),
            Php::ByVal("port", Php::Type::Numeric)
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
        hotrod.method("size",  &Hotrod::stats);

        extension.add(std::move(hotrod));
        
        // @todo    add your own functions, classes, namespaces to the extension
        
        // return the extension
        return extension;
    }
}
