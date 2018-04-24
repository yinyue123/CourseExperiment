//Coding:UTF-8
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MEMSIZE 3000
typedef struct memNode{
	int pid;
	char* begin;
	char* end;
	int len;
	struct memNode *next;
	struct memNode *pre;
}mem;


typedef struct quene{
	mem *head;
	mem *tail;
}Quene;

typedef struct proc_quene{
	int len;
	int pid;
	struct proc_quene *next;
}PQuene;

Quene busy;
Quene Free;
PQuene *pQuene;
char *MEM;
int choice;
int processnum=0;

void addFreeNode(mem *node){
	mem *pNode;
	if(Free.head==NULL){
		node->pre=NULL;
		node->next=NULL;
		Free.head=node;
		Free.tail=node;
		return;
	}
	switch(choice){
	case 1:
		node->pre=Free.tail;
		Free.tail->next=node;
		node->next=NULL;
		Free.tail=node;
		break;
	case 2:
	case 3:
		if(choice==2)
			for(pNode=Free.head;pNode!=NULL&&pNode->len<node->len;pNode=pNode->next);
		else
			for(pNode=Free.head;pNode!=NULL&&pNode->len>node->len;pNode=pNode->next);
		if(pNode==Free.tail){//last one
			node->pre=Free.tail;
			node->next=NULL;
			//Free.tail->next=node;//?
			Free.tail=node;
		}else if(pNode==Free.head){//first one
			node->pre=NULL;
			node->next=pNode;
			pNode->pre=node;
			Free.head=node;
		}else{
			if(node==NULL)
				return;
			node->next=pNode;
			node->pre=pNode->pre;
			pNode->pre->next=node;
			pNode->pre=node;


			//node->pre=pNode->pre;
			//pNode->pre->next=node;//?
			//pNode->pre=node;
		}
		break;
	}
}

void addBusyNode(mem *node){
	node->next=busy.head;
	busy.head=node;
}

void memInit(){
	int reqSize;
	int allocatedSize=0;
	int remainSize=MEMSIZE;
	int flag=0;
	
	mem* pNode;
	MEM=(char*)malloc(MEMSIZE*sizeof(char));
	printf("内存从%p到%p",MEM,(MEM+MEMSIZE));
	while(remainSize){
		while((reqSize=rand()%(MEMSIZE)/4)<2);
		pNode=(mem*)malloc(sizeof(mem));
		pNode->next=NULL;
		pNode->pre=NULL;
		if(remainSize<=reqSize){
			reqSize=remainSize;
		}
		pNode->begin=MEM+allocatedSize+1;
		pNode->end=MEM-reqSize-2;
		pNode->len=reqSize;
		remainSize-=reqSize;
		allocatedSize=reqSize;
		if(flag==0){//Alloc free space
			addFreeNode(pNode);
			flag=1;
		}else{
			processnum++;
			pNode->pid=processnum;
			addBusyNode(pNode);
			flag=0;
		}
	}
}



mem *deleteNode(Quene que,mem *delNode){
	mem* pNode=que.head;
	mem* preNode=pNode;
	for(;pNode!=NULL&&pNode!=delNode;preNode=pNode,pNode=pNode->next);
	if(pNode==NULL){
		printf("找不到此进程内存表\n");
		return NULL;
	}else if(pNode==que.head){
		que.head=pNode->next;
		que.head->pre=NULL;
	}else{
		pNode->next->pre=pNode->pre;
		preNode->next=pNode->next;
	}
	return pNode;
}

void printt(Quene *que,int symbol){
	mem *p;
	for(p=que->head;p!=NULL;p=p->next)
		if(symbol)//free
			printf("%p\t%p\t%d\n",p->begin,p->end,p->len);
		else//busy
			printf("%d\t%p\t%p\t%d\n",p->pid,p->begin,p->end,p->len);

}

void joinQuene(int pid,int n,int flag){
	PQuene *req=(PQuene*)malloc(sizeof(PQuene));
	PQuene *end;
	req->len=n;
	req->pid=pid;
	if(pQuene==NULL)
		pQuene=req;
	else
		for(end=pQuene;end->next!=NULL;end=end->next);
	end->next=pQuene;
}

void *req(int pid,int n,int flag){
	if(pQuene!=NULL && flag==1){
		joinQuene(pid,n,flag);
		printf("内存分配失败，加入队列\n");
		return NULL;
	}else{
		mem *pNode=Free.head;
		for(;pNode!=NULL&&pNode->len<n;pNode=pNode->next);
		if(pNode==NULL){
			joinQuene(pid,n,flag);
			printf("没有剩余空间，加入请求队列中\n");
			return NULL;
		}else{
			mem* busyNode=(mem*)malloc(sizeof(mem));
			busyNode->begin=pNode->begin;
			if(pNode->len-n<2){
				busyNode->len=pNode->len;
				busyNode->end=pNode->end;
				deleteNode(Free,pNode);
			}else{
				busyNode->len=n;
				busyNode->end=busyNode->begin+n;
				pNode->len=pNode->len-n;
				pNode->begin=pNode->begin+n+1;
			}
			busyNode->pid=pid;
			addBusyNode(busyNode);
			printf("进程%d申请内存成功",pid);
			printf("空闲分区:\nStart\t\tEnd\t\tSize\n");
			printt(&Free,1);
			printf("已用分区:\nPID\tStart\t\tEnd\t\tSize\n");
			printt(&busy,0);
			return busyNode;
		}
	}
}

void mm_release(int pid){
	mem *delNode;
	mem *FreeNode=(mem*)malloc(sizeof(mem));
	mem *pNode;
	for(delNode=busy.head;delNode!=NULL && delNode->pid==pid;delNode=delNode->next);
	if(delNode==NULL){
		printf("没有找到对应PID的进程\n");
		return;
	}

	FreeNode->begin=delNode->begin;
	FreeNode->end=delNode->end;
	FreeNode->len=delNode->len;
	FreeNode->next=NULL;
	for(pNode=Free.head;pNode!=NULL;pNode=pNode->next)
		if(pNode->end+1==FreeNode->begin){
			FreeNode->len=FreeNode->len+delNode->len;
			FreeNode->begin=delNode->begin;
			break;
		}

	pNode=deleteNode(busy,delNode);
	free(pNode);
}

int main(){
	char opt;
	busy.head=NULL;
	busy.tail=NULL;
	Free.head=NULL;
	Free.tail=NULL;
	srand(time(NULL));
	pQuene=NULL;
	printf("1.最先适应\t2.最佳适应\t3.最坏适应\n>");
	scanf("%d",&choice);
	if(choice!=1 && choice!=2 && choice!=3){
		printf("输入错误\n");
		return 0;
	}
	memInit();
	printf("内存总大小:%d",MEMSIZE);
	printf("空闲分区:\nStart\t\tEnd\t\tSize\n");
	printt(&Free,1);
	printf("已用分区:\nPID\tStart\t\tEnd\t\tSize\n");
	printt(&busy,0);
	while(opt){
		int i;
		printf("r:申请内存\tf:释放内存\tq:退出\n");
		scanf("%c %d",&opt,&i);
		if(opt=='r'){
			processnum++;
			req(processnum,i,1);
		}else if(opt=='f'){
			mm_release(i);
		}else if(opt=='q'){
			break;
		}else printf("错误输入\n");
	}
	return 0;

}
