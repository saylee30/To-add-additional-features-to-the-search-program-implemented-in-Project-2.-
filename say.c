
/*
Name: Saylee Raut
BlazerId: saylee30
Project #: Homework 3
To compile: gcc HW3.c
To run: ./<name of executable> <commands and arguments> <directory>
ex: ./executable -f txt -S -s 100 -t f ./projects/
*/
//Done with the help of sample hw2
#include <stdio.h>  //printf()
#include <stdlib.h> // exit()
#include <dirent.h>
#include <sys/types.h> // pid_t
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h> // fork
#include <string.h>

char **array_element = NULL;
char **val = NULL;

char E_array = 0;
//int arrayBlocks = 0;

typedef struct
{

    int S_flag;              
    int s_flag;            
    int f_flag;   
    int t_flag;       
    int e_flag;           
    int E_flag;                             
    int fileSize;         
    char filterTerm[300]; 
    char fileType[2];     
    char efile[100];
    char Efile[100];
    char *buf;
    int c;
    //extern char *optarg;

} FlagArgs;
int status;
pid_t pid; 
// type definition of the function pointer. It's void because it won't return anything
typedef void FileHandler(char *filePath, char *dirfile, FlagArgs flagArgs, int nestingCount);
int E_flag = 0, e_flag = 0;

// the function that will be used for this assignment
void myPrinterFunction(char *filePath, char *dirfile, FlagArgs flagArgs, int nestingCount)
{
    ///char *dir = "." 
    struct stat buf;                  // buffer for data about file
    lstat(filePath, &buf);           // very important that you pass the file path, not just file name
    char line[100];                  // init some memory for the line that will be printed
    strcpy(line, "");                // verify a clean start
    strcat(line, dirfile);           // init the line with the file name

//Executed with multiple options using S and f

    if (flagArgs.S_flag) // S case
    {
        char strsize[10];                          // allocate memory for the string format of the size
        sprintf(strsize, " %d", (int)buf.st_size); // assign the size to the allocated string
        strcat(line, strsize);                     // concatenate the line and the size
    }
    if (flagArgs.s_flag) // s case
    {
        if (flagArgs.fileSize > (int)buf.st_size) // if the file size is less than the expected
        {
            strcpy(line, ""); // clear the line print
        }
    }
    if (flagArgs.f_flag) // f case
    {
        if (strstr(dirfile, flagArgs.filterTerm) == NULL) // if the filter does not appear in the file
        {
            strcpy(line, ""); // clear the line print
        }
    }
    if (flagArgs.t_flag) // t case
    {
        if (strcmp(flagArgs.fileType, "f") == 0) // if the provided t flag is "f"
        {
            if (S_ISDIR(buf.st_mode) != 0) // if the file is a dir
            {
                strcpy(line, ""); // clear the line print
            }
        }
        if (strcmp(flagArgs.fileType, "d") == 0) // if the provided t flag is "d"
        {
            if (S_ISREG(buf.st_mode) != 0) // if the file is a regular file
            {
                strcpy(line, ""); // clear the line print
            }
        }
    }
//Executed with multiple options using e
// Done with the help of lecture-20 file
    if (flagArgs.e_flag) // e case
    {
        
       /* if (argc < 2) {
            print ("Usage: %s <command> [args]\n", argv[0]);
            exit(-1);
        } i*/                             //Sanitize arguments
        strstr(dirfile, flagArgs.efile) == NULL; // each matching file using fork/exec/wait
        pid = fork();                      //using fork()
        if (pid == 0) // child process
        {
            execv(array_element[0], array_element);   //Using exec()
            printf("Failed :-(\n");
            perror("execv");
            exit(-1);
        }
        else if (pid > 0)
        {
            
            wait(&status);                           //Using Wait()
            if (WIFEXITED(status)) {
                
            } 
        }
        else
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        
        return 0;
    }    
    // optional arguments for each matching file using fork/exec/wait 

    if (strcmp(line, "") != 0) // check to prevent printing empty lines
    {
        int i = 0;
        for (i = 0; i <= nestingCount; i++) // tab printer
        {
            printf("\t"); // print a tab for every nesting
        }
        printf("%s\n", line); // print the line after the tabs
    }
}

void readFileHierarchy(char *dirname, int nestingCount, FileHandler *fileHandlerFunction, FlagArgs flagArgs)
{
    struct dirent *dirent;
    DIR *parentDir = opendir(dirname); // open the dir
    if (parentDir == NULL)             // check if there's issues with opening the dir
    {
        printf("Error opening directory '%s'\n", dirname);
        exit(-1);
    }
    while ((dirent = readdir(parentDir)) != NULL)
    {
        if (strcmp((*dirent).d_name, "..") != 0 &&
            strcmp((*dirent).d_name, ".") != 0) 
        {
            char pathToFile[300];                                                      
            sprintf(pathToFile, "%s/%s", dirname, ((*dirent).d_name));                 
            fileHandlerFunction(pathToFile, (*dirent).d_name, flagArgs, nestingCount); 
            if ((*dirent).d_type == DT_DIR)                                            
            {
                nestingCount++;                                                             
                readFileHierarchy(pathToFile, nestingCount, fileHandlerFunction, flagArgs); 
                nestingCount--;                                                             
            }
        }
    }
    closedir(parentDir); // make sure to close the dir
}
//Ref - https://www.youtube.com/watch?v=KArSxvz9XlA 
//System function
e_file(char *new)
{
    int number;
    number = system(new);
    return number;
}
E_file(char *new)
{
    int number;
    number = system(new);
    return number;
}

int main(int argc, char **argv)
{
    // init opt :
    int opt = 0;
    // init a flag struct with 0s
    FlagArgs flagArgs = {
        .S_flag = 0,
        .s_flag = 0,
        .f_flag = 0,
        .t_flag = 0,
        .e_flag = 0,
        .E_flag = 0};

    // Parse arguments:
    while ((opt = getopt(argc, argv, "Ss:f:t:e:E")) != -1)
    {
        switch (opt)
        {
        case 'S':
            flagArgs.S_flag = 1; // set the S_flag to a truthy value
            break;

        case 's':
            flagArgs.s_flag = 1;              // set the s_flag to a truthy value
            flagArgs.fileSize = atoi(optarg); // set fileSize to what was provided
            break;

        case 'f':
            flagArgs.f_flag = 1;                 // set the f_flag to a truthy value
            strcpy(flagArgs.filterTerm, optarg); // set filterTerm to what was provided
            break;

        case 't':
            flagArgs.t_flag = 1;               // set the t_flag to a truthy value
            strcpy(flagArgs.fileType, optarg); // set fileType to what was provided
            break;

        case 'e':                           // Ref- https://www.youtube.com/watch?v=KArSxvz9XlA
            flagArgs.e_flag = 1;
            e_file(argv[2]);
            break;

        case 'E':                           // Ref- https://www.youtube.com/watch?v=KArSxvz9XlA
            flagArgs.E_flag = 1;
            e_file(argv[2]);
            break;

        }
    }
    
    //Ref- https://www.geeksforgeeks.org/getopt-function-in-c-to-parse-command-line-arguments/
   // Parse command line argument
    int optind = 0 ;

        for(optind= 0; optind < argc; optind++)
        {     
        printf("extra Function %s\n", argv[optind]); 
        }
    optind ++;

    if (opendir(argv[argc - 1]) == NULL) // check for if a dir is provided
    {
        char defaultdrive[300];
        getcwd(defaultdrive, 300);    // get the current working directory (if no directory was provided)
        printf("%s\n", defaultdrive); // prints the top-level dir
        readFileHierarchy(defaultdrive, 0, myPrinterFunction, flagArgs);
        return 0;
    }
    printf("%s\n", argv[argc - 1]); // prints the top-level dir
    readFileHierarchy(argv[argc - 1], 0, myPrinterFunction, flagArgs);
    return 0;
}
