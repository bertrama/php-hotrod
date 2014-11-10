--TEST--
Failed connection.
--FILE--
<?php
  $hotrod = new HotRod\Simple('localhost', 1);
  if ($hotrod()) {
    print "HOTROD Succesful";
  }
  else {
    print "HOTROD Failed";
  }
?>
--EXPECT--
HOTROD Failed
