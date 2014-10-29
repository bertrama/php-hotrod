#include <phpcpp.h>

#include <infinispan/hotrod/ConfigurationBuilder.h>
#include <infinispan/hotrod/RemoteCacheManager.h>
#include <infinispan/hotrod/RemoteCache.h>

#include <stdlib.h>

using namespace infinispan::hotrod;

#define HOTROD_DEFAULT_HOST                    "127.0.0.1"
#define HOTROD_DEFAULT_PORT                    11222

#define HOTROD_VERBOSITY_SILENT                0
#define HOTROD_VERBOSITY_EXCEPTION             1
#define HOTROD_VERBOSITY_NOTICE                2
#define HOTROD_VERBOSITY_WARNING               3
#define HOTROD_VERBOSITY_ERROR                 4

#define HOTROD_INI_CONNECTION_TIMEOUT          "hotrod.connection_timeout"
#define HOTROD_INI_FORCE_RETURN_VALUES         "hotrod.force_return_values"
#define HOTROD_INI_KEY_SIZE_ESTIMATE           "hotrod.key_size_estimate"
#define HOTROD_INI_PING_ON_STARTUP             "hotrod.ping_on_startup"
#define HOTROD_INI_PROTOCOL_VERSION            "hotrod.protocol_version"
#define HOTROD_INI_SOCKET_TIMEOUT              "hotrod.socket_timeout"
#define HOTROD_INI_TCP_NO_DELAY                "hotrod.tcp_no_delay"
#define HOTROD_INI_VALUE_SIZE_ESTIMATE         "hotrod.value_size_estimate"

#define HOTROD_INI_DEFAULT_CONNECTION_TIMEOUT  60000
#define HOTROD_INI_DEFAULT_FORCE_RETURN_VALUES false
#define HOTROD_INI_DEFAULT_KEY_SIZE_ESTIMATE   64
#define HOTROD_INI_DEFAULT_PING_ON_STARTUP     true
#define HOTROD_INI_DEFAULT_PROTOCOL_VERSION    Configuration::PROTOCOL_VERSION_12
#define HOTROD_INI_DEFAULT_SOCKET_TIMEOUT      60000
#define HOTROD_INI_DEFAULT_TCP_NO_DELAY        true
#define HOTROD_INI_DEFAULT_VALUE_SIZE_ESTIMATE 512

class Hotrod : public Php::Base, public Php::ArrayAccess {
private:
    RemoteCacheManager *_cm = nullptr;
    RemoteCache<std::string, std::string> *_cache = nullptr;
    int _verbosity = HOTROD_VERBOSITY_SILENT;

    Php::Value _get(const Php::Value &key) {
        std::string *pvalue = nullptr;
        if (_cache != nullptr) {
            try {
                pvalue = _cache->get(std::string((const char *) key));
            }
            catch (...) {
                tell("Hotrod::get(): failed");
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
                tell("Hotrod::put(): failed");
            }
        }
    }

    void _remove(const Php::Value &key) {
        if (_cache != nullptr) {
            try {
                _cache->remove(std::string((const char *) key));
            }
            catch (...) {
                tell("Hotrod::remove(): failed");
            }
        }
    }

    void tell(std::string message) {
        switch (_verbosity) {
        case HOTROD_VERBOSITY_SILENT:
            break;

        case HOTROD_VERBOSITY_EXCEPTION:
            throw Exception(message);
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

    ConfigurationBuilder initializeBuilder() {
        ConfigurationBuilder builder;
        builder.connectionTimeout(Php::ini_get(HOTROD_INI_CONNECTION_TIMEOUT));
        builder.forceReturnValues(Php::ini_get(HOTROD_INI_FORCE_RETURN_VALUES));
        builder.keySizeEstimate(Php::ini_get(HOTROD_INI_KEY_SIZE_ESTIMATE));
        builder.pingOnStartup(Php::ini_get(HOTROD_INI_PING_ON_STARTUP));
        builder.protocolVersion(Php::ini_get(HOTROD_INI_PROTOCOL_VERSION));
        builder.socketTimeout(Php::ini_get(HOTROD_INI_SOCKET_TIMEOUT));
        builder.tcpNoDelay(Php::ini_get(HOTROD_INI_TCP_NO_DELAY));
        builder.valueSizeEstimate(Php::ini_get(HOTROD_INI_VALUE_SIZE_ESTIMATE));
        return builder;
    }

public:
    static const int SILENT    = HOTROD_VERBOSITY_SILENT;
    static const int EXCEPTION = HOTROD_VERBOSITY_EXCEPTION;
    static const int NOTICE    = HOTROD_VERBOSITY_NOTICE;
    static const int WARNING   = HOTROD_VERBOSITY_WARNING;
    static const int ERROR     = HOTROD_VERBOSITY_ERROR;

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
            hostname = HOTROD_DEFAULT_HOST;
        }

        if (params.size() > 1) {
            port = params[1];
        }
        else {
            port = HOTROD_DEFAULT_PORT;
        }
        if (params.size() > 2) {
            _verbosity = params[2];
        }
    
        try {
            ConfigurationBuilder b;
            b.addServer().host(hostname).port(port);
            _cm = new RemoteCacheManager(b.build(), false);
            _cm->start();
            _cache = new RemoteCache<std::string, std::string>(_cm->getCache<std::string, std::string>());
        }
        catch (...) {
            tell("Hotrod::constructor(): failed");
        }
    }

    virtual bool offsetExists(const Php::Value &key) override {
        if (_cache != nullptr) {
            try {
                return _cache->containsKey(std::string((const char *) key));
            }
            catch (...) {
                tell("Hotrod::offsetExists(): failed");
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
        _remove(params[0]);
    }

    void clear() {
        if (_cache != nullptr) {
            try {
                _cache->clear();
            }
            catch (...) {
                tell("Hotrod::clear(): failed");
            }
        }
    }

    Php::Value stats() {
        if (_cache != nullptr) {
            try {
                return Php::Value(_cache->stats());
            }
            catch (...) {
                tell("Hotrod::stats(): failed");
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
                tell("Hotrod::size(): failed");
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
            Php::ByVal("port", Php::Type::Numeric, false),
            Php::ByVal("verbosity", Php::Type::Numeric, false)
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

        extension.add(Php::Ini( HOTROD_INI_CONNECTION_TIMEOUT, HOTROD_INI_DEFAULT_CONNECTION_TIMEOUT ));
        extension.add(Php::Ini( HOTROD_INI_FORCE_RETURN_VALUES, HOTROD_INI_DEFAULT_FORCE_RETURN_VALUES ));
        extension.add(Php::Ini( HOTROD_INI_KEY_SIZE_ESTIMATE, HOTROD_INI_DEFAULT_KEY_SIZE_ESTIMATE ));
        extension.add(Php::Ini( HOTROD_INI_PING_ON_STARTUP, HOTROD_INI_DEFAULT_PING_ON_STARTUP ));
        extension.add(Php::Ini( HOTROD_INI_PROTOCOL_VERSION, HOTROD_INI_DEFAULT_PROTOCOL_VERSION ));
        extension.add(Php::Ini( HOTROD_INI_SOCKET_TIMEOUT, HOTROD_INI_DEFAULT_SOCKET_TIMEOUT ));
        extension.add(Php::Ini( HOTROD_INI_TCP_NO_DELAY, HOTROD_INI_DEFAULT_TCP_NO_DELAY ));
        extension.add(Php::Ini( HOTROD_INI_VALUE_SIZE_ESTIMATE, HOTROD_INI_DEFAULT_VALUE_SIZE_ESTIMATE ));

        extension.add(std::move(hotrod));
        
        // return the extension
        return extension;
    }
}
