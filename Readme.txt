a.      Name - AMRUTHA KUMARASWAMY

b.      This project is an implementation of a computational offloading where a client will sends a file with '.csv' format containing N 		numbers to a main server(aws) using a TCP connection and this server inturn divides this data into N/3 sets and sends each set to a 		backend server(serverA, serverB, serverC) using a UDP connection.
	The client also sends a computational value such as SUM, MINIMUM, MAXIMUM or SUM OF SQUARES to the server instructing it to perform 		these arithematic operations on the sent data. The server forwards this computational value to the backend servers which performs 		these operations and returns the result to the main server. The main server finally combines all these results into one and returns 		it back to the client.

c.      client.c - reads the 'nums.csv' file and sends the data along with the reduction type to the AWS server and prints the result 		obtained from the server using TCP connection.
   	aws.c - receives the data from client and divides this data into N/3 numbers and sends it to backend servers A,B and C using a UDP 		connection and the results from these are combined and the final result is sent to the client.
        serverA.c - receives first set of N/3 data from the aws server and performs the necessary reduction type and returns the result 	back to the AWS server.
        serverB.c - receives second set of N/3 data from the aws server and performs the necessary reduction type and returns the result 		back to the AWS server.
        serverC.c - receives last set of N/3 data from the aws server and performs the necessary reduction type and returns the result back 		to the AWS server.
        Makefile - Complies all the code files and runs the individual source code files of aws, serverA, serverB and serverC

d.      Please perform the following steps to run the code:
	1) Open a terminal in nunki.usc.edu and run the 'make all' command
	2) On seperate terminals run 'make serverA', 'make serverB', 'make serverC' and 'make aws' to boot up the servers: serverA, 		serverB, serverC and aws
	3) Then on a new terminal run './client <reduction_type>' to boot up the client and obtain the final result. The reduction type 	should be either 'sum' or 'min' or 'max' or 'sos' 

e.      The numbers read from the .csv file, the count of these numbers and the reduction type(encoded as integers) are sent as a single 		packet as an integer array to the aws from the client. Also, the aws sends N/3 numbers, the count and the reduction type as a 		single packet to each of the 3 backend servers as an integer array. The intermediate results is sent back from the backend servers 		to the aws server as signed long int format and the aws sends the final result to the client as a signed long int format.

f.      Some of the idiosyncrasies of the project are:
        -The reduction type should be in lower case and the make commands entered from the command line should be as quoted in the above 		points i.e., the code is case sensitive
        -The code accepts only a file of '.csv.' format and the name of the file should be 'nums.csv'
	-The file can have a maximum of 999 numbers and this count should be a multiple of 3. Also the file should be in the same directory 		as the client.
        - The values in the input file should be integers.
        - This code works for a single client at a time. Multiple clients trying to connect to a server at the same time will cause the 	program to fail
	- It is necessary to boot up all the backend Servers first followed by the aws server and then the client

g.      The concept of creating a socket using TCP and UDP protocols, and the connect, bind, accept, receive and send commands of TCP and 		UDP connections are referred from "Beej's Guide to Network Programming Using Internet Sockets" pdf and the blocks of such codes are 		referenced in the code files.

