#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ACL.h"
#include "RBAC.h"
#define THR 50

float avg;

int calcFR(char user)
{
    char file[10] = {'u','s','e','r',user,'.','t','x','t'};
    char header[1000];
    int fly[3000];
    int dwell[3000];
    int firstKeyCode, secondKeyCode, flyTime, firstDwell, secondDwell;
    int x;
    int i = 0;
    int FRrate = 0;
    float sumDi = 0;
    float sumMono = 0;
    float D = 0;
    int section = 500 * (i+1); //data section

    //open user file
    FILE *fp = fopen(file,"r");
    fgets(header, sizeof header, fp); //get rid of header line

    //read in fly time and dwell time
    while(fscanf(fp,"%d %d %d %d %d",&firstKeyCode, &secondKeyCode, &flyTime, &firstDwell, &secondDwell) != EOF)
    {
        fly[i] = flyTime;
        dwell[i] = firstDwell;
        i++;
    }
    fclose(fp);

    //for 5 data section, calculate false reject
    for(i=0;i<5;i++)
    {
		D = 0.0;
		sumDi = 0.0;
		sumMono = 0.0;
        //calculate sum of Di
        for(x = 0; x < 500; x++)
        {
            if(fly[section] > 0)
            {
                sumDi = sumDi + abs((float) fly[x] - (float) fly[section]) / (float) fly[section];
            }
            section = section + 1;
        }

        //calculate sum of mono
        section = 500 * (i+1); //reset counter
        for(x = 0; x < 500; x++)
        {
            if(dwell[section] > 0)
            {
                sumMono = sumMono + abs((float) dwell[x] - (float) dwell[section]) / (float) dwell[section];
            }
            section = section + 1;
        }

        //calculate deviation
        D = (sumDi / 499 + sumMono / 500) * 50;
        avg = avg+D;
        if(D >= THR)
        {
            FRrate = FRrate+1;
        }
        printf("FR's D %f \n", D);
    }
    avg = avg/5.0;
    return FRrate;
}

//calculate false accept
int calcFA(char user, char temp)
{
    char ufile[10] = {'u','s','e','r',user,'.','t','x','t'};  //user file
    char ofile[10] = {'u','s','e','r',temp,'.','t','x','t'};  //other user file
    char header[1000];
    char head[1000];
    int ufly[3000];
    int udwell[3000];
    int ofly[3000];
    int odwell[3000];
    int x;
    int firstKeyCode, secondKeyCode, flyTime, firstDwell, secondDwell;
    int fkc, skc, ft, fd, sd;
    int z = 0;
    int FArate = 0;
    float sumDi = 0;
    float sumMono = 0;
    float D = 0;
    int section = 500 * (z+1); //data section

    //open user file
    FILE *fu = fopen(ufile,"r");
    fgets(header, sizeof header, fu); //get rid of header line

    //read in fly time and dwell time
    while(fscanf(fu,"%d %d %d %d %d",&firstKeyCode, &secondKeyCode, &flyTime, &firstDwell, &secondDwell) != EOF)
    {
        ufly[z] = flyTime;
        udwell[z] = firstDwell;
        z++;
    }
    fclose(fu);

    //open other user file
    z = 0; //reset counter
    FILE *fo = fopen(ofile,"r");
    fgets(head, sizeof head, fo); //get rid of header line

    //read in fly time and dwell time
    while(fscanf(fo,"%d %d %d %d %d",&fkc, &skc, &ft, &fd, &sd) != EOF)
    {
        ofly[z] = ft;
        odwell[z] = fd;
        z++;
    }
    fclose(fo);

    //for 5 data sections, calculate false accept
    for(z=0;z<5;z++)
    {
        //calculate the sum of Di
		D = 0.0;
		sumDi = 0.0;
		sumMono = 0.0;
        for(x = 0; x < 500; x++)
        {
            if(ofly[section] > 0)
            {
                sumDi = sumDi + abs((float) ufly[x] - (float) ofly[section]) / (float) ofly[section];
            }
            section = section + 1;
        }

        //calculate the sum of mono
        section = 500 * (z+1); //reset counter
        for(x = 0; x < 500; x++)
        {
            if(odwell[section] > 0)
            {
                sumMono = sumMono + abs((float) udwell[x] - (float) odwell[section]) / (float) odwell[section];
            }
            section = section + 1;
        }

        //calculate deviation
		D = 0.0;
        D = (sumDi / 499 + sumMono / 500) * 50;
        if(D <= THR)
        {
            FArate = FArate+1;
        }
        printf("FA's D %f \n", D);
    }
    return FArate ;
}

//Authorization Module
void authorize(int id, int fnum, char option, int auth)
{
    if(auth == 1) //ACL
    {
        acl(id, fnum, option);
    }
    else
    {
        rbac(id, fnum, option);
    }
}

int main(void)
{
    int option;
    char user;
    char temp;
    int i;
    int id;
    int fnum, auth;
    int FR = 0;
    int FA = 0;
    float FAR;
    float FRR;

    printf("Enter user ID number: "); //1-5
    scanf("%c",&user) ;
    fgetc(stdin); //gets rid of new line

    printf("Enter file number: "); //1-6
    scanf("%d",&fnum) ;
    fgetc(stdin); //gets rid of new line

    printf("\n");
    printf("Enter (1) for read \n");
    printf("Enter (2) for write \n");
    printf("Enter (3) for execute \n");
    printf("Your selection: ");
    scanf("%d", &option); //fgets(option,1,stdin);
    fgetc(stdin); //gets rid of new line

    id = user - '0'; //convert char into int

    FR = calcFR(user); //calculate False Reject for given user
    printf("\n");

    for(i = 1; i < 6; i++)  //calculate False Accept for given user
    {
        temp = (char)(((int)'0')+i); //convert int to char
        if(temp != user) //not comparing with user itself
        {
            FA = FA + calcFA(user,temp); //FA += calc(i); //false accept
        }
    }

    FAR = FA / (float) 20; //4 users x 5 data blocks
    FRR = FR / (float) 25; //5 users x 5 data blocks

    printf("\n");
    printf("False Accept Rate is: %f\n", FAR);
    printf("False Reject Rate is: %f\n", FRR);

    if(avg >= THR)
    {
        printf("\n");
        printf("Enter (1) for ACL Authorization Module \n");
        printf("Enter (2) for RBAC Authorization Module \n");
        printf("Your selection: ");
        scanf("%d",&auth) ;
        fgetc(stdin);
        authorize(id, fnum, option, auth);
    }
    else
    {
        printf("Access Failed! \n");
    }

    printf("Press enter to continue...\n");
	getchar();
    return 0;
}
