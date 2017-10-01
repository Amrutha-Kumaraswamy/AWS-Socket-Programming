
/* The Backend Server A program which performs the reduction operation on the set of 'N/3' integers obtained from the AWS server and returns back the result*/
/* This program has certain blocks of code taken from "Beej's Guide to Network Programming Using Internet Sockets" document for creating a socket using UDP protocol, and the bind, receive from and send to commands of UDP connections */

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<strings.h>

/*predefining the ERROR*/
#define ERROR -1

int main() 
{
        
        struct sockaddr_in server_A;
        struct sockaddr_in receive;
        long signed int array[5000];
        int k, sock, len, fromlen, count;
        const char *red_type = NULL;
        long int ans;
        long int result;

	/*Creating Datagram Socket for UDP*/
        sock = socket(AF_INET, SOCK_DGRAM, 0);
        if(sock<0) 
	{
                perror("Creating Socket");
                exit(-1);
        }
        len = sizeof(server_A);
        bzero(&server_A, len);
	/*Assiging port number and IP address*/
        server_A.sin_family=AF_INET;
        server_A.sin_addr.s_addr=INADDR_ANY;
        server_A.sin_port=htons(21822);
	/*Binding the socket to a particular port*/
        if(bind(sock, (struct sockaddr *)&server_A, len)==ERROR) 
	{
                perror("bind:");
        }
	printf("The Server A is up and running using udp port 21822\n");
        fromlen = sizeof(struct sockaddr_in);

        while(1) 
	{	
		ans=0;
		/*Receiving data and reduction type from the AWS server*/
                if(recvfrom(sock,array, 5000,0,(struct sockaddr *)&receive, &fromlen)==ERROR) 
		{
                        perror("recvfrom");
                }
                count = array[1];
                printf("The Server A has received %d numbers\n", count);
		/*Performing the reduction operation on the received data*/
		if(array[0]==0)
		{	red_type="SUM";
			for(k=2;k<count+2;k++)
			{
			ans=ans+array[k];
			}
		}
		else if(array[0]==1)
		{
			red_type="MIN";	
			ans=array[2];
			for(k=2;k<count+2;k++)
			{
				if(ans>array[k])
				{
				ans=array[k];
				}
			}
		}
		else if(array[0]==2)
		{
			red_type="MAX";
			ans=array[2];
			for(k=2;k<count+2;k++)
			{
				if(ans<array[k])
				{
				ans=array[k];
				}
			}
		}
		else if(array[0]==3)
		{
			red_type="SOS";
			for(k=2;k<count+2;k++)
			{
				ans=ans+(array[k]*array[k]);
			}
		}

                result = htonl(ans);
                printf("The Server A has successfully finished the reduction %s: %ld\n", red_type, ans);
		/*Sending the computed data to the AWS server*/
                if(sendto(sock, &result, sizeof(result),0,(struct sockaddr *)&receive, fromlen)==ERROR)
		{
                        perror("sendto:");
                }
                printf("The Server A has successfully finished sending the reduction value to AWS server.\n");
        }
	return 0;
}

