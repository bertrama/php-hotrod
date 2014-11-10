--TEST--
Trying a get/put for a string.
--FILE--
<?php

$hotrod = new HotRod\Simple('localhost', 18210);
if (!$hotrod()) {
    print "\$$hotrod() should have been true\n";
}
$hotrod->clear();

$value = 'test-value-1';
$hotrod['put1'] = $value;
if ($hotrod['put1'] == $value) {
    print "Get 1 looks good.\n";
}
if ($hotrod->get('put1') == $value) {
    print "Get 2 looks good.\n";
}

$value = 'test-value-2';
$hotrod->put('put2', $value);
if ($hotrod['put2'] == $value) {
    print "Get 3 looks good.\n";
}
if ($hotrod->get('put2') == $value) {
    print "Get 4 looks good.\n";
}
?>
--EXPECT--
Get 1 looks good.
Get 2 looks good.
Get 3 looks good.
Get 4 looks good.
