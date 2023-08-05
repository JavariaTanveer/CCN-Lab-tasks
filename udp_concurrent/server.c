// Server side implementation of UDP client-server model 
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


char* itoa(int value, char* result, int base) { // used this function for Port conversion to string purpose only.
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

// Driver code 
int main() 
{ 
	int sockfd;
	int starting_port = PORT + 1; 
	char buffer[MAXLINE] = {'\0'}; 
	char *hello = "Bye From Server"; 
	struct sockaddr_in servaddr, new_servaddr, cliaddr; 
	
	// Creating socket file descriptor 
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
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
	


	int len = sizeof(struct sockaddr);
	int n;
	int newsockfd = 0;
	int pid = 0;

	while(1)
	{	
		n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
		buffer[n] = '\0'; 
		printf("Client : %s\n", buffer); 

		if ( (newsockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
		{ 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
		} 

			new_servaddr.sin_family = AF_INET; // IPv4 
			new_servaddr.sin_addr.s_addr = INADDR_ANY; 
			new_servaddr.sin_port = htons(starting_port);

			if ( bind(newsockfd, (const struct sockaddr *)&new_servaddr, sizeof(new_servaddr)) < 0 ) 
			{ 
				perror("bind failed"); 
				exit(EXIT_FAILURE); 
			} 

			pid = fork();

			if(pid == -1)
			{
				exit(-1);
			}
			else if(pid == 0)
			{
				close(sockfd);
				//communication
				n = recvfrom(newsockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len); 
				buffer[n] = '\0'; 
				printf("Client : %s\n", buffer); 
				sendto(newsockfd, (const char*)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
				close(newsockfd);
				exit(1);

			}
			else
			{
				close(newsockfd);
				itoa(starting_port,buffer,10);
				sendto(sockfd, buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
				starting_port += 1;
			}
	}
	return 0; 
}