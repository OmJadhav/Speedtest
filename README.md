# Speedtest

### Install
make 

### Clean
make clean

### launching server
./server -s \<IP_ADDR\> -p \<PORT_NO\>

###launching Client
./client -s \<IP_ADDR\> -p \<PORT_NO\>

###Logic
##### Throughput Calculation 
1. Throughput is calculated by sending large data in the
chunks of  message size (here I have used 64K but it can be tuned for better throughput performance wrt machines used)
2. clock_gettime is used to take time stamps before and after the transfers
