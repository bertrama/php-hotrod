php-hotrod
==========

Hotrod extension for php.

Depends on: [PHP-CPP](http://www.php-cpp.com/) and [the Hot Rod C++ client](https://github.com/infinispan/cpp-client).

Building
========

You'll need to have a copy of [PHP-CPP](http://www.php-cpp.com/) installed.  It has to be compiled against the php libraries for the same php api you intend to deploy with.

You'll also need to have a copy of the [Hot Rod C++ client](https://github.com/infinispan/cpp-client).  I compiled against 6.0.2.Final.

The included Makefile may work for you.  I compiled it with gcc.

Installing
==========

`make install` might work for you.  If not copy the hotrod binary to your extensions directory, and the hotrod ini file to the appropriate php conf.d directory.

Running
=======

`php -i | grep hotrod` should find something if the module is installed.  Once you've confirmed it's there, you can use the Hot Rod client in your php code.  The constructor for the HotRod class looks like:
`__construct(hostname = "127.0.0.1", port = 11222, cache_name = null, verbosity = HotRod::SILENT)`

The HotRod class implements the ArrayAccess interface, so you can read and write from it like you might a php associataive array.  It also supports a get(), put(), and remove() methods.  The put() method supports additional parameters for specifying a lifetime, and an idle time.  If a cache value is not set, or the cache is unavailable, HotRod should return NULL.

```
<?php

  $cache = new HotRod();
  
  $cache["some-key"] = "some-value";
  $cache["some-array"] = ["this", "array", "gets", "serialized"];
  
  $value = $cache["some-key"];
  $cache->put("some-key", "some-other-value", 1414800000, 3600);
  unset($cache["some-key"]);
  
```

Roadmap
=======

* Implement the multiGet() method.
* Implement the getWithMetaData() method.
* Provide a class implementing the Drupal 7 DrupalCacheInterface over HotRod.
* Provide a class implementing the Drupal 8 \Drupal\Core\Cache\CacheBackendInterface.

