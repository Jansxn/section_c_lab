#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

int main()
{
	int sd;
	struct sockaddr_in address;

	sd=socket(AF_INET,SOCK_DGRAM,0);
	address.sin_family=AF_INET;
	address.sin_addr.s_addr=htonl(INADDR_ANY);
	address.sin_port=htons(9704);

	int n, m;

	printf("Enter number of rows & columns\n");
	scanf("%d", &m);
	scanf("%d", &n);
	int *row = calloc (n, sizeof(int));

	int len=sizeof(address);
	int l=sendto(sd,&m,sizeof(int),0,(struct sockaddr *)&address, len);
	l=sendto(sd,&n,sizeof(int),0,(struct sockaddr *)&address, len);

	for (int i= 0 ;i<m; i++){
		for (int j=0;j<n;j++){
			int num;
			printf("Enter %d row, %d col: ", i, j);
			scanf("%d", &row[j]);
		}

		l=sendto(sd,row,sizeof(int)*n,0,(struct sockaddr *)&address, len);
	}

	// int ** res = calloc (m, sizeof(int *));
	// for (int i =0 ; i<n; i++){
	// 	res[i] = calloc (n, sizeof(int));
	// }

	// printf("the server echo is\n");
	// l=recvfrom(sd,res,sizeof(res),0,(struct sockaddr *)&address,&len);
	// printf("Hello - %d\n", res[0][0]);
	// for (int i= 0; i<m; i++){
	// 	for (int j=0; j<n; j++){
	// 		printf("%d, ", res[i][j]);
	// 	}
	// 	printf("\n");
	// }

	return 0;
}