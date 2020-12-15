#include "server.h"

Node *first = NULL;
GNode *group = NULL;


int main()
{
	int ret, i;
	int SockFd;
	int fd[SIZE] = {0};

	//signal(SIGINT, StopServer);

	ret = LinkInit(&first); 
	if(ret == FAILURE)
	{
		printf("Link init failure \n");
	}
	else
	{
		printf("Link init success \n");
	
	}

	SockFd = ServerInit(); 
	if(SockFd == FAILURE)
	{
		printf("Server Init Failure \n");
	}
	else
	{
		printf("Server Init success \n");
	}

	ret = sqlite3_open("login.db",&ppdb); 
	if(ret != SQLITE_OK)
	{
		perror("sqlite3_open");
		exit(1);
	}
	
	ret = LinkReadData(); 
	if(ret == FAILURE)
	{
		printf("Link read data failure \n");
	}
	else
	{
		printf("Link read data success \n");
	}

	
	while(1)
	{
		ret = ServerAccept(SockFd);
		if(ret == FAILURE)
		{
			printf("accept failure \n");
		}	
	}

	return 0;
}














