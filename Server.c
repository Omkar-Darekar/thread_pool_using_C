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

#define PORT 8889
#define	thread_limit 10

pthread_mutex_t lock= PTHREAD_MUTEX_INITIALIZER;
struct main_struct{
	char buffer[50];
	int struct_sock;
	int gFlag;

}obj[thread_limit];

void *socketThread(struct main_struct sock[]){
	int newSocket = sock->struct_sock;

	write(newSocket,sock->buffer,sizeof(sock->buffer));
	printf("\n Message sent successfully to : %d \n",newSocket);
	sock->gFlag=newSocket;
}

int main(){
	int master_socket, newSocket,tmpFlag=0,s=0;
	struct sockaddr_in address;
	char bbuf[50],buf[20];
	int k;

	for(k=0;k<thread_limit;k++){
		strcpy(obj[k].buffer,"\n \n\n \t HELLO Clinet . I am Server . How do dooooo \n");
		obj[k].gFlag=-1;

	}


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
	address.sin_addr.s_addr = inet_addr("127.0.0.1");//Set IP address to localhost 

	if(bind(master_socket, (struct sockaddr *) &address, sizeof(address))<0){
		puts("\n--------- BIND failed \n");
		puts("\n---------  socket and addresses are not get binded up ----------- \n "); 
		exit(0);
	}

	//Listen on the socket, with 40 max connection requests queued
	if(listen(master_socket,500)==0){
		printf("Listening from port ====   %d \n", PORT); 
		printf("Listening\n");
	}else{
		puts("\n ----- socket not listening -----  \n  ");
		exit(0);
	}

	puts("\n Waiting for connections ...\n ----- Run client side script ------ \n"); 

	pthread_t tid[thread_limit];

	for(k=0;k<thread_limit;k++){
		tid[k]=-1;
	}
	while(1)
	{
		//Accept call creates a new socket for the incoming connection
		addr_size = sizeof(address);
	newSocket = accept(master_socket, (struct sockaddr *) &address, &addr_size);

		while(1){
			tmpFlag=0;
			for(s=0;s<thread_limit;s++){		
				if((obj[s].gFlag)>-1){
					printf("\n Checking free thread for you -----  \n");
				}else{
					obj[s].struct_sock=newSocket;
					obj[s].gFlag=0;

					printf("\n We got free thread for you!!!! Enjoy..... \n");
					if( pthread_create(&tid[s], NULL, socketThread, &(obj[s])) != 0 ){
						printf("Failed to create thread\n");
						break;
					}
					else{
						printf("\n pthread_creation success \n");
						printf("newSocket -->> %d",newSocket);
					}
					tmpFlag=1;
					break;
				}
			}

			if(tmpFlag==1){
				break;
			
			}else{

				while(1){
						
					if(s>thread_limit-1){
						s=0;
					}
					
					if((obj[s].gFlag)>0){
						pthread_join(tid[s],NULL);
						obj[s].gFlag=-1;
						s++;
						break;
					}else{
						s++;
					}
				}				
			}

		}// End of 2 while()	
	}

	return 0;
}

