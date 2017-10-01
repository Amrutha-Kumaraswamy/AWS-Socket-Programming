
/* The AWS Server program which performs the Mapreduce on a set of 'N' integers and performing computation on these integers based on the reduction type i.e., min, max, sum and sos sent by the client*/
/* This program has certain blocks of code taken from "Beej's Guide to Network Programming Using Internet Sockets" document for creating a socket using TCP and UDP protocols, and the connect, bind, accept, receive and send commands of TCP and UDP connections */

#include<stdio.h>
#include<stdlib.h>
#include<strings.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
#include<errno.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>
#include<strings.h>

/*predefining the ERROR and Maximum input data*/
#define ERROR -1
#define MAX_DATA 5000

/*Function prototype for the function backend, which sends data and reduction type to the backends servers and receives the result from these servers*/
int backend(long signed int data[MAX_DATA], char name, int port);

int main() {

        struct sockaddr_in server;
        struct sockaddr_in client;
        int sock,new;
        int sockaddr_len = sizeof(struct sockaddr_in);
        int data_len;
        long signed int data[MAX_DATA];
        int i,n,count;
        int output_A, output_B, output_C;
        long signed int remote_data[MAX_DATA];
        const char *red_type = NULL;
        long int output,result;
	/*Creating a STREAM Socket for TCP Connection*/
        if((sock = socket(AF_INET, SOCK_STREAM, 0)) == ERROR)           
	{
                perror("server socket: ");
                exit(-1);
        }
	/*The boot up message*/
        printf("The AWS is up and running\n");
	/*Assigning the IP address and Port Number to the AWS Server*/
        server.sin_family = AF_INET;
        server.sin_port = htons(25822);
        server.sin_addr.s_addr = inet_addr("127.0.0.1");
        bzero(&server.sin_zero, 8);
	/*Binding the Socket*/
        if((bind(sock, (struct sockaddr *)&server, sockaddr_len)) == ERROR) 
	{
                perror("bind: ");
		exit(-1);
	}
	/*Waiting for a call from a client*/
	if((listen(sock, 5)) == -1) 
	{
                perror("listen");
                exit(-1);
        }

        while(1) 
	{	/*Accepting call from a Client*/
                if((new = accept(sock, (struct sockaddr *)&client,&sockaddr_len)) == ERROR ) 
		{
                        perror("accept:");
                        exit(-1);
                }

                data_len = 1;
                while(data_len) 
		{	/*Receiving the data sent from the client*/
                        if(data_len = recv(new, data, MAX_DATA, 0)==ERROR) 
			{
                                perror("receive:");
                                exit(-1);
                        }
		/*Message confirmation of the reception of data*/
                printf("The AWS has received %ld numbers from the client using TCP over port %d\n", data[1]-2, ntohs(server.sin_port));
			/*Copying the reduction type from the received data to send it to the backend server */
                        remote_data[0]=data[0];
			/*Copying the count of numbers from the received data to send it to the backend server */
                        remote_data[1]=data[1]/3;
			count=data[1]/3;
                        n=2;
			/*Message confirmation pf sending N/3 numbers to each backend server*/
			printf("The AWS sent %d numbers to Backend-Server A\n", count);
			printf("The AWS sent %d numbers to Backend-Server B\n", count);
			printf("The AWS sent %d numbers to Backend-Server C\n", count);
                        /*Dividing the numbers in sets of N/3*/
			/*Sending first N/3 numbers to server A*/
			for(i=2; i<count+2; i++)
			{
                                remote_data[n]=data[i];
                                n++;
                        }
			/*Calling the backend function with data and port number*/
			output_A = backend(remote_data,'A', 21822);
                        n=2;     
			/*Dividing the numbers in sets of N/3*/
			/*Sending second set of N/3 numbers to server B*/               
                        for(i=count+2; i<((count*2)+2); i++) 
			{
                                remote_data[n]=data[i];
                                n++;
                        }
                        /*Calling the backend function with data and port number*/
                        output_B = backend(remote_data, 'B', 22822);
                        n=2;
			/*Dividing the numbers in sets of N/3*/
			/*Sending last set of N/3 numbers to server C*/  
                        for(i=((count*2)+2); i<((count*3)+2); i++) 
			{
                                remote_data[n]=data[i];
                                n++;
                        }
                        /*Calling the backend function with data and port number*/
			output_C = backend(remote_data,'C', 23822);
		/*Computing the final result of all the intermediate results*/
		if(data[0]==0)
		{
			red_type ="SUM";
			result=output_A+output_B+output_C;
		}
		else if(data[0]==1)
		{
			red_type ="MIN";
			if((output_A<output_B)&&(output_A<output_C))
			{
				result=output_A;
			}
			else if((output_B<output_A)&&(output_B<output_C))
			{
				result=output_B;
			}
			else result=output_C;
		}  
		else if(data[0]==2) 
		{
			red_type ="MAX";
			if((output_A>output_B)&&(output_A>output_C))
			{
				result=output_A;
			}
			else if((output_B>output_A)&&(output_B>output_C))
			{
				result=output_B;
			}
			else result=output_C;
		}    
		else if(data[0]==3)
		{
			red_type ="SOS";
			result=output_A+output_B+output_C;
		}                 
                        printf("The AWS has successfully finished the reduction %s: %ld\n", red_type, result);
                        output = htonl(result);
			/*Sending the final result to the client*/
                        send(new, &output, sizeof(output), 0);
                        printf("The AWS has successfully finished sending the reduction value to client.\n");
                }
		/*Closing the Sockets*/
                close(new);
        }
	close(sock);
        return 0;
}

/*Function performing the sending and receiving of UDP data*/
int backend(long signed int data[MAX_DATA], char name, int port) 
{

        int udp_sock,i,length;
        struct sockaddr_in remote_server, server;
        struct hostent *host;
        long int ans;
        const char *red_type = NULL;
	/*Creating Datagram Socket for UDP*/
        if((udp_sock = socket(AF_INET, SOCK_DGRAM, 0)) == ERROR) 
	{
                perror("udp socket: ");
                return -1;
        }
	/*Assiging port number and IP address*/
        remote_server.sin_family = AF_INET;
        server.sin_port=htons(24822);
        host = gethostbyname("localhost");

        if(host==0) 
	{
		perror("unknown host");
                return -1;
        }

        bcopy((char *)host->h_addr, (char *)&remote_server.sin_addr, host->h_length);

        remote_server.sin_port = htons(port);
        length=sizeof(struct sockaddr_in);
	/*Sending N/3 data, count and reduction type to backend server*/
        if(sendto(udp_sock,data,5000,0,(struct sockaddr *)&remote_server,length)==ERROR) 
	{
                perror("Send to:");
                return -1;
        }
	/*Receiving the result of the computation performed on N/3 data*/
        if(recvfrom(udp_sock,&ans,500,0,(struct sockaddr *)&server, &length)==ERROR) 
	{
                perror("Recv from:");
                return -1;
        }
        if(data[0]==0)
	 {
                red_type ="SUM";
         } 
	else if(data[0]==1) 
	{
                red_type ="MIN";
        } 
	else if(data[0]==2) 
	{
                red_type ="MAX";
        } 
	else if(data[0]==3) 
	{
                red_type ="SOS";
        }
	/*Displaying the reduction results from each backend servers*/
        printf("The AWS received reduction result of %s from Backend-Server %c using UDP over port 24822 and it is %ld\n", red_type, name,ans);

return ntohl(ans);
}

