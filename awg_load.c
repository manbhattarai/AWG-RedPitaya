#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>

#define DATA_IN_ADDRESS 0x41200000
#define READ_ENABLE_ADDRESS 0x41210000
#define WRITE_ADDR_ADDRESS 0x41220000
#define WRITE_ENABLE_ADDRESS 0x41230000

int main(int argc, char * argv[])
{
    int fd;
    fd = open("/dev/mem",O_RDWR|O_SYNC);
    if (fd<0)
    {
        perror("Failed to open /dev/mem");
        return 1;
    }

    void *data_in_map;
    data_in_map = mmap(NULL,32768,PROT_READ|PROT_WRITE,MAP_SHARED,fd,DATA_IN_ADDRESS);
    if (data_in_map == MAP_FAILED)
    {
        perror("Failed to map GPIO memory for data in");
        close(fd);
        return 1;
    }

    void *re_map;
    re_map = mmap(NULL,32768,PROT_READ|PROT_WRITE,MAP_SHARED,fd,READ_ENABLE_ADDRESS);
    if (re_map == MAP_FAILED)
    {
        perror("Failed to map GPIO memory for read enable");
        close(fd);
        return 1;
    }

    void *waddr_map;
    waddr_map = mmap(NULL,32768,PROT_READ|PROT_WRITE,MAP_SHARED,fd,WRITE_ADDR_ADDRESS);
    if (waddr_map == MAP_FAILED)
    {
        perror("Failed to map GPIO memory for write address");
        close(fd);
        return 1;
    }

    void *we_map;
    we_map = mmap(NULL,32768,PROT_READ|PROT_WRITE,MAP_SHARED,fd,WRITE_ENABLE_ADDRESS);
    if (we_map == MAP_FAILED)
    {
        perror("Failed to map GPIO memory for write enable");
        close(fd);
        return 1;
    }

    volatile unsigned int *data_in_data; 
    volatile unsigned int *re_data; 
    volatile unsigned int *waddr_data; 
    volatile unsigned int *we_data;

    data_in_data = (volatile unsigned int *)data_in_map;
    re_data      = (volatile unsigned int *)re_map;
    waddr_data   = (volatile unsigned int *)waddr_map;
    we_data      = (volatile unsigned int *)we_map;

    *we_data = 1;
    *re_data = 0;
    int N = atoi(argv[1]);
    char fname[32];
    strcpy(fname, argv[2]);
    int buffer[131072];

    FILE *f0;
    f0 = fopen(fname,"r");
    int val;
    int count = 0;
    while(fscanf(f0,"%d",&val) != EOF)
    {
	buffer[count] = val;
	count++;
    }
    
    int count = 0;
    while (count < N)
    {
	*waddr_data = count;	
	*data_in_data = buffer[count];
	count++;
    }

    *we_data = 0;
    *re_data = 1;
    printf("Loading successful.\n"); 

    munmap(data_in_map,32768);
    munmap(re_map,32768);
    munmap(waddr_map,32768);
    munmap(we_map,32768);
    close(fd);
    return 0;
}
