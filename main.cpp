#include <stdlib.h>

#include <phpcpp.h>

#include <infinispan/hotrod/ConfigurationBuilder.h>
#include <infinispan/hotrod/RemoteCacheManager.h>
#include <infinispan/hotrod/RemoteCache.h>

#include "hotrod.h"
#include "simple.h"


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

        Php::Class<HotRod::Simple> simple("HotRod\\Simple");

        simple.method("__construct", &HotRod::Simple::__construct, {
            Php::ByVal("hostname", Php::Type::String, false),
            Php::ByVal("port", Php::Type::Numeric, false),
            Php::ByVal("name", Php::Type::String, false),
            Php::ByVal("verbosity", Php::Type::Numeric, false)
        });

        simple.method("put", &HotRod::Simple::put, {
            Php::ByVal("key", Php::Type::String),
            Php::ByVal("value"),
            Php::ByVal("life", Php::Type::Numeric, false),
            Php::ByVal("idle", Php::Type::Numeric, false)
        });

        simple.method("get", &HotRod::Simple::get, {
            Php::ByVal("key", Php::Type::String)
        });

        simple.method("remove", &HotRod::Simple::remove, {
            Php::ByVal("key", Php::Type::String)
        });

        simple.method("clear", &HotRod::Simple::clear);
        simple.method("stats", &HotRod::Simple::stats);
        simple.method("size",  &HotRod::Simple::size);

        extension.add(Php::Ini( HOTROD_INI_CONNECTION_TIMEOUT, HOTROD_INI_DEFAULT_CONNECTION_TIMEOUT ));
        extension.add(Php::Ini( HOTROD_INI_FORCE_RETURN_VALUES, HOTROD_INI_DEFAULT_FORCE_RETURN_VALUES ));
        extension.add(Php::Ini( HOTROD_INI_KEY_SIZE_ESTIMATE, HOTROD_INI_DEFAULT_KEY_SIZE_ESTIMATE ));
        extension.add(Php::Ini( HOTROD_INI_PING_ON_STARTUP, HOTROD_INI_DEFAULT_PING_ON_STARTUP ));
        extension.add(Php::Ini( HOTROD_INI_PROTOCOL_VERSION, HOTROD_INI_DEFAULT_PROTOCOL_VERSION ));
        extension.add(Php::Ini( HOTROD_INI_SOCKET_TIMEOUT, HOTROD_INI_DEFAULT_SOCKET_TIMEOUT ));
        extension.add(Php::Ini( HOTROD_INI_TCP_NO_DELAY, HOTROD_INI_DEFAULT_TCP_NO_DELAY ));
        extension.add(Php::Ini( HOTROD_INI_VALUE_SIZE_ESTIMATE, HOTROD_INI_DEFAULT_VALUE_SIZE_ESTIMATE ));

        extension.add(std::move(simple));
        
        // return the extension
        return extension;
    }
}
