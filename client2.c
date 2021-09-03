#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <malloc.h>
#include <arpa/inet.h>
#include <inttypes.h>


#define QUEUESIZE 10
#define SEC_TO_NANOSEC 1000000000L
//#define LOOP 10

void checkHostEntry(struct hostent * hostentry);
void checkIPbuffer(char *IPbuffer);
char *findip();
struct timeval tim;
uint64_t timestamp[2];
char *timer[2];
void error(char *msg)
{
  perror(msg);
  exit(0);
}

int main(int argc, char *argv[])
{
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char MSG[256];
	uint32_t SENDER_AEM;
	uint64_t RCVER_AEM;
	char totDATA[280];
	char SENDER_ADDR[20];
	char RCVER_ADDR[20];
	 
	if (argc < 3) {
		fprintf(stderr,"usage %s hostname port\n", argv[0]);
		exit(0);
	}
	portno = atoi(argv[2]);
	//find clients IP adress
	strcpy(SENDER_ADDR,findip());
	//Begin sending messages
	printf("Connected from IP: %s\n",SENDER_ADDR);
	while(1){
	//Read Message and Info//
	printf("Please enter your AEM: ");
	char* SENDERS_AEM=itoa(SENDER_AEM);
	bzero(SENDERS_AEM,4);
	fgets(SENDERS_AEM,4,stdin);
	printf("Please enter the receivers AEM: ");
	char* RCVERS_AEM=itoa(RCVER_AEM);
	bzero(RCVERS_AEM,4);
	fgets(RCVERS_AEM,4,stdin);
	printf("Please enter the receivers adress(type 'recv' to download your messages): ");
	bzero(RCVER_ADDR,20);
	fgets(RCVER_ADDR,20,stdin);
	strtok(RCVER_ADDR,"\n");
	bzero(MSG,256);
	if (strcmp(RCVER_ADDR,"recv")!=0){
		printf("Please enter the message: ");
		fgets(MSG,256,stdin);	
		strtok(MSG,"\n");
	}
	
	//create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");
	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, 
	(char *)&serv_addr.sin_addr.s_addr,
	server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
		error("ERROR connecting");

  //combine data and send//
    gettimeofday(&tim,NULL);
	timestamp[0]=tim.tv_sec*1000000+tim.tv_usec;
	timer[0] = timestamp[0];
	bzero(totDATA,280);
	strcat(totDATA,SENDERS_AEM);
	strcat(totDATA,"_");
	strcat(totDATA,RCVERS_AEM);
	strcat(totDATA,"_");
	strcat(totDATA,timer[0]);
	strcat(totDATA,"_");
	strcat(totDATA,MSG);
		
	n = write(sockfd,totDATA,strlen(totDATA));
	if (n < 0) 
		error("ERROR writing to socket");
	bzero(totDATA,280);
	n = read(sockfd,totDATA,280);
	gettimeofday(&tim,NULL);
	timestamp[1]=tim.tv_sec*1000000+tim.tv_usec;	
	if (n < 0) 
		error("ERROR reading from socket");
	printf("%s\n",totDATA);
	//printf("\ntime1=%ld time2=%ld\n",timestamp[0],timestamp[1]);
	}
	return 0;
}

//****************FINDING IP****************//
void checkHostName(int hostname)
{
    if (hostname == -1)
    {
        perror("gethostname");
        exit(1);
    }
}
 
// Returns host information corresponding to host name
void checkHostEntry(struct hostent * hostentry)
{
    if (hostentry == NULL)
    {
        perror("gethostbyname");
        exit(1);
    }
}
 
// Converts space-delimited IPv4 addresses
// to dotted-decimal format
void checkIPbuffer(char *IPbuffer)
{
    if (NULL == IPbuffer)
    {
        perror("inet_ntoa");
        exit(1);
    }
}
 
// Driver code
char *findip()
{
    char hostbuffer[256];
    char *IPbuffer;
    struct hostent *host_entry;
    int hostname;
    // To retrieve hostname
    hostname = gethostname(hostbuffer, sizeof(hostbuffer));
    checkHostName(hostname); 
    // To retrieve host information
    host_entry = gethostbyname(hostbuffer);
    checkHostEntry(host_entry);
 
    // To convert an Internet network
    // address into ASCII string
    IPbuffer = inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0])); 
    return IPbuffer;
}
