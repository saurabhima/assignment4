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
    char *retval1, *retval2, *retval3, *retval4;
    char *pt1 = "]d";
    char *pt2 = "...";
    char *pt3 = ",";
    char *pt4 = "\'";
    int retint = 0;
    retval1 = strstr(pattern, pt1);
    retval2 = strstr(pattern, pt2);
    retval3 = strstr(pattern, pt3);
    retval4 = strstr(pattern, pt4);
    if (retval1 != NULL && retval2 == NULL && retval3 == NULL) {
        retint = 1;
    } else
        if (retval1 != NULL && retval2 != NULL && retval3 == NULL) {
        retint = 2;
    } else
        if (retval1 != NULL && retval2 == NULL && retval3 != NULL) {
        retint = 3;
    }
    else {
        retint = 4;
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
    char* slash = "\\";
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
        } else
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

void parse_data_without_regex(char *file, int notflag, char *regexpattern, char *replacement, int flag_value) {
    char str[MAX_STR_LENGTH];

    printf("\nThis is plain Parsing");
    FILE *fileptr1;
    fileptr1 = fopen(file, "r");
    if (fileptr1 == NULL) {
        fputs("File does not exist", stderr);
        exit(1);

    }
    char *check;

    int lines = count_lines(fileptr1);
    rewind(fileptr1);
    int matchcount = 0;
    while (fgets(str, MAX_STR_LENGTH, fileptr1)) {
        check = strtok(str, " ");
        while (check != NULL) {
            if (strcmp(check, regexpattern) != 0)
                printf(" %s", check);
            else
                if (strcmp(check, regexpattern) == 0 && flag_value == 0 && notflag != 1)
                printf(" %s", replacement);

            else
                if (strcmp(check, regexpattern) == 0 && flag_value != 0) {
                matchcount++;
                if (matchcount == flag_value) {
                    printf(" %s", replacement);
                }
                else printf(" %s", check);

            }
            check = strtok(NULL, " ");
        }



    }
    fclose(fileptr1);
}

void parse_data_with_question(char *file, int notflag, char *regexpattern, char *replacement, int flag_value) {
    char str[MAX_STR_LENGTH];

    printf("\nThis is Question Parsing\n");
    FILE *fileptr1;
    fileptr1 = fopen(file, "r");
    if (fileptr1 == NULL) {
        fputs("File does not exist", stderr);
        exit(1);

    }

    char *check;

    int lines = count_lines(fileptr1);
    rewind(fileptr1);
    char checkpat[MAX_STR_LENGTH];
    strcpy(checkpat, regexpattern);
    int matchcount = 0;
    char finalstr1[MAX_STR_LENGTH], finalstr2[MAX_STR_LENGTH];
    int x, y = 0, z = 0;
    int delimit = 0;
    for (x = 0; x < strlen(checkpat); x++) {
        if (checkpat[x] == '?') delimit = x;
    }
    char prevchar = checkpat[delimit - 1];

    for (x = 0; x < delimit - 1; x++) {
        finalstr1[y] = checkpat[x];
        y++;
        finalstr2[z] = checkpat[x];
        z++;
    }
    finalstr2[z++] = prevchar;
    for (x = delimit + 1; x <= strlen(checkpat); x++) {
        finalstr1[y] = checkpat[x];
        y++;
        finalstr2[z] = checkpat[x];
        z++;
    }

    //printf("\nREGEX Pattern1-->%s",finalstr1);
    //printf("\nREGEX Pattern2-->%s",finalstr2);
    while (fgets(str, MAX_STR_LENGTH, fileptr1)) {
        check = strtok(str, " ");
        while (check != NULL) {
            if (strcmp(check, finalstr1) != 0 && strcmp(check, finalstr2) != 0) {
                printf(" %s", check);
            } else
                if ((strcmp(check, finalstr1) == 0 || strcmp(check, finalstr2) == 0) && flag_value == 0)
                printf(" %s", replacement);

            else
                if ((strcmp(check, finalstr1) == 0 || strcmp(check, finalstr2) == 0) && flag_value != 0) {
                matchcount++;
                if (matchcount == flag_value) {
                    printf(" %s", replacement);
                }
                else printf(" %s", check);

            }
            check = strtok(NULL, " ");
        }



    }
    fclose(fileptr1);
}

void parse_data_with_star(char *file, int notflag, char *regexpattern, char *replacement, int flag_value) {
    char str[MAX_STR_LENGTH];

    printf("\nThis is Star Parsing\n");
    FILE *fileptr1;
    fileptr1 = fopen(file, "r");
    if (fileptr1 == NULL) {
        fputs("File does not exist", stderr);
        exit(1);

    }

    char *check;

    int lines = count_lines(fileptr1);
    rewind(fileptr1);
    int matchcount = 0;
    char checkpat[MAX_STR_LENGTH];
    strcpy(checkpat, regexpattern);
    char finalstr1[MAX_STR_LENGTH], finalstr2[10][MAX_STR_LENGTH];
    int x, y = 0, z = 0;
    int delimit = 0;
    for (x = 0; x < strlen(checkpat); x++) {
        if (checkpat[x] == '*') delimit = x;
    }
    char prevchar = checkpat[delimit - 1];

    for (x = 0; x < delimit - 1; x++) {
        finalstr1[y] = checkpat[x];
        y++;
    }
    for (x = delimit + 1; x <= strlen(checkpat); x++) {
        finalstr1[y] = checkpat[x];
        y++;
    }

    int a;
    for (a = 0; a < 10; a++) {
        y = 0;
        for (x = 0; x < delimit; x++) {
            finalstr2[a][y] = checkpat[x];
            y++;
        }
        for (x = 0; x < a; x++) {
            finalstr2[a][y] = prevchar;
            y++;
        }
        for (x = delimit + 1; x <= strlen(checkpat); x++) {
            finalstr2[a][y] = checkpat[x];
            y++;
        }

        printf("\nPattern2-->%s", finalstr2[a]);
    }

    printf("\nREGEX Pattern1-->%s", finalstr1);
    //printf("\nREGEX Pattern2-->%s",finalstr2);
    while (fgets(str, MAX_STR_LENGTH, fileptr1)) {
        check = strtok(str, " ");
        while (check != NULL) {
            for (a = 0; a < 10; a++) {
                if (strcmp(check, finalstr1) != 0 && strcmp(check, finalstr2[a]) != 0) {
                    printf(" %s", check);
                }
                else
                    if ((strcmp(check, finalstr1) == 0 || strcmp(check, finalstr2[a]) == 0) && flag_value == 0)
                    printf(" %s", replacement);

                else
                    if ((strcmp(check, finalstr1) == 0 || strcmp(check, finalstr2[a]) == 0) && flag_value != 0) {
                    matchcount++;
                    if (matchcount == flag_value) {
                        printf(" %s", replacement);
                    }
                    else printf(" %s", check);

                }
            }
            check = strtok(NULL, " ");
        }



    }
    fclose(fileptr1);


}

void parse_data(char *file, int deltype, int notflag, char *regexpattern, char *replacement, int flag_value) {
    char str[MAX_STR_LENGTH];



    char *retval1, *retval2, *retval3;
    char *pt1 = "?";
    char *pt2 = "*";
    int retint;
    retval1 = strstr(regexpattern, pt1);
    retval2 = strstr(regexpattern, pt2);
    if (retval1 == NULL && retval2 == NULL) {
        retint = 1;
        parse_data_without_regex(file, notflag, regexpattern, replacement, flag_value);
    } else
        if (retval1 != NULL && retval2 == NULL) {
        parse_data_with_question(file, notflag, regexpattern, replacement, flag_value);
        retint = 2;

    } else
        if (retval1 == NULL && retval2 != NULL) {
        parse_data_with_star(file, notflag, regexpattern, replacement, flag_value);
        retint = 3;

    } else
        if (retval1 != NULL && retval2 != NULL) {
        retint = 4;

    }


}

void subsitute_data(char *pattern, char *file, int deltype, FILE* fileptr1) {

    char *retval1, *retval2, *retval3, *pattern1, *pattern2, *regexpattern, *replacement, *flags;
    pattern1 = pattern;
    int notflag = -1;
    char *pt1 = "s/";
    char *pt2 = "/s";
    char *pt3 = "/";

    retval2 = strstr(pattern1, pt2);
    printf("\n Optional Flag-->%s\n", retval2);
    if (retval2 != NULL) {
        notflag = 1;
        //printf("\n NOT operation has been used\n");
    }
    retval1 = strstr(pattern1, pt1);
    //retval1=retval1+2;
    //printf( " %s\n", retval1 );
    retval1 = retval1 + 2;
    retval3 = strtok(retval1, pt3);
    regexpattern = retval3;
    int count = 0;
    while (retval3 != NULL) {
        //printf( " %s\n", retval3 );
        retval3 = strtok(NULL, pt3);
        count++;
        if (count == 1)replacement = retval3;
        if (count == 2)flags = retval3;

    }

    printf("\nRegex Pattern-->%s", regexpattern);
    printf("\nReplacement Pattern-->%s", replacement);
    printf("\nFLAG Pattern-->%s", flags);
    int flag_value = -1;
    if (strcmp(flags, "g") == 0)flag_value = 0;
    else
        flag_value = atoi(flags);

    printf("\n FLAG-->%d\n", flag_value);

    parse_data(file, deltype, notflag, regexpattern, replacement, flag_value);


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
            } else
                if (ptcheck == 4) {//printf("\nThis is Substitution operation");
                subsitute_data(pattern, file, ptcheck, fileptr1);
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
