#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifndef RBAC_H
#define RBAC_H

/*
Roles:
Sales = User4, User5
Technical Staff = User2, User3
Manager = User1

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
File            1 2 3 4 5 6
Manager:    1/0 7 7 7 7 7 7   //rwx
Tech Staff: 2/1 6 6 6 6 6 6   //rw
Salse:      3/2 4 4 4 4 4 4   //r
*/

//Assign Roles
//Manager = 1
//Tech Staff = 2
//Salse = 3
//[User1, User2, User3, User4, User5]
int rlist[5] = {1,2,2,3,3};

void rbac(int id, int fnum, int option)
{
    int temp[18];
    int matrix[3][6] = {0}; //4 rotws, 6 columns
    int i, x = 0;
    int permission;
    int role;

    //read in matrix
    FILE *fp = fopen("RBAC.txt","r");
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
    printf("\n");

    //find permissions in matrix given file number and user id
    permission =  matrix[id-1][fnum-1];

    //find role
    role = rlist[id-1];

    if(permission == 7 && role == 1) //rwx and manager
    {
        if(option == 1) //read
        {
            read2(fnum);
        }
        else if(option == 2) //write
        {
            write2(fnum);
        }
        else //execute
        {
            execute2(fnum);
        }
    }
    else if(permission == 6 && role == 2) //rw and tech staff
    {
        if(option == 1) //read
        {
            execute2(fnum);
        }
        else //write
        {
            write2(fnum);
        }
    }
    else if(permission == 4 && role == 3) //r and sales
    {
        read2(fnum);
    }
    else
    {
        printf("Access Denied \n");
    }
}

void read2(int fnum)
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

void write2(int fnum)
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

void execute2(int fnum)
{
    char text;
    text = (char)(((int)'0')+fnum); //convert int fnum into char
    char file[10] = {'f','i','l','e',text,'.','t','x','t'};
    //FILE *fp = fopen(file,"r");
}

#endif
