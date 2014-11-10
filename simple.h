#include "hotrod.h"
#include "base.h"

#ifndef HOTROD_SIMPLE_H
#define HOTROD_SIMPLE_H

namespace HotRod {
    class Simple : public Base, public Php::Base, public Php::ArrayAccess {
    private:
        int _verbosity = HOTROD_VERBOSITY_SILENT;

        Php::Value doGetPhp(const Php::Value &key); 
        void doPutPhp(const Php::Value &key, const Php::Value &value, uint64_t life, uint64_t idle); 
        void doRemovePhp(const Php::Value &key); 
        void tell(std::string message);

    public:
        static const int SILENT    = HOTROD_VERBOSITY_SILENT;
        static const int EXCEPTION = HOTROD_VERBOSITY_EXCEPTION;
        static const int NOTICE    = HOTROD_VERBOSITY_NOTICE;
        static const int WARNING   = HOTROD_VERBOSITY_WARNING;
        static const int ERROR     = HOTROD_VERBOSITY_ERROR;
    
        // The constructor doesn't get passed arguments from the new statement in php.
        Simple();

        // Is this best to do here, or in an __destruct() function?
        virtual ~Simple();

        void __construct(Php::Parameters &params); 

        virtual bool offsetExists(const Php::Value &key) override;

        virtual Php::Value offsetGet(const Php::Value &key) override;

        virtual void offsetSet(const Php::Value &key, const Php::Value &value) override;

        virtual void offsetUnset(const Php::Value &key) override;

        Php::Value get(Php::Parameters &params);

        void put(Php::Parameters &params);

        void remove(Php::Parameters &params);

        void clear(); 

        Php::Value stats();

        Php::Value size(); 

        Php::Value __invoke(Php::Parameters &params); 
    };
}

#endif
