--TEST--
Clear: $cache->clear() some expired records.
--FILE--
<?php
    $GLOBALS['conf']['hotrod']['hostname'] = 'localhost';
    $GLOBALS['conf']['hotrod']['port'] = 18210;
    define('REQUEST_TIME', time());

    $cache = new HotRod\DrupalCache('cache');
    $cache->clear('*', TRUE);

    $cache->set('node:permanent:1', 'node1!', 0);
    $cache->set('node:permanent:2', 'node2!', 0);
    $cache->set('node:permanent:3', 'node3!', 0);

    $cache->set('node:temporary:1', 'node4!', -1);
    $cache->set('node:temporary:2', 'node5!', -1);
    $cache->set('node:temporary:3', 'node6!', -1);

    $cache->set('node:expire:1', 'node4!', REQUEST_TIME + 1);
    $cache->set('node:expire:2', 'node5!', REQUEST_TIME + 2);
    $cache->set('node:expire:3', 'node6!', REQUEST_TIME + 60);

    $keys = array(
      'node:permanent:1',
      'node:permanent:2',
      'node:permanent:3',
      'node:temporary:1',
      'node:temporary:2',
      'node:temporary:3',
      'node:expire:1',
      'node:expire:2',
      'node:expire:3',
    );

    $keys_set = TRUE;
    foreach ($keys as $key) {
      if (is_null($cache->get($key))) {
        $keys_set = FALSE;
      }
    }

    if ($keys_set == TRUE) {
      print "Looks good.\n";
    }

    sleep(3);
    $cache->clear();

    $keys_expected = array(
      'node:permanent:1',
      'node:permanent:2',
      'node:permanent:3',
      'node:expire:3',
    );
    $keys_not_expected = array(
      'node:temporary:1',
      'node:temporary:2',
      'node:temporary:3',
      'node:expire:1',
      'node:expire:2',
    );

    $expected = TRUE;
    $not_expected = TRUE;
    foreach ($keys_expected as $key) {
      if (is_null($cache->get($key))) {
        $expected = TRUE;
      }
    }
    foreach ($keys_not_expected as $key) {
      if (!is_null($cache->get($key))) {
        $not_expected = TRUE;
      }
    }
    
    if (!$expected) {
      print "Not all expected keys were still set.\n";
    }
    if (!$not_expected) {
      print "Not all expected keys were removed.\n";
    }
    if ($expected && $not_expected) {
      print "Looks good.\n";
    }


?>
--EXPECT--
Looks good.
Looks good.
