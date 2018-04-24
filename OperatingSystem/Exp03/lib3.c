#include <stdio.h>
#include <stdlib.h>

#define MaxNumber 200
int PageOrder[MaxNumber];
int PageNum,LackNum,MinBlockNum;
int PageDisCount[MaxNumber];
int LRUtime[MaxNumber];

float  LackPageRate;
int LackPageNum;
int VirtualQueue[MaxNumber];

void input();
void initial();
void FIFO();//先进先出
void OPI();//最佳置换
void LRU();//最久未使用
void display();

void input(){
	printf("请输入最小物理块数:");
	scanf("%d",&MinBlockNum);
	printf("请输入页面个数:");
	scanf("%d",&PageNum);
	printf("请输入页面序列:");
	for (int i=0;i<PageNum;i++){
		scanf("%d",&PageOrder[i]);
	}
}

void initial()
{
	LackPageNum = MinBlockNum;
	LackPageRate = 0.0;

	for(int i = 0;i<PageNum;i++){
		PageDisCount[i] = 0;
		VirtualQueue[i] = -1;
	}

	for (int i = 0;i<MinBlockNum;i++)
	{
		int isInQueue2 = 0;
		int dis = 0;
		LRUtime[i] = 0;
		for (int j = 0;j<MinBlockNum;j++)
			if (VirtualQueue[j] == PageOrder[i])
				isInQueue2 = 1;
		if (!isInQueue2){
			VirtualQueue[i] = PageOrder[i];
			for (int k = 0;k<i;k++){
				LRUtime[k]++;   //之前的页面对应的时间+1
			}
			display();
		}
		else
			LRUtime[i] = 0;  //重新更新为0，表示最近刚刚使用
	}
}

void FIFO()
{
	printf("FIFO算法页面置换结果如下:\n");;
	initial();
	int isInQueue;
	int point = 0;  //指向最老的页面
	for (int i = MinBlockNum;i<PageNum;i++){
		isInQueue = 0;
		for (int k = 0;k<MinBlockNum;k++){
			if (VirtualQueue[k] == PageOrder[i]){
				isInQueue = 1;
			}
		}

		if (!isInQueue){
			LackPageNum++;

			VirtualQueue[point] = PageOrder[i];
			display();
			point++;
			if (point == MinBlockNum){
				point = 0;
			}
		}
	}

	LackPageRate = (LackPageNum * 1.0)/PageNum;
	printf("缺页数为:%d\t",LackPageNum);
	printf("缺页率为:%.3f\n",LackPageRate);
}

void OPI()
{
	int isInQueue;
	int dis;   //表示队列每个值距离下一次访问的距离
	int point;  //指向最长时间未被访问的下标

	printf("OPI算法页面序列如下:\n");
	initial();

	for(int i = MinBlockNum;i<PageNum;i++){
		isInQueue = 0;
		for (int k = 0;k<MinBlockNum;k++){
			if (VirtualQueue[k] == PageOrder[i]){
				isInQueue = 1;
			}
		}

		if (!isInQueue){
			LackPageNum++;//计算当前队列每一页对应的下一次出现的距离
			for (int s = 0;s < MinBlockNum;s++){
				dis = 1;
				for (int t = i;t<PageNum;t++){
					if (VirtualQueue[s] != PageOrder[t]){
						dis++;
					}
					else{
						break;
					}
				}
				PageDisCount[s] = dis;
			}
			point = 0;
			for (int m = 1;m < MinBlockNum;m++){
				if (PageDisCount[point] < PageDisCount[m]){
					point = m;
				}
			}


			VirtualQueue[point] = PageOrder[i];
			display();

		}
	}

	LackPageRate = (LackPageNum*1.0)/PageNum;
	printf("缺页数为:%d\t",LackPageNum);
	printf("缺页率为:%.3f\n",LackPageRate);
}

void LRU()
{
	printf("最久未使用算法页面置换情况如下:\n");

	initial();
	int isInQueue;
	int point,k;  //指向最长时间未被访问的下标

	for(int i = MinBlockNum;i<PageNum;i++){
		isInQueue = 0;
		for (k = 0;k<MinBlockNum;k++){
			if (VirtualQueue[k] == PageOrder[i]){
				isInQueue = 1;
			}
		}

		if (!isInQueue){
			LackPageNum++;
			point = 0;
			for (int j = 1;j<MinBlockNum;j++){
				if (LRUtime[point]<LRUtime[j]){
					point = j;
				}
			}

			for (int s = 0;s<MinBlockNum;s++){
				if (VirtualQueue[s] != VirtualQueue[point]){
					LRUtime[s]++;
				}
			}

			VirtualQueue[point] = PageOrder[i];
			LRUtime[point] = 0;

			display();
		}else{
			for (int s = 0;s<MinBlockNum;s++){
				if (VirtualQueue[s] != PageOrder[i]){
					LRUtime[s]++;
				}
				else
					LRUtime[s] = 0;
			}
		}
	}//for

	LackPageRate = (LackPageNum*1.0)/PageNum;
	printf("缺页数为:%d\t",LackPageNum);
	printf("缺页率为:%.3f\n",LackPageRate);
}

void display(){
	for (int i = 0;i<MinBlockNum && VirtualQueue[i]>=0;i++){
		printf("%d\t",VirtualQueue[i]);
	}
	printf("\n");
}

int main(){
	input();
	int chooseAlgorithm;

	while(1)
	{
		printf("1使用FIFO算法\n");
		printf("2使用最佳页面置换算法\n");
		printf("3使用最久未使用算法\n");
        printf("4退出\n");
        printf("请选择:\n");
		scanf("%d",&chooseAlgorithm);
		switch(chooseAlgorithm)
		{
		case 1:
			FIFO();
			break;
		case 2:
			OPI();
			break;
		case 3:
			LRU();
			break;
		case 4:
			return 0;
		default:
			printf("输入错误,请重新输入\n\n");break;
		}
	}
	return 0;
}

