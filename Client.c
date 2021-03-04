#include<stdio.h>
#include <netdb.h> 
#include <stdio.h> 
#include <arpa/inet.h>
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include<pthread.h>
#include <unistd.h>
#define MAX 100
#define SockAdd struct sockaddr
#define PORT 8080
#define max_thread 3000

int mainSock;
struct sockaddr_in servInfo;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;	


void func(int mainSock){
	char msg[300];
	char buf[300];

	bzero(buf,sizeof(buf));
	puts("\n------------------------  Listening from client side -------------- \n");
	read(mainSock,buf,sizeof(buf));
	
	printf("Server message ---->> %s",buf);
}


void *mulClient(void *vargp){
	pthread_mutex_lock(&lock);
	mainSock= socket(AF_INET,SOCK_STREAM,0);

	if(mainSock ==-1){
		printf("\n Main Socket Failed \n");
	}else{
		printf("\n Main Socket Success \n");
	}

	bzero(&servInfo, sizeof(servInfo));
	
	servInfo.sin_family = AF_INET; 
	//servInfo.sin_addr.s_addr = INADDR_ANY; 
	//servInfo.sin_addr.s_addr = "10.0.2.15"; 
	servInfo.sin_port = htons(PORT); 

	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, "10.0.2.15", &servInfo.sin_addr)<=0)  { 
		printf("\nInvalid address/ Address not supported \n"); 
		exit(0);	
	} 


	if(connect(mainSock,(SockAdd*)&servInfo,sizeof(servInfo))!=0){
		puts("\n Inside Clinet Connection Failed \n");
		exit(0);	
	}else{
		puts("\n Inside Client Connect \n");
		printf("mainSocket ==>> %d \n",mainSock);
	}

	func(mainSock);
	pthread_mutex_unlock(&lock);

}

int main(){
	int i;
	pthread_t thread_id[max_thread];

	for(i=0;i<max_thread;i++){
		if((pthread_create(&thread_id[i],NULL,mulClient,NULL))!=0)
			printf("\n Thread creation failed at ============ >>>>> %d \n ",i);
		else 
			printf("\n Thread successfully created at =======>> %d\n",i);
	}

	for(i=0;i<max_thread;i++){		
		pthread_join(thread_id[i],NULL);
		printf("\n after thread_id[%d] joining \n",i);
	}

	return 0;
}
