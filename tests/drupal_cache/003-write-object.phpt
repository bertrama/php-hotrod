--TEST--
Writing to a successful cache initialization.
--FILE--
<?php

$GLOBALS['conf']['hotrod']['hostname'] = 'localhost';
$GLOBALS['conf']['hotrod']['port'] = 18210;
define('REQUEST_TIME', time());

$cache = new HotRod\DrupalCache('cache');

$key = 'test';
$value = new stdClass();
$value->prop1 = "Property 1";
$value->prop2 = "Silence";
$value->prop3 = array(1, 2, 3);
$value->prop4 = new stdClass();
$value->prop4->prop5 = "sappy";
$value->prop4->prop6 = "happy";

$cache->set($key, $value);
$result = $cache->get($key);
if (is_object($result) && serialize($result->data) == serialize($value)) {
    print "Looks good.\n";
}

?>
--EXPECT--
Looks good.
