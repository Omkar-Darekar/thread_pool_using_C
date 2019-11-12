#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include<pthread.h>
#include<signal.h>

#define PORT 8887
#define	thread_limit 10

pthread_mutex_t lock= PTHREAD_MUTEX_INITIALIZER;
struct main_struct{
	int struct_index;
	char buffer[80];
	int struct_sock;
	int gFlag;
//	pthread_t tid[thread_limit];	
};

void *socketThread(struct main_struct *sock){
	
	printf("********Inside Thread Functio  ************* \t sock ->struct_sock = %d \t sock->gFlag=> %d \n\n ",sock->struct_sock,sock->gFlag);
//	usleep(200);

	while(1){
		if((sock->gFlag)>0 && (sock->struct_sock)>0){
			int index=sock->struct_index;
			int newSocket = sock->struct_sock;
			char sBuf[200];

			snprintf(sBuf,50," Current Socket -> %d \t ",newSocket);
			strcat(sBuf,sock->buffer);
			printf("\n index = %d \t newSocket= %d\t sBuf ==>>%s \t <--This thread came to function()\n",index,newSocket,sBuf);
			
			write(newSocket,sBuf,sizeof(sBuf));
			printf("\n Message sent successfully to : %d \n",newSocket);

			sock->gFlag=0;
			close(newSocket);
			sock->struct_sock=-1;
		}
	}
}

int main(){
	int master_socket, newSocket;
	struct sockaddr_in address;
	int k;
	struct main_struct obj[thread_limit];
	
	for(k=0;k<thread_limit;k++){
		strcpy(obj[k].buffer," I am new Message  \t");
		obj[k].gFlag=0;
		obj[k].struct_sock=-1;
	}

	//struct sockaddr_storage address;
	socklen_t addr_size;

	//Create the socket. 
	if((master_socket = socket(AF_INET, SOCK_STREAM, 0))==0){
		puts("\n Socket not properly created \n");
		puts("\n----- Check port number\nCheck IP address ---- \n");		 
		perror("\n -----Terminated ------\n"); 
		exit(0);
	}

	// Configure settings of the server address struct

	address.sin_family = AF_INET; // Address family = Internet 
	address.sin_port = htons(PORT);  //Set port number
	//	address.sin_addr.s_addr = inet_addr("127.0.0.1");//Set IP address to localhost 
	address.sin_addr.s_addr= INADDR_ANY;
	
	if(bind(master_socket, (struct sockaddr *) &address, sizeof(address))<0){
		puts("\n--------- BIND failed \n");
		puts(" Curren	\n---------  socket and addresses are not get binded up ----------- \n "); 
		exit(0);
	}
	
	//Listen on the socket, with 40 max connection requests queued
	if(listen(master_socket,5000)==0){
		printf("Listening from port ====   %d \n", PORT); 
		printf("Listening\n");
	}else{
		puts("\n ----- socket not listening -----  \n  ");
		exit(0);
	}

	puts("\n Waiting for connections ...\n ----- Run client side script ------ \n"); 

	int i = 0,x=0;
	int tmpFlag=0,s=0;
	pthread_t tid[thread_limit];

	for(i=0;i<thread_limit;i++){	

		if(pthread_create(&tid[i], NULL, (void*)socketThread, &(obj[i])) != 0 ){
			printf("Failed to create thread\n");
			break;
		}else{
			puts("\n Thread successfully creating \n");
		}
	}
	
	while(1){
		//Accept call creates a new socket for the incoming connection

		addr_size = sizeof(address);
		puts(" \n Before Accept *********** \n");

		newSocket = accept(master_socket, (struct sockaddr *) &address, &addr_size);
//		bzero(&address,sizeof(&address));
				
		puts("\n After Accept ************* \n");
		printf("\n newSocket ->> %d \t master_socket -->> %d \t addr_size -->> %d\n",newSocket,master_socket,addr_size);
		if(newSocket==-1){
			//sleep(600);
		}

		puts("\n ********************** Before pthread_create() ***********************\n");

		while(1){
			tmpFlag=0;
			for(s=0;s<thread_limit;s++){		
				if((obj[s].gFlag)== 0 && (obj[s].struct_sock)==-1 ){
					
					printf("before = obj[s].struct_sock = %d \n ",obj[s].struct_sock);
							
					obj[s].struct_sock=newSocket;
					obj[s].struct_index=s;
					obj[s].gFlag=newSocket;

					printf("newSocket = >%d \t after = obj[s].struct_sock = %d \t gFlag => %d\n ",newSocket,obj[s].struct_sock,obj[s].gFlag);

					//close(newSocket);
					printf("\n We got free thread for you!!!! Enjoy..... \n");

					tmpFlag=1;
					break;

				}else{
					printf("\n Checking free thread for you -----  \n");
				}
			}

			if(tmpFlag==1){
				break;
			}
		}// End of 2 while()
	}
	for(i=0;i<thread_limit;i++){
		pthread_join(tid[i],NULL);	
		printf("\n =============  Joining all the thread ====== ");
	}
return 0;
}

