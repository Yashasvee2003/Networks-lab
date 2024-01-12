#Create a simulator object
set ns [new Simulator]

#Define different colors for data flows (for NAM)
$ns color 1 Blue
$ns color 2 Red
$ns color 3 Green
$ns color 4 Yellow
#Open the NAM trace file
set nf [open out.nam w]
$ns namtrace-all $nf

set tracefile [open out.tr w]
$ns trace-all $tracefile

#Define a 'finish' procedure
proc finish {} {
        global ns nf
        $ns flush-trace
        #Close the NAM trace file
        close $nf
        #Execute NAM on the trace file
        exec nam out.nam &
        exec awk -f Throughput.awk out.tr &
        exit 0
}



#Create four nodes
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]
set n6 [$ns node]


#Create links between the nodes
$ns duplex-link $n0 $n2 2Mb 10ms DropTail
$ns duplex-link $n1 $n2 2Mb 10ms DropTail
$ns duplex-link $n2 $n3 1.7Mb 20ms DropTail
$ns duplex-link $n2 $n4 2Mb 10ms DropTail
$ns duplex-link $n2 $n5 2Mb 10ms DropTail
$ns duplex-link $n3 $n4 2Mb 10ms DropTail
$ns duplex-link $n3 $n6 2Mb 10ms DropTail

#Set Queue Size of link (n2-n3) to 10


#Give node position (for NAM)
$ns duplex-link-op $n0 $n2 orient right
$ns duplex-link-op $n1 $n2 orient right-up
$ns duplex-link-op $n2 $n3 orient right
$ns duplex-link-op $n2 $n4 orient right-down
$ns duplex-link-op $n2 $n5 orient right-up
$ns duplex-link-op $n3 $n6 orient right
$ns duplex-link-op $n3 $n4 orient left-down

#Monitor the queue for link (n2-n3). (for NAM)



set tcp [new Agent/TCP]
$tcp set class_ 3
$ns attach-agent $n0 $tcp
set sink [new Agent/TCPSink]
$ns attach-agent $n6 $sink
$ns connect $tcp $sink
$tcp set fid_ 4

#Setup a FTP over TCP connection
set ftp [new Application/FTP]
$ftp attach-agent $tcp
$ftp set type_ FTP





#Schedule events for the CBR and FTP agents
# use DV or LS
$ns rtproto DV
$ns rtmodel-at 3.0 down $n2 $n3

$ns at 1.0 "$ftp start"
$ns at 4.0 "$ftp stop"




#Detach tcp and sink agents (not really necessary)
$ns at 9.5 "$ns detach-agent $n0 $tcp ; $ns detach-agent $n3 $sink"

#Call the finish procedure after 5 seconds of simulation time
$ns at 10.0 "finish"

#Print CBR packet size and interval


#Run the simulation
$ns run

