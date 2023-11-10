#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>

int main(){
	DIR * dir = opendir("/home/jason");
	struct dirent * entry;
	struct stat statbuf;

	while ((entry = readdir(dir))!=NULL){
		lstat(entry->d_name, &statbuf);
		if (S_ISDIR(statbuf.st_mode)) printf("Name: %s\n", entry->d_name);
		printf("%x", statbuf.st_mode);
		struct tm* timeinfo = localtime(&statbuf.st_mtime);
		printf("Time: %s\n", asctime(timeinfo));
	}
	closedir(dir);
	return 0;
}