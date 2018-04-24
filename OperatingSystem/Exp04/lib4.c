#include <stdio.h>
#include <stdlib.h>
#define FALSE 0
#define TRUE 1
#define W 10
#define R 20
int  ProNum;
int  ResKind;
int  AllResource[W];
int  MAX[W][R];
int  Available[R];
int  Allocation[W][R];
int  NEED[W][R];
int  Request[R];

void showdata()
{
    int  i,j;
    printf("\nResourse available:\n");
    for(j=0;j<ResKind;j++)
        printf("R%d :%d ",j,Available[j]);
    printf("\n\n");
    printf("need:\n");
    for(j=0;j<ResKind;j++)
        printf("\tR%d",j);
    printf("\n");
    for(i=0;i<ProNum;i++)
        for(i=0;i<ProNum;i++)
        {
            printf("P%d:",i);
            for(j=0;j<ResKind;j++)
                printf("\t%d",NEED[i][j]);
            printf("\n");
        }
    printf("\n");
    printf("allocation:\n");
    for(j=0;j<ResKind;j++)
        printf("\tR%d",j);
    printf("\n");
    for(i=0;i<ProNum;i++)
    {
        printf("P%d:",i);
        for(j=0;j<ResKind;j++)
            printf("\t%d",Allocation[i][j]);
        printf("\n");

    }
}

void changdata(int  k)
{
    int  j;
    for(j=0;j<ResKind;j++)
    {
        Available[j]=Available[j]-Request[j];
        Allocation[k][j]=Allocation[k][j]+Request[j];
        NEED[k][j]=NEED[k][j]-Request[j];
    }
}


void rstordata(int k)
{
    int j;
    for(j=0;j<ResKind;j++)
    {
        Available[j]=Available[j]+Request[j];
        Allocation[k][j]=Allocation[k][j]-Request[j];
        NEED[k][j]=NEED[k][j]+Request[j];
    }
}


int check(int s)
{	int WORK[R],FINISH[W];
    int i,j,k=0;
    for(i=0;i<ProNum;i++)FINISH[i]=FALSE;

    for(j=0;j<ResKind;j++)
    {
        WORK[j]=Available[j];
        i=s;
        do
        {
            if(FINISH[i]==FALSE&&NEED[i][j]<=WORK[j])
            {
                WORK[j]=WORK[j]+Allocation[i][j];
                FINISH[i]=TRUE;
                i=0;
            }
            else
            {i++;}
        }while(i<ProNum);

        for(i=0;i<ProNum;i++)
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
        while(i<0||i>=ProNum)
        {
            printf("Input the allocation process number（P0-P%d) :",ProNum-1);
            printf("p");
            scanf("%d",&i);
            if(i<0||i>=ProNum)
                printf("Invalid pid\n");
        }
        printf("Input process P%d alloc resouce:\n",i);
        for(j=0;j<ResKind;j++)
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
                if(Request[j]>Available[j])
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
    scanf("%d",&ProNum);
    printf("Resouce kind:");
    scanf("%d",&ResKind);
    printf("Input all resource:\n");
    for(i=0;i<ResKind;i++){
        printf("R%d ",i);
        scanf("%d",&AllResource[i]);
    }

    printf("Input the max resouce:\n");
    for(i=0;i<ProNum;i++){
        printf("P%i ",i);
        for(j=0;j<ResKind;j++){
            do{
                scanf("%d",&MAX[i][j]);
                if(MAX[i][j]>AllResource[j])
                    printf("Error:The process needs is bigger than annonced mount.\n");
            }while(MAX[i][j]>AllResource[j]);
        }
    }
    printf("Input the resouce allocation:\n");
    for(i=0;i<ProNum;i++)
    {
        printf("P%i ",i);
        for(j=0;j<ResKind;j++)
        {
            do
            {
                scanf("%d",&Allocation[i][j]);
                if(Allocation[i][j]>MAX[i][j])
                    printf("Error:Occupied resouce is bigger than annonced mount.\n");
            }while(Allocation[i][j]>MAX[i][j]);
        }
    }
    for(j=0;j<ResKind;j++){
        p=AllResource[j];
        for(i=0;i<ProNum;i++){
            p=p-Allocation[i][j];
            Available[j]=p;
            if(Available[j]<0)
                Available[j]=0;
        }
    }
    for(i=0;i<ProNum;i++)
        for(j=0;j<ResKind;j++)
            NEED[i][j]=MAX[i][j]-Allocation[i][j];
    showdata();
    bank();
    return 0;
}

