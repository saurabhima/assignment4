#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/stat.h>
#include <dirent.h>

#define MAX_STR_LENGTH 200

int start_del_line=0,end_del_line=0;

int count_lines(FILE *ptr) {
    rewind(ptr);
    char ch;
    int count = 0;
    while (1) {
        ch = getc(ptr);
        if (ch == EOF)break;
        if (ch == '\n')count++;
    }
    return (count);
}


int check_pattern(char *pattern) {
    char *retval1, *retval2,*retval3;
    char *pt1 = "]d";
    char *pt2 = "*";
    char *pt3 = "+";
    int retint=0;
    retval1 = strstr(pattern, pt1);
    retval2 = strstr(pattern, pt2);
    retval3 = strstr(pattern, pt3);
    if (retval1 != NULL && retval2 == NULL && retval3 == NULL) {
        retint = 1;
    } 
    
    return (retint);
}

int check_pattern_delete(char *pattern) {
    
    int retint=0;
    int len=strlen(pattern);
    int ct;
    char checkpat[MAX_STR_LENGTH];
    strcpy(checkpat, pattern);
    //printf("\nChecking Delete Pattern-->%s",checkpat);
    int stloc=0,endloc=0;
    for(ct=0;ct<len;ct++)
    {
        
        if(checkpat[ct]=='[') stloc=ct;
        if(checkpat[ct]==']') endloc=ct;

    }
    start_del_line=stloc+1;
    end_del_line=endloc-1;
    if(endloc-stloc==2){
       
        
        retint=1;
        }
    return (retint);
}

int check_pattern_not(char *pattern) {
    
    char *retval1;
    char *pt1 = "\[";
    int retint=0;
    retval1 = strstr(pattern, pt1);
    if (retval1 != NULL) {
       //printf("\nNOT operator found");
       retint = 1;
    } 
    
    return (retint);
}
    
void delete_data_from_file(char *file,int start_del_line,int end_del_line,int notflag)
{

printf("\nThis is Delete operation with Start Line-->%d End Line-->%d NOT Flag-->%d",start_del_line,end_del_line,notflag);
FILE *fileptr1;
fileptr1 = fopen(file, "r");
            if (fileptr1 == NULL) {
                fputs("File does not exist", stderr);
                exit(1);
                
            }
int lines=count_lines(fileptr1);
rewind(fileptr1);
fclose(fileptr1);
}


void delete_data(char *pattern,char *file)
{

int lineflag=check_pattern_delete(pattern);
int notflag=check_pattern_not(pattern);
delete_data_from_file(file,start_del_line,end_del_line,notflag); 


}

int main(int argc, char *argv[]) {
char *command, *file, *option, *pattern;
FILE *fileptr1;
char str[MAX_STR_LENGTH];
int argno = argc;
if (argno < 3) {
        printf("\nArguement Error..Please Check\n");
        exit(0);
    }
    
switch (argno) {
        case 3:
        {
         pattern = argv[argno-2];
         file = argv[argno-1];
         //printf("\nPattern-->%s",pattern);
         //printf("\nFile-->%s",file);
         int ptcheck = check_pattern(pattern);
         if(ptcheck==1)
            {//printf("\nThis is delete operation");
             delete_data(pattern,file);
            }
            
        
        break;      
         }
        

        default:
        {
        break;
        }
   }
return(0);
}
