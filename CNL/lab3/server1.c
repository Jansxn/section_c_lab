#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

int main()
{
	int sd;
	struct sockaddr_in sadd,cadd;

	int m, n;

	//Create a UDP socket
	sd=socket(AF_INET,SOCK_DGRAM,0);

	//Construct the address for use with sendto/recvfrom... */
	sadd.sin_family=AF_INET;
	sadd.sin_addr.s_addr=htonl(INADDR_ANY);//**
	sadd.sin_port=htons(9704);

	int result=bind(sd,(struct sockaddr *)&sadd,sizeof(sadd));

	printf("Waiting for data...\n");

	int len=sizeof(cadd);
	int l=recvfrom(sd,&m,sizeof(int),0,(struct sockaddr *)&cadd,&len);
	l=recvfrom(sd,&n,sizeof(int),0,(struct sockaddr *)&cadd,&len);

	int **mat = calloc (m, sizeof(int *));
	int *row = calloc (n, sizeof (int));
	for (int i=0; i<m; i++){
		mat[i] = calloc (n, sizeof(int));
	}

	for (int i =0 ; i<m ;i++){
		l=recvfrom(sd,row,sizeof(int)*n,0,(struct sockaddr *)&cadd,&len);
		printf("Recieved row %d\n", i);
		for (int j =0 ; j<n; j++){
			mat[i][j] = row[j];
		}
	}

	printf("the server send is\n");
	for (int i= 0; i<m; i++){
		for (int j=0; j<n; j++){
			printf("%d, ", mat[i][j]);
		}
		printf("\n");
	}


	l =sendto(sd,mat,sizeof(mat),0,(struct sockaddr *)&cadd,len);
	return 0;
}