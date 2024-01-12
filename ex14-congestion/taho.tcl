# Create a simulator object
set ns [new Simulator]
 
# Define different colors
# for data flows (for NAM)
$ns color 1 Blue
$ns color 2 Red

#Open the NAM trace file
set nf [open taho.nam w]
$ns namtrace-all $nf

set tf [open taho.tr w]
$ns trace-all $tf

#Define a 'finish' procedure
proc finish {} {
        global ns nf tf
        $ns flush-trace
        #Close the NAM trace file
        close $nf
        close $tf
        #Execute NAM on the trace file
        exec nam taho.nam &
	#exec awk -f Throughput.awk taho.tr &
  
        exit 0
}


# Create four nodes
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]


# Create links between the nodes
$ns duplex-link $n0 $n1 10Mb 10ms DropTail
$ns duplex-link $n1 $n2 2Mb 10ms DropTail

# Set Queue Size of link (n2-n3) to 10
$ns queue-limit $n0 $n1 10
 
# Give node position (for NAM)
#$ns duplex-link-op $n0 $n2 orient right
$ns duplex-link-op $n0 $n1 orient right-down
$ns duplex-link-op $n2 $n1 orient left-down
 
# Monitor the queue for link (n2-n3). (for NAM)
$ns duplex-link-op $n0 $n1 queuePos 0.5
 
 
# Setup a TCP connection
set tcp [new Agent/TCP]
$tcp set window_ 10 ;# Set the window size (e.g., 10 packets)
$tcp set packetSize_ 1000 ;# Set the packet size (e.g., 1000 bytes)
$tcp set timeout_ 1.0 ;# Set the timeout (e.g., 1.0 seconds)
$ns attach-agent $n0 $tcp
 
set sink [new Agent/TCPSink]
$ns attach-agent $n2 $sink
$ns connect $tcp $sink
$tcp set fid_ 1
 
# Setup a FTP over TCP connection
set ftp [new Application/FTP]
$ftp attach-agent $tcp
$ftp set type_ FTP

 
 
# Schedule events for the CBR and FTP agents
#$ns at 0.1 "$cbr start"
$ns at 0.1 "$ftp start"
$ns at 4.0 "$ftp stop"
#$ns at 4.5 "$cbr stop"
 
# Detach tcp and sink agents
# (not really necessary)
#$ns at 4.5 "$ns detach-agent $n0 $tcp ; $ns detach-agent $n2 $sink"
 
# Call the finish procedure after
# 5 seconds of simulation time
$ns at 5.0 "finish"
 
# procedure to plot the congestion window
proc plotWindow {tcpSource outfile} {
   global ns
   set now [$ns now]
   set cwnd [$tcpSource set cwnd_]
   set color b

# the data is recorded in a file called congestion.xg (this can be plotted # using xgraph or gnuplot. this example uses xgraph to plot the cwnd_
   puts  $outfile  "$now $cwnd"
   $ns at [expr $now+0.1] "plotWindow $tcpSource  $outfile"
}

set outfile [open  "taho.xg"  w]
$ns  at  0.0  "plotWindow $tcp  $outfile"


#Running the simulation
$ns run
