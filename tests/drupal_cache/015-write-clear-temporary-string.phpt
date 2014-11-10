--TEST--
Writing to an initialized cache.  Set temporary + clear().
--FILE--
<?php
    $GLOBALS['conf']['hotrod']['hostname'] = 'localhost';
    $GLOBALS['conf']['hotrod']['port'] = 18210;
    define('REQUEST_TIME', time());

    $cache = new HotRod\DrupalCache('cache');

    $cache->set('test', 'value', -1);
    $result = $cache->get('test');
    if (is_object($result) && $result->data == 'value') {
      print "Looks good.\n";
    }
    else {
      print_r($result);
    }
    $cache->clear();
    $result = $cache->get('test');
    if (is_null($result)) {
        print "Looks good.\n";
    }
    else {
      print_r($result);
    }
?>
--EXPECT--
Looks good.
Looks good.
