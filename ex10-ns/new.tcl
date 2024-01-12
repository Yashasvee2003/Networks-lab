set ns [new Simulator]

$ns color 1 Blue
$ns color 2 Red

set nf [open tcp.nam w]
$ns namtrace-all $nf
set tf [open tcp.tr w]
$ns trace-all $tf

proc finish {} {

    global ns nf tf
    $ns flush-trace
    
    close $nf
    close $tf

    exec nam tcp.nam &
    exec awk -f Throughput.awk tcp.tr &
    exit 0
}


set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]

$ns duplex-link $n0 $n2 2Mb 10ms DropTail
$ns duplex-link $n1 $n2 2Mb 10ms DropTail
$ns duplex-link $n2 $n3 1.7Mb 20ms DropTail
$ns duplex-link $n2 $n4 1.7Mb 20ms DropTail
$ns duplex-link $n2 $n5 2Mb 20ms DropTail


set tcp [new Agent/TCP]
$tcp set class_ 2
$ns attach-agent $n0 $tcp

set sink [new Agent/TCPSink]
$ns attach-agent $n3 $sink

$ns connect $tcp $sink
$tcp set fid_ 1

set ftp [new Application/FTP]
$ftp attach-agent $tcp
$ftp set type_ FTP


$ns at 0.5 "$ftp start"
$ns at 5.5 "$ftp stop"
$ns at 6.5 "finish"

$ns run