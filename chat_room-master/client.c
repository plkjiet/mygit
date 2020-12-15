#include "client.h"

char buf[10];
char buffer[10];
extern char user_level[16];		
int ret;
Info1 loginInfo;				
extern pthread_mutex_t mutex;   
extern pthread_cond_t cond;		
extern int sockfd;				
extern pthread_t tid_send;		
extern pthread_t tid_recv;		
extern FILE *fp; 				
extern int outputFd;			
time_t t;					



void StopClient(int num)
{
	strcpy(loginInfo.buf, "quit");
	send(sockfd, &loginInfo, sizeof(loginInfo), 0);
	close(sockfd);
	fclose(fp);
	close(outputFd);
	printf("BYE ! \n");
	pthread_cancel(tid_recv);
	exit(1);
}



int ClientInit()
{
	int sockfd;
	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if(-1 == sockfd)
	{
		perror("socket");
		exit(1);
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = PF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = inet_addr("192.168.238.129");
	ret = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if(-1 == ret)
	{
		perror("connect");
		exit(1);
	}
	
	return sockfd;

}


int UserReg(int sockfd)
{
	memset(buf, 0, sizeof(buf));
	Info1 regInfo;
	memset(&regInfo, 0, sizeof(regInfo));
	strcpy(regInfo.buf, "register");
	strcpy(regInfo.level, "normal");
	printf("please input name you want register \n");
	scanf("%s", regInfo.name);
	printf("please input id you want register \n");
	scanf("%s", regInfo.id);
	printf("please input passwd you want register \n");
	scanf("%s", regInfo.passwd);

	ret = send(sockfd, &regInfo, sizeof(regInfo), 0);
	if(ret == -1)
	{
		perror("send");
		exit(1);
	}
	
	ret = recv(sockfd, buf, sizeof(buf), 0);
	if(ret == -1)
	{
		perror("recv");
		exit(1);
	}

	if(strcmp(buf, "false") == 0)
	{
		return FAILURE;
	}
	else
	{
		return SUCCESS;
	}

}


int UserLogin(int sockfd)
{
	Log templog;
	char choise[10];
	memset(&loginInfo, 0, sizeof(loginInfo));
	memset(buf, 0, sizeof(buf));

	strcpy(loginInfo.buf, "login");
	printf("please input name you want login \n");
	scanf("%s", loginInfo.name);
	printf("please input id you want login \n");
	scanf("%s", loginInfo.id);
	printf("please input passwd \n");
	scanf("%s", loginInfo.passwd);

	ret = send(sockfd, &loginInfo, sizeof(loginInfo), 0);
	if(ret == -1)
	{
		perror("send");
		exit(1);
	}

	ret = recv(sockfd, &templog, sizeof(templog), 0);
	if(ret == -1)
	{
		perror("recv");
		exit(1);
	}

	if(strcmp(templog.buf, "success") == 0)
	{
		strcpy(user_level, templog.level);
		return SUCCESS;
	}
	else
	{
		printf("failure \n");
		return FAILURE;
	}
}


int Quit(int sockfd)
{
	strcpy(loginInfo.buf, "quit");
	ret = send(sockfd, &loginInfo, sizeof(loginInfo), 0);
	if(ret == -1)
	{
		perror("send");
		exit(1);
	}
	close(sockfd);
	printf("bye \n");
	exit(1);
}


int IntoMyZone(int sockfd)
{
	char choise[10];
	char flag[10];
	char tempbuf[1024];
	char limitname[32];

	while(1)
	{
		pthread_cond_signal(&cond);

		system("clear");
		printf("hello \n");
		printf("level: %s			 \n", user_level);
		printf("welcome user name : %s \n", loginInfo.name);
		printf("1.check online friend \n"); 
		printf("2.sned message (private) \n");		
		printf("3.send message (group) to online friends \n");	
		printf("4.creat a group chat \n");	
		printf("5.enter a group chat \n");	
		printf("6.check chat records \n");
		printf("7.clear chat record  \n");
		printf("8.become admin       \n");
		printf("9.limit someone		 \n");
		printf("10.become normal	 \n");
		printf("11.send a file 		 \n");

		scanf("%s", choise);
		switch(atoi(choise))
		{
			case 1:
				printf("please input 'quit' to retuen previous screen \n");
				ret = CheckFriend(sockfd);
				if(ret == SUCCESS)
				{
					printf("check online finish \n");
				}


				break;
			case 2:
				ret = CheckLimit();
				if(ret == TRUE)
				{
					break;
				}
				else
				{
					printf("please input 'quit' to retuen previous screen \n");
				}
				
				ret = PrivateChat(sockfd);
				if(ret == SUCCESS)
				{
					printf("send mesg finish \n");
				}
				break;

			case 3:
				ret = CheckLimit();
				if(ret == TRUE)
				{
					break;
				}
				else
				{
					printf("please input 'quit' to retuen previous screen \n");
				}

				ret = GroupChat(sockfd);
				if(ret == SUCCESS)
				{
					printf("send mesg finish \n");
				}

				break;
			case 4:
				ret = CheckLimit();
				if(ret == TRUE)
				{
					break;
				}
				else
				{
					printf("please input 'quit' to retuen previous screen \n");
				}

				ret = CreatGroup(sockfd);
				if(ret == SUCCESS)
				{
					printf("creat chat room success \n");
				}
				else
				{
					printf("creat char room failure \n");
				}

			
				break;
			case 5:
				ret = CheckLimit();
				if(ret == TRUE)
				{
					break;
				}
				else
				{
					printf("please input 'quit' to retuen previous screen \n");
				}

			 	ret = EnterGroup(sockfd);
				if(ret == SUCCESS)
				{
					printf("ok \n");
					break;
				}
				else
				{
					printf("not ok \n ");
					break;
				}
			case 6:
				printf("please input 'quit' to retuen previous screen \n");
				ret = ChatRecords();
				if(ret == SUCCESS)
				{
					printf("success \n");
				}

				break;

			case 7:
				printf("please input 'quit' to retuen previous screen \n");
				ret = ClearRecord();
				if(ret == SUCCESS)
				{
					printf("clear success \n");
				}
				else
				{
					printf("clear failure \n");
				}

				while(1)
				{
					printf("please input 'quit' to retuen previous screen \n");
					scanf("%s", buffer);
					if(strcmp(buffer, "quit") == 0)
					{
						break;
					}
					else
					{
						printf("please input 'quit' to retuen previous screen \n");
					}
				}

			case 8:
				ret = CheckLimit();
				if(ret == TRUE)
				{
					break;
				}
				else
				{
					printf("please input 'quit' to retuen previous screen \n");
				}

				ret = BeAdmin(sockfd);
				if(ret == SUCCESS)
				{
					printf("become success \n");
				}

				break;
				
			case 9:
				ret = CheckLimit();
				if(ret == TRUE)
				{
					break;
				}
				else
				{
					printf("please input 'quit' to retuen previous screen \n");
				}

				printf("please input user name you want limit		  \n");
				scanf("%s", limitname);
				ret = Limitsb(sockfd, limitname);
				if(ret == SUCCESS)
				{
					printf("become success \n");
				}	
				break;
			case 10:
				printf("please input 'quit' to retuen previous screen \n");
				ret = BeNormal(sockfd);
				if(ret == SUCCESS)
				{
					printf("become success \n");
				}

				break;

			case 11:
				ret = CheckLimit();
				if(ret == TRUE)
				{
					break;
				}
				else
				{
					printf("please input 'quit' to retuen previous screen \n");
				}
				
				ret = SendFile(sockfd);
				if(ret == SUCCESS)
				{
					printf("send file finish \n");
				}
				break;

		}	
	}
}


int SendFile(int fd)
{
	char filename[20];
    int openFd;
    ssize_t numRead;

	memset(&loginInfo.buf, 0, sizeof(loginInfo.buf));
	strcpy(loginInfo.buf, "sendfile");

	printf("please choose friend you want to sendfile \n");
	scanf("%s", loginInfo.toname);
	printf("please choose file you want send 		\n");
	scanf("%s", filename);
    openFd = open(filename, O_RDONLY);
    while ((numRead = read(openFd, loginInfo.mesg, 64)) > 0)
	{
		loginInfo.size = numRead;
		ret = send(sockfd, &loginInfo, sizeof(loginInfo), 0);
		if(ret == -1)
		{
			perror("send");
		}
		else
		{
			printf("send success \n");
		}
	}
	close(openFd);

	while(1)
	{
		scanf("%s", loginInfo.mesg);
		t = time(NULL);
		fprintf(fp, "sned to %s time %s ", loginInfo.toname, ctime(&t));
		fprintf(fp, "mesg %s \n", loginInfo.mesg);
		
		if(strcmp(loginInfo.mesg, "quit") == 0)
		{
			printf("recv quit \n");
			return SUCCESS;
		}
	}
}


int CheckLimit()
{
	if(strcmp(user_level, "limit") == 0)
	{
		printf("you are be limited , don't have this function \n");
		printf("please input 'quit' to retuen previous screen \n");
		while(1)
		{
			scanf("%s", buffer);
			if(strcmp(buffer, "quit") == 0)
			{
				return TRUE;
			}
			else
			{
				printf("please input 'quit' to retuen previous screen \n");
			}
		}
	}
	else
	{
		return SUCCESS;
	}
}



int Limitsb(int sockfd, char *name)
{
	Log TempInfo;
	memset(&loginInfo.buf, 0, sizeof(loginInfo.buf));
	
	strcpy(loginInfo.buf, "limitsb");
	strcpy(loginInfo.lmname, name);
	ret = send(sockfd, &loginInfo, sizeof(loginInfo), 0);
	if(ret == -1)
	{
		perror("send");
	}

	ret = recv(sockfd, &TempInfo, sizeof(TempInfo), 0);
	if(ret < 0)
	{
		perror("recv");
	}

	if(strcmp(TempInfo.buf, "success") == 0)
	{
		printf("you are admin , you can limit \n");
	}
	else 
	{
		printf("you are not admin , limited authority \n");
	}
	
	pthread_cond_signal(&cond);

	while(1)
	{
		scanf("%s", buffer);
		if(strcmp(buffer, "quit") == 0)
		{
			return SUCCESS;
		}
		else
		{
			printf("please input 'quit' to retuen previous screen \n");
		}
	}
}


int BeAdmin(int sockfd)
{
	strcpy(loginInfo.buf, "beadmin");
	
	ret = send(sockfd, &loginInfo, sizeof(loginInfo), 0);
	if(ret == -1)
	{
		perror("send");
	}

	while(1)
	{
		scanf("%s", buffer);
		if(strcmp(buffer, "quit") == 0)
		{
			return SUCCESS;
		}
		else
		{
			printf("please input 'quit' to retuen previous screen \n");
		}
	}
}


int BeNormal(int sockfd)
{
	strcpy(loginInfo.buf, "benormal");
	strcpy(loginInfo.level, "normal");
	
	ret = send(sockfd, &loginInfo, sizeof(loginInfo), 0);
	if(ret == -1)
	{
		perror("send");
	}

	while(1)
	{
		scanf("%s", buffer);
		if(strcmp(buffer, "quit") == 0)
		{
			return SUCCESS;
		}
		else
		{
			printf("please input 'quit' to retuen previous screen \n");
		}
	}
}


int ClearRecord()
{
	printf("clearing ... \n");
//	fp = fopen("./chat_records", "w");

//	fclose(fp);

	return SUCCESS;
}




int ChatRecords()
{
	printf("show .....\n");
	char record[1024];
	memset(record, 0, sizeof(record));
	int len;
	
	printf("show chatting records \n");
	fseek(fp , 0L , SEEK_END);
	len = ftell(fp);
	rewind(fp);
	fseek(fp , 0L , SEEK_SET);
	fread(record , 1 , len ,fp);
	printf("%s",record);
	while(1)
	{
		scanf("%s", buffer);
		if(strcmp(buffer, "quit") == 0)
		{
			return SUCCESS;
		}
		else
		{
			printf("please input 'quit' to retuen previous screen \n");
		}
	}
}


int EnterGroup(int sockfd)
{
	memset(buf, 0, sizeof(buf));
	memset(buffer, 0, sizeof(buffer));
	memset(&loginInfo.buf, 0, sizeof(loginInfo.buf));
	char rmname[32];
	char choise[10];
	strcpy(loginInfo.buf, "entergroup");
//	strcpy(loginInfo.level, "normal");
	printf("please input chat group name you want enter \n");
	scanf("%s", loginInfo.group);

	ret = send(sockfd, &loginInfo, sizeof(loginInfo), 0);
	if(ret == -1)
	{
		perror("send");
	}

	ret = recv(sockfd, buf, sizeof(buf), 0);
	if(ret < 0)
	{
		perror("recv");
	}

	//	pthread_cond_signal(&cond); 
	
	if(strcmp(buf, "success") == 0) 
	
	{
		while(1)
		{	
			pthread_cond_signal(&cond);
			system("clear");
			printf("hello user : %s         \n", loginInfo.name);
			printf("welcome to chatroom	%s  \n", loginInfo.group);
			printf("level: %s 				\n", user_level);
			printf("1.check online friend 	\n");
			printf("2.send mesg 			\n");
			printf("3.become admin 			\n");
			printf("4.kick someone			\n");
			printf("5.quit 					\n");
			scanf("%s",choise);	
			switch(atoi(choise))
			{
				case 1:
					printf("please input 'quit' to retuen previous screen \n");
					ret = RoomCheckon(sockfd);
					if(ret == SUCCESS)
					{
						printf("check online finish \n");
					}

					break;
			
				case 2:
					ret = CheckLimit();
					if(ret == TRUE)
					{
						break;
					}
					else
					{
						printf("please input 'quit' to retuen previous screen \n");
					}
				
					ret = RoomChat(sockfd);
					if(ret == SUCCESS)
					{
						printf("send mesg finish \n");
					}

					break;
			
				case 3:
					ret = CheckLimit();
					if(ret == TRUE)
					{
						break;
					}
					else
					{
						printf("please input 'quit' to retuen previous screen \n");
					}
				
					ret = RoomAdmin(sockfd);
					if(ret == SUCCESS)
					{
						printf("became success \n");
					}

					break;
			
				case 4:
					ret = CheckLimit();
					if(ret == TRUE)
					{
						break;
					}
					else
					{
						printf("please input 'quit' to retuen previous screen \n");
					}
				
					printf("please input name you want kick \n");
					scanf("%s",rmname);
					ret = RoomKick(sockfd, rmname);

					break;
			
				case 5:

					ret = RoomQuit(sockfd);
					IntoMyZone(sockfd);

					break;
				
			}
		}
	}
	else if(strcmp(buf, "false") == 0)
	{	
		pthread_cond_signal(&cond);
		printf("please in put right char room name \n");
		while(1)
		{
			scanf("%s", buffer);
			if(strcmp(buffer, "quit") == 0)
			{
				break;
			}
			else
			{
				printf("please input 'quit' to retuen previous screen \n");
			}
		}
	}

	return SUCCESS;
}


int RoomAdmin(int sockfd)
{
	memset(buf, 0, sizeof(buf));
	memset(buffer, 0, sizeof(buffer));
	memset(&loginInfo.buf, 0, sizeof(loginInfo.buf));
	//memset(&loginInfo.root, 0, sizeof(loginInfo.root));
	
	strcpy(loginInfo.buf, "roomadmin");
	//strcpy(loginInfo.root, "admin");
	ret = send(sockfd, &loginInfo, sizeof(loginInfo), 0);
	if(ret == -1)
	{
		perror("send");
	}
	
//	printf("you are admin now \n");
	while(1)
	{
		scanf("%s", buffer);
		if(strcmp(buffer, "quit") == 0)
		{
			return SUCCESS;
		}
		else
		{
			printf("please input 'quit' to retuen previous screen \n");
		}
	}
}


int RoomKick(int sockfd, char *name)
{
	memset(buf, 0, sizeof(buf));
	memset(buffer, 0, sizeof(buffer));
	memset(&loginInfo.buf, 0, sizeof(loginInfo.buf));
	
	strcpy(loginInfo.buf, "roomkick");
	strcpy(loginInfo.rmname, name);
	ret = send(sockfd, &loginInfo, sizeof(loginInfo), 0);
	if(ret == -1)
	{
		perror("send");
	}

	ret = recv(sockfd, buf, sizeof(buf), 0);
	if(ret < 0)
	{
		perror("recv");
	}

	if(strcmp(buf, "success") == 0)
	{
		printf("you are admin , you can kick \n");
	}
	else 
	{
		printf("you are not admin , limited authority \n");
	}
	
	pthread_cond_signal(&cond);

	while(1)
	{
		scanf("%s", buffer);
		if(strcmp(buffer, "quit") == 0)
		{
			return SUCCESS;
		}
		else
		{
			printf("please input 'quit' to retuen previous screen \n");
		}
	}
}


int RoomQuit(int sockfd)
{
	strcpy(loginInfo.buf, "roomquit");
	ret = send(sockfd, &loginInfo, sizeof(loginInfo), 0);
	if(ret == -1)
	{
		perror("send");
	}

}


int RoomCheckon(int sockfd)
{
	memset(buf, 0, sizeof(buf));
	memset(buffer, 0, sizeof(buffer));
	memset(&loginInfo.buf, 0, sizeof(loginInfo.buf));
	strcpy(loginInfo.buf, "roomcheckon");	
	ret = send(sockfd, &loginInfo, sizeof(loginInfo), 0);
	if(ret == -1)
	{
		perror("send");
	}
	
	while(1)
	{
		scanf("%s", buffer);
		if(strcmp(buffer, "quit") == 0)
		{
			return SUCCESS;
		}
		else
		{
			printf("please input 'quit' to retuen previous screen \n");
		}
	}
}


int RoomChat(int sockfd)
{
	memset(&loginInfo.buf, 0, sizeof(loginInfo.buf));
	strcpy(loginInfo.buf, "roomchat");

	printf("please input message you want send \n");
	while(1)
	{
		scanf("%s", loginInfo.mesg);
		t = time(NULL);
		fprintf(fp, "sned charroom time %s ", ctime(&t));
		fprintf(fp, "mesg %s \n", loginInfo.mesg);
		
		if(strcmp(loginInfo.mesg, "quit") == 0)
		{
			return SUCCESS;
		}

		ret = send(sockfd, &loginInfo, sizeof(loginInfo), 0);
		if(ret == -1)
		{
			perror("send");
		}
	}
}


int CreatGroup(int sockfd)
{	
	int num[10];
	int	i = 0;
	
	memset(buf, 0, sizeof(buf));
	memset(buffer, 0, sizeof(buffer));
	memset(&loginInfo.buf, 0, sizeof(loginInfo.buf));
	
	strcpy(loginInfo.buf, "creatgroup");
	printf("please input group name you want creat \n");
	scanf("%s", loginInfo.group);
	//printf("please input member name you want add \n");
	//scanf("%s", loginInfo.addname);
	ret = send(sockfd, &loginInfo, sizeof(loginInfo), 0);
	if(ret == -1)
	{
		perror("send");
	}

	ret = recv(sockfd, buf, sizeof(buf), 0);
	if(ret < 0)
	{
		perror("recv");
	}

	if(strcmp(buf, "success") == 0)
	{
		printf("creat chat room %s success \n", loginInfo.group);
	}
	else
	{
		printf("chatroom has created \n");
	}
	
	while(1)
	{
		scanf("%s", buf);
		if(strcmp(buf, "quit") == 0)
		{
			return SUCCESS;
		}
		else
		{
			printf("please input 'quit' to retuen previous screen \n");
		}
	}
}


int GroupChat(int sockfd)
{
	memset(&loginInfo.buf, 0, sizeof(loginInfo.buf));
	strcpy(loginInfo.buf, "group");

	printf("please input message you want send \n");
	while(1)
	{
		scanf("%s", loginInfo.mesg);
		t = time(NULL);
		fprintf(fp, "sned group time %s ", ctime(&t));
		fprintf(fp, "mesg %s \n", loginInfo.mesg);
		
		if(strcmp(loginInfo.mesg, "quit") == 0)
		{
			return SUCCESS;
		}

		ret = send(sockfd, &loginInfo, sizeof(loginInfo), 0);
		if(ret == -1)
		{
			perror("send");
		}
	}
}


int PrivateChat(int sockfd)
{
	memset(&loginInfo.buf, 0, sizeof(loginInfo.buf));
	strcpy(loginInfo.buf, "private");

	printf("please choose friend you want to chat \n");
	scanf("%s", loginInfo.toname);
	printf("please input message you want send \n");
	while(1)
	{
		scanf("%s", loginInfo.mesg);
		t = time(NULL);
		fprintf(fp, "sned to %s time %s ", loginInfo.toname, ctime(&t));
		fprintf(fp, "mesg %s \n", loginInfo.mesg);
		
		if(strcmp(loginInfo.mesg, "quit") == 0)
		{
			printf("recv quit \n");
			return SUCCESS;
		}
		ret = send(sockfd, &loginInfo, sizeof(loginInfo), 0);
		if(ret == -1)
		{
			perror("send");
		}
	}
}


int CheckFriend(int sockfd)
{
	memset(buf, 0, sizeof(buf));
	memset(buffer, 0, sizeof(buffer));
	memset(&loginInfo.buf, 0, sizeof(loginInfo.buf));
	strcpy(loginInfo.buf, "checkon");

	ret = send(sockfd, &loginInfo, sizeof(loginInfo), 0);
	if(ret == -1)
	{
		perror("send");
	}
	
	while(1)
	{
		scanf("%s", buffer);
		if(strcmp(buffer, "quit") == 0)
		{
			return SUCCESS;
		}
		else
		{
			printf("please input 'quit' to retuen previous screen \n");
		}
	}
}










