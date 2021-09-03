#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/sysinfo.h>
#include <inttypes.h>
#include <signal.h>

#define SEC_TO_NANOSEC 1000000000L
#define NUM_THREADS  4

typedef const struct{
	const uint32_t* AEMs[2];
	const char* IPs[2];
}Parameters_t;

Parameters_t Parameter[2] = {{7480,8566},{"10.0.74.80","10.0.85.66"}};

typedef struct {
    char** const buffer;
    int head;
    int tail;
    const int maxlen;
} circ_bbuf_t;

struct circ_bbuf_t my_circ_buf;
void checkHostEntry(struct hostent * hostentry);
void checkIPbuffer(char *IPbuffer);
char *findip();
struct timeval tim;
uint64_t timestamp[0];
char TIMER[21];
void display_message(int s);
 int size=0;
void error(char *msg)
{
  perror(msg);
  exit(0);
}
 const int maxlen=2000;
 int head=0;
 int tail=0;
 int t;
 struct sysinfo info;
 int thread_ids[2]={0,1};
pthread_mutex_t mutex,lock;
pthread_cond_t cond;
int num_threads= 0;
int num_threads1=0;
void *connection(void* socket);
void *listening(void* socket);


	

int main(int argc, char *argv[]){
	int sockfd, newsockfd, portno, clilen;
	char buff[500];
	struct sockaddr_in serv_addr, cli_addr;
	int n,i,j,f1;
	int t1,k1=1;
	struct hostent *server;
	pthread_t threads[2];
	
	/* Initialize mutex and condition variable objects */
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init (&cond, NULL);
  
	//CLIENT OPERATION//
    char MSG[256];
	uint32_t SENDER_AEM;
	uint32_t RCVER_AEM;
	char SENDERS_AEM[10]
	char RCVERS_AEM[10];
	char totDATA1[500];
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
	t=createRandoms(1,5,1);
	l=random(2);
	random_message_and_AEM=random_message_and_AEM(t,l);
    signal(SIGALRM, random_message_and_AEM);
    alarm(t);     //Alarm signal every 1-5 seconds.
	while(1){
	//Read Message and Info//
	/*printf("Please enter your adress: ");
	bzero(SENDER_ADDR,20);
	fgets(SENDER_ADDR,20,stdin);
	strtok(SENDER_ADDR,"\n");*/
	printf("Please enter your AEM: ");
	bzero(SENDERS_AEM,10);
	fgets(SENDERS_AEM,10,stdin);
	sprintf( SENDERS_AEM, "&#37;u", SENDER_AEM );
    
	if(SENDER_AEM==7480)
		SENDER_ADDR="10.0.74.80";
	else if(SENDER_AEM==8566)
		SENDER_ADDR="10.0.85.66";
	if(RCVER_AEM==7480)
		RCVER_ADDR="10.0.74.80";
	else if(RCVER_AEM==8566)
		RCVER_ADDR="10.0.85.66";
	
	

	serv_addr.sin_addr.s_addr=RCVER_ADDR;
	cli_addr.sin_addr.s_addr=SENDER_ADDR;
	

	      
	
	
	
	  
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

  //Combine data//
    
	gettimeofday(&tim,NULL);
	timestamp[0]=tim.tv_sec*1000000+tim.tv_usec;
	// length of 2**64 - 1, +1 for nul.
    char TIMERS[21];
    // copy to buffer
    sprintf(TIMERS, "%" PRIu64, timestamp[0]);
	strcat(totDATA1,SENDERS_AEM);
	strcat(totDATA1,"_");
	strcat(totDATA1,RCVERS_AEM);
	strcat(totDATA1,"_");
	strcat(totDATA1,TIMERS);
	strcat(totDATA1,"_");
	strcat(totDATA1,MSG);
    
	//Check if this message has ever gone to this recipient comparing the recipient's AEM characters and the MSG characters with other messages AEMs and MSG//
	pthread_mutex_lock(&mutex);
	
	
	for(i=0;i<size;i++){
		for(j=11;j<21;j++){
        if(strcmp(my_circ_buf.buffer[i][j], totDATA1[j]) != 0) //we compare the RCVERS_AEM char 11-21
	             t1=0;}}
    for(i=0;i<size;i++){
		for(j=43;j<299;j++){
		if(strcmp(my_circ_buf.buffer[i][j], totDATA1[j]) != 0)
	             k1=0;}}
		
	pthread_mutex_unlock(&mutex);
	
    //if this message has not ever gone to this recipient send, else don't send//
    if(t1==0&&k1==0){	
	n = write(sockfd,totDATA1,strlen(totDATA1));
	if (n < 0) 
		error("ERROR writing to socket");
	
	//Save the message in the List if it has been sent 
	
	pthread_mutex_lock(&mutex);
	
	if(n>=0){
	
	f1=circ_bbuf_push(&my_circ_buf,totDATA1);
    if(f1==-1){
		circ_bbuf_pop(&my_circ_buf,&totDATA1);
	circ_bbuf_push(&my_circ_buf,totDATA1);}
	size++;}
	
		
	pthread_mutex_unlock(&mutex);}
	
	bzero(totDATA1,500);
	//read data from socket
	n = read(sockfd,totDATA1,500);
	if (n < 0) 
		error("ERROR reading from socket");
	printf("%s\n",totDATA1);
	return 0;
	}
	

//SERVER OPERATION//

	
	pthread_t thread_id;
	pthread_t thread_id1;
	if (argc < 2) {
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}
	if (pthread_mutex_init(&mutex,NULL)!=0){
		error("ERROR initializing mutex.");
		return 1;
	}
	
	
 
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
    error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	
	
	
	while(1){
		if (num_threads1>=20){
			pthread_mutex_lock(&lock);
			pthread_cond_wait(&cond,&lock);
			pthread_mutex_unlock(&lock);
		}
		
	
		if (pthread_create(&thread_id1,NULL,listening,(void*)&sockfd)!=0){
			printf("ERROR creating thread");
			return 1;	
		} 
		else{
			pthread_mutex_lock(&lock);
			num_threads1++;
			pthread_mutex_unlock(&lock);
		}
	}
	
	
	clilen = sizeof(cli_addr);
	while(1){
		if (num_threads>=20){
			pthread_mutex_lock(&lock);
			pthread_cond_wait(&cond,&lock);
			pthread_mutex_unlock(&lock);
		}
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (pthread_create(&thread_id,NULL,connection,(void*)&newsockfd)!=0){
			printf("ERROR creating thread");
			return 1;	
		}
		else{
			pthread_mutex_lock(&lock);
			num_threads++;
			pthread_mutex_unlock(&lock);
		}
	}
	if (newsockfd < 0) error("ERROR on accept");
	return 0; 
	
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);
  pthread_exit(NULL);


}

//Pop from buffer of 2000 messages
int circ_bbuf_pop(circ_bbuf_t *c, char **data1)
{
    int next;

    if (c->head == c->tail)  // if the head == tail, we don't have any data
        return -1;

    next = c->tail + 1;  // next is where tail will point to after this read.
    if(next >= c->maxlen)
        next = 0;

    **data1 = c->buffer[c->tail];// Read data and then move 
    c->tail = next;              // tail to next offset.
    return 0;  // return success to indicate successful push.
}

//Push from buffer of 2000 messages
int circ_bbuf_push(circ_bbuf_t *c, char *data1)
{
    int next;

    next = c->head + 1;  // next is where head will point to after this write.
    if (next >= c->maxlen)
        next = 0;

    if (next == c->tail)  // if the head + 1 == tail, circular buffer is full
        return -1;

    c->buffer[c->head] = *data1;  // Load data and then move
    c->head = next;             // head to next data offset.
    return 0;  // return success to indicate successful push.
	
}
	
	//********SERVER OPERATION FUNCTIONS***********//
void	*listening(void *socket){
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
           sizeof(serv_addr)) < 0) 
           error("ERROR on binding");
	listen(sockfd,50);//50 socket nodes available
	
}


//**********SERVER AND CLIENT OPERATION FUNCTION***********//
void *connection(void *socket){
	char buff[500],*token;
	char *dilim="_";
	int n,curr,i,tosend,f;
	int socket_in=*(int*)socket;
	int t,k=1;
	int f;
	bzero(buff,500);
	n = read(socket_in,buff,500);
		
 	if (n < 0) error("ERROR reading from socket");
	
	
	//realloc MEMORY//+mutex CHECK IF THE MESSAGE HAS COME ONE TIME OR MORE AND IF CAME MORE THAN 1 TIME WE DON'T PUT IT IN OUR LIST,ELSE WE PUT IT
	pthread_mutex_lock(&mutex);
	
	if(n>=0){
	for(i=0;i<size;i++){
        if(strcmp(&my_circ_buf[i], buff) == 0)
	t=0;}
    if(t!=0){
	f=circ_bbuf_push(&my_circ_buf,buff);
    if(f==-1){
		circ_bbuf_pop(&my_circ_buf,&buff);
	circ_bbuf_push(&my_circ_buf,buff);}
	size++;
	curr=size-1}}
		
	pthread_mutex_unlock(&mutex);

/////////////////////////////////////////////////////////?????????????????????????????????????????????????????????????????
	//check if receive only
	char current_rcv[20];
	strcpy(current_rcv,RCVER_ADDR);
	if(strcmp(current_rcv,"recv")==0){
		strcpy(current_rcv,SENDER_ADDR);
		pthread_mutex_lock(&mutex);
		size--;
		circ_bbuf_pop(&my_circ_buf,&buff);
		pthread_mutex_unlock(&mutex);
	}
	else{
		strcpy(current_rcv,SENDER_ADDR);
		token=strtok(NULL,dilim);
		strcpy(MSG,token);
	}
	//search for receive
	pthread_mutex_lock(&mutex);
	bzero(buff,500);
	int found=0;
	i=0;
	while(i<=size){
		if (strcmp(current_rcv,RCVER_ADDR)==0&&i!=size&&strlen(buff)<500){
			
			gettimeofday(&tim,NULL);
	        timestamp[0]=tim.tv_sec*1000000+tim.tv_usec;
	        // length of 2**64 - 1, +1 for nul.
            char TIMERS[21];
            // copy to buffer
            sprintf(TIMERS, "%" PRIu64, timestamp[0]);
	        strcat(buff,SENDERS_AEM);
	        strcat(buff,"_");
	        strcat(buff,RCVERS_AEM);
	        strcat(buff,"_");
	        strcat(buff,TIMERS);
	        strcat(buff,"_");
	        strcat(buff,MSG);
		for(i=0;i<size;i++){
            if(buff[i]!=buff[size])
	          k=0;}
               if(t==0)
	             f=circ_bbuf_push(&my_circ_buf,buff);
               if(f==-1){
		       circ_bbuf_pop(&my_circ_buf,&buff);
	        circ_bbuf_push(&my_circ_buf,buff);}
			found=1;
			i--;
		}
		else if((i==size-1&&found==0)||(size==0&&found==0)) {
			strcat(buff,"Done. You have no new messages.");
		}
		i++;
	}
	pthread_mutex_unlock(&mutex);
	/*for (i=0;i<size;i++){
		printf("%s %s %s\n",MEM[i].SENDER_ADDR,MEM[i].RCVER_ADDR,MEM[i].MESSAGE);
	}*/
  	printf(" num threads=%d\n",num_threads);
	printf("\nsize=%d\n",size);
/////////////////////////////////////////////////////////?????????????????????????????????????????????????????????????????		
	n = write(socket_in,buffer,strlen(buffer));
  	if (n < 0) error("ERROR writing to socket");
	pthread_mutex_lock(&lock);
	num_threads--;
	num_threads1--;
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&lock);
	
  	return 0;
}
	//*************CLIENT OPERATION FUNCTIONS*******************//


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

// Generates and prints 'count' random 
// numbers in range [lower, upper]. 
int createRandoms(int lower, int upper,  
                             int count) 
{ 
    int i; 
    for (i = 0; i < count; i++) { 
        int num = (rand() % 
           (upper - lower + 1)) + lower; 
        return num; 
    } 
}

   //Function for alarm set up
void random_message_and_AEM(int s,int l) {
     	char* words[2]; // 1
        words[0] = "blah";
        words[1] = "hmm";

        char random; // 2
        srand(time(NULL));
        random = words[rand() % 2]; // 3
	    strcpy(MSG,random);
		RCVER_AEM=parameter[0][l];
		bzero(RCVERS_AEM,10);
	    sprintf( RCVERS_AEM, "&#37;u", RCVER_AEM );
        alarm(s);    //for every 1-5 second
        signal(SIGALRM, random_message);
}

int random(int range){
    int num;
    srand(time(NULL));
    num = rand() % range;
    return num;
}