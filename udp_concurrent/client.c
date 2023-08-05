// Client side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define PORT	 8080
#define MAXLINE 1024 

// Driver code 
int main() 
{ 
	
	int sockfd; 
	char buffer[MAXLINE] = {'\0'}; 
	char *hello = "Hello I'm Client"; 
	struct sockaddr_in	 servaddr; 

	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	
	int n;
	int len = sizeof(struct sockaddr); 
	
	sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
	printf("Hello message sent.\n"); 
		
	n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len); 
	buffer[n] = '\0'; 
	int newport = atoi(buffer);
	printf("You connected with Port: %d\n",newport);

	servaddr.sin_port = htons(newport);

	char input[MAXLINE];
	printf("Enter a message to Server: ");
    fgets(input, MAXLINE, stdin);
	//scanf("%s",input);

	if ((strlen(input) > 0) && (input[strlen (input) - 1] == '\n')){ //used this to remove endline at the end of input.
        input[strlen (input) - 1] = '\0';
	}

	sendto(sockfd, input, strlen(input), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
	printf("your message sent.\n");

	n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len); 
	buffer[n] = '\0'; 
	printf("Server : %s\n", buffer);

	
	close(sockfd); 
	return 0; 
} 

