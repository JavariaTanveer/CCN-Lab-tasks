// Client side implementation of UDP client-server model 
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

// Driver code 
int main() { 
	int sockfd; 
	char buffer[MAXLINE]; 
	fd_set readfds;
	int maxsd=0;
	char *hello = "Hello from client"; 
	struct sockaddr_in	 servaddr; 

	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	
	connect (sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)); 
	while(1)
	{
		int sockstdin=STDIN_FILENO;
		FD_ZERO(&readfds);
		FD_SET(sockfd,&readfds);
		FD_SET(sockstdin,&readfds);
		if(maxsd<sockfd)
		{
			maxsd=sockfd;
		}
		
		if(maxsd<sockstdin)
		{
			maxsd=sockstdin;
		}
		select(maxsd+1,&readfds,NULL,NULL,NULL);
		if(FD_ISSET(sockfd,&readfds))
		{
			int n=recv(sockfd,buffer,1024,0);
			buffer[n]='\0';
			write(1,buffer,strlen(buffer));
		}
		if(FD_ISSET(sockstdin,&readfds))
		{
			scanf("%s",buffer);
			send(sockfd,buffer,strlen(buffer),0);
			if(strcmp(buffer, "exit")==0)
		  		break;
		}
	}
	close(sockfd);
	return 0; 
} 

