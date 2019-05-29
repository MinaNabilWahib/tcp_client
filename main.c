// Write CPP code here 
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <zconf.h>
#include <arpa/inet.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr
#define maxprocesses 10


struct transaction{
    int arrivalTime;
    int sourceAccount;
    int destinationAccount;
    int amountTotransfer;
    int timeNeeded;
};

//spliting function
int split(char cmd[],char*parsed[],char splitter[]){
    char *ptr = strtok(cmd, splitter);
    int i = 0;
    while (ptr != NULL) {
        parsed[i] = ptr;
        ptr = strtok(NULL, splitter);
        i = i + 1;
    }
    return i;
}
//convert from string to integer
int toString(char a[]) {
    int c, sign, offset, n;

    if (a[0] == '-') {  // Handle negative integers
        sign = -1;
    }

    if (sign == -1) {  // Set starting position to convert
        offset = 1;
    }
    else {
        offset = 0;
    }

    n = 0;

    for (c = offset; a[c] != '\0'; c++) {
        if (a[c]!='\n'){
            n = n * 10 + a[c] - '0';
        }

    }

    if (sign == -1) {
        n = -n;
    }

    return n;
}

void func(int sockfd)
{
    char buff[MAX];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        printf("Enter the string : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("From Server : %s", buff);
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}

int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    char fileName[50];
    FILE *fptr;
    char line[24];
    int j=0 ;
    char delim[] = " ";
    char *stri[6] = {'\0'};
    struct transaction * transactions = (struct process*)malloc(maxprocesses *  sizeof(struct transaction));



    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else {
        printf("connected to the server..\n");
    }

    // reading the client file
    printf("enter server file name : \n");
    scanf("%[^\n]%*c", fileName);
    if ((fptr = fopen(fileName, "r")) == NULL)
    {
        printf("Error! opening file");
        // Program exits if file pointer returns NULL.
        exit(1);
    }
    while (fgets(line, 24, (FILE*)fptr)!=NULL){
        int trans_para=split(line,stri,delim);
        //  accounts[j].
        transactions[j].arrivalTime= toString(stri[0]);

        transactions[j].sourceAccount=toString(stri[1]);
        transactions[j].destinationAccount=toString(stri[2]);
        transactions[j].amountTotransfer=toString(stri[3]);
        transactions[j].timeNeeded=toString(stri[4]);

        j++;

    }

    fclose(fptr);

    for (int i =0 ;i<j ; i++){
        printf("%d %d %d %d %d\n",transactions[i].arrivalTime, transactions[i].sourceAccount,transactions[i].destinationAccount,transactions[i].amountTotransfer,transactions[i].timeNeeded);
    }


    // function for chat
    func(sockfd);

    // close the socket
    close(sockfd);
} 
