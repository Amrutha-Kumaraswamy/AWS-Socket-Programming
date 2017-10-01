
/* The Client program which performs the operation of sending a set of 'N' integers and the reduction type requested by the user to the AWS server and then displays the final result received*/
/* This program has certain blocks of code taken from "Beej's Guide to Network Programming Using Internet Sockets" document for creating a socket using TCP protocol, and the connect, bind, accept, receive and send commands of TCP connection */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<strings.h>
#include<arpa/inet.h>
#include<string.h>
#include<strings.h>

/*predefining the ERROR and Maximum input data*/
#define ERROR -1
#define MAX_DATA 5000

int main(int argc, char **argv) {

        struct sockaddr_in remote_server;
        int sock,len;
        int count=0, j=0;
	long signed int input[MAX_DATA];
	long int result,ans;
        char c[1000];       
        char *reduction_type = NULL;
	FILE *fp;
	/*Creating a STREAM Socket for TCP Connection*/
        if((sock = socket(AF_INET, SOCK_STREAM, 0)) == ERROR) 
	{
                perror("socket");
                exit(-1);
        }
	/*Assigning the IP address and Port Number to the Client*/
        remote_server.sin_family = AF_INET;
        remote_server.sin_port = htons(25822);
        remote_server.sin_addr.s_addr = inet_addr("127.0.0.1");
        bzero(&remote_server.sin_zero, 8);
	
        if((connect(sock, (struct sockaddr *)&remote_server, sizeof(struct sockaddr_in))) == ERROR) 
	{
                perror("connect");
                exit(-1);
	}
	/*The boot up message*/
	printf("The client is up and running. \n");
	/*Retrieving the reduction type from the command prompt*/
	reduction_type = argv[1];
	/*encoding the reduction type into an integer for transmission as a single packet*/
        if(strcmp("sum", reduction_type)==0) 
	{
		reduction_type="SUM";
                input[0]=0;
        }
	 else if(strcmp("min",reduction_type)==0) 
	{
		reduction_type="MIN";
                input[0]=1;
        } 
	else if(strcmp("max",reduction_type)==0) 
	{
		reduction_type="MAX";
                input[0]=2;
        } else if(strcmp("sos",reduction_type)==0) 
	{
		reduction_type="SOS";
                input[0]=3;
        } else 
	{
                printf("Invalid reduction type\n");
                exit(-1);
        }
        count++;

        while(1) {
		/*Reading input data from the nums.csv file*/
                fp = fopen("nums.csv", "r");
                count = 2;
                if(fp != NULL) 
		{
                        while(fgets(c, 1000, fp)) 
			{
                                input[count] = atoi(c);
                                count = count+1;
                        }
                        input[1] = count;
			/*Sending the data to the AWS*/
                        send(sock, input, sizeof(input), 0);
                        printf("The client has sent the reduction type %s to AWS\n",reduction_type);
                        printf("The client has sent %d numbers to AWS\n", count-2);
			/*Receiving the final result from the AWS Server*/
                        recv(sock, &result, sizeof(result), 0);
			ans = htonl(result);
			printf("The client has received reduction %s: %ld \n", reduction_type, ans);

                        fclose(fp);
                        exit(-1);
                } 
		else 
		{
                        printf("error in opening file");
                        exit(-1);
                }
        }
	/*Closing the Socket*/
        close(sock);

return 0;
}

