#ifndef HOTROD_DEFINITIONS_H
#define HOTROD_DEFINITIONS_H

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
#define HOTROD_INI_DEFAULT_FORCE_RETURN_VALUES 0
#define HOTROD_INI_DEFAULT_KEY_SIZE_ESTIMATE   64
#define HOTROD_INI_DEFAULT_PING_ON_STARTUP     1
#define HOTROD_INI_DEFAULT_PROTOCOL_VERSION    infinispan::hotrod::Configuration::PROTOCOL_VERSION_12
#define HOTROD_INI_DEFAULT_SOCKET_TIMEOUT      60000
#define HOTROD_INI_DEFAULT_TCP_NO_DELAY        1
#define HOTROD_INI_DEFAULT_VALUE_SIZE_ESTIMATE 512

#define DRUPAL_CACHE_PERMANENT 0
#define DRUPAL_CACHE_TEMPORARY -1
#define DRUPAL_CACHE_TEMPORARY_OFFSET 2591999
#define DRUPAL_CACHE_RELATIVE_OFFSET 2592000

#endif
