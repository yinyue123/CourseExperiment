#include <stdio.h>
#include <stdlib.h>
#define FALSE 0
#define TRUE 1
#define W 10
#define R 20
int  M;
int  N;
int  ALL_RESOURCE[W];
int  MAX[W][R];
int  AVAILABLE[R];
int  ALLOCATION[W][R];
int  NEED[W][R];
int  Request[R];

void showdata()
{
    int  i,j;
    printf("\nResourse available:\n");
    for(j=0;j<N;j++)
        printf("R%d :%d ",j,AVAILABLE[j]);
    printf("\n\n");
    printf("need:\n");
    for(j=0;j<N;j++)
        printf("\tR%d",j);
    printf("\n");
    for(i=0;i<M;i++)
        for(i=0;i<M;i++)
        {
            printf("P%d:",i);
            for(j=0;j<N;j++)
                printf("\t%d",NEED[i][j]);
            printf("\n");
        }
    printf("\n");
    printf("allocation:\n");
    for(j=0;j<N;j++)
        printf("\tR%d",j);
    printf("\n");
    for(i=0;i<M;i++)
    {
        printf("P%d:",i);
        for(j=0;j<N;j++)
            printf("\t%d",ALLOCATION[i][j]);
        printf("\n");

    }
}

void changdata(int  k)
{
    int  j;
    for(j=0;j<N;j++)
    {
        AVAILABLE[j]=AVAILABLE[j]-Request[j];
        ALLOCATION[k][j]=ALLOCATION[k][j]+Request[j];
        NEED[k][j]=NEED[k][j]-Request[j];
    }
}


void rstordata(int k)
{
    int j;
    for(j=0;j<N;j++)
    {
        AVAILABLE[j]=AVAILABLE[j]+Request[j];
        ALLOCATION[k][j]=ALLOCATION[k][j]-Request[j];
        NEED[k][j]=NEED[k][j]+Request[j];
    }
}


int check(int s)
{	int WORK[R],FINISH[W];
    int i,j,k=0;
    for(i=0;i<M;i++)FINISH[i]=FALSE;

    for(j=0;j<N;j++)
    {
        WORK[j]=AVAILABLE[j];
        i=s;
        do
        {
            if(FINISH[i]==FALSE&&NEED[i][j]<=WORK[j])
            {
                WORK[j]=WORK[j]+ALLOCATION[i][j];
                FINISH[i]=TRUE;
                i=0;
            }
            else
            {i++;}
        }while(i<M);

        for(i=0;i<M;i++)
            if(FINISH[i]==FALSE)
            {
                printf("System is not safe,fail to alloc!\n");
                return 1;
            }
    }
    printf("System issafe,alloc successfully!\n");
    return 0;
}

void bank()
{
    int i=0,j=0;
    char flag='Y';

    while(flag=='Y'||flag=='y')
    {
        i=-1;
        while(i<0||i>=M)
        {
            printf("Input the allocation process number（P0-P%d) :",M-1);
            printf("p");
            scanf("%d",&i);
            if(i<0||i>=M)
                printf("Invalid pid\n");
        }
        printf("Input process P%d alloc resouce:\n",i);
        for(j=0;j<N;j++)
        {
            printf("R%d: ",j);
            scanf("%d",&Request[j]);
            if(Request[j]>NEED[i][j])
            {
                printf("Process P%d alloc resouce amount is bigger than the process also need R%d.",i,j);
                printf(" The alloc is not reasonable. Fail to alloc.");
                flag='N';
                break;
            }
            else
            {
                if(Request[j]>AVAILABLE[j])
                {
                    printf("Process P%d alloc resouce amount is bigger than the process also need R%d.",i,j);
                    printf(" The alloc is not reasonable. Fail to alloc.");
                    flag='N';
                    break;
                }
            }
        }
        if(flag=='Y'||flag=='y')
        {
            changdata(i);
            if(check(i))
            {
                rstordata(i);
                showdata();
            }
            else
                showdata();
        }
        else
            showdata();
        getchar();
        printf("Continue(Y/N)?");
        scanf("%c",&flag);
    }
}

int  main()//主函数
{
    int i=0,j=0,p;
    printf("Process number:");
    scanf("%d",&M);
    printf("Resouce kind:");
    scanf("%d",&N);
    printf("Input all resource:\n");
    for(i=0;i<N;i++){
        printf("R%d ",i);
        scanf("%d",&ALL_RESOURCE[i]);
    }

    printf("Input the max resouce:\n");
    for(i=0;i<M;i++){
        printf("P%i ",i);
        for(j=0;j<N;j++){
            do{
                scanf("%d",&MAX[i][j]);
                if(MAX[i][j]>ALL_RESOURCE[j])
                    printf("Error:The process needs is bigger than annonced mount.\n");
            }while(MAX[i][j]>ALL_RESOURCE[j]);
        }
    }
    printf("Input the resouce allocation:\n");
    for(i=0;i<M;i++)
    {
        printf("P%i ",i);
        for(j=0;j<N;j++)
        {
            do
            {
                scanf("%d",&ALLOCATION[i][j]);
                if(ALLOCATION[i][j]>MAX[i][j])
                    printf("Error:Occupied resouce is bigger than annonced mount.\n");
            }while(ALLOCATION[i][j]>MAX[i][j]);
        }
    }
    for(j=0;j<N;j++){
        p=ALL_RESOURCE[j];
        for(i=0;i<M;i++){
            p=p-ALLOCATION[i][j];
            AVAILABLE[j]=p;
            if(AVAILABLE[j]<0)
                AVAILABLE[j]=0;
        }
    }
    for(i=0;i<M;i++)
        for(j=0;j<N;j++)
            NEED[i][j]=MAX[i][j]-ALLOCATION[i][j];
    showdata();
    bank();
    return 0;
}

