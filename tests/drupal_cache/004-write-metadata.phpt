--TEST--
Cache records contain appropriate metadata keys.
--FILE--
<?php
    $GLOBALS['conf']['hotrod']['hostname'] = 'localhost';
    $GLOBALS['conf']['hotrod']['port'] = 18210;
    define('REQUEST_TIME', time());
    $cache = new HotRod\DrupalCache('cache');
    $cache->set('test', 'value');
    $result = $cache->get('test');
    if (is_object($result)) {
        $metadata = array_keys((array) $result);
        sort($metadata);
        if (serialize($metadata) == serialize(array('cid', 'created', 'data', 'expire', 'temporary'))) {
            print "Looks good.\n";
        }
    }
  
?>
--EXPECT--
Looks good.
