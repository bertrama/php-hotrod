--TEST--
Trying a get/put for an array.
--FILE--
<?php

$hotrod = new HotRod\Simple('localhost', 18210);
if (!$hotrod()) {
    print "\$$hotrod() should have been true\n";
}
$hotrod->clear();

$value = array(1, 2, 3, 'some-string');
$hotrod['put1'] = $value;
$test = $hotrod['put1'];

if (serialize($test) == serialize($value)) {
    print "Get 1 looks good.\n";
}

$test = $hotrod->get('put1');
if (serialize($test) == serialize($value)) {
    print "Get 2 looks good.\n";
}

?>
--EXPECT--
Get 1 looks good.
Get 2 looks good.
