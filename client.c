// James Maestro
// Client portion of SBCS Protocal
/////////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>		
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <sys/select.h>
#define STDIN fileno(stdin)

///////////////////////////////MSG FORMAT/////////////////////////////////////////////////////
struct message
{
    // Header
    unsigned int version:9;
    unsigned int type:7;
    unsigned int length:16;

    // Msg attribute
    unsigned int Atype:16;
    unsigned int Alength:16;
    char Payload[512];
};


/////////////////////////////////ERROR MSG FUNCTION///////////////////////////////////////////

void error(char *msg)
{
    perror(msg);
    exit(0);
};

/////////////////////////////////////////JOIN SERVER FUNCTION//////////////////////////////////////////

void JoinServer(char *buffer,int socketfd, int size_buffer)
{
   struct  message msg;
    msg.version=3; //set the protocal version is 3
    msg.type=2; // join message is type 2
    msg.Atype=2; //username used in chatting
    msg.Alength=(4+size_buffer);
    bzero((char*)&msg.Payload,sizeof(msg.Payload));
    int i=0;
    for (i=0;i<size_buffer;i++)
    msg.Payload[i]=buffer[i];
    msg.length=(8+size_buffer); 

    if (send(socketfd,msgChar,sizeof(msgChar),0)<0)
    {
    	error("Error writing to socket\n");
    }
    printf("The join message has been sent successfully\n");
}

/////////////////////////////////////SEND MSG TO SERVER///////////////////////////////////////////////

void SendMsg(int socketfd)
{
    struct message msg;
	char buffer[512];
    int size_buffer;

    fgets(msg.Payload,sizeof(msg.Payload),stdin);
    msg.version=3;
    msg.type=4; //send message is type 4
    msg.Atype=4; //message is type 4
    msg.Alength=(4+sizeof(msg.Payload);
    msg.length=(8+sizeof(msg.Payload);
    if (send(socketfd,msgChar,sizeof(msgChar),0)<0)
    {
    	error("Error writing to socket\n");
    }
}

//////////////////////////////////////////////////MAIN FUNCTION////////////////////////////////////


int main(int argc,char * argv[])
{
	int sockfd, portno, connfd;
	struct sockaddr_in serv_addr;
	fd_set readfd;
	int fdmax;
	char *username;
	int buf_len;
	message msg_rcv;

	char serv_addr_str[100];
	char buffer[10000];
	username = argv[1];
	buf_len = strlen(username);
	int time = 0;
	int idle;
	typedef struct timeval {
		long tv_sec;
		long tv_usec;
	} TIMEVAL, *PTIMEVAL, *LPTIMEVAL;

	TIMEVAL timeout;
	timeout.tv_usec = 10000;
	timeout.tv_sec = 0;

	sockfd = socket(AF_INET, SOCK_STREAM, 0); //create socket
	if (sockfd<0)
	{
		printf("Error creating socket\n"); exit(1);
	}
	else
	{
		printf("Socket is successfully created\n");
	}

	inet_pton(AF_INET, argv[2], &(serv_addr.sin_addr));

	memset(&serv_addr, '\0', sizeof(serv_addr)); //zero structure out
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[3]));

	connfd = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	if (connfd< 0)
	{
		printf("Error connecting\n");
	}
	else
	{
		printf("Client is connected to the server\n");
	}


	JoinServer(username, sockfd, buf_len);
	idle = 0;

	//Clear the socket set
	FD_ZERO(&readfd);
	FD_SET(0, &readfd);
	FD_SET(sockfd, &readfd);

	while (1)
	{
		time = time + 1;
		timeout.tv_usec = 10000;

		// Figures out if you idle
		if (time>1000 && idle == 0) {
			printf("You're now idle.\n");
			idle = 1;
			message idle;
			idle.version = 3;
			idle.type = 9;
			strcpy(idle.Payload, username);
			idle.length = (8 + sizeof(msg_rcv.Payload));
			idle.Atype = 0;
			idle.Alength = (4 + sizeof(msg_rcv.Payload));

			if (send(sockfd, (void*)&idle, sizeof(idle), 0)<0)
			{
				printf("Idle message send failure");
			}
		}
		fdmax = sockfd;
		if (select(fdmax + 1, &readfd, NULL, NULL, &timeout)<0)
		{
			perror("Select failed\n"); exit(1);
		}
		else
		{
			if (FD_ISSET(sockfd, &readfd))
			{
				// This whole section determins the message sent to the server
				recv(sockfd, (void*)&msg_rcv, sizeof(msg_rcv), 0);
				if (msg_rcv.version == 3 && msg_rcv.type == 3 && msg_rcv.Atype == 4)
				{
					printf("%s", msg_rcv.Payload);
				}
				if (msg_rcv.type == 7)
				{
					printf("ACK message: %s\n", msg_rcv.Payload);
				}
				if (msg_rcv.type == 5)
				{
					printf("NAK message:%s\n", msg_rcv.Payload);
					exit(0);
				}
				if (msg_rcv.type == 8)
				{
					printf("ONLINE message: %s had joined the chat!\n", msg_rcv.Payload);
				}
				if (msg_rcv.type == 6)
				{
					printf("OFFLINE message: %s had left the chat :(\n", msg_rcv.Payload);
				}
				if (msg_rcv.type == 9)
				{
					printf("%s is idle\n", msg_rcv.Payload);
				}
			}

			if (FD_ISSET(0, &readfd))
			{
				SendMsg(sockfd);
				time = 0;
				idle = 0;
			}

			FD_SET(0, &readfd);
			FD_SET(sockfd, &readfd);
		}
	}
	// closes out socket and client
	close(sockfd);
	printf("Client close\n");
	return 0;

   
}