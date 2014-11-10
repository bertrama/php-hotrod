--TEST--
Writing to a failed cache initialization.
--FILE--
<?php
    $GLOBALS['conf']['hotrod']['hostname'] = 'localhost';
    $GLOBALS['conf']['hotrod']['port'] = 1;
    define('REQUEST_TIME', time());
    $cache = new HotRod\DrupalCache('cache');
    $cache->set('test', 'value');
    if (is_null($cache->get('test'))) {
      print "Looks good.";
    }
?>
--EXPECT--
Looks good.
