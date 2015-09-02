#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
 
#define MSG_SIZE 80
#define MAX_CLIENTS 150
#define MYPORT 7400
 
void exitClient(int fd, fd_set *readfds, char fd_array[], int *num_clients)
{
    int i;
    close(fd);
    FD_CLR(fd, readfds);    /*clear the leaving client from the set*/
    for (i = 0; i < (*num_clients) - 1; i++)
        if (fd_array[i] == fd)
        break;          
    for (; i < (*num_clients) - 1; i++)
        (fd_array[i]) = (fd_array[i + 1]);
        (*num_clients)--;
}
 
int main(int argc, char *argv[]) 
{
    int i=0;
    int count=0;
    char pass[1];
    int port,result;
    int num_clients = 0;
    int server_sockfd, client_sockfd;
    struct sockaddr_in server_address;
    int addresslen = sizeof(struct sockaddr_in);
    int fd;
    char fd_array[MAX_CLIENTS];
    fd_set readfds, testfds, clientfds;
    char msg[MSG_SIZE + 1];     
    char kb_msg[MSG_SIZE + 10];       
 
    /*Server*/
 
    if(argc==1 || argc == 3){
        if(argc==3){
            if(!strcmp("-p",argv[1])){
                sscanf(argv[2],"%i",&port);
            }
            else{
            printf("Invalid parameter.\nUsage: chat [-p PORT] HOSTNAME\n");
            exit(0);
            }
        }
        else
            port=MYPORT;
    printf("\n\t******************** iBOT Server ********************\n");
    printf("\n\n\t Authentication :\n\n\t Password :    ");
    scanf("%s",&pass);
    if(strcmp(pass,"cyber")!=0){
        printf("\n\n !! failure !!\n\n " ); 
        exit(0);
    } 
    printf("\n*** Server waiting (enter \"quit\" to stop): \n");
    fflush(stdout);
 
    /* Create and name a socket for the server */
 
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);
    bind(server_sockfd, (struct sockaddr *)&server_address, addresslen);
 
    /* Create a connection queue and initialize a file descriptor set */
 
    listen(server_sockfd, 1);
    FD_ZERO(&readfds);
    FD_SET(server_sockfd, &readfds);
    FD_SET(0, &readfds);    /* Add keyboard to file descriptor set */
 
    /* Now wait for clients and requests */
 
    while (1){
        testfds = readfds;
        select(FD_SETSIZE, &testfds, NULL, NULL, NULL);
                     
        /* If there is activity, find which descriptor it's on using FD_ISSET */
 
        for (fd = 0; fd < FD_SETSIZE; fd++) {    
            if (FD_ISSET(fd, &testfds)) {
                if (fd == server_sockfd) {  /* Accept a new connection request */
                    client_sockfd = accept(server_sockfd, NULL, NULL);      
                    if (num_clients < MAX_CLIENTS) {
                        FD_SET(client_sockfd, &readfds);
                        fd_array[num_clients]=client_sockfd;
  
                        /*Client ID*/
 
                        printf("\n -> Bot No. %d standby for orders\n",++num_clients);
                        printf("\n >> ");
                        fflush(stdout);
                        send(client_sockfd,msg,strlen(msg),0);
                    }
                    else{
                        sprintf(msg, "XSorry, too many clients.  Try again later.\n");
                        write(client_sockfd, msg, strlen(msg));
                        close(client_sockfd);
                    }
                }
                else
                    if (fd == 0){ 
                        printf(" >> ");       /* Process keyboard activity */                
                        fgets(kb_msg, MSG_SIZE + 1, stdin);       
                        if (strcmp(kb_msg, "quit\n")==0) {
                            sprintf(msg, "iBot Server is shutting down.\n");
                            for (i = 0; i < num_clients ; i++) {
                                write(fd_array[i], msg, strlen(msg));
                                close(fd_array[i]);
                            }
                            close(server_sockfd);
                            exit(0);
                        }
                        else{
                            sprintf(msg, "M%s", kb_msg);
                            for (i = 0; i < num_clients ; i++)
                                write(fd_array[i], msg, strlen(msg));
                        }
                    }
                    else
                        if(fd){                 
                            result = read(fd, msg, MSG_SIZE);   /*read data from open socket*/             
                            if(result==-1) 
                                perror("read()");
                            else
                                if(result>0){
                                    sprintf(kb_msg,"MClient CID %2d",fd);   /*read 2 bytes client id*/
                                    msg[result]='\0';
                     
                            /*concatinate the client id with the client's message*/
       
                                strcat(kb_msg," ");
                                strcat(kb_msg,msg+1);                                        
                     
                            /*print to other clients*/
 
                                for(i=0;i<num_clients;i++){
                                    if (fd_array[i] != fd)                     /*dont write msg to same client*/
                                        write(fd_array[i],kb_msg,strlen(kb_msg));
                                }
                     
                            /*print to server  */
 
                                printf("%s",kb_msg+1);
                     
                            /*Exit Client*/
 
                                if(msg[0] == 'X'){
                                    exitClient(fd,&readfds, fd_array,&num_clients);
                                }   
                            }                                   
                        }                  
                        else{                           
                            exitClient(fd,&readfds, fd_array,&num_clients);  /* A client is leaving */
                        }
            }
        }
    }
  }
}