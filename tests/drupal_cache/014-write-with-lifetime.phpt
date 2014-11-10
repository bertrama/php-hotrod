--TEST--
Writing to a successful cache initialization.
--FILE--
<?php
    $GLOBALS['conf']['hotrod']['hostname'] = 'localhost';
    $GLOBALS['conf']['hotrod']['port'] = 18210;
    define('REQUEST_TIME', time());
    $cache = new HotRod\DrupalCache('cache');
    $cache->set('test', 'value', REQUEST_TIME + 60);
    $result = $cache->get('test');
    if (is_object($result)
        && $result->data == 'value'
        && $result->expire - REQUEST_TIME == 60
        && $result->temporary == FALSE) {
      print "Looks good.\n";
    }
    else {
      print_r($result);
    }
?>
--EXPECT--
Looks good.
