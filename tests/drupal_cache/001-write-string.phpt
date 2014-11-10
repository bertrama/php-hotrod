--TEST--
Writing to a successful cache initialization.
--FILE--
<?php
    $GLOBALS['conf']['hotrod']['hostname'] = 'localhost';
    $GLOBALS['conf']['hotrod']['port'] = 18210;
    define('REQUEST_TIME', time());
    $cache = new HotRod\DrupalCache('cache');
    $cache->set('test', 'value');
    $result = $cache->get('test');
    if (is_object($result) && $result->data == 'value') {
      print "Looks good.\n";
    }
?>
--EXPECT--
Looks good.
