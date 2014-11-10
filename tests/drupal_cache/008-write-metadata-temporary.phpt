--TEST--
Cache records contain appropriate data in temporary.
--FILE--
<?php
    $GLOBALS['conf']['hotrod']['hostname'] = 'localhost';
    $GLOBALS['conf']['hotrod']['port'] = 18210;
    define('REQUEST_TIME', 1400000000);
    $cache = new HotRod\DrupalCache('cache');
    $cache->set('test', 'value');
    $result = $cache->get('test');
    if (is_object($result) && $result->temporary === false) {
        print "Looks good.\n";
    }
  
?>
--EXPECT--
Looks good.
