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
#define MAX_DELETE_ARRAY_LENGTH 1000


int start_del_line = 0, end_del_line = 0;
int del_line_array[MAX_DELETE_ARRAY_LENGTH];
int single_line_del = 0;
int del_array_count = 0;

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
    char *retval1, *retval2, *retval3;
    char *pt1 = "]d";
    char *pt2 = "...";
    char *pt3 = ",";
    int retint = 0;
    retval1 = strstr(pattern, pt1);
    retval2 = strstr(pattern, pt2);
    retval3 = strstr(pattern, pt3);
    if (retval1 != NULL && retval2 == NULL && retval3 == NULL) {
        retint = 1;
    } else
        if (retval1 != NULL && retval2 != NULL && retval3 == NULL) {
        retint = 2;
    } else
        if (retval1 != NULL && retval2 == NULL && retval3 != NULL) {
        retint = 3;
    }

    return (retint);
}

int check_pattern_delete(char *pattern, int deltype, FILE *fileptr1) {

    int retint = 0;
    int len = strlen(pattern);
    int ct;
    char checkpat[MAX_STR_LENGTH];
    strcpy(checkpat, pattern);
    //printf("\nChecking Delete Pattern-->%s",checkpat);
    int stloc = 0, endloc = 0;
    for (ct = 0; ct < len; ct++) {

        if (checkpat[ct] == '[') stloc = ct;
        if (checkpat[ct] == ']') endloc = ct;

    }


    if (deltype == 1) {

        char st1[10];
        strncpy(st1, checkpat + stloc + 1, endloc - stloc - 1);
        //printf("\nline string-->%s", st1);
        start_del_line = atoi(st1);
        end_del_line = atoi(st1);

        retint = 1;
    } else
        if (deltype == 2) {

        char *retval1, *retval2, *retval3;
        char *pt1 = "[";
        char *pt2 = "...";
        char *pt3 = "]";


        //retval1 = strtok(pattern, pt1);
        retval1 = strstr(pattern, pt1);
        //printf("\n%s",retval1+1);
        retval2 = strtok(retval1 + 1, pt2);
        start_del_line = atoi(retval2);
        //retval2 =strstr(retval1+1, pt3);   
        //retval2 = strstr(retval1+1, pt2);
        //printf("\n%s",retval2);
        retval2 = strtok(NULL, pt2);
        //printf("\n%s",retval2);
        retval3 = strtok(retval2, pt3);
        //printf("\n%s",retval3);
        end_del_line = atoi(retval3);
        //printf("\nStart Delete Line-->%d", start_del_line);
        //printf("\nEnd Delete Line-->%d", end_del_line);
        retint = 2;
    } else
        if (deltype == 3) {
        //del_line_array=malloc(count_lines(fileptr1)*sizeof(int));

        char *retval1, *retval2, *retval3;
        char *pt1 = "[";
        char *pt2 = ",";
        char *pt3 = "]";

        //retval1 = strtok(pattern, pt1);
        retval1 = strstr(pattern, pt1);
        //printf("\n%s", retval1 + 1);
        retval3 = strtok(retval1 + 1, pt3);
        //printf("\n%s", retval3);
        retval2 = strtok(retval3, pt2);
        del_line_array[0] = atoi(retval2);
        //printf("\n%d",del_line_array[0]);

        while (1) {
            //printf("\n%d", del_line_array[del_array_count]);
            retval2 = strtok(NULL, pt2);
            if (retval2 != NULL) {
                del_line_array[++del_array_count] = atoi(retval2);
            } else
                break;


        }
        del_line_array[++del_array_count] = '\0';
        retint = 3;
    }

    return (retint);

}

int check_pattern_not(char *pattern) {

    char *retval1;
    char slash = '\\';
    char *pt1 = "-";
    int retint = 0;
    retval1 = strstr(pattern, pt1);
    //printf("\n%s", retval1);
    if (retval1 != NULL) {
        printf("\nNOT operator found\n");
        retint = 1;
    }

    return (retint);
}

void delete_data_from_file_single_line(char *file, int start_del_line, int notflag) {

    char str[MAX_STR_LENGTH];
    //printf("\nThis is Delete operation with Start Line-->%d NOT Flag-->%d\n", start_del_line, notflag);
    FILE *fileptr1;
    fileptr1 = fopen(file, "r");
    if (fileptr1 == NULL) {
        fputs("File does not exist", stderr);
        exit(1);

    }
    int lines = count_lines(fileptr1);
    rewind(fileptr1);
    int linecount = 0;

    while (fgets(str, MAX_STR_LENGTH, fileptr1)) {
        if (linecount != start_del_line && notflag == 0) {
            printf("%d %s", linecount, str);
        } else
            if (linecount == start_del_line && notflag != 0) {
            printf("%d %s", linecount, str);
        }
        linecount++;
    }

    fclose(fileptr1);
}

void delete_data_from_file_range_line(char *file, int start_del_line, int end_del_line, int notflag) {
    char str[MAX_STR_LENGTH];
    //printf("\nThis is Delete operation with Start Line-->%d End Line--> %d  NOT Flag-->%d\n", start_del_line, end_del_line, notflag);
    FILE *fileptr1;
    fileptr1 = fopen(file, "r");
    if (fileptr1 == NULL) {
        fputs("File does not exist", stderr);
        exit(1);

    }
    int lines = count_lines(fileptr1);
    rewind(fileptr1);
    int linecount = 0;

    while (fgets(str, MAX_STR_LENGTH, fileptr1)) {
        if ((linecount < start_del_line || linecount > end_del_line) && notflag == 0) {
            printf("%d %s", linecount, str);
        } else
            if ((linecount >= start_del_line && linecount <= end_del_line) && notflag != 0) {
            printf("%d %s", linecount, str);
        }
        linecount++;
    }

    fclose(fileptr1);
}

void delete_data_from_file_multiple_line(char *file, int notflag) {
    char str[MAX_STR_LENGTH];
    //printf("\nThis is Delete operation with Start Line-->%d End Line--> %d  NOT Flag-->%d\n", start_del_line, end_del_line, notflag);
    FILE *fileptr1;
    fileptr1 = fopen(file, "r");
    if (fileptr1 == NULL) {
        fputs("File does not exist", stderr);
        exit(1);

    }
    int lines = count_lines(fileptr1);
    rewind(fileptr1);
    int linecount = 0;
    while (fgets(str, MAX_STR_LENGTH, fileptr1)) {
        int x, printline = -1;
        for (x = 0; x < del_array_count; x++) {
            if (linecount == del_line_array[x]) printline = 0;
        }

        if (printline != 0 && notflag == 0) {
            printf("%d %s", linecount, str);
        }
        else
            if (printline == 0 && notflag != 0) {
            printf("%d %s", linecount, str);
        }
        linecount++;
    }
    //free(del_line_array);   
    fclose(fileptr1);
}

void delete_data(char *pattern, char *file, int deltype, FILE* fileptr1) {

    int lineflag = check_pattern_delete(pattern, deltype, fileptr1);
    int notflag = check_pattern_not(pattern);
    //printf("\nCheck Delete Pattern Flag-->%d", lineflag);
    if (lineflag == 1) {
        delete_data_from_file_single_line(file, start_del_line, notflag);
    } else
        if (lineflag == 2) {
        delete_data_from_file_range_line(file, start_del_line, end_del_line, notflag);
    } else
        if (lineflag == 3) {
        delete_data_from_file_multiple_line(file, notflag);
    }




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
            pattern = argv[argno - 2];
            file = argv[argno - 1];
            //printf("\nPattern-->%s", pattern);
            //printf("\nFile-->%s",file);
            int ptcheck = check_pattern(pattern);
            if (ptcheck == 1) {//printf("\nThis is delete operation");
                delete_data(pattern, file, ptcheck, fileptr1);
            } else
                if (ptcheck == 2) {//printf("\nThis is Range Delete operation");
                delete_data(pattern, file, ptcheck, fileptr1);
            } else
                if (ptcheck == 3) {//printf("\nThis is Multiple line Delete operation");
                delete_data(pattern, file, ptcheck, fileptr1);
            }



            break;
        }


        default:
        {
            break;
        }
    }
    return (0);
}
