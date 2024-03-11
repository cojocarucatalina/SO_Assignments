#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define MAGIC "RA"
#define SECT_TYPE 80, 52, 12, 91, 54
#define MAX_VERSION 96
#define MIN_VERSION 70
#define MAX_SECTION 16
#define MIN_SECTION 4
#define SECTION_HEADER_SIZE (15 + 4 + 4 + 4)
#define MAX_LINE 1024

// LIST

int listDir(char *path, int rec, int perm, int size, int succes)
{

    DIR *dir = NULL;
    dir = opendir(path); //deschidem directorul
    struct dirent *entry = NULL;

    struct stat statbuf;
    char filePath[512];
    if (NULL == dir)
    {
        printf("ERROR\ninvalid directory path");
        return -1;
    }
    else
    {
        if (succes == 0)
        {
            succes = 1;
            printf("SUCCESS\n");
        }
    }

    while ((entry = readdir(dir)) != NULL)
    {

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        // nu la infinit
        off_t sizeOf =sizeof(filePath);

        snprintf(filePath, sizeOf, "%s/%s", path, entry->d_name);

        if (lstat(filePath, &statbuf) == -1)
        {
            printf("ERROR\ninvalid directory path");
            continue; 
        }
        else
        {
            int statBuf = S_ISDIR(statbuf.st_mode);
            if (rec && statBuf)
            {

                listDir(filePath, rec, perm, size, 1);
            }
            if (statBuf)
               { printf("%s\n", filePath); }
         else if (!perm || (perm && (statbuf.st_mode & S_IWUSR)))
            {
                if (!size || (size && statbuf.st_size > size))
                {
                    printf("%s\n", filePath);
                }
            }
        }
    }
    closedir(dir);
    return 0;
}

//cypry oprisa os-lab-examples


off_t fileSize(char *path)
{

    int fd = open(path, O_RDONLY);
    if (fd == -1)
    {
        printf("ERROR\n");
        return -1;
    }

    struct stat st;
    if (stat(path, &st) == 0)
    {
        off_t size = st.st_size;
        // printf("The size of %s is %ld bytes.\n", path, size);
        return size;
    }
    else
    {
        printf("ERROR\n");
        return -1;
    }
}


int parse(char *path)
{

    int fd = open(path, O_RDONLY);
    if (fd == -1)
    {
        perror("eroare\n");
        return -1;
    }

    struct stat st;
    if (fstat(fd, &st) == -1)
    {
        perror("eroare\n");
        return -1;
    }

    off_t verif;
    // off_t file_size = lseek(fd, 0, SEEK_END);
    off_t offset = fileSize(path) - 2;

    char *magic = malloc(3);
    verif = lseek(fd, offset, SEEK_SET);
    if (verif == -1)
        printf("magic");
    magic[3] = '\0';

    if (read(fd, magic, 2) == -1)
    {
        // perror("error");
        printf("magic");
        return -1;
    }

    if (strcmp(magic, MAGIC) == 0)
    {
        offset = fileSize(path) - 4;

        off_t *header_size = malloc(sizeof(off_t));
        verif = lseek(fd, offset, SEEK_SET);
        if (verif == -1)
            printf("hs\n");

        if (read(fd, header_size, 2) == -1)
        {
            // perror("error");
            printf("hs\n");
            return -1;
        }

        int no_of_section = 0; /// sect_offset
        offset = fileSize(path) - *header_size + 4;
        verif = lseek(fd, offset, SEEK_SET);
        if (verif == -1)
        {
            printf("sections 1\n");
            return -1;
        }
        if (read(fd, &no_of_section, 1) == -1)
        {
            // perror("eroare\n");
            printf("sections 2\n");
            return -1;
        }

        int version = 0; /// sect_offset
        offset = fileSize(path) - *header_size;
        verif = lseek(fd, offset, SEEK_SET);
        if (verif == -1)
        {
            printf("version 1\n");
            return -1;
        }
        if (read(fd, &version, 4) == -1)
        {
            // perror("eroare\n");
            printf("version 2 \n");
            return -1;
        }

        if ((no_of_section < MIN_SECTION || no_of_section > MAX_SECTION)) // verificare NO_OF_SECTIONS
        {
            printf("ERROR\nwrong sect_nr\n"); // mesaj de eroare
            free(magic);
            free(header_size);
            return -1;
        }
        if ((version < MIN_VERSION || version > MAX_VERSION)) // verificare NO_OF_SECTIONS
        {
            printf("ERROR\nwrong version\n"); // mesaj de eroare
            free(magic);
            free(header_size);
            return -1;
        }

        off_t offset = fileSize(path) - *header_size + 20;

        verif = lseek(fd, offset, SEEK_SET);
        if (verif == -1)
            printf("name");

        for (int i = 0; i < no_of_section; i++)
        {

            // char* name = malloc(16);

            // name[15]='\0';

            // if(read(fd,name,15)==-1){
            //     printf("name");
            //     return -1;}

            int TYPE = 0;

            // offset +=15;
            // verif = lseek(fd, offset, SEEK_SET);
            // if(verif == -1) printf("hs\n");

            //  printf("%ld PRINT offset             1\n",offset);
            if (read(fd, &TYPE, 4) == -1)
            {
                printf("type\n");
                return -1;
            }

            offset += 4;
            if (!(TYPE == 80 || TYPE == 52 || TYPE == 12 || TYPE == 91 || TYPE == 54))
            {
                printf("ERROR\nwrong sect_types\n");
                free(magic);
                free(header_size);
                return -1;
            }

            offset += (23);
            verif = lseek(fd, offset, SEEK_SET);
            if (verif == -1)
                printf("name");
        }

        printf("SUCCESS\n");

        printf("version=%d\n", version);
        printf("nr_sections=%d\n", no_of_section);

        offset = fileSize(path) - *header_size + 5;

        for (int i = 0; i < no_of_section; i++)
        {

            char *name = malloc(16);
            verif = lseek(fd, offset, SEEK_SET);
            if (verif == -1)
                printf("name");
            name[16] = '\0';

            if (read(fd, name, 15) == -1)
            {
                // perror("error");
                printf("name");
                return -1;
            }

            int TYPE = 0;
            off_t OFFSET = 0;
            int SIZE = 0;

            offset += 15;
            verif = lseek(fd, offset, SEEK_SET);
            if (verif == -1)
                printf("hs\n");

            if (read(fd, &TYPE, 4) == -1)
            {
                // perror("error");
                printf("type\n");
                return -1;
            }

            offset += 4;
            verif = lseek(fd, offset, SEEK_SET);
            if (verif == -1)
                printf("hs\n");

            if (read(fd, &OFFSET, 4) == -1)
            {
                // perror("error");
                printf("offset\n");
                return -1;
            }

            offset += 4;
            verif = lseek(fd, offset, SEEK_SET);
            if (verif == -1)
                printf("hs\n");

            if (read(fd, &SIZE, 4) == -1)
            {
                // perror("error");
                printf("size\n");
                return -1;
            }

            printf("section%d: %s %d %d\n", i + 1, name, TYPE, SIZE);

            // offset+=SECTION_HEADER_SIZE;
            offset += 4;
            free(name);
        }
        free(magic);
        free(header_size);
        close(fd);
    }
    else
    {
        printf("ERROR\nwrong magic\n");
        free(magic);
        return -1;
    }
    return 0;
}


int verificareSF(char *path)
{

    int fd = open(path, O_RDONLY);
    if (fd == -1)
    {
        perror("eroare\n");
        return -1;
    }

    struct stat st;
    if (fstat(fd, &st) == -1)
    {
        perror("eroare\n");
        return -1;
    }

    off_t verif;
    // off_t file_size = lseek(fd, 0, SEEK_END);
    off_t offset = fileSize(path) - 2;

    char *magic = malloc(3);
    verif = lseek(fd, offset, SEEK_SET);
    if (verif == -1)
     //   printf("magic");
    magic[3] = '\0';

    if (read(fd, magic, 2) == -1)
    {
        // perror("error");
      //  printf("magic");
        return -1;
    }

    if (strcmp(magic, MAGIC) == 0)
    {
        offset = fileSize(path) - 4;

        off_t *header_size = malloc(sizeof(off_t));
        verif = lseek(fd, offset, SEEK_SET);
        if (verif == -1)
        //    printf("hs\n");

        if (read(fd, header_size, 2) == -1)
        {
            // perror("error");
        //    printf("hs\n");
            return -1;
        }

        int no_of_section = 0; /// sect_offset
        offset = fileSize(path) - *header_size + 4;
        verif = lseek(fd, offset, SEEK_SET);
        if (verif == -1)
        {
      //      printf("sections 1\n");
            return -1;
        }
        if (read(fd, &no_of_section, 1) == -1)
        {
            // perror("eroare\n");
        //    printf("sections 2\n");
            return -1;
        }

        int version = 0; /// sect_offset
        offset = fileSize(path) - *header_size;
        verif = lseek(fd, offset, SEEK_SET);
        if (verif == -1)
        {
       //     printf("version 1\n");
            return -1;
        }
        if (read(fd, &version, 4) == -1)
        {
            // perror("eroare\n");
        //    printf("version 2 \n");
            return -1;
        }

        if ((no_of_section < MIN_SECTION || no_of_section > MAX_SECTION)) // verificare NO_OF_SECTIONS
        {
       //     printf("ERROR\nwrong sect_nr\n"); // mesaj de eroare
            free(magic);
            free(header_size);
            return -1;
        }
        if ((version < MIN_VERSION || version > MAX_VERSION)) // verificare NO_OF_SECTIONS
        {
        //    printf("ERROR\nwrong version\n"); // mesaj de eroare
            free(magic);
            free(header_size);
            return -1;
        }

        off_t offset = fileSize(path) - *header_size + 20;

        verif = lseek(fd, offset, SEEK_SET);
        if (verif == -1)
         //   printf("name");

        for (int i = 0; i < no_of_section; i++)
        {

            // char* name = malloc(16);

            // name[15]='\0';

            // if(read(fd,name,15)==-1){
            //     printf("name");
            //     return -1;}

            int TYPE = 0;

            // offset +=15;
            // verif = lseek(fd, offset, SEEK_SET);
            // if(verif == -1) printf("hs\n");

            //  printf("%ld PRINT offset             1\n",offset);
            if (read(fd, &TYPE, 4) == -1)
            {
          //      printf("type\n");
                return -1;
            }

            offset += 4;
            if (!(TYPE == 80 || TYPE == 52 || TYPE == 12 || TYPE == 91 || TYPE == 54))
            {
           //     printf("ERROR\nwrong sect_types\n");
                free(magic);
                free(header_size);
                return -1;
            }

            offset += (23);
            verif = lseek(fd, offset, SEEK_SET);
           // if (verif == -1)
          //      printf("name");
        }

       // printf("SUCCESS\n");
       return 2;

            // offset+=SECTION_HEADER_SIZE;
            offset += 4;
        
        free(magic);
        free(header_size);
        close(fd);
    }
    else
    {
       // printf("ERROR\nwrong magic\n");
        free(magic);
        return -1;
    }
    return 0;
}

int verificareSF2(char *path){

    if(parse(path)==-1) return -1;
    else return 0;
}

int extract(char *path, int section, int line)
{
    //int offArray = 0;
    int no_of_sections = 0;
    int sizeArray = 0;

   int conot=0;
   //int contor =0;

    int fd = open(path, O_RDONLY);
    if (fd == -1)
    {
        perror("eroare\n");
        return -1;
    }

    struct stat st;
    if (fstat(fd, &st) == -1)
    {
        perror("eroare\n");
        return -1;
    }

//printf("\n%d\n", verificareSF(path));
 //   if(verificareSF(path)==-1){printf("ERROR\ninvalid file1\n"); return -1;}

        off_t offset;
        offset = fileSize(path) - 4;

        off_t *header_size = malloc(sizeof(off_t));

        off_t verif;
        verif = lseek(fd, offset, SEEK_SET);

        if(verif == - 1) {printf("EROARE HEADER_SIZE");return -1;}
        if (read(fd, header_size, 2) == -1) {printf("header_size"); return -1;}
 
        offset= fileSize(path)-*header_size+4;

        if(lseek(fd, offset, SEEK_SET) == -1){printf("Sections 1"); return -1;}
     
        if(read(fd,&no_of_sections,1) == -1){printf("Sections 2"); return -1;}

   //     printf("\nHSSSS 1 %d\n",*header_size);


off_t dimensiune = 0;
dimensiune = lseek(fd, 0, SEEK_END);

     //   printf("\nDIMENS 1 %d\n",dimensiune);

//int var = dimensiune - *header_size ;

                // printf("\nSECT   %d\n",section);
                // printf("\nNO OF SECT %d\n",no_of_sections);
                // printf("\nvar %d\n",var);
                // var+=5;
                // printf("\nvar  2 %d\n",var);
                // var= var + 27*(section -1);
                // printf("\nvar  3  %d\n",var);
                // var= var+19;
                // printf("\nvar  4  %d\n",var);

        offset =(dimensiune-*header_size)+5+27*(section-1)+19; 

        lseek(fd, offset, SEEK_SET);

// printf("\n%d\n", cur);
// printf("\n%d\n", offset);

       // if(verif== -1){ printf("ERROR 1"); return -1;}

    //  for (int i=0;i< no_of_sections;i++){
int offArray = 0 ;


        read(fd,&offArray, sizeof(offArray)); //ultima sectiuen incepe la 

        offset+=4;
        lseek(fd, offset, SEEK_SET);

             read(fd,&sizeArray,4); //cati octeti are sectiunea

    if (no_of_sections< section) {printf("ERROR\ninvalid section"); return -1;}


    offset= offArray;
    lseek(fd, offArray, SEEK_SET);

     int line_count = 0;
     char c ;

     for (int i=0; i< sizeArray; i++){
        if(read(fd, &c,1)==-1){
            printf("Error \n");
            return -1;
        }
        if(c=='\n') 
        line_count++;
     }
     if (line_count < line) {printf("ERROR\ninvalid line\n"); return -1;}
  
     offset=offArray;
     lseek(fd, offArray, SEEK_SET);

    conot=0;

    while(conot < line -1){       
         if(read(fd, &c,1)==-1) {printf("ERROR 3"); return -1;}
         offset+=1;
         if(c == '\n') conot++;
     }
     offset+=1;
     char caract;
     char sir[sizeArray+1];
     printf("SUCCESS\n");
    int contor=0;
     while(caract!='\n'){
         if(read(fd, &caract,1)==-1) {printf("ERROR 4"); return -1;}
         // printf("%c",caract);
        sir[contor]=caract;
        contor++;
        offset+=1;
     }
     sir[contor]='\0';
     printf("%s",sir);
     free(header_size);
     close(fd);

 lseek(fd,0,SEEK_SET);
    return 0;
}


int verif13 (char *path)
{
//     int no_of_sections = 0;

//    int verif13 =-1;
//    //int contor =0;

//     int fd = open(path, O_RDONLY);
//     if (fd == -1)
//     {
//         lseek(fd,0,SEEK_SET);
//         perror("eroare\n");
//         return -1;
//     }
//   //  lseek(fd,0,SEEK_SET);

//     struct stat st;
//     if (fstat(fd, &st) == -1)
//     {
//         lseek(fd,0,SEEK_SET);
//         perror("eroare\n");
//         return -1;
//     }

//    // lseek(fd,0,SEEK_SET);
//         off_t offset = 0;


//       //  off_t *header_size = malloc(sizeof(off_t));


//         off_t verif;
//         //verif = 

//        off_t dimensiune = 0;
//        dimensiune = fileSize(path);

//         printf("\n%d DIM\n",dimensiune);

//         offset =dimensiune-4; 
//         lseek(fd, offset, SEEK_SET);

//         off_t *header_size = malloc(sizeof(off_t));

//       //  verif = lseek(fd, offset, SEEK_SET);

//        // if(verif == - 1) {printf("EROARE HEADER_SIZE");return -1;}
//       //  if (read(fd, header_size, 2) == -1) {printf("header_size"); return -1;}

//         pread(fd,header_size,2,offset);

//         printf("\n%d HS????\n",*header_size);
 
//         offset= fileSize(path)-*header_size+4;
//         printf("\n%d offsset\n",offset);


//         if(lseek(fd, offset, SEEK_SET) == -1){printf("Sections 1"); free(header_size); lseek(fd,0,SEEK_SET); return -1;}
     
//         if(read(fd,&no_of_sections,1) == -1){printf("Sections 2");free(header_size); lseek(fd,0,SEEK_SET); return -1;}
        
//           printf("\n%d sections\n",no_of_sections);

//int *offArray=malloc((no_of_sections+1)*sizeof(int));

 // printf(" intrari: %s\n", path);

    int no_of_sections = 0;
    int verif13=-1;

    int fd = open(path, O_RDONLY);
    if (fd == -1)
    {
        perror("eroare\n");
        return -1;
    }
        
        lseek(fd,0,SEEK_SET);

        off_t offset =0;
        offset = fileSize(path) - 4;

       // int *header_size = malloc(sizeof());
       short int hs;

        off_t verif = 0;
        verif = lseek(fd, offset, SEEK_SET);
  //  printf("\n%d offset\n",offset);
  //  printf("\n%d verif\n",verif);

short int header_size;

        if(verif == - 1) {printf("EROARE HEADER_SIZE");return -1;}
        if (pread(fd, &hs, sizeof(hs),offset) == -1) {//printf("header_size");
        //free(header_size);
         return -1;}

      //  memcpy(&header_size,hs,sizeof(int));
      header_size = hs;

    //     printf("\n%d\n",header_size);
 
        offset= fileSize(path)-header_size+4;

        if(lseek(fd, offset, SEEK_SET) == -1){//printf("Sections 1"); 
       // free(header_size);
        return -1;}
     
        if(read(fd,&no_of_sections,1) == -1){//printf("Sections 2"); 
       // free(header_size);
        return -1;}

int offArray[no_of_sections];
int sizeArray[no_of_sections];


       off_t dimensiune = 0;
        dimensiune = lseek(fd, 0, SEEK_END);

        offset =(dimensiune-header_size)+5+15+4; 
      //  lseek(fd, offset, SEEK_SET);

     // free(header_size);

        for (int i=0; i<no_of_sections;i++){

            lseek(fd, offset, SEEK_SET);
            read(fd,&offArray[i],4); //ultima sectiuen incepe la 
            offset+=4;
            lseek(fd, offset, SEEK_SET);
            read(fd,&sizeArray[i],4); //cati octeti are sectiunea
            offset+=(4+15+4);

          //  printf("\n%d offArray\n",offArray[i]);
          //  printf("\n%d sizeArray\n",sizeArray[i]);
        }

    char c;
    int line_count =0;

    for (int i=0;i<no_of_sections;i++){

     offset=offArray[i];      ////i=0 suntem la sectiunea 1
     lseek(fd, offset, SEEK_SET);

    // lseek(fd, *offArray, SEEK_SET);
   //int conot=0;

    for (int j=0; j< sizeArray[i]; j++){

        if(read(fd, &c,1)==-1){
           // free(header_size);
            lseek(fd,0,SEEK_SET);
            printf("Error \n");
            return -1;
        }
        offset++;
        if(c=='\n') 
        line_count++;
        j++;
        }
        if (line_count> 13) {verif13 = 13; break;}
        if(verif13 == 13) break;
     }

    // printf("\n%d verif\n",verif13);
    lseek(fd,0,SEEK_SET);
   // printf("SUCCESS\n");
    return verif13;
}


int listRec(char *path, int succes)                 //// cypryoprisa /os-lab-examples / git
{
    //  DIR *dir = opendir(path);
    // if (dir == NULL) {
    //     perror("opendir");
    //     return;
    // }

    // struct dirent *entry = NULL;

    // while((entry==readdir(dir))!=NULL){
    //     if((strcmp(entry->d_name,".") == 0) || strcmp(entry->d_name,".." == 0)) continue;
     
    //  char fullPath[512];
    //  snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
    //  if(verif13(fullPath) != -1) printf("%s\n",fullPath);

    //  if(S_ISDIR(statbuf.st_mode)) listRec(fullPath);
    // }

    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;

    dir = opendir(path);
    if(dir == NULL) {
        perror("Could not open directory");
        return -1;
    }
    while((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if(lstat(fullPath, &statbuf) == 0) {
                if(verif13(fullPath)!= -1){
                 if(succes==0) {printf("SUCCESS\n");}
                printf("%s\n", fullPath); succes=1;}
                if(S_ISDIR(statbuf.st_mode)) {
                    listRec(fullPath,1);
                }
            }
        }
    }
    closedir(dir);
    return 0;
}


int main(int argc, char **argv)
{

    if (argc >= 2)
    {
        if (strcmp(argv[1], "variant") == 0)
        {
            printf("89410\n");
            return 0;
        }
    }

    // list ----- 2.3

    int rec = 0;
    int size = 0;
    int perm = 0;
    char *path = NULL;

    if (argc >= 2)
    {

        if (strcmp(argv[1], "list") == 0)
        {

            for (int i = 1; i < argc; i++)
            {

                if (strncmp(argv[i], "path=", 5) == 0)
                {

                    path = argv[i] + 5;
                } // argumentul fara 'path='

                if (strcmp(argv[i], "recursive") == 0)
                {

                    rec = 1;
                }

                if (strcmp(argv[i], "has_perm_write") == 0)
                {

                    perm = 1;
                }

                if (strncmp(argv[i], "size_greater=", 13) == 0)
                {

                    size = atoi(argv[i] + 13);
                }
            }

            listDir(path, rec, perm, size, 0);
        }

        // parse   ----- 2.4

        if (strcmp(argv[1], "parse") == 0)
        {

            for (int i = 1; i < argc; i++)
            {

                if (strncmp(argv[i], "path=", 5) == 0)
                {

                    // strcpy(path, argv[i]+5);}
                    path = argv[i] + 5;
                }
            }
            parse(path);
        }

        // extract ---- 2.5
        if (strcmp(argv[1], "extract") == 0)
        {
            int section;
            int line;
          //  for (int i = 1; i < argc; i++)
          //  {

                if (strncmp(argv[2], "path=", 5) == 0)
                {
                    path = argv[2] + 5;
                }

                if (strncmp(argv[3], "section=", 8) == 0)
                {

                    section = atoi(argv[3] + 8);
                }

                if (strncmp(argv[4], "line=", 5) == 0)
                {

                    line = atoi(argv[4] + 5);
                }
          //  }
            extract(path, section, line);
        }

    if (strcmp(argv[1], "findall") == 0)
        {

            for (int i = 1; i < argc; i++)
            {

                if (strncmp(argv[i], "path=", 5) == 0)
                {

                    // strcpy(path, argv[i]+5);}
                    path = argv[i] + 5;
                }
            }
            listRec(path,0);
        }

    }

    return 0;
}
