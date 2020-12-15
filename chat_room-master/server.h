#ifndef __SERVER_H
#define __SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/time.h>
#include <sqlite3.h>

#define PORT 		8888
#define FAILURE		10000
#define SUCCESS		10001
#define TRUE		10002
#define FALSE		10003
#define SIZE 		1000



struct sockaddr_in server_addr;
struct sockaddr_in client_addr;


struct login
{
	char buf[10];
	char level[16];
};
typedef struct login Log;




struct user_info
{
	char flag[16];   
	char buf[16]; 		
	char name[32];	
	char id[32];	
	char passwd[32];
	char toname[32];
	char mesg[64];	
	char rmname[32];
	char group[32];	
	char level[16];  
	char lmname[16]; 
	ssize_t size;
};
typedef struct user_info Info1;


struct link_info
{
	char flag[16];   
	char name[32];	
	char id[32];	
	char passwd[32];
	char level[16]; 
	int	 fd;		
};
typedef struct link_info Info2;


struct link_node
{
	struct link_info *info;
	struct link_node *next;	
};
typedef struct link_node Node;


struct group_info
{
	char flag[16];  
	char name[32];	
	char level[16];	
	int	 fd;		
};
typedef struct group_info Info3;


struct group_node
{
	struct group_info *info;
	struct group_node *next;
};
typedef struct group_node GNode;

sqlite3 *ppdb;

int ServerInit();
int ServerAccept(int sockFd);
void *ClientHandler(void *arg);
int LinkChange(Node *l, Info1 *ChangeInfo, char *flag);
int DataChange(Info1 ChangeData, char *flag);
int DataWrite(Info1 WriteInfo, int fd);
int DataShow();
int LinkReadData();
int ReadData(void *para, int columnCount, char **columnValue, char **columnName);
int show(void *para, int columnCount, char **columnValue, char **columnName);
int LinkInit(Node **l);
int LinkShow(Node *l);
int LinkInsert(Node *l, Info1 *InsertInfo, int fd);
int TraversFriend(Node *l, Info1 CheckInfo, char *flag);
int RepetitionName(Node *l, char *name);
int RepetitionId(Node *l, char *id);
int RepetitionPasswd(Node *l, char *passwd);
int LoginCheckon(Node *l, char *name);
int LoginCheck(Node *l, char *name, char *id, char *passwd);
int GroupLinkInit(GNode **l);
int GroupLinkShow(GNode *l);
int GroupLinkInsert(GNode *l, Info1 *InsertInfo, int fd, char *root);
int CheckOn(Node *l, char *flag, int fd);
int RoomCheckon(GNode *l, char *flag, int fd);
int GetFd(Node *l, char *name);
int GroupChat(Node *l, char *flag, char *name, char *mesg);
int RoomChat(GNode *l, char *flag, char *name, char *group, char *mesg);
int CkeckAdmin(GNode *l, char *name);
int RoomKick(GNode *l, char *name, int tofd);
int GroupLinkRoot(GNode *l, char *name, char *root);
int GroupLinkFlag(GNode *l, char *name, char *flag);
int SendWelcome(Node *l, char *name);
int SendQuit(Node *l, char *name);




#endif




