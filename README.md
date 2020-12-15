# Final project:Instant messaging system

There are 2 parts of this project: server-side and client-side

1.The client is responsible for providing interfaces to users, sending requests to the server, and receiving messages from the server.So the server needs two threads, one is responsible for sending and the other is responsible for receiving

2.The server side needs to receive the client connection request at any time, and a separate thread is created when it does not receive a client connection request.

Service-Terminal:
1. Create a socket
sockfd = socket(PF_INET, SOCK_STREAM, 0);
The second parameter here is to create a socket stream for the TCP server
2. Binding
memset(&server_addr, 0, sizeof(server_addr));
server_addr.sin_family = PF_INET;
server_addr.sin_port = htons(PORT);
server_addr.sin_addr.s_addr = inet_addr("192.168.238.129");
bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
3. Monitor
listen(sockfd, 5);
4. Receive
fd[i] = accept(sockFd, (struct sockaddr *)&client_addr, &length);
5. Create thread
pthread_create(&tid, NULL, ClientHandler, &fd[i]);
6. Send/receive
RecvInfo and TempInfo are both structures, both server and client must be defined
recv(fd ,&RecvInfo, sizeof(RecvInfo), 0);
send(tofd, &TempInfo, sizeof(TempInfo), 0);

Client:
1. Create a socket
sockfd = socket(PF_INET, SOCK_STREAM, 0);
2. Connect
memset(&server_addr, 0, sizeof(server_addr));
server_addr.sin_family = PF_INET;
server_addr.sin_port = htons(PORT);
server_addr.sin_addr.s_addr = inet_addr("192.168.238.129");
connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
3. Receive and send
