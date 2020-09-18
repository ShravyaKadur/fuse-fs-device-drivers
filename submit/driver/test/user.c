#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/poly_list.h>
#include <string.h>

#define DEVICE "/dev/poly_list"
int main(void) {
    int fp,i=0;
    ssize_t bytes;
    char message[10];
    char add[10];

    strcpy(add, "10\0");

    fp = open(DEVICE, O_RDWR);
//    printf("add %s\n",add);

      for(i=0; i<5; i++){
    	  sprintf(add, "%d", i+1);
		  strcat(add, "\0");
    	  bytes =write(fp, add, WRITESIZE);
    	  strcpy(add, "\0");
      }


      for(i=0;i<4;i++){
    	  bytes = read(fp, message, sizeof(message));
    	  printf("val= %s\n", message);
    	  strcpy(message, "\0");

      }
    ioctl(fp, STACK, (void *)NULL);

    bytes = read(fp, message, sizeof (message));
	printf("val = %s\n", message);
	strcpy(message, "\0");

    ioctl(fp, STACK, (void *)NULL);
//    strcpy(add,"\0");
    for(i=0; i<4; i++){
  	  sprintf(add, "%d", i+1);
	  strcat(add, "\0");
  	  bytes =write(fp, add, WRITESIZE);
  	  strcpy(add, "\0");
    }

    for(i=0;i<4;i++){
    	bytes = read(fp, message, sizeof (message));
  	    printf("val = %s\n", message);

    	strcpy(message, "\0");
    }

//    strcpy(message, "\0");
//    ioctl(fp, QUEUE, (void *)NULL);
//    strcpy(add, "\0");
//
//    sprintf(add, "%d", 9);
//    strcpy(add, "9\0");
//    bytes =write(fp, add, WRITESIZE);
//    strcpy(add, "\0");

    close(fp);

}

