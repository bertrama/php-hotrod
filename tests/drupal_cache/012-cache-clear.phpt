--TEST--
Cache clear('*', true).
--FILE--
<?php
    $GLOBALS['conf']['hotrod']['hostname'] = 'localhost';
    $GLOBALS['conf']['hotrod']['port'] = 18210;
    define('REQUEST_TIME', 1400000000);

    $cache = new HotRod\DrupalCache('cache');
    $cache->set('key1', 'value1');
    $cache->set('key2', 'value2');
    $cache->set('key3', 'value3');

    $cache->set('llave1', 'verdad1');
    $cache->set('llave2', 'verdad2');
    $cache->set('llave3', 'verdad3');

    $keys = array('key1', 'key2', 'key3', 'key4', 'llave1', 'llave2', 'llave3', 'llave4');
    $cache->clear('*', true);
    $result = $cache->getMultiple($keys);
    if (serialize(array_keys($result)) == serialize(array())
        && serialize(array_values($keys)) == serialize(array('key1', 'key2', 'key3', 'key4', 'llave1', 'llave2', 'llave3', 'llave4'))) {
        print "Looks good.\n";
    }
?>
--EXPECT--
Looks good.
