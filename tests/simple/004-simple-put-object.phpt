--TEST--
Trying a get/put for an object.
--FILE--
<?php

$hotrod = new HotRod\Simple('localhost', 18210);
if (!$hotrod()) {
    print "\$$hotrod() should have been true\n";
}
$hotrod->clear();

$value = new stdClass();
$value->prop1 = "Property 1";
$value->prop2 = "Silence";
$value->prop3 = array(1, 2, 3);
$value->prop4 = new stdClass();
$value->prop4->prop5 = "sappy";
$value->prop4->prop6 = "happy";
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
