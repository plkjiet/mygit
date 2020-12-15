#include "client.h"

pthread_mutex_t mutex;  
pthread_cond_t cond;	
int sockfd;			
pthread_t tid_send;			
pthread_t tid_recv;		
time_t tt;
FILE *fp;
//char version;
char user_level[16];
int outputFd;




void *SendMsg(void *arg)
{
	int ret;
	int SockFd = *(int *)arg;
	
	while(1)
	{
//		pthread_mutex_lock(&mutex);
		char choise[20];
		printf("1.register	\n");
		printf("2.login		\n");
		printf("3.exit		\n");
		scanf("%s", choise);
		switch(atoi(choise))
		{
			case 1:
				ret = UserReg(SockFd);
				if(ret == FAILURE)
				{
					printf("register failure \n");
				}
				else
				{
					printf("register success \n");
				}

				break;

			case 2:
				ret = UserLogin(SockFd);
				if(ret == FAILURE)
				{
					printf("login failure \n");
				}
				else
				{
					printf("login success \n");
					IntoMyZone(SockFd);
				}
				break;
		
			case 3:
				ret = Quit(SockFd);
				if(ret == FAILURE)
				{
					printf("quit failure \n");
				}
				else
				{
					printf("quit success \n");
				}
				break;
		
		}
	}

}

void *RecvMsg(void *arg)
{
	int ret;
	int old;
	int fd = *(int *)arg;
	Info1 RecvInfo;

	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old);
	pthread_cond_wait(&cond, &mutex);
	while(1)
	{
		memset(&RecvInfo, 0, sizeof(RecvInfo));
		
		ret = recv(fd, &RecvInfo, sizeof(RecvInfo), 0);
		if(ret < 0)
		{
			perror("recv");
		}
	
		if(strcmp(RecvInfo.buf, "checkon") == 0)
		{
			printf("this is online frend : ");
			printf("name : %s \n", RecvInfo.name);
		}
		
		
		else if(strcmp(RecvInfo.buf, "private") == 0)
		{
			tt = time(NULL);
			printf("you have a new message(private) %s  ", ctime(&tt));
			fprintf(fp, "private  %s \n", ctime(&tt));
			printf("from : %s \n", RecvInfo.name);
			fprintf(fp, "from : %s \n", RecvInfo.name);
			printf("recv : %s \n", RecvInfo.mesg);
			fprintf(fp, "recv : %s \n\n", RecvInfo.mesg);
		}
		
		else if(strcmp(RecvInfo.buf, "privatefalse") == 0)
		{
			printf("user %s not exit \n", RecvInfo.toname);
		}
		
		else if(strcmp(RecvInfo.buf, "filefalse") == 0)
		{
			printf("user %s not exit \n", RecvInfo.toname);
		}
		
		else if(strcmp(RecvInfo.buf, "group") == 0)
		{
			tt = time(NULL);
			printf("you have a new message(group)  %s ", ctime(&tt));
			fprintf(fp, "group %s \n", ctime(&tt));
			printf("from : %s \n", RecvInfo.name);
			fprintf(fp, "from : %s \n", RecvInfo.name);
			printf("recv : %s \n", RecvInfo.mesg);
			fprintf(fp, "recv : %s \n\n", RecvInfo.mesg);
		}
		
		else if(strcmp(RecvInfo.buf, "roomcheckon") == 0)
		{
			printf("this is online frend : ");
			printf("name : %s \n", RecvInfo.name);
		}
		
		else if(strcmp(RecvInfo.buf, "roomchat") == 0)
		{
			tt = time(NULL);
			printf("you have a new message(group) from chatroom %s : %s ", RecvInfo.group, ctime(&tt));
			fprintf(fp, "chatroom %s %s \n", ctime(&tt), RecvInfo.group);
			printf("from : %s \n", RecvInfo.name);
			fprintf(fp, "from : %s \n", RecvInfo.name);
			printf("recv : %s \n", RecvInfo.mesg);
			fprintf(fp, "recv : %s \n\n", RecvInfo.mesg);
		}
		
		else if(strcmp(RecvInfo.buf, "roomquit") == 0)
		{
			pthread_cond_signal(&cond);
			IntoMyZone(sockfd);
		}
		else if(strcmp(RecvInfo.buf, "roomkick") == 0)
		{
			pthread_cond_signal(&cond);
			printf("you are kicked by admin \n");
			IntoMyZone(sockfd);
		}
		
		else if(strcmp(RecvInfo.buf, "wait") == 0)
		{
			printf("recv in wait \n");
			pthread_cond_wait(&cond, &mutex);
		}
		
		else if(strcmp(RecvInfo.buf, "welcome") == 0)
		{
			printf("user %s login \n", RecvInfo.name);
		}
		
		else if(strcmp(RecvInfo.buf, "quitsend") == 0)
		{
			printf("user %s logout \n", RecvInfo.name);
		}
		
		else if(strcmp(RecvInfo.buf, "beadmin") == 0)
		{
			printf("you are limited or have be admin  \n");
		}
		
		else if(strcmp(RecvInfo.buf, "benormal") == 0)
		{
			printf("you are limited or hanv be normal \n");
		}
		
		else if(strcmp(RecvInfo.buf, "roomadmin") == 0)
		{
			printf("you are have be admin			 \n");
		}
		
		else if(strcmp(RecvInfo.buf, "roomkick") == 0)
		{
			printf("you are not admin can't limit	 \n");
		}
		
		else if(strcmp(RecvInfo.buf, "chlevel") == 0)
		{
			memset(user_level, 0, sizeof(user_level));
			printf("become admin ....\n");
			strcpy(user_level, "admin");
			printf("become success \n");
		}
		
		else if(strcmp(RecvInfo.buf, "chnormal") == 0)
		{
			memset(user_level, 0, sizeof(user_level));
			printf("become normal ....\n");
			strcpy(user_level, "normal");
			printf("become success \n");
		}
		
		else if(strcmp(RecvInfo.buf, "limited") == 0)
		{
			memset(user_level, 0, sizeof(user_level));
			printf("you are be limited by admin \n");
			strcpy(user_level, "limit");
			printf("please shua xin \n");
		}
		
		else if(strcmp(RecvInfo.buf, "sendfile") == 0)
		{			
			int	openFlags;
    		mode_t filePerms;
    		char buf[64];
  			openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    		filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |S_IROTH | S_IWOTH;     
  		    outputFd = open("recvfile", openFlags, filePerms);
    		ret = lseek(outputFd, 64, SEEK_END);
			printf("ret = %d \n",ret);
			write(outputFd, RecvInfo.mesg, sizeof(RecvInfo.mesg));
			
			/*if (write(outputFd, RecvInfo.mesg, sizeof(RecvInfo.mesg)) != sizeof(RecvInfo.mesg))
			{
				printf("error \n");
			}
			else
			{
				printf("write \n");
			}
*/
		//	close(outputFd);

		/*	FILE *fd;
			fd = fopen("recvfile", "a");
			fwrite(&RecvInfo.mesg, sizeof(RecvInfo.mesg), 1, fd);
			fclose(fd);
		*/
		}
		//接收到其他的东西
		else
		{
			printf("not info \n");
		}
	}

}

int main()
{
	int ret;

	signal(SIGINT, StopClient); 
	

	fp = fopen("./chat_records", "a+");
	sockfd = ClientInit();
	if(sockfd == FAILURE)
	{	
		printf("init failure \n");
		exit(1);
	}
	
	ret = pthread_create(&tid_send, NULL, SendMsg, &sockfd);
	if(ret != 0)
	{
		perror("pthread_creat");
		exit(1);
	}

	ret = pthread_create(&tid_recv, NULL, RecvMsg, &sockfd);
	if(ret != 0)
	{
		perror("pthread_creat");
		exit(1);
	}


	while(1)
	{
	}

	return 0;
}





















