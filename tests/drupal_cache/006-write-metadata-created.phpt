--TEST--
Cache records contain appropriate data in created.
--FILE--
<?php
    $GLOBALS['conf']['hotrod']['hostname'] = 'localhost';
    $GLOBALS['conf']['hotrod']['port'] = 18210;
    define('REQUEST_TIME', 1400000000);
    $cache = new HotRod\DrupalCache('cache');
    $cache->set('test', 'value');
    $result = $cache->get('test');
    if (is_object($result) && $result->created == REQUEST_TIME) {
        print "Looks good.\n";
    }
  
?>
--EXPECT--
Looks good.
