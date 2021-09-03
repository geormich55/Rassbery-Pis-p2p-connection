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
#define FILENAME "Statistics.txt"
#define NUM_THREADS  2
#define PORT 2288
#define delim "_"

struct Data
{
    uint32_t Sender;
    uint32_t Receiver;
    uint64_t timestamp;
    char text[256];
};
//Forward declerations of variables
char *circ_buffer[2000];
static int head=0;
int tail=0;
int sent=0;

struct tm *tmp;     //structure useful for converting Linux timestamp to normal date and time
struct Data messg;
uint64_t timestamp;
pthread_mutex_t mutex,lock;
pthread_cond_t cond;
int num_threads;
int size=0;

//Declerations of functions
void *connection(void *socket);
void *server (void *varg);
void *client();
int random_time();
void Statistics(int sockfd, char *IP, socklen_t client_length, struct sockaddr_in *c);
char *random_message();

//Receiver AEMs
uint32_t receiver[10]={8787,5465,6546,6548,8973,8164,8778,8979,9054,1564};

//Pop from buffer of 2000 messages
int circ_bbuf_pop(char **data1)
{
    int next;

    if (head == tail)  // if the head == tail, we don't have any data
        return -1;

    next = tail + 1;  // next is where tail will point to after this read.
    if(next >=2000)
        next = 0;

    *data1 = circ_buffer[tail];// Read data and then move
    tail = next;              // tail to next offset.
    return 0;  // return success to indicate successful push.
}

//Push from buffer of 2000 messages
int circ_bbuf_push( char **data1)
{
    int next;

    next = head + 1;  // next is where head will point to after this write.
    if (next >= 2000)
        next = 0;

    if (next == tail)  // if the head + 1 == tail, circular buffer is full
        return -1;

    circ_buffer[head] = *data1;  // Load data and then move
    head = next;             // head to next data offset.
    return 0;  // return success to indicate successful push.

}

int main(int argc, const char* argv[])
{
    int sockfd, newsockfd;
	struct sockaddr_in server_addr, client_addr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	pthread_t thread_server, thread_client;
	pthread_attr_t attr1, attr2;

	socklen_t clilen;

    messg.Sender=7480;
    //ring_buffer.buffer[0]="Hello";

	if (sockfd < 0)
	{
		perror("ERROR opening socket");
		exit(EXIT_FAILURE);
	}

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

	//On a server, INADDR_ANY is an argument to bind that tells the socket to listen on all available interfaces.
	//On a client, it's an argument to connect that tells the client which server to connect to but would appear
	//to be meaningless other than as an alternative way of specifying "this host".

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;   //or server_addr.sin_addr.s_addr = inet_ntoa("10.0.88.16");
	server_addr.sin_port = htons(PORT);

	if (bind(sockfd,(struct sockaddr *)&server_addr ,sizeof(server_addr)) < 0)
	{
		perror("ERROR on binding");
		exit(EXIT_FAILURE);
	}

	if (listen(sockfd, 20)==0)
       printf("Listening...\n");//20-queue client

	clilen=sizeof(client);

	while(1)
	{
        newsockfd = accept(sockfd, (struct sockaddr*)&client_addr, (socklen_t*)&clilen);
		if (newsockfd<0)
		{
			perror("ERROR on accepting connection");
			exit(-1);
        }
        //Identify the Raspberry
        getpeername(newsockfd, (struct sockaddr*) &client_addr, (socklen_t*) &clilen);
        printf("Client %s: come in.\n", inet_ntoa(client_addr.sin_addr));
        Statistics(newsockfd, inet_ntoa(client_addr.sin_addr), clilen, &client_addr);
        pthread_attr_init(&attr1);
        pthread_attr_setdetachstate(&attr1, PTHREAD_CREATE_JOINABLE);
        pthread_create(&thread_server, &attr1, connection, (void *)&newsockfd);
        pthread_attr_destroy(&attr1);
        pthread_join(thread_server,NULL);

        pthread_attr_init(&attr2);
        pthread_attr_setdetachstate(&attr2, PTHREAD_CREATE_JOINABLE);
        pthread_create(&thread_client, &attr2, client, NULL);
        pthread_attr_destroy(&attr2);
        pthread_join(thread_client, NULL);
    }
}

void *connection(void *socket)
{
    char temp_buffer[500];
    int i,n;  //helpers
    int comparator;
    int socket_in=*(int*)socket;

    bzero(temp_buffer, 500);

    pthread_mutex_lock(&mutex);
    n = read(socket_in, temp_buffer, 500);
    if (n==0)
    {
        perror("Can't read from socket");
        exit(-1);
    }
    pthread_mutex_unlock(&mutex);
    if(n>0)
    {
        for (i=0;i<tail;i++)
        {
            if (strcmp(circ_buffer[i],temp_buffer)==0) comparator=0;   //if the received message already exists comparator=0
        }
        if (comparator!=0)
        {
            circ_bbuf_push(temp_buffer);
        }
        else printf("Message already received");
    }
    pthread_mutex_unlock(&mutex);
    return 0;
}

char *random_message()
{
	char *letters[26]={"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};
	char *message;
	char sender[11], receiver[11];
    int i, j, n, f1, t1, k1;
	sprintf(sender,"%"PRIu32, messg.Sender);
	strcat(message, sender);
	strcat(message, "_");
	//Choose receiver from the list above
	srand(time(NULL));
	int rand_num=rand()%10;
	sprintf(receiver,"%" PRIu32, receiver[rand_num]);
	strcat(message, receiver);
	strcat(message, "_");
    timestamp=time(NULL);
    char timer[21];
    sprintf(timer,"%"PRIu64"\n",timestamp);
    strcat(message,timer);
    strcat(message,"_");
	//Create random message
	for (int i=0;i<256;i++)
	{
		srand(time(NULL));
		int rand_num=rand()%26;
		strcat(message, letters[rand_num]);
	}

	//Save the message in the List if it has been sent

	pthread_mutex_lock(&mutex);

	//Check if this message has ever gone to this recipient comparing the recipient's AEM characters and the MSG characters with other messages AEMs and MSG//
	pthread_mutex_lock(&mutex);


	for(i=0;i<size+1;i++)
	{
		for(j=12;j<22;j++)
		{
            if(strcmp(circ_buffer[i][j], message[j]) != 0) //we compare the RCVERS_AEM char 11-21
                t1=0;
        }
    }

    for(i=0;i<size+1;i++)
    {
		for(j=46;j<302;j++)
		{
            if(strcmp(circ_buffer[i][j], message[j]) != 0) //we compare the message char 44-300
                k1=0;
        }
    }

	pthread_mutex_unlock(&mutex);

	if(n>=0)
	{
        f1=circ_bbuf_push(message);
        if(f1==-1)
        {
            circ_bbuf_pop(message);
            circ_bbuf_push(message);
        }
        size++;
    }


	pthread_mutex_unlock(&mutex);
	return message;
}
///////////////////////////////////////////////////////////////////////////////////
void *client()
{
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *host_entry;

    if ((sockfd=socket(AF_INET, SOCK_STREAM, 0))<0)
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    memset(&serv_addr,0,sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0]));

    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) perror("Can't connect cyka");
    while (1)
    {
        //First we have to check if we have any messages to be sent
        if (tail==sent)
        {
            printf("Nothing new to send");
        }
        else
        {
            while (sent<=tail)
            {
                send(sockfd, circ_buffer[sent+1],strlen(circ_buffer[sent+1]),0);
                sent++;
            }
        }
        //Then we create a new message every 60-300 seconds
        int rand_sec=random_time();
        alarm(rand_sec);
        char *created_msg = random_message();
    }
}

int random_time()
{
    int min_seconds=60;
    int max_seconds=300;
    int random_second=min_seconds+(rand()%(max_seconds-min_seconds+1));
    return random_second;
}

void Statistics(int sockfd, char *IP, socklen_t client_length, struct sockaddr_in *c)
{
    system("clear");
    FILE *file;
    file=fopen(FILENAME, "a");
    if (file==NULL)
    {
        printf("Error non-existing file");
        exit(1);
    }
    /*Bank new;
    new.bank_account=++id_count;
    printf("Please enter the name of the owner:");
    scanf("%s",&new.name);
    printf("\n");
    printf("Please enter the balance:");
    scanf("%f",&new.balance);*/

    timestamp=time(NULL);
    char timer[21];
    sprintf(timer,"%"PRIu64"\n",timestamp);
    strftime(timer,sizeof(timer), "%x -%I:%M%p", tmp);
    fprintf(file,"%s: ->", timer);

    getpeername(sockfd, (struct sockaddr*) &client, (socklen_t*) &client_length);
    fprintf(file, "Sent message to IP:%s\n\n\n", inet_ntoa(c->sin_addr));

    fclose(file);
}
