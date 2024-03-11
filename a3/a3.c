// #include <stdio.h>
// #include <stdlib.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <errno.h>
// #include <string.h>
// #include <sys/types.h>
// #include <sys/stat.h>

// #define RESPONSE_PIPE "RESP_PIPE_89410"
// #define REQUEST_PIPE "REQ_PIPE_89410"

// #define MAX_BUFFER_SIZE 256

// int main()
// {
//     unlink(RESPONSE_PIPE);
//    // unlink(REQUEST_PIPE);

//     //char size;
//    //char *buffer = NULL;

//     char *start = (char*)malloc(6 * sizeof(char));
//     start[0] = 0x73;
//     start[1] = 0x74;
//     start[2] = 0x61;
//     start[3] = 0x72;
//     start[4] = 0x74;
//     start[5] = 0x23;

//     int fd1 = -1;
//     int fd2 = -1;

//     if (mkfifo(RESPONSE_PIPE, 0600) != 0)
//     {
//         free(start);
//         close(fd1);
//         close(fd2);
//          unlink(RESPONSE_PIPE);
//        unlink(REQUEST_PIPE);
//         printf("ERROR\ncannot create the response pipe\n");
//         return 1;
//     }

//     fd1 = open(REQUEST_PIPE, O_RDONLY);
//     if (fd1  < 0)
//      {
//         unlink(RESPONSE_PIPE);
//        unlink(REQUEST_PIPE);
//         free(start);
//         close(fd1);
//         close(fd2);
//        // printf("%d\n",fd1);
//         printf("ERROR\ncannot open the request  pipe\n");
//         return 1;
//     }

//     fd2 = open("RESP_PIPE_89410",O_WRONLY);
//     if (fd2 < 0)
//     {
//         free(start);
//         close(fd1);
//         close(fd2);
//         unlink(RESPONSE_PIPE);
//        unlink(REQUEST_PIPE);
//        printf("ERROR\ncannot open the response pipe\n");
//        return 1;
//     }

//     //char x = 5;
//     //write(fd2, &x, 1);
//     //char start[] = "START";
//     // if (write(fd2, &start, 5) != 5)
//     // {
//     //     printf("write start error\n");
//     // }

//     // write(fd2,connect_msg,1);
//     // write(fd2,connect_msg+1,connect_msg[0]);
//     // printf("SUCCESS\n");

//     unsigned int var = 89410;
//     //unsigned char *c = (unsigned char *)(&var);
//    // unsigned int c = 89410;
//    // unsigned char req_size;
//     char * req = (char*)malloc(250 * sizeof(char));

//    // echo = "VARIANT 89410"
//    char* echo = (char*)malloc(8 * sizeof(char));
//     echo[0] = 0x56;
//     echo[1] = 0x41;
//     echo[2] = 0x52;
//     echo[3] = 0x49;
//     echo[4] = 0x41;
//     echo[5] = 0x4e;
//     echo[6] = 0x54;
//     echo[7] = 0x23;

//     unsigned int  *varianta = (unsigned int*) malloc(4*sizeof(unsigned int));
//     //varianta[0] = 0x20;
//     varianta[0] = 0x00;
//     varianta[1] = 0x01;
//     varianta[2] = 0x5d;
//     varianta[3] = 0x42;

//     // ssize_t bytes_written = write(fd1, start, 6);
//     // if (bytes_written != -1) {
//     //     printf("SUCCESS\n");
//     // } else {
//     //     printf("ERROR: Failed to write data\n");
//     // }

//     write(fd2,start,1);
//     write(fd2,start+1,start[0]);

//     // write(fd2,echo,1);
//     // write(fd2,echo+1,echo[0]);
//     // write(fd2,c,sizeof(unsigned));

//     printf("SUCCESS\n");

//     // write(fd2,echo,1);
//     // write(fd2,echo+1,echo[0]);
//     // write(fd2,varianta,1);
//     // write(fd2,varianta+1,varianta[0]);

//     // if(write(fd1, start, (*start) + 1) != -1){
//     //     printf("SUCCES\n");
//     // }

//     for (;;){
//         //char buffer[MAX_BUFFER_SIZE];
//         //read(fd1, buffer, MAX_BUFFER_SIZE);

//         int k=0;
//         int stop =0;
//         while(stop == 0){
//             read(fd1, &req[k],sizeof(unsigned));
//             if(req[k] == '#'){
//                 stop=1;
//             }
//             k++;
//         }
//         //req[k]='\0';
//         //printf("%s",req);

//         if((strncmp(req,"ECHO",4))){
//              write(fd2,"ECHO#",5);
//              write(fd2,"VARIANT#",8);
//             //  write(fd2,echo,1);
//             //  write(fd2,echo+1,echo[0]);
//             //  write(fd2,varianta,1);
//             //  write(fd2,varianta+1,varianta[0]);
//             write(fd2,&var,sizeof(unsigned));
//         }
//         if((strncmp(req,"EXIT",4))){
//              close(fd1);
//         close(fd2);
//         free(echo);
//         free(start);
//         free(req);
//         unlink(REQUEST_PIPE);
//         unlink(RESPONSE_PIPE);
//         break;
//         }

//     }

//         close(fd1);
//         close(fd2);
//         free(echo);
//         free(start);
//         free(req);
//        // unlink(REQUEST_PIPE);
//        // unlink(RESPONSE_PIPE);

//         return 0;
//     }

#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define RESPONSE_PIPE "RESP_PIPE_89410"
#define REQUEST_PIPE "REQ_PIPE_89410"
#define SHM_NAME "/dczGSAhF"
#define SHM_SIZE 4016854
#define SHM_PERMISSIONS 0664

int main()
{
    if (mkfifo(RESPONSE_PIPE, 0600) != 0)
    {
        printf("ERROR\ncannot create the response pipe\n");
        return 1;
    }

    int fd1 = -1;
    int fd2 = -1;

    int shm_fd;
    char *shm;
    unsigned int mem;
    unsigned int offset;
    unsigned int value;
    unsigned int section;
    unsigned int no_of_bytes;
    unsigned int logical_offset;
    // unsigned int var = 89410;
    // unsigned char *c = (unsigned char *)(&var);
    unsigned int c = 89410;
    // unsigned int nr = 4016854;
    char *req = (char *)malloc(250 * sizeof(char));
    char *req1 = (char *)malloc(250 * sizeof(char));
   // char *req2 = (char *)malloc(250 * sizeof(char));
    int fd_map;
    off_t size;
    char * data;

    fd1 = open(REQUEST_PIPE, O_RDONLY);
    if (fd1 == -1)
    {
        printf("ERROR\ncannot open the request pipe\n");
        unlink(RESPONSE_PIPE);
        return 1;
    }

    fd2 = open(RESPONSE_PIPE, O_WRONLY);
    if (fd2 == -1)
    {
        printf("ERROR\ncannot open the response pipe\n");
        close(fd1);
        unlink(RESPONSE_PIPE);
        return 1;
    }

    if (write(fd2, "START#", 6) != 6)
    {
        printf("ERROR\n");
        return 1;
    }
    printf("SUCCESS\n");

    for (;;)
    {

        int k = 0;
        int j = 0;
        for (k = 0; k < 256; k++)
        {
            read(fd1, &req[k], sizeof(unsigned char));
            if (req[k] == '#')
            {
                break;
            }
            // k++;
        }

        req[k] = '\0';
        printf("%s\n", req);

        // int ok=0;

        if (strncmp(req, "ECHO", 4) == 0)
        {
            write(fd2, "ECHO#", 5);
            write(fd2, "VARIANT#", 8);
            //  write(fd2,echo,1);
            //  write(fd2,echo+1,echo[0]);
            //  write(fd2,varianta,1);
            //  write(fd2,varianta+1,varianta[0]);
            write(fd2, &c, sizeof(unsigned int));
        }

        if (strncmp(req, "CREATE_SHM", 10) == 0)
        {

            read(fd1, &mem, sizeof(unsigned int));
            printf("%d\n", mem);
            write(fd2, "CREATE_SHM#", 11);

            shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0664);
            if (shm_fd == -1)
            {
                printf("shm_open");
                write(fd2, "ERROR#", 6);
            }

            if (ftruncate(shm_fd, mem) == -1)
            {
                printf("ftruncate");
                write(fd2, "ERROR#", 6);
            }

            shm = (char *)mmap(NULL, mem, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
            if (shm == (void *)-1)
            {
                printf("mmap");
                write(fd2, "ERROR#", 6);
            }

            write(fd2, "SUCCESS#", 8);
        }

        if (!(strncmp(req, "MAP_FILE", 8)))
        {
            for (j = 0; j < 256; j++)
            {
                read(fd1, &req1[j], sizeof(unsigned char));
                if (req1[j] == '#')
                {
                    break;
                }
            }
            req1[j]='\0';
            write(fd2, "MAP_FILE#", 9);
            printf("%s\n",req1);
            int ok=1;

            fd_map = open(req1,O_RDONLY);
            if(fd_map == -1){
                printf("fd_map");
                write(fd2, "ERROR#", 6);
                ok=0;
            }

            size = lseek(fd_map,0,SEEK_END);
            if(size == -1){
                printf("size");
                write(fd2, "ERROR#", 6);
              ok=0;
            }

            data = (char *)mmap(NULL,size,PROT_READ,MAP_PRIVATE,fd_map,0);

            if (data == (void *)-1){
                printf("mmap");
                write(fd2, "ERROR#", 6);
                ok=0;
            }
            if(ok==1){
             write(fd2, "SUCCESS#", 8);
            }
            //else{write(fd2, "ERROR#", 6);}
            
        }

        if((strncmp(req,"WRITE_TO_SHM",12))==0){

            read(fd1, &offset, sizeof(unsigned int));
            read(fd1, &value, sizeof(unsigned int));
            write(fd2, "WRITE_TO_SHM#", 12);

        if(offset< 0 && offset >4016854){
            printf("offset");
            write(fd2, "ERROR#", 6);
        }

            write(fd2, "SUCCESS#", 8);

        }

        if((strncmp(req,"READ_FROM_FILE_OFFSET",21))==0){

            read(fd1, &offset, sizeof(unsigned int));
            read(fd1, &no_of_bytes, sizeof(unsigned int));
            write(fd2, "READ_FROM_FILE_OFFSET#", 21);
            write(fd2, "SUCCESS#", 8);

        }

        if((strncmp(req,"READ_FROM_FILE_SECTION",22))==0){

            read(fd1, &section, sizeof(unsigned int));
            read(fd1, &logical_offset, sizeof(unsigned int));
            read(fd1, &no_of_bytes, sizeof(unsigned int));
            write(fd2, "READ_FROM_FILE_SECTION#", 22);
            write(fd2, "SUCCESS#", 8);

        }

        if((strncmp(req,"READ_FROM_LOGICAL_SPACE_OFFSET",30))==0){

            read(fd1, &logical_offset, sizeof(unsigned int));
            read(fd1, &no_of_bytes, sizeof(unsigned int));
            write(fd2, "READ_FROM_LOGICAL_SPACE_OFFSET#",31);
            write(fd2, "SUCCESS#", 8);

        }

        if (!(strncmp(req, "EXIT", 4)))
        {
            close(fd1);
            close(fd2);
            free(req);
            free(req1);
            close(shm_fd);
            close(fd_map);
            munmap(shm, SHM_SIZE);
            munmap(data, size);
            
            unlink(REQUEST_PIPE);
            unlink(RESPONSE_PIPE);
            break;
        }
    }
    return 0;
}
