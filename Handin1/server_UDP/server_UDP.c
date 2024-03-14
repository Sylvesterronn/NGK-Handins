/* Creates a datagram server.  The port
number is passed as an argument.  This
server runs forever
Based on example: https://brightspace.au.dk/d2l/le/lessons/126839/units/1669475
Modified: Sylvester Rønn Andersen & Nikolaj Dahl Møgelvang
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PORTNR 9000

void error(const char *msg)
{
	perror(msg);
	exit(0);
}

void handle_command(char command,char *response)
{
	FILE *file;
	char buf[256];

	if(command=='U' || command=='u')
	{
		file=fopen("/proc/uptime","r");
		if(file==NULL)
		{
			strcpy(response,"Error reading /proc/uptime");
			return;
		}
		fgets(buf,sizeof(buf),file);
		fclose(file);
		strcpy(response, buf);
		
	}
	else if(command=='L' || command=='l')
	{
		file=fopen("/proc/loadavg","r");
		if(file==NULL)
		{
			strcpy(response,"Error reading /proc/uptime");
			return;
		}
		fgets(buf,sizeof(buf),file);
		fclose(file);
		strcpy(response, buf);

		
	}
	else
	{
		printf("ERROR\n");
		strcpy(response,"Error in typing");
		return;
	}

	//Something to send 
	printf("Response %s\n",response);
	
	
	memset(response, 0, sizeof(response));

	// Copy the contents of buf into response
	strcpy(response, buf);

}

int main(int argc, char *argv[])
{
	printf("Starting UDP server...\n");
	int sock, n,s;
	socklen_t fromlen;
	struct sockaddr_in server;
	struct sockaddr_in from;
	char buf[256];
	char response[256];  // Declare response variable
	
	sock=socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) error("ERROR, socket");

	bzero(&server,sizeof(server));
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=PORTNR;

	printf("Binding...\n");
	if (bind(sock,(struct sockaddr *)&server,sizeof(server))<0)
	    error("ERROR, binding");

	fromlen = sizeof(from);
	while (1) {
		printf("Receive...\n");
		n = recvfrom(sock,buf,sizeof(buf),0,(struct sockaddr*)&from,&fromlen);
		if (n < 0) error("ERROR, recvfrom");
        buf[n]=0;  //handle null termination 

		printf("Received a datagram: %s ", buf);

        // Process the received data and generate a response
        handle_command(buf[0], response);

        // Print the response to the console
        printf("Response: %s\n", response);

        // Copy the response into buf
        strncpy(buf, response, sizeof(buf));

		// Print the contents of buf
        printf("Buffer contents after copying response: %s\n", buf);

		n = sendto(sock,buf, strlen(buf),0,(struct sockaddr*)&from,fromlen);
		if (n  < 0) error("ERROR, sendto");
		
	}

	return 0;
}
