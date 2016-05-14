#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifndef ACL_H
#define ACL_H

/*
Permission Codes
0 = ---
1 = x
2 = w
3 = wx
4 = r
5 = rx
6 = rw
7 = rwx

Access Control Matrix
        0 1 2 3 4 5
File    1 2 3 4 5 6
User1/0 7 7 7 7 7 7
User2/1 6 0 5 0 4 0
User3/2 0 0 6 6 0 0
User4/3 6 0 0 4 4 0
User5/4 0 7 0 4 0 4
*/
//options:  1 = read, 2 = write, 3 = execute

void acl(int id, int fnum, int option)
{
    int temp[30];
    int matrix[5][6] = {0}; //5 rotws, 6 columns
    int i, x = 0;
    int permission;

    //read in matrix
    FILE *fp = fopen("ACL.txt","r");
    while(fscanf(fp,"%d", &i) != EOF)
    {
        temp[x] = i;
        x++;
    }
    fclose(fp);

    int a = 0;
    for(a = 0; a < 6; a++)
    {
        matrix[0][a] = temp[a];
    }
    for(a = 6; a < 12; a++)
    {
        matrix[1][a] = temp[a];
    }
    for(a = 12; a < 18; a++)
    {
        matrix[2][a] = temp[a];
    }
    for(a = 18; a < 24; a++)
    {
        matrix[3][a] = temp[a];
    }
    for(a = 24; a < 30; a++)
    {
        matrix[4][a] = temp[a];
    }

    printf("\n");

    //find permissions in matrix given file number and user id
    permission =  matrix[id-1][fnum-1];
    //printf("%d %d %d \n", permission, id, fnum);

    if(permission == 0) //---
    {
        printf("Access Denied \n");
    }
    else if(permission == 1) //x
    {
        if(option == 3) //execute
        {
            execute(fnum);
        }
        else
        {
            printf("Access Denied \n");
        }
    }
    else if(permission == 2) //w
    {
        if(option == 2) //write
        {
            write(fnum);
        }
        else
        {
            printf("Access Denied \n");
        }
    }
    else if(permission == 3) //wx
    {
        if(option == 2) //write
        {
            write(fnum);
        }
        else if(option == 3) //execute
        {
            execute(fnum);
        }
        else
        {
            printf("Access Denied \n");
        }
    }
    else if(permission == 4) //r
    {
        if(option == 1) //read
        {
            read(fnum);
        }
        else
        {
            printf("Access Denied \n");
        }
    }
    else if(permission == 5) //rx
    {
        if(option == 1) //read
        {
            read(fnum);
        }
        else if(option == 3) //execute
        {
            execute(fnum);
        }
        else
        {
            printf("Access Denied \n");
        }
    }
    else if(permission == 6) //rw
    {
        if(option == 1) //read
        {
            read(fnum);
        }
        else if(option == 2) //write
        {
            write(fnum);
        }
        else
        {
            printf("Access Denied \n");
        }
    }
    else
    {
        if(option == 1) //read
        {
            read(fnum);
        }
        else if(option == 2) //write
        {
            write(fnum);
        }
        else //execute
        {
            execute(fnum);
        }
    }
}

void read(int fnum)
{
    int c;
    char text;
    text = (char)(((int)'0')+fnum); //convert int fnum into char
    char file[10] = {'f','i','l','e',text,'.','t','x','t'};
    FILE *fp = fopen(file,"r");
    printf("\n");
    printf("%s opened: \n", file);
    while ((c = getc(fp)) != EOF)
    {
        putchar(c);
    }
    fclose(fp);
    printf("\n");
}

void write(int fnum)
{
    char text;
    text = (char)(((int)'0')+fnum); //convert int fnum into char
    char file[10] = {'f','i','l','e',text,'.','t','x','t'};
    FILE *fp = fopen(file,"w");
    char sentence[500];
    printf("Enter text to write to file: ");
    fgets(sentence,500,stdin);
    fprintf(fp, "%s", sentence);
    fclose(fp);
}

void execute(int fnum)
{
    char text;
    text = (char)(((int)'0')+fnum); //convert int fnum into char
    char file[10] = {'f','i','l','e',text,'.','t','x','t'};
    //FILE *fp = fopen(file,"r");
}

#endif
