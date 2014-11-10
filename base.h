#include "hotrod.h"

#ifndef HOTROD_BASE_H
#define HOTROD_BASE_H

namespace HotRod {
    class Base {
    protected:
        int _port = HOTROD_DEFAULT_PORT;
        std::string *_name = nullptr;
        std::string _hostname = HOTROD_DEFAULT_HOST;

        infinispan::hotrod::RemoteCacheManager *_cm = nullptr;
        infinispan::hotrod::RemoteCache<std::string, std::string> *_cache = nullptr;

        virtual void initialize();
        virtual void initializeCache();
        virtual void initializeCacheManager();

        virtual std::string *doGet(const std::string &key);
        virtual void doPut(const std::string &key, const std::string &value, uint64_t life, uint64_t idle);
        virtual void doRemove(const std::string &key);

        virtual void doClear();
        virtual std::map<std::string, std::string> getStats();
        virtual uint64_t getSize();
        virtual std::set<std::tr1::shared_ptr<std::string>> getKeys();

    public:
        Base &setHostname(const std::string &hostname) {
            _hostname = hostname;
            return *this;
        }

        Base &setCacheName(const std::string &name) {
            if (_name != nullptr) {
              delete _name;
            }
            _name = new std::string(name);
            return *this;
        }

        Base &setPort(int port) {
            _port = port;
            return *this;
        }

        Base();
        virtual ~Base();
    };
}

#endif
