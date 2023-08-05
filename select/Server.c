// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define PORT 8080
#define MAXLINE 1024 

void helperfunction(int newfd,char buffer[])
{
				 char first[20];
				 char second[20];
				 for(int i=0;i<strlen(buffer);i++)
				 {
					 if(buffer[i]!=',')
					 {
						 first[i]=buffer[i];
					 }
					 else
					 {
						 first[i]='\0';
						 i++;
						 int j=0;
						 for(;i<strlen(buffer);i++)
						 {
							 second[j]=buffer[i];
							 j++;
						 }
						 second[j]='\0';
					 }
				 }
				int x = atoi(first);
				int y = atoi(second);
				sprintf(buffer, "Numbers:%d , %d \n", x, y);
				send(newfd,buffer, strlen(buffer),0);
				sprintf(buffer, "Sum: %d + %d = %d\n", x, y, x+y);
				send(newfd,buffer, strlen(buffer),0);
				sprintf(buffer, "Sub: %d - %d = %d\n", x, y, x-y);
				send(newfd,buffer, strlen(buffer),0);
				sprintf(buffer, "Div: %d / %d = %d\n", x, y, x/y);
				send(newfd,buffer, strlen(buffer),0);
				sprintf(buffer, "Mul: %d x %d = %d\n", x, y, x*y);
				send(newfd,buffer, strlen(buffer),0);
}

// Driver code 
int main()
{ 
	int sockfd; 
	char buffer[MAXLINE]; 
	char *hello = "Please Enter any two numbers or Type 'exit' to Exit:"; 
	struct sockaddr_in servaddr, cliaddr; 
	
	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr, 0, sizeof(cliaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	servaddr.sin_port = htons(PORT); 
	
	// Bind the socket with the server address 
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	listen (sockfd,5);
	int newfd;
	while(1)
	{
		int len= sizeof(cliaddr), n; 
		printf("\nHello Server is Ready");
		newfd= accept(sockfd, (struct sockaddr*) &cliaddr, &len);
		while(1)
		{
			send(newfd,hello, strlen(hello),0);
			n = recv(newfd, (char *)buffer, MAXLINE,0); 
			buffer[n] = '\0'; 
			 if(strcmp(buffer, "exit")==0)
		  		break;
			 else
			 {
				helperfunction(newfd,buffer);
			 }
		}
		break;
	}
	close(newfd);
	return 0; 
} 

