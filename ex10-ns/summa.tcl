# Create a simulator object
set ns [new Simulator]
 

# Define different colors
# for data flows (for NAM)
$ns color 1 Blue



set nf [open out.nam w]
$ns namtrace-all $nf

proc finish {} {
    global ns nf

    # Flush the trace file
    $ns flush-trace

    # Close the NAM trace file
    close $nf

    # Run NAM on the trace file
    exec nam out.nam &

    # Exit the simulation
    exit 0

   
}



# Create four nodes
set n0 [$ns node]
set n2 [$ns node]

# Create links between the nodes
$ns duplex-link $n0 $n2 2Mb 10ms DropTail

# Give node position (for NAM)
$ns duplex-link-op $n0 $n2 orient right-down


#always give class_ 2 for TCP
# Setup a TCP connection
set tcp [new Agent/TCP]
$tcp set class_ 2
$ns attach-agent $n0 $tcp
 
set sink [new Agent/TCPSink]
$ns attach-agent $n2 $sink
$ns connect $tcp $sink
$tcp set fid_ 1
 
# Setup a FTP over TCP connection
set ftp [new Application/FTP]
$ftp attach-agent $tcp
$ftp set type_ FTP

# scheduling of events
$ns at 1.0 "$ftp start"
$ns at 4.0 "$ftp stop"

# Run the simulation
# $ns run 5.0

# Set the end time of the simulation
$ns at 6.0 "finish"
$ns run
