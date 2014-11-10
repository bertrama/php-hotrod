--TEST--
Writing to a failed cache initialization.
--FILE--
<?php 
    $hotrod = new HotRod\Simple('localhost', 1);
    if ($hotrod()) {
        print "\$hotrod() should have been false.";
    }

    $hotrod['test'] = 'value';

    if (is_null($hotrod['test'])) {
        print "Looks good.";
    }
?>
--EXPECT--
Looks good.
