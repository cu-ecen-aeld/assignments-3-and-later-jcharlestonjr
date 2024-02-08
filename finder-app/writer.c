#include <stdio.h>
#include <syslog.h>

int main(int argc, char *argv[]){
	openlog(NULL,0,LOG_USER);

	if(argc != 2){
		syslog(LOG_ERR,"Invalid # of args: %d", argc);
		return 1;
	}
	if(argv[1] == NULL){
		syslog(LOG_ERR,"No File Specified");
		return 1;
	}
	if(argv[2] == NULL){
                syslog(LOG_ERR,"No Input Specified");
                return 1;
        }

	FILE *newfile;
	newfile=fopen(argv[1],"a");
	fprintf(newfile, "%s\n", argv[2]);
	fclose(newfile);
	syslog(LOG_DEBUG,"WRITE %s to %s",argv[2],argv[1]);

	return 0;
}
