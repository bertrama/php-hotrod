<?php

  $hotrod = new HotRod\Simple('localhost');
  if ($hotrod()) {
    print "HOTROD Succesful\n";
  }
  else {
    print "HOTROD Failed\n";
  }

  $hotrod["12345"] = "54321";
  print_r($hotrod["12345"]);

  
  print "Testing Hotrod\n";
  $hotrod = new HotRod\Simple('localhost', 18210);
  if ($hotrod()) {
    print "HOTROD Succesful\n";
  }
  else {
    print "HOTROD Failed\n";
  }
  $hotrod->clear();
  print $hotrod->get('key'); echo "\n";
  $hotrod->put('key', 'value');
  print "Called put on HR object.\n";
  echo $hotrod->get('key'); echo "\n";
  print "Called get on HR object.\n";

  print "Testing phase 2\n";
  $hotrod["newkey"] = "newval";

  print $hotrod["newkey"]; print "\n";
  print_r($hotrod->stats());

  $hotrod["array"] = array(1, 2, 3, "string");
  print_r($hotrod["array"]);

  $hotrod["hash"] = array('key1' => 'value1', 'key2' => 'value2');
  print_r($hotrod["hash"]);
