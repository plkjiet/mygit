#include "server.h"

int ret;
extern Node *first;  	

extern GNode *group;	

char groupname[32] = {0};

char null[32] = {0};
char buf[10] = "false";
char buffer[10] = "success";
char buff[10] = "failure";



int ServerInit()
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
	ret = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if(ret < 0)
	{
		perror("bind");
		exit(1);
	}

	
	ret = listen(sockfd, 5);
	if(ret < 0)
	{
		perror("listen");
		exit(1);
	}

	return sockfd;
}


int ServerAccept(int sockFd)
{
	pthread_t tid;
	int fd[SIZE];
	int i = 0;
	int length = sizeof(client_addr);
	
	
	fd[i] = accept(sockFd, (struct sockaddr *)&client_addr, &length);	
	if(fd[i] == -1)
	{
		perror("accept");
	}
	else
	{
		printf("client connect success \n");
		printf("port is %d fd is %d \n", client_addr.sin_port, fd[i]);	
	}

	printf("creat pthread \n");
	
	ret = pthread_create(&tid, NULL, ClientHandler, &fd[i]);
	if(ret != 0)
	{
		perror("pthread_creat");
	}

	return SUCCESS;
}


void *ClientHandler(void *arg)
{
	Info1 RecvInfo;  
	int fd = *(int *)arg;
	char buf_name[10] = "name";
	char buf_id[10] = "id";
	char buf_passwd[10] = "passwd";

	
	pthread_detach(pthread_self());
	while(1)
	{
		
		ret = recv(fd ,&RecvInfo, sizeof(RecvInfo), 0);
		if(ret == -1)
		{
			perror("recv");
			exit(1);
		}
		
		
		if(strcmp(RecvInfo.buf, "register") == 0) 
		{
			printf("recv flag   : %s \n", RecvInfo.flag);
			printf("recv name   : %s \n", RecvInfo.name);
			printf("recv id     : %s \n", RecvInfo.id);
			printf("recv passwd : %s \n", RecvInfo.passwd);
			printf("recv buf    : %s \n", RecvInfo.buf);
			printf("recv toname : %s \n", RecvInfo.toname);
			printf("recv mesg   : %s \n", RecvInfo.mesg);
			printf("recv rmname : %s \n", RecvInfo.rmname);
			printf("recv group  : %s \n", RecvInfo.group);
			printf("recv level  : %s \n", RecvInfo.level);
			
			，
			if(RepetitionName(first, RecvInfo.name) == TRUE) 
			{
				send(fd, buf, sizeof(buf), 0);
				continue;
			}

			
			if(RepetitionId(first, RecvInfo.id) == TRUE)
			{
				send(fd, buf, sizeof(buf), 0);
				continue;
			}

			
			strcpy(RecvInfo.flag, "register"); 

			ret = DataWrite(RecvInfo, fd);  
			if(ret == SUCCESS)
			{
				printf("register info write in datebase success \n");
			}
			else
			{
				printf("register info write in datebase failure \n");
			}

			
			ret = LinkInsert(first, &RecvInfo, fd); 
			if(ret == SUCCESS)
			{
				printf("register info write in link success \n");
			}
			else
			{
				printf("register info write in link failure \n");
			}
			
			printf("this is link info \n");
			
			ret = LinkShow(first); 
			if(ret == SUCCESS)
			{
				printf("link show success \n");
			}
			else
			{
				printf("link show failure \n");
			}
			
			printf("this is data info \n");

			ret = DataShow(); 
			if(ret == SUCCESS)
			{
				printf("data show success \n");
			}
			else
			{
				printf("data show failure \n");
			}

			send(fd, buffer, sizeof(buffer), 0);
			
			memset(&RecvInfo, 0, sizeof(RecvInfo));
		}
 
		else if(strcmp(RecvInfo.buf, "login") == 0)  
		{
			printf("recv flag   : %s \n", RecvInfo.flag);
			printf("recv name   : %s \n", RecvInfo.name);
			printf("recv id     : %s \n", RecvInfo.id);
			printf("recv passwd : %s \n", RecvInfo.passwd);
			printf("recv buf    : %s \n", RecvInfo.buf);
			printf("recv toname : %s \n", RecvInfo.toname);
			printf("recv mesg   : %s \n", RecvInfo.mesg);
			printf("recv rmname : %s \n", RecvInfo.rmname);
			printf("recv group  : %s \n", RecvInfo.group);
			printf("recv level  : %s \n", RecvInfo.level);
			
			
			if(LoginCheck(first, RecvInfo.name, RecvInfo.id, RecvInfo.passwd) == TRUE && LoginCheckon(first, RecvInfo.name) == TRUE)
			{
				

				ret = SendLogin(first, RecvInfo.name, "success", fd);
				if(ret == SUCCESS)
				{
					printf("login success \n");
				}
				else
				{
					printf("login failure \n");
				}
				
				LinkChange(first, &RecvInfo, "login"); 
				DataChange(RecvInfo, "login");
				
				
				LinkChangeFd(first, &RecvInfo, fd);
				DataChangeFd(RecvInfo, fd);		
				
				
				ret = SendWelcome(first, RecvInfo.name);
				if(ret == SUCCESS)
				{
					printf("success \n");
				}
				else
				{
					printf("failure \n");
				}
			}
			else
			{	
				
				ret = SendLogin(first, RecvInfo.name, "failure", fd);
				if(ret == SUCCESS)
				{
					printf("login success \n");
				}
				else
				{
					printf("login failure \n");
				}
				/*ret = send(fd, buf, sizeof(buf), 0);
				if(ret == -1)
				{
					perror("send");
				}
				else
				{
					printf("send success \n");
				}*/
			}

		}

		else if(strcmp(RecvInfo.buf, "quit") == 0) 
		{
			printf("recv flag   : %s \n", RecvInfo.flag);
			printf("recv name   : %s \n", RecvInfo.name);
			printf("recv id     : %s \n", RecvInfo.id);
			printf("recv passwd : %s \n", RecvInfo.passwd);
			printf("recv buf    : %s \n", RecvInfo.buf);
			printf("recv toname : %s \n", RecvInfo.toname);
			printf("recv mesg   : %s \n", RecvInfo.mesg);
			printf("recv rmname : %s \n", RecvInfo.rmname);
			printf("recv group  : %s \n", RecvInfo.group);
			printf("recv level  : %s \n", RecvInfo.level);
			
			
			ret = SendQuit(first, RecvInfo.name);
			if(ret == SUCCESS)
			{
				printf("success \n");
			}
			else
			{
				printf("failure \n");
			}

			
			LinkChange(first, &RecvInfo, "quit");
			DataChange(RecvInfo, "quit");
			
			
			LinkChangeFd(first, &RecvInfo, 0);
			DataChangeFd(RecvInfo, 0);
			
			
			pthread_exit(0);
			
		}
		else if(strcmp(RecvInfo.buf, "checkon") == 0) 
		{

			printf("recv flag   : %s \n", RecvInfo.flag);
			printf("recv name   : %s \n", RecvInfo.name);
			printf("recv id     : %s \n", RecvInfo.id);
			printf("recv passwd : %s \n", RecvInfo.passwd);
			printf("recv buf    : %s \n", RecvInfo.buf);
			printf("recv toname : %s \n", RecvInfo.toname);
			printf("recv mesg   : %s \n", RecvInfo.mesg);
			printf("recv rmname : %s \n", RecvInfo.rmname);
			printf("recv group  : %s \n", RecvInfo.group);
			printf("recv level  : %s \n", RecvInfo.level);
		
			
			ret = CheckOn(first, "login", fd);
			if(ret == SUCCESS)
			{	
				printf("check frind success \n");
			}
			else
			{
				printf("check frind failure \n");
		
			}
		}

		else if(strcmp(RecvInfo.buf, "private") == 0) 
		{
			int tofd;
			printf("recv flag   : %s \n", RecvInfo.flag);
			printf("recv name   : %s \n", RecvInfo.name);
			printf("recv id     : %s \n", RecvInfo.id);
			printf("recv passwd : %s \n", RecvInfo.passwd);
			printf("recv buf    : %s \n", RecvInfo.buf);
			printf("recv toname : %s \n", RecvInfo.toname);
			printf("recv mesg   : %s \n", RecvInfo.mesg);
			printf("recv rmname : %s \n", RecvInfo.rmname);
			printf("recv group  : %s \n", RecvInfo.group);
			printf("recv level  : %s \n", RecvInfo.level);
	
			LinkShow(first);

			
			tofd = GetFd(first, RecvInfo.toname);
			if(tofd != FAILURE)
			{
				printf("get fd success : %d \n", tofd);
				
				ret = send(tofd, &RecvInfo, sizeof(RecvInfo), 0);
				if(ret == -1)
				{
					perror("send");
				}
				else
				{
					printf("send success \n");
				}
			}
			else
			{
				
				strcpy(RecvInfo.buf, "privatefalse");
				tofd = GetFd(first, RecvInfo.name);
				ret = send(tofd, &RecvInfo, sizeof(RecvInfo), 0);
				if(ret == -1)
				{
					perror("send");
				}
				else
				{
					printf("send success \n");
				}
				printf("failure \n");
			}
		}
		else if(strcmp(RecvInfo.buf, "group") == 0) 
		{
			char *name;
			printf("recv flag   : %s \n", RecvInfo.flag);
			printf("recv name   : %s \n", RecvInfo.name);
			printf("recv id     : %s \n", RecvInfo.id);
			printf("recv buf    : %s \n", RecvInfo.buf);
			printf("recv toname : %s \n", RecvInfo.toname);
			printf("recv mesg   : %s \n", RecvInfo.mesg);
			printf("recv rmname : %s \n", RecvInfo.rmname);
			printf("recv group  : %s \n", RecvInfo.group);
			printf("recv level  : %s \n", RecvInfo.level);
			LinkShow(first);

			
			ret = GroupChat(first, "login", RecvInfo.name, RecvInfo.mesg);
			if(ret == SUCCESS)
			{
				printf("send success \n");
			}
			else
			{
				printf("send failure \n");
			}
		}
		else if(strcmp(RecvInfo.buf, "creatgroup") == 0) 
		{
			Info1 TempInfo;
			
			strcpy(TempInfo.buf, "wait");		
			ret = send(fd, &TempInfo, sizeof(TempInfo), 0);
			if(ret == -1)
			{
				printf("send wait failure \n");
			}
			else
			{
				printf("send wait success \n");
			}
			
			printf("recv flag   : %s \n", RecvInfo.flag);
			printf("recv name   : %s \n", RecvInfo.name);
			printf("recv buf    : %s \n", RecvInfo.buf);
			printf("recv toname : %s \n", RecvInfo.toname);
			printf("recv mesg   : %s \n", RecvInfo.mesg);
			printf("recv rmname : %s \n", RecvInfo.rmname);
			printf("recv group  : %s \n", RecvInfo.group);
			printf("recv level  : %s \n", RecvInfo.level);
			
			
			if(strcmp(groupname, null) == 0)
			{
				
				strcpy(groupname, RecvInfo.group);
				printf("group name is : %s\n", groupname);
				
				send(fd, buffer, sizeof(buffer), 0);
				if(ret == -1)
				{
					perror("send");
				}
				else
				{
					printf("send success \n");
				}
				
				ret = GroupLinkInit(&group);  
				if(ret == FAILURE)
				{
					printf("Group Link init failure \n");
				}		
				else
				{
					printf("Group Link init success \n");
				}
			
				ret = GroupLinkShow(group);
				if(ret = SUCCESS)
				{
					printf("this is group link list \n");
				}
				else
				{
					printf("show failure \n");
				}
			}
			else
			{
				
				send(fd, buf, sizeof(buf), 0);
				if(ret == -1)
				{
					perror("send");
				}
				else
				{
					printf("send success \n");
				}				
			}
		}
		else if(strcmp(RecvInfo.buf, "entergroup") == 0) 	
		{
			Info1 TempInfo;
			
			strcpy(TempInfo.buf, "wait");
			ret = send(fd, &TempInfo, sizeof(TempInfo), 0);
			if(ret == -1)
			{
				printf("send wait failure \n");
			}
			else
			{
				printf("send wait success \n");
			}

			printf("recv flag   : %s \n", RecvInfo.flag);
			printf("recv name   : %s \n", RecvInfo.name);
			printf("recv buf    : %s \n", RecvInfo.buf);
			printf("recv toname : %s \n", RecvInfo.toname);
			printf("recv mesg   : %s \n", RecvInfo.mesg);
			printf("recv rmname : %s \n", RecvInfo.rmname);
			printf("recv group  : %s \n", RecvInfo.group);
			printf("recv level  : %s \n", RecvInfo.level);

			
			if(strcmp(groupname, RecvInfo.group) == 0) 
			{
				
				send(fd, buffer, sizeof(buffer), 0);
				if(ret == -1)
				{
					perror("send");
				}
				else
				{
					printf("send success \n");
				}

				
				ret = GroupLinkInsert(group, &RecvInfo, fd, "normal");
				if(ret == SUCCESS)
				{
					printf("group info write in link success \n");
				}
				else
				{
					printf("group info write in link failure \n");
				}
				
				
				ret = GroupLinkShow(group);
				if(ret = SUCCESS)
				{
					printf("this is group link list \n");
				}
				else
				{
					printf("show failure \n");
				}
			}
			else
			{
				
				send(fd, buf, sizeof(buf), 0);
				if(ret == -1)
				{
					perror("send");
				}
			
			}
		}
		else if(strcmp(RecvInfo.buf, "roomcheckon") == 0) 
		{
			printf("recv flag   : %s \n", RecvInfo.flag);
			printf("recv name   : %s \n", RecvInfo.name);
			printf("recv buf    : %s \n", RecvInfo.buf);
			printf("recv toname : %s \n", RecvInfo.toname);
			printf("recv mesg   : %s \n", RecvInfo.mesg);
			printf("recv rmname : %s \n", RecvInfo.rmname);
			printf("recv group  : %s \n", RecvInfo.group);
			printf("recv level  : %s \n", RecvInfo.level);
			
			
			ret = RoomCheckon(group, "online", fd);
			if(ret = SUCCESS)
			{
				printf("chekc on success \n");
			}
			else
			{
				printf("check on failure \n");
			}

		}
		else if(strcmp(RecvInfo.buf, "roomchat") == 0) 
		{
			printf("recv flag   : %s \n", RecvInfo.flag);
			printf("recv name   : %s \n", RecvInfo.name);
			printf("recv buf    : %s \n", RecvInfo.buf);
			printf("recv toname : %s \n", RecvInfo.toname);
			printf("recv mesg   : %s \n", RecvInfo.mesg);
			printf("recv rmname : %s \n", RecvInfo.rmname);
			printf("recv group  : %s \n", RecvInfo.group);
			printf("recv level  : %s \n", RecvInfo.level);
		
			
			ret = RoomChat(group, "online", RecvInfo.name, RecvInfo.group, RecvInfo.mesg);
			if(ret == SUCCESS)
			{
				printf("send success \n");
			}
			else
			{
				printf("send failure \n");
			}

		}
		else if(strcmp(RecvInfo.buf, "roomkick") == 0) 
		{
			Info1 TempInfo;
			int tofd;
			
			strcpy(TempInfo.buf, "wait");		
			ret = send(fd, &TempInfo, sizeof(TempInfo), 0);
			if(ret == -1)
			{
				printf("send wait failure \n");
			}
			else
			{
				printf("send wait success \n");
			}
			
			printf("recv flag   : %s \n", RecvInfo.flag);
			printf("recv name   : %s \n", RecvInfo.name);
			printf("recv buf    : %s \n", RecvInfo.buf);
			printf("recv toname : %s \n", RecvInfo.toname);
			printf("recv mesg   : %s \n", RecvInfo.mesg);
			printf("recv rmname : %s \n", RecvInfo.rmname);
			printf("recv group  : %s \n", RecvInfo.group);
			printf("recv level  : %s \n", RecvInfo.level);
			
			
			ret = CkeckAdmin(group, RecvInfo.name); 
			if(ret == TRUE)
			{
				printf("this is admin \n");
				
				
				send(fd, buffer, sizeof(buffer), 0);
				if(ret == -1)
				{
					perror("send");
				}
				else
				{
					printf("send success \n");
				}

				
				tofd = GetFd(first, RecvInfo.rmname);
				if(tofd != FAILURE)
				{
					
					ret = GroupLinkFlag(group, RecvInfo.rmname, "quit");
					if(ret == SUCCESS)
					{
						printf("quit success \n");
					}

					printf("get fd success %d \n", tofd);
					
					ret = send(tofd, &RecvInfo, sizeof(RecvInfo), 0);
					if(ret == -1)
					{
						perror("send");
					}
					else
					{
						printf("send success \n");
					}
				}
				else
				{
					printf("failure \n");
				}

			}
			else
			{
				printf("this is not admin \n");
				
				send(fd, buf, sizeof(buf), 0);
				if(ret == -1)
				{
					perror("send");
				}
				else
				{
					printf("send success \n");
				}
			}

			ret = GroupLinkShow(group);
			if(ret = SUCCESS)
			{
				printf("this is group link list \n");
			}
			else
			{
				printf("show failure \n");
			}

		}
		else if(strcmp(RecvInfo.buf, "roomadmin") == 0) 
		{
			printf("recv flag   : %s \n", RecvInfo.flag);
			printf("recv name   : %s \n", RecvInfo.name);
			printf("recv buf    : %s \n", RecvInfo.buf);
			printf("recv toname : %s \n", RecvInfo.toname);
			printf("recv mesg   : %s \n", RecvInfo.mesg);
			printf("recv rmname : %s \n", RecvInfo.rmname);
			printf("recv group  : %s \n", RecvInfo.group);
			printf("recv level  : %s \n", RecvInfo.level);
			
			
			ret = CheckNormal(group, RecvInfo.name);
			
			if(ret == TRUE)
			{
				printf("not admin is normal\n");
				ret = GroupLinkRoot(group, RecvInfo.name, "admin");
				if(ret == SUCCESS)
				{
					printf("become admin success \n");
				}
			}
			
			else
			{
				printf("is admin or limit \n");
				ret = send(fd, &RecvInfo, sizeof(RecvInfo), 0);
				if(ret == -1)
				{
					perror("send");
				}
				else
				{
					printf("send success 11 \n");
				}
				continue;
			}
		
		
			ret = GroupLinkShow(group);
			if(ret = SUCCESS)
			{
				printf("this is group link list \n");
			}
			else
			{
				printf("show failure \n");
			}
		

		}
		else if(strcmp(RecvInfo.buf, "roomquit") == 0) 
		{
			printf("recv flag   : %s \n", RecvInfo.flag);
			printf("recv name   : %s \n", RecvInfo.name);
			printf("recv buf    : %s \n", RecvInfo.buf);
			printf("recv toname : %s \n", RecvInfo.toname);
			printf("recv mesg   : %s \n", RecvInfo.mesg);
			printf("recv rmname : %s \n", RecvInfo.rmname);
			printf("recv group  : %s \n", RecvInfo.group);
			printf("recv level  : %s \n", RecvInfo.level);
 			
			
			ret = GroupLinkFlag(group, RecvInfo.name, "quit");
			if(ret == SUCCESS)
			{
				printf("quit success \n");
			}

			ret = GroupLinkShow(group);
			if(ret = SUCCESS)
			{
				printf("this is group link list \n");
			}
			else
			{
				printf("show failure \n");
			}
			
		}
		else if(strcmp(RecvInfo.buf, "beadmin") == 0) 
		{
			printf("recv flag   : %s \n", RecvInfo.flag);
			printf("recv name   : %s \n", RecvInfo.name);
			printf("recv buf    : %s \n", RecvInfo.buf);
			printf("recv toname : %s \n", RecvInfo.toname);
			printf("recv mesg   : %s \n", RecvInfo.mesg);
			printf("recv rmname : %s \n", RecvInfo.rmname);
			printf("recv group  : %s \n", RecvInfo.group);
			printf("recv level  : %s \n", RecvInfo.level);
		
			ret = LinkShow(first); 
			if(ret == SUCCESS)
			{
				printf("link show success \n");
			}
			else
			{
				printf("link show failure \n");
			}
			
			printf("this is data info \n");

			ret = DataShow(); 
			if(ret == SUCCESS)
			{
				printf("data show success \n");
			}
			else
			{
				printf("data show failure \n");
			}

			
			ret = CheckNormal(first, RecvInfo.name);
			
			if(ret == TRUE)
			{
				printf(" is normal\n");
				ret = DataLevel(RecvInfo, "admin");
				if(ret == SUCCESS)
				{
					printf("data change success \n");
				}
				else
				{
					printf("data change failure \n");
				}
		
				ret = ChangeLevel(first, RecvInfo.name, "admin");
				if(ret == SUCCESS)
				{
					printf("become admin \n");
					Info1 tempinfo;
					strcpy(tempinfo.buf, "chlevel");
					ret = send(fd, &tempinfo, sizeof(tempinfo), 0);
					if(ret == -1)
					{
						perror("send");
					}
					else
					{
						printf("send success 11 \n");
					}
				}
				else
				{
					printf("failure  \n");
				}
			}
			
			else
			{
				printf("is admin or limit \n");
				ret = send(fd, &RecvInfo, sizeof(RecvInfo), 0);
				if(ret == -1)
				{
					perror("send");
				}
				else
				{
					printf("send success 11 \n");
				}
				continue;
			}
		

		}
		else if(strcmp(RecvInfo.buf, "limitsb") == 0) 
		{
			Info1 TempInfo;
			Log tempinfo;
			int tofd;
			
			strcpy(TempInfo.buf, "wait");		
			ret = send(fd, &TempInfo, sizeof(TempInfo), 0);
			if(ret == -1)
			{
				printf("send wait failure \n");
			}
			else
			{
				printf("send wait success \n");
			}
			
			printf("recv flag   : %s \n", RecvInfo.flag);
			printf("recv name   : %s \n", RecvInfo.name);
			printf("recv buf    : %s \n", RecvInfo.buf);
			printf("recv toname : %s \n", RecvInfo.toname);
			printf("recv mesg   : %s \n", RecvInfo.mesg);
			printf("recv rmname : %s \n", RecvInfo.rmname);
			printf("recv group  : %s \n", RecvInfo.group);
			printf("recv level  : %s \n", RecvInfo.level);
			printf("recv limitsb: %s \n", RecvInfo.lmname);
	
			
			ret = CheckAdmin(first, RecvInfo.name);
			if(ret == TRUE)
			{
				printf("you are admin  you can limit \n");
				ret = ChangeLimit(first, RecvInfo.lmname);
				if(ret == TRUE)
				{
					strcpy(tempinfo.buf, "success");
					ret = send(fd, &tempinfo, sizeof(tempinfo), 0);
					if(ret == -1)
					{
						printf("send limit failure \n");
					}
					else
					{
						printf("send limit success \n");
					}

					tofd = GetFd(first, RecvInfo.lmname);
					strcpy(TempInfo.buf, "limited");		
					ret = send(tofd, &TempInfo, sizeof(TempInfo), 0);
					if(ret == -1)
					{
						printf("send limit ed failure \n");
					}
					else
					{
						printf("send limit ed success \n");
					}
					
				}
				else
				{
					strcpy(tempinfo.buf, "failure");
					ret = send(fd, &tempinfo, sizeof(tempinfo), 0);
					if(ret == -1)
					{
						printf("send limit failure \n");
					}
					else
					{
						printf("send limit success \n");
					}
				
				}
			}
			else
			{
				printf("you are not admin  you can not limit \n");
				strcpy(tempinfo.buf, "failure");
				ret = send(fd, &tempinfo, sizeof(tempinfo), 0);
				if(ret == -1)
				{
					printf("send limit failure \n");
				}
				else
				{
					printf("send limit success \n");
				}
			
			}

		}
		else if(strcmp(RecvInfo.buf, "benormal") == 0) 
		{
			printf("recv flag   : %s \n", RecvInfo.flag);
			printf("recv name   : %s \n", RecvInfo.name);
			printf("recv buf    : %s \n", RecvInfo.buf);
			printf("recv toname : %s \n", RecvInfo.toname);
			printf("recv mesg   : %s \n", RecvInfo.mesg);
			printf("recv rmname : %s \n", RecvInfo.rmname);
			printf("recv group  : %s \n", RecvInfo.group);
			printf("recv level  : %s \n", RecvInfo.level);
		
			
			ret = CheckAdmin(first, RecvInfo.name);
			if(ret == TRUE)
			{
				printf(" is admin\n");
				ret = DataLevel(RecvInfo, "normal");
				if(ret == SUCCESS)
				{
					printf("data change success \n");
				}
				else
				{
					printf("data change failure \n");
				}
				ret = ChangeLevel(first, RecvInfo.name, "normal");
				if(ret == SUCCESS)
				{
					printf("become normal \n");
					Info1 tempinfo;
					strcpy(tempinfo.buf, "chnormal");
					ret = send(fd, &tempinfo, sizeof(tempinfo), 0);
					if(ret == -1)
					{
						perror("send");
					}
					else
					{
						printf("send success 11 \n");
					}
				}
				else
				{
					printf("failure  \n");
				}
			}
			else
			{
				printf("is normal or limit \n");
				ret = send(fd, &RecvInfo, sizeof(RecvInfo), 0);
				if(ret == -1)
				{
					perror("send");
				}
				else
				{
					printf("send success 11 \n");
				}
				continue;
			}
		}
		else if(strcmp(RecvInfo.buf, "sendfile") == 0) 
		{
			int tofd;
			printf("recv flag   : %s \n", RecvInfo.flag);
			printf("recv name   : %s \n", RecvInfo.name);
			printf("recv id     : %s \n", RecvInfo.id);
			printf("recv passwd : %s \n", RecvInfo.passwd);
			printf("recv buf    : %s \n", RecvInfo.buf);
			printf("recv toname : %s \n", RecvInfo.toname);
			printf("recv mesg   : %s \n", RecvInfo.mesg);
			printf("recv rmname : %s \n", RecvInfo.rmname);
			printf("recv group  : %s \n", RecvInfo.group);
			printf("recv level  : %s \n", RecvInfo.level);
			printf("recv size   : %d \n", RecvInfo.size);

			LinkShow(first);

			
			tofd = GetFd(first, RecvInfo.toname);
			if(tofd != FAILURE)
			{
				printf("get fd success : %d \n", tofd);
				ret = send(tofd, &RecvInfo, sizeof(RecvInfo), 0);
				if(ret == -1)
				{
					perror("send");
				}
				else
				{
					printf("send success \n");
				}
			}
			
			else
			{
				strcpy(RecvInfo.buf, "filefalse");
				//tofd = GetFd(first, RecvInfo.name);
				ret = send(fd, &RecvInfo, sizeof(RecvInfo), 0);
				if(ret == -1)
				{
					perror("send");
				}
				else
				{
					printf("send success \n");
				}
				printf("failure \n");
			}
		}
		else 
		{
			
		}


	}
}


int DataLevel(Info1 ChangeData, char *level)
{
	char sql[1024] = {0};

	sprintf(sql, "update Users set level = '%s' where name = '%s';", level, ChangeData.name);
	ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
	if(ret != SQLITE_OK)
	{
		printf("sqlite3_exec error : %s \n",sqlite3_errmsg(ppdb));
		exit(1);
	}
	return SUCCESS;
}


int SendLogin(Node *l, char *name, char *buf, int tofd)
{
	Log templogin;
	strcpy(templogin.buf, buf);

	if(l == NULL) 
	{
		return FAILURE;
	}

	if(buf == NULL) 
	{
		return FAILURE;
	}
	
	if(name == NULL) 
	{
		return FAILURE;
	}

	Node *p = l;

	printf("in func login  \n");

	while(p->next != NULL)
	{
		p = p->next;
		if(strcmp(p->info->name, name) == 0) 
		{
			strcpy(templogin.level, p->info->level);
			ret = send(tofd, &templogin, sizeof(templogin), 0);
			if(ret == -1)
			{
				printf("sned false \n");
			}
			else
			{
				printf("send success \n");
			}
		}
	}
	return SUCCESS;
}


int CheckAdmin(Node *l, char *name)
{
	if(l == NULL) 
	{
		return FAILURE;
	}

	if(name == NULL) 
	{
		return FAILURE;
	}

	Node *p = l;

	printf("in checknn \n");

	while(p->next != NULL)
	{
		p = p->next;
		if((strcmp(p->info->name, name) == 0) && (strcmp(p->info->level, "admin") == 0))
		{
			printf("name is %s \n", p->info->name);
			printf("level is %s \n", p->info->level);
			return TRUE;
		}
		else
		{
			printf("name is %s \n", p->info->name);
			printf("level is %s \n", p->info->level);
		}
	/*	if(strcmp(p->info->name, name) == 0) 
		{
			if(strcmp(p->info->level, "admin") == 0)
			{
				return TRUE;
			}
			else
			{
				return SUCCESS;
			}
		}*/
	}
	
	return SUCCESS;
}


int CheckNormal(Node *l, char *name)
{
	if(l == NULL) 
	{
		return FAILURE;
	}

	if(name == NULL) 
	{
		return FAILURE;
	}

	Node *p = l;

	printf("in check normal \n");
	printf("name is %s \n", name);

	while(p->next != NULL)
	{
		p = p->next;
		if((strcmp(p->info->name, name) == 0) && (strcmp(p->info->level, "normal") == 0))
		{
			printf("name is %s \n", p->info->name);
			printf("level is %s \n", p->info->level);
			return TRUE;
		}
		else
		{
			printf("name is %s \n", p->info->name);
			printf("level is %s \n", p->info->level);
			//return SUCCESS;
		}
	}

	return SUCCESS;
}


int ChangeLimit(Node *l, char *name)
{
	if(l == NULL) 
	{
		return FAILURE;
	}

	if(name == NULL) 
	{
		return FAILURE;
	}

	Node *p = l;

	printf("in change \n");

	while(p->next != NULL)
	{
		p = p->next;
		if(strcmp(p->info->name, name) == 0)  
		{
			if(strcmp(p->info->level, "normal") == 0)
			{
				strcpy(p->info->level, "limit");
				return TRUE;
			}
			else
			{
				return SUCCESS;
			}
		}
	}

	return FAILURE;
}

int ChangeLevel(Node *l, char *name, char *level)
{
	if(l == NULL) 
	{
		return FAILURE;
	}

	if(name == NULL) 
	{
		return FAILURE;
	}

	Node *p = l;

	printf("in change \n");

	while(p->next != NULL)
	{
		p = p->next;
		if(strcmp(p->info->name, name) == 0) 
		{
			strcpy(p->info->level, level);
		}
	}

	return SUCCESS;
}



int SendQuit(Node *l, char *name)
{
	printf("send quit \n");
	Info1 TempInfo;
	strcpy(TempInfo.buf, "quitsend");
	strcpy(TempInfo.name, name);
	if(l == NULL) 
	{
		printf("l null \n");
		return FAILURE;
	}
	
	Node *p = l;
	int tofd;
	while(p->next != NULL)  
	{
		p = p->next;
		
		if((strcmp(p->info->flag, "login") == 0) && (strcmp(p->info->name, name) != 0))
		{
			tofd = GetFd(first, p->info->name);
			ret = send(tofd, &TempInfo, sizeof(TempInfo), 0);
			if(ret == -1)
			{
				perror("send");
			}
			else
			{
				printf("success \n");
			}
		}
	}

	return SUCCESS;
}


int SendWelcome(Node *l, char *name)
{
	Info1 TempInfo;
	strcpy(TempInfo.buf, "welcome");
	strcpy(TempInfo.name, name);
	if(l == NULL) 
	{
		printf("l null \n");
		return FAILURE;
	}
	
	Node *p = l;
	int tofd;
	while(p->next != NULL)
	{
		p = p->next;
		
		if((strcmp(p->info->flag, "login") == 0) && (strcmp(p->info->name, name) != 0))
		{
			tofd = GetFd(first, p->info->name);
			ret = send(tofd, &TempInfo, sizeof(TempInfo), 0);
			if(ret == -1)
			{
				perror("send");
			}
			else
			{
				printf("success \n");
			}
		}
	}

	return SUCCESS;
}


int GroupLinkFlag(GNode *l, char *name, char *flag)
{
	if(l == NULL) 
	{
		return FAILURE;
	}

	if(flag == NULL) 
	{
		return FAILURE;
	}
	
	if(name == NULL) 
	{
		return FAILURE;
	}

	GNode *p = l;

	printf("in flag \n");
	while(p->next != NULL)
	{
		p = p->next;
		if(strcmp(p->info->name, name) == 0)
		{
			strcpy(p->info->flag, flag);
			//return SUCCESS;
		}
	}

	return SUCCESS;
}


int GroupLinkRoot(GNode *l, char *name, char *root)
{
	if((l == NULL) || (root == NULL))
	{
		return FAILURE;
	}
	GNode *p = l;

	printf("in root \n");
	while(p->next != NULL)
	{
		p = p->next;
		if(strcmp(p->info->name, name) == 0)
		{
			strcpy(p->info->level, root);
			return SUCCESS;
		}
	}

	return SUCCESS;
}



int RoomKick(GNode *l, char *name, int tofd)
{
	Info1 TempInfo;
	GNode *p = l;

	printf("name is %s \n", name);
	printf("tofd is %d \n", tofd);
	printf("in kick \n");
	int ToFd;
	if(l == NULL || name == NULL)
	{
		printf("null \n");
		return FAILURE;
	}
 
	while(p->next != NULL)
	{
		printf("in while \n");
		p = p->next;
		if(strcmp(p->info->name, name) == 0)
		{
			printf("in if \n");
			strcpy(p->info->flag, "quit");
			printf("in if  2\n");
			strcpy(TempInfo.buf, "roomquit");
			ret = send(tofd, &TempInfo, sizeof(TempInfo), 0);
			if(ret == -1)
			{
				printf("send failure \n");
			}
			else
			{
				printf("send success \n");
			}
		}
	}

	return SUCCESS;
}


int CkeckAdmin(GNode *l, char *name)
{
	if(l == NULL || name == NULL)
	{
		return FAILURE;
	}

	GNode *p = l;

	while(p->next != NULL)
	{
		p = p->next;
		if((strcmp(p->info->name, name) == 0 )&& (strcmp(p->info->level, "admin") == 0))
		{
			return TRUE;		
		}
	}

	return SUCCESS;
}


int RoomChat(GNode *l, char *flag, char *name, char *group, char *mesg)
{
	if(l == NULL || flag == NULL || NULL == mesg || group == NULL)
	{
		return FAILURE;
	}

	GNode *p = l;
	Info1 TempInfo;
	int TempFd;
	
	while(p->next != NULL)
	{
		p = p->next;
		if(strcmp(p->info->flag, flag) == 0)
		{
			strcpy(TempInfo.buf, "roomchat");
			strcpy(TempInfo.name, name);
			strcpy(TempInfo.group, group);
			strcpy(TempInfo.mesg, mesg);
			TempFd = p->info->fd;
			ret = send(TempFd, &TempInfo, sizeof(TempInfo), 0);
			if(ret == -1)
			{
				printf("send failure \n");
			}
			else
			{
				printf("send success \n");
			}

		}
	}
}


int GroupChat(Node *l, char *flag, char *name, char *mesg)
{
	if(l == NULL || flag == NULL || NULL == mesg)
	{
		return FAILURE;
	}

	Node *p = l;
	Info1 TempInfo;
	int TempFd;
	
	while(p->next != NULL)
	{
		p = p->next;
		if(strcmp(p->info->flag, flag) == 0)
		{
			strcpy(TempInfo.buf, "group");
			strcpy(TempInfo.name, name);
			strcpy(TempInfo.mesg, mesg);
			TempFd = p->info->fd;
			ret = send(TempFd, &TempInfo, sizeof(TempInfo), 0);
			if(ret == -1)
			{
				printf("send failure \n");
			}
			else
			{
				printf("send success \n");
			}
		
		}
	
	}

	return SUCCESS;
}


int GetFd(Node *l, char *name)
{
	if(l == NULL || name == NULL)
	{
		return FAILURE;
	}

	Node *p = l;

	while(p->next != NULL)
	{
		p = p->next;
		if((strcmp(p->info->name, name) == 0) && (strcmp(p->info->flag, "login") == 0))
		{
			return p->info->fd;
		}
	}
	return FAILURE;

}


int RoomCheckon(GNode *l, char *flag, int fd)
{
	Info1 TempInfo;
	if(NULL == l)
	{
		return FAILURE;
	}

	GNode *p = l;
	while(p->next != NULL)
	{
		p = p->next;
		if(strcmp(p->info->flag, flag) == 0)
		{	
			strcpy(TempInfo.buf, "roomcheckon");
			strcpy(TempInfo.name, p->info->name);
			printf("online name : %s \n", TempInfo.name);
			ret = send(fd, &TempInfo, sizeof(TempInfo), 0);
			if(ret == -1)
			{
				printf("send failure \n");
			}
			else
			{
				printf("send success \n");
			}
		
		}

	}

	return SUCCESS;
}


int CheckOn(Node *l, char *flag, int fd)
{
	Info1 TempInfo;
	if(NULL == l)
	{
		return FAILURE;
	}
	
	Node *p = l;

	while(p->next != NULL)
	{
		p = p->next;
		if(strcmp(p->info->flag, flag) == 0)
		{
			strcpy(TempInfo.buf, "checkon");
			strcpy(TempInfo.flag, p->info->flag);
			strcpy(TempInfo.name, p->info->name);
			strcpy(TempInfo.id, p->info->id);
			strcpy(TempInfo.passwd, p->info->passwd);
			printf("login name : %s \n", TempInfo.name);
			ret = send(fd, &TempInfo, sizeof(TempInfo), 0);
			if(ret == -1)
			{
				printf("send failure \n");
			}
			else
			{
				printf("send success \n");
			}
		}
	}
	
	return SUCCESS;
}


int LinkChangeFd(Node *l, Info1 *ChangeInfo, int fd)
{
	Node *p = l;

	if((l == NULL) || (ChangeInfo == NULL) )
	{
		return FAILURE;
	}

	while(p->next != NULL)
	{
		p = p->next;
		if(strcmp(p->info->name, ChangeInfo->name) == 0)
		{
			p->info->fd = fd;
			return SUCCESS;
		}
	}

	return SUCCESS;
}


int LinkChange(Node *l, Info1 *ChangeInfo, char *flag)
{
	Node *p = l;

	if((l == NULL) || (ChangeInfo == NULL) || (flag == NULL))
	{
		return FAILURE;
	}


	while(p->next != NULL)
	{
		p = p->next;
		if(strcmp(p->info->name, ChangeInfo->name) == 0)
		{
			strcpy(p->info->flag, flag);
			return SUCCESS;
		}
	}

	return SUCCESS;
}


int DataChangeFd(Info1 ChangeData, int fd)
{
	char sql[1024] = {0};

	sprintf(sql, "update Users set fd = '%d' where name = '%s';", fd, ChangeData.name);
	ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
	if(ret != SQLITE_OK)
	{
		printf("sqlite3_exec error : %s \n",sqlite3_errmsg(ppdb));
		exit(1);
	}
}


int DataChange(Info1 ChangeData, char *flag)
{
	char sql[1024] = {0};

	sprintf(sql, "update Users set flag = '%s' where name = '%s';", flag, ChangeData.name);
	ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
	if(ret != SQLITE_OK)
	{
		printf("sqlite3_exec error : %s \n",sqlite3_errmsg(ppdb));
		exit(1);
	}
}



int DataWrite(Info1 WriteInfo, int fd)
{
	char sql[1024] = {0};

	sprintf(sql, "create table if not exists Users (flag text, name text, id text, passwd text, fd integer, level text);");
	ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
	if(ret != SQLITE_OK)
	{
		printf("1.sqlite3_exec error : %s \n",sqlite3_errmsg(ppdb));
		return FAILURE;
	}

	sprintf(sql, "insert into Users (flag, name, id, passwd, fd, level) values('%s', '%s', '%s', '%s', '%d', '%s');"
			,WriteInfo.flag, WriteInfo.name, WriteInfo.id, WriteInfo.passwd, fd, WriteInfo.level);
	ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
	if(ret != SQLITE_OK)
	{
		printf("2.sqlite3_exec error : %s \n",sqlite3_errmsg(ppdb));
		return FAILURE;
	}

	return SUCCESS;
}
	

int DataShow()
{
	char sql[1024];
	sprintf(sql, "select * from Users;"); 
	ret = sqlite3_exec(ppdb, sql, show, NULL, NULL);
	if(ret != SQLITE_OK)
	{
		printf("3.sqlite3_exec error  : %s \n",sqlite3_errmsg(ppdb));
		return FAILURE;
	}
	return SUCCESS;
}


int LinkReadData()
{
	char sql[1024] = {0};
	if(link == NULL)
	{
		return FAILURE;
	}

	sprintf(sql, "select * from Users;");
	ret = sqlite3_exec(ppdb, sql, ReadData, NULL, NULL);
	if(ret != SQLITE_OK)
	{
		printf("4.sqlite3_exec error  : %s \n",sqlite3_errmsg(ppdb));
	}
	
	return SUCCESS;
}


int ReadData(void *para, int columnCount, char **columnValue, char **columnName)
{
	Info1 linkInfo;
	int fd;

	strcpy(linkInfo.flag, columnValue[0]);
	strcpy(linkInfo.name, columnValue[1]);
	strcpy(linkInfo.id, columnValue[2]);
	strcpy(linkInfo.passwd, columnValue[3]);
	fd = atoi(columnValue[4]);
	strcpy(linkInfo.level, columnValue[5]);

	LinkInsert(first, &linkInfo, fd);

	return 0;
}

int show(void *para, int columnCount, char **columnValue, char **columnName)
{
	int i;
	for(i = 0; i < columnCount; i++)
	{
		printf("%s = %s ",columnName[i], columnValue[i]);
	}
	printf("\n");

	return 0;
}


int GroupLinkInit(GNode **l)
{
	*l = (GNode *)malloc(sizeof(GNode));
	if(NULL == *l)
	{
		return FAILURE;
	}

	(*l)->info = NULL;
	(*l)->next = NULL;
	
	return SUCCESS;
}


int GroupLinkInsert(GNode *l, Info1 *InsertInfo, int fd, char *root)
{
	if(l == NULL)
	{
		return FAILURE;
	}
	
	if(InsertInfo == NULL)
	{
		return FAILURE;
	}

	GNode *p = l;
	while(p->next != NULL)
	{
		p = p->next;
	}
	GNode *n = (GNode *)malloc(sizeof(GNode));
	Info3 *info = (Info3 *)malloc(sizeof(Info3));

	p->next = n;

	strcpy(info->flag, "online");
	strcpy(info->name, InsertInfo->name);
	strcpy(info->level, root);
	info->fd = fd;

	n->info = info;
	n->next = NULL;

	return SUCCESS;
}


int GroupLinkShow(GNode *l)
{
	printf("in link show \n");
	if(NULL == l)
	{
		return FAILURE;
	}
	
	GNode *p = l;

	while(p->next != NULL)
	{
		p = p->next;
		printf("flag : %s name: %s fd: %d level: %s\n", p->info->flag, p->info->name, p->info->fd, p->info->level);
	}

	return SUCCESS;
}


int LinkInit(Node **l)
{
	*l = (Node *)malloc(sizeof(Node));
	if(NULL == *l)
	{
		return FAILURE;
	}

	(*l)->info = NULL;
	(*l)->next = NULL;
	
	return SUCCESS;
}


int LinkInsert(Node *l, Info1 *InsertInfo, int fd)
{
	if(l == NULL)
	{
		return FAILURE;
	}
	
	if(InsertInfo == NULL)
	{
		return FAILURE;
	}

	Node *p = l;
	while(p->next != NULL)
	{
		p = p->next;
	}
	Node *n = (Node *)malloc(sizeof(Node));
	Info2 *info = (Info2 *)malloc(sizeof(Info2));

	p->next = n;

	strcpy(info->flag, InsertInfo->flag);
	strcpy(info->name, InsertInfo->name);
	strcpy(info->id, InsertInfo->id);
	strcpy(info->passwd, InsertInfo->passwd);
	strcpy(info->level, InsertInfo->level);
	info->fd = fd;

	n->info = info;
	n->next = NULL;

	return SUCCESS;
}


int LinkShow(Node *l)
{
	printf("in link show \n");
	if(NULL == l)
	{
		return FAILURE;
	}
	
	Node *p = l;

	while(p->next != NULL)
	{
		p = p->next;
		printf("link info :%s %s %s %s %s fd:%d \n", p->info->flag, p->info->name, p->info->id, p->info->passwd, 
			p->info->level,	p->info->fd);
	}

	return SUCCESS;
}


int RepetitionName(Node *l, char *name)
{
	if(l == NULL || name == NULL)
	{
		return FAILURE;
	}

	Node *p = l;

	while(p->next != NULL)
	{
		p = p->next;
		if(strcmp(p->info->name, name) == 0)
		{
			return TRUE;
		}
	}
	return SUCCESS;
}


int RepetitionId(Node *l, char *id)
{
	if(l == NULL || id == NULL)
	{
		return FAILURE;
	}

	Node *p = l;

	while(p->next != NULL)
	{
		p = p->next;
		if(strcmp(p->info->id, id) == 0)
		{
			return TRUE;
		}
	}
	return SUCCESS;
}


int RepetitionPasswd(Node *l, char *passwd)
{
	if(l == NULL || passwd == NULL)
	{
		return FAILURE;
	}

	Node *p = l;

	while(p->next != NULL)
	{
		p = p->next;
		if(strcmp(p->info->passwd, passwd) == 0)
		{
			return TRUE;
		}
	}
	return SUCCESS;
}


int LoginCheck(Node *l, char *name, char *id, char *passwd)
{
	if(l == NULL || passwd == NULL || id == NULL || passwd == NULL)
	{
		return FAILURE;
	}

	Node *p = l;

	while(p->next != NULL)
	{
		p = p->next;
		if(strcmp(p->info->name, name) == 0) 
		{
			if(strcmp(p->info->id, id) == 0)
			{
				if(strcmp(p->info->passwd, passwd) == 0)
				{
					return TRUE;
				}
			}
		}
	}

	return SUCCESS;
}


int LoginCheckon(Node *l, char *name)
{
	if(l == NULL || name == NULL)
	{
		return FAILURE;
	}

	Node *p = l;

	while(p->next != NULL)
	{
		p = p->next;
		if(strcmp(p->info->name, name) == 0) 
		{
			if( (strcmp(p->info->flag, "register") == 0) || (strcmp(p->info->flag, "quit") == 0) )
			{
					return TRUE;
			}
		}
	}

	return SUCCESS;
}








