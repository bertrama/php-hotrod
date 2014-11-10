--TEST--
Writing to a successful cache initialization.
--FILE--
<?php
    $GLOBALS['conf']['hotrod']['hostname'] = 'localhost';
    $GLOBALS['conf']['hotrod']['port'] = 18210;
    define('REQUEST_TIME', time());
    $cache = new HotRod\DrupalCache('cache');
    $key = 'test';
    $value = array(1, 2, 3, 'string');
    $cache->set($key, $value);
    $result = $cache->get($key);
    if (is_object($result) && serialize($result->data) == serialize($value)) {
      print "Get 1 looks good.\n";
    }

    $value = array('key1' => 'value1', 'key2' => 'value2');
    $cache->set($key, $value);
    $result = $cache->get($key);
    if (is_object($result) && serialize($result->data) == serialize($value)) {
      print "Get 2 looks good.\n";
    }
?>
--EXPECT--
Get 1 looks good.
Get 2 looks good.
