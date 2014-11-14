load ./tcllispsim.so lispsim

set s [Simulator]

set logger [new_Logger "example-scenario"]
$s attachLogger $logger

# create hosts
set h1 [Host]
$h1 setIP "10.10.0.1"
$h1 setNetMask "255.255.255.0"
$s addNode $h1

set h2 [Host]
$h2 setIP "10.20.0.1"
$h2 setNetMask "255.255.255.0"
$s addNode $h2

# create routers
set r1 [Router]
$r1 setIP "10.10.0.2"
$r1 setNetMask "255.255.255.0"
$s addNode $r1

set r2 [Router]
$r2 setIP "10.20.0.2"
$r2 setNetMask "255.255.255.0"
$s addNode $r2

# create ITRs
#set itr [ITR]
set itr [XTR]
$itr setIP "10.10.0.3"
$itr setNetMask "255.255.255.0"
$itr setMapDB "10.30.0.1"
$itr setCacheTTL 1.0
$s addNode $itr

# creater ETRs
#set etr [ETR]
set etr [XTR]
$etr setIP "10.20.0.3"
$etr setNetMask "255.255.255.0"
$etr setMapDB "10.30.0.1"
$s addNode $etr

# create MapDBs
set db [MapDB]
$db setIP "10.30.0.1"
$db setNetMask "255.255.255.0"
$s addNode $db

# create links
set l1 [Link]
$l1 link $h1 $r1
$s addLink $l1

set l2 [Link]
$l2 link $r1 $itr
$s addLink $l2

set l3 [Link]
$l3 link $itr $etr
$l3 setDelay 0.2
$s addLink $l3

set l4 [Link]
$l4 link $etr $r2
$s addLink $l4

set l5 [Link]
$l5 link $r2 $h2
$s addLink $l5

set l6 [Link]
$l6 link $itr $db
$s addLink $l6

set l7 [Link]
$l7 link $etr $db
$s addLink $l7

# insert static routes in routing tables
$r1 insertRoute "10.20.0.0" "255.255.255.0" $l2
$itr insertRoute "10.20.0.0" "255.255.255.0" $l3
$etr insertRoute "10.20.0.0" "255.255.255.0" $l4
$r2 insertRoute "10.20.0.1" "255.255.255.255" $l5

$itr insertRoute "10.30.0.0" "255.255.255.0" $l6
$etr insertRoute "10.30.0.0" "255.255.255.0" $l7
$db insertRoute "10.10.0.0" "255.255.255.0" $l6
$db insertRoute "10.20.0.0" "255.255.255.0" $l7

$itr insertRoute "192.168.0.0" "255.255.255.0" $l3
$etr insertRoute "192.168.0.0" "255.255.255.0" $l3

# insert static eid to rloc mapping in mapdb
$db insertMap "10.10.0.1" "192.168.0.1"
#$db insertMap "10.20.0.1" "192.168.0.2"

# insert eid to rloc mapping in mapdb
$etr sendMap 1.0 "10.20.0.1" "192.168.0.2"

# test
set h1ip [$h1 getIP]
puts -nonewline "Node: "
puts [$h1ip toStr]
$h1 printNeighbours

set h2ip [$h2 getIP]
puts -nonewline "Node: "
puts [$h2ip toStr]
$h2 printNeighbours

# generate traffic

set pkt1 [Packet]
$pkt1 setSrcIP [$h1 getIP]
$pkt1 setDstIP [$h2 getIP]
$h1 sendPacket 0.5 $pkt1

set pkt2 [Packet]
$pkt2 setSrcIP [$h1 getIP]
$pkt2 setDstIP [$h2 getIP]
$h1 sendPacket 2.0 $pkt2

set pkt3 [Packet]
$pkt3 setSrcIP [$h1 getIP]
$pkt3 setDstIP [$h2 getIP]
$h1 sendPacket 3.0 $pkt3

set pkt4 [Packet]
$pkt4 setSrcIP [$h1 getIP]
$pkt4 setDstIP [$h2 getIP]
$h1 sendPacket 4.0 $pkt4

# start the simulation
$s start
