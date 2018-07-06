#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h> 
#include <stdlib.h> 

#define maxsize 1000 //定义最大数组域

void FIFO(int [], int );
void SSTF(int [], int );
void SCAN(int [], int );
void CSCAN(int [], int );

// 操作界面 
int main()
{
	int c;
	int count;                      //int m=0; 
	int cidao[maxsize];             //定义磁道号数组 
	int i = 0;
	int b;
	printf("\n --------------------------------------------------\n");
	printf("       磁盘调度算法模拟");
	printf("\n --------------------------------------------------\n");

	printf("请先输入磁道数量：\n");
	scanf("%d", &b);
	
	printf("请先输入磁道序列：\n");
	for (i = 0; i < b; i++) {
		scanf("%d", &cidao[i]);
	}     

	printf("\n 磁道读取结果：\n");
	for (i = 0; i < b; i++)
	{
		printf("%d  ", cidao[i]);           //输出读取的磁道的磁道号
	}

	count = b;
	
	printf("\n           ");
	while (1) {
		printf("\n  算法选择：\n");   
		printf(" 1、先进先出算法（FIFO）\n");   
		printf(" 2、最短服务时间优先算法（SSTF）\n");   
		printf(" 3、扫描算法（SCAN）\n");  
		printf(" 4、循环扫描算法（C-SCAN）\n");
		printf(" 5. 退出\n");
		printf("\n");  	 	
		printf("请选择：");  
		scanf("%d", &c);  	 	
		if (c > 5)
			break;

		switch (c)                       //算法选择 
		{
			case 1:
				FIFO(cidao, count);     //先进先出算法  	 	
				printf("\n"); 
				break; 
			case 2:
				SSTF(cidao, count);    //最短服务时间优先算法 
				printf("\n");  	 	 	
				break;  	 	 	
			case 3:
				SCAN(cidao,count);//扫描算法，待补充！  	 	 	
				printf("\n");  	 	 	
				break;  	 	 	
			case 4: 
				CSCAN(cidao,count);//循环扫描算法，待补充！ 
				printf("\n");  	 	 	
				break;  	 	 	
			case 5:
				exit(0);
		}
	}
	return 0;
}

//先进先出调度算法 
void FIFO(int array[], int m)
{
	int sum = 0, j, i, now;
	float avg;
	
	printf("\n 请输入当前的磁道号：");
	scanf("%d", &now);

	printf("\n FIFO 调度结果:  ");
	printf("%d ", now);
	
	for (i = 0; i < m; i++)
		printf("%d ", array[i]);
	
	sum = abs(now - array[0]);
	
	for (j = 1; j < m; j++)
		sum += abs(array[j] - array[j - 1]);   //累计总的移动距离 
	
	avg = (float)sum / m;                      //计算平均寻道长度
	printf("\n 移动的总道数： %d \n", sum);
	printf(" 平均寻道长度： %f \n", avg);
}

//最短服务时间优先调度算法 
void SSTF(int array[], int m)
{
	int temp;
	int k = 1;
	int now, l, r;
	int i, j, sum = 0;
	float avg;
	for (i = 0; i < m; i++) {
		for (j = i + 1; j < m; j++)//对磁道号进行从小到大排列 
		{
			if (array[i] > array[j])//两磁道号之间比较 
			{
				temp = array[i];
				array[i] = array[j];
				array[j] = temp;
			}
		}
	}
	for (i = 0; i < m; i++)               //输出排序后的磁道号数组 
		printf("%d  ", array[i]);
	printf("\n 请输入当前的磁道号：");
	scanf("%d", &now);
	printf("\n SSTF 调度结果:  ");
	if (array[m - 1] <= now)             //判断整个数组里的数是否都小于当前磁道号 
	{
		for (i = m - 1; i >= 0; i--)     //将数组磁道号从大到小输出 
			printf("%d  ", array[i]);
		sum = now - array[0];           //计算移动距离 
	}
	else if (array[0] >= now)          //判断整个数组里的数是否都大于当前磁道号 
	{
		for (i = 0; i < m; i++)        //将磁道号从小到大输出 
			printf("%d  ", array[i]);
		sum = array[m - 1] - now;      //计算移动距离 
	}
	else {

		while (array[k] < now)        //逐一比较以确定 K 值
		{
			k++;
		}

		l = k - 1;
		r = k;

		//确定当前磁道在已排的序列中的位置
		while ((l >= 0) && (r < m)) {
			if ((now - array[l]) <= (array[r] - now))      //判断最短距离 
			{
				printf("%d  ", array[l]);
				sum += now - array[l];                    //计算移动距离 
				now = array[l];
				l = l - 1;
			}
			else {
				printf("%d  ", array[r]);
				sum += array[r] - now;                    //计算移动距离 
				now = array[r];
				r = r + 1;
			}
		}
		if (l = -1) {
			for (j = r; j < m; j++)
			{
				printf("%d  ", array[j]);
			}
			sum += array[m - 1] - array[0];               //计算移动距离 
		}
		else {
			for (j = l; j >= 0; j--)
			{
				printf("%d  ", array[j]);
			}
			sum += array[m - 1] - array[0];               //计算移动距离 
		}
	}
	avg = (float)sum / m;
	printf("\n 移动的总道数： %d \n", sum);
	printf(" 平均寻道长度： %f \n", avg);
}


void SCAN(int array[], int m) {
	//对磁道号进行从小到大排列 
	for (int i = 0; i < m; i++) {                
		for (int j = i + 1; j < m; j++)
		{
			if (array[i] > array[j])             //两磁道号之间比较 
			{
				int temp = array[i];
				array[i] = array[j];
				array[j] = temp;
			}
		}
	}
	int now;
	int max;
	printf("\n 最小的磁道号默认为0，请输入最大的磁道号：");
	scanf("%d", &max);
	printf("\n 请输入当前的磁道号：");
	scanf("%d", &now);
	printf("\n SCAN 调度结果:  ");
	int sum = 0;
	if (now <= array[0]) {
		for (int i = 0; i < m; i++) {
			printf("%d%c", array[i], i == m - 1 ? '\n':',');
		}
		sum += array[m-1] - now;
	} else if(now >= array[m - 1]){
		for (int i = m - 1 ; i >= 0; i--) {
			printf("%d%c", array[i], i == 0 ? '\n' : ',');
		}
		sum += now - array[0];
	} else {
		int k = 1;
		while (array[k] < now) { k++; }
		int l = k - 1;
		int r = k;
		for (int i = r; i < m ; i ++) { printf("%d,", array[i] ); }
		for (int i = l; i >= 0; i --) { printf("%d%c", array[i], i == 0 ? '\n':','); }
		sum += (array[m - 1] - now) + array[m - 1] + array[0];
	}
	float avg = (float)sum / m;
	printf("\n 移动的总道数： %d \n", sum);
	printf(" 平均寻道长度： %f \n", avg);
}

void CSCAN(int array[], int m) {
	//对磁道号进行从小到大排列 
	for (int i = 0; i < m; i++) {                
		for (int j = i + 1; j < m; j++)
		{
			if (array[i] > array[j])             //两磁道号之间比较 
			{
				int temp = array[i];
				array[i] = array[j];
				array[j] = temp;
			}
		}
	}
	int now, max;
	printf("\n 最小的磁道号默认为0，请输入最大的磁道号：");
	scanf("%d", &max);
	printf("\n 请输入当前的磁道号：");
	scanf("%d", &now);
	printf("\n SCAN 调度结果:  ");
	int sum = 0;
	if (now <= array[0]) {
		for (int i = 0; i < m; i++) { printf("%d", array[i]); }
		sum += array[m - 1] - now;
	} else if(now >= array[m - 1]){
		if (now == array[m - 1]) { printf("%d,", array[m - 1]); m--; }
		for (int i = 0; i < m; i++) { printf("%d%c", array[i], i == m - 1 ? '\n':':'); }
		sum += (max - now) + 1 + array[m -1 ];             // max - 当前的磁道      当前的磁道不用扫描，直接读取
	} else {
		int k = 1;
		while ( k < m && array[k] < now  ) { k++; }
		int l = k - 1;
		int r = k;

		for (int i = r; i < m; i++) {  printf("%d,", array[m - 1]); }
		for (int i = 0; i <= l; i++) { printf("%d%c", array[i], i == i?'\n':','); }
		sum += (max - now) + 1 + array[l];
	}

	float avg = (float)sum / m;
	printf("\n 移动的总道数： %d \n", sum);
	printf(" 平均寻道长度： %f \n", avg);
}