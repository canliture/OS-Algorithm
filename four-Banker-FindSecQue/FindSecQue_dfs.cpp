/*
 Function: 根据用户输入的状态（
							 系统各类资源总量   resources[M],
							 系统各类资源可用量 available[M],
							 系统各个进程对各类资源的总需求量 claim[N][M],
							 系统各个进程已经占用各种资源的量 alloc[N][M]);
							 系统中进程是够运行结束，释放所占用的所有资源 finished[N];
		   找到所有的安全序列
 Time: 2018-6-27;
*/

#define _CRT_SECURE_NO_WARNINGS              

#include <stdio.h>
#include <vector>
#include <set>
#include <queue>
using namespace std;

struct State {                       
	vector<int > resources;             //系统各类资源总量
	vector<int > available;             //系统各类资源可用量
	vector< vector<int> > claim;        //进程对各类资源的总需求量
	vector< vector<int> > alloc;        //进程对各类资源的已分配量
	set<int > finished;                 //某个进程是否已经运行完成
} state;

int N;      //进程个数         N 个进程
int M;      //资源种类数       M 种资源种类

vector<int > ans;                      //安全序列

void findSecQue_dfs(int);                 //找到安全序列
void printCondition(const State&);    //打印当前系统，进程资源分配状态
void printSymbolR();                  //UI杂项

int main() {
	
	//初始化进程个数和资源种类数   N, M
	printf("\n\nEnter the N and M ! N 表示进程的个数, M表示资源的种类数: ");
	scanf("%d%d", &N, &M);

	//初始化   各种资源总量:resources,  可用的资源总量:available
	printf("\n初始化%d类资源的总量Resources: ", M);
	for (int i = 0; i < M; i++) {
		int x;
		scanf("%d", &x);
		state.resources.push_back(x);
		state.available.push_back(x);
	}

	//初始化   各个进程对资源的需求情况:claim 
	printf("\n\n");
	for (int i = 0; i < N; i++) {
		printf("进程%d对各类资源的需求Claim[%d]: ", i, i);
		vector<int> line;                           //输入一行
		for (int j = 0; j < M; j++) {
			int req;
			scanf("%d", &req);
			line.push_back(req);
		}
		state.claim.push_back(line);       //进程声明
	}

	//初始化   各个进程已经分配了多少：alloc
	printf("\n\n");
	for (int i = 0; i < N; i++) {
		printf("进程%d已经分配各类资源的量alloc[%d]: ", i, i);
		vector<int> line;
		for (int j = 0; j < M; j++) {
			int req;
			scanf("%d", &req);
			line.push_back(req);

			state.available[j] -= req;          //已经分配了，那么必须要减少可用量
		}
		state.alloc.push_back(line);
	}
	printf("\n\n");

	//初始化   进程已经完成运行的有哪些  finished
	printf("\n\n");
	for (int i = 0; i < N; i++) {
		bool isFinished = true;
		for (int j = 0; j < M; j++) {
			if (state.claim[i][j] - state.alloc[i][j] > 0) {
				isFinished = false;
			}
		}
		if (isFinished)
			state.finished.insert(i);
	}
	printf("\n\n");

	//查看整个系统，进程的情况
	printCondition(state);

	//查找安全序列
	printf("---------安全序列S---------- \n");
	findSecQue_dfs(0);
	printf("\n-------------------------- \n\n");


	system("pause");
	return 0;
}

void findSecQue_dfs(int k) {

	if (k == N) {                                                        //如果找到N个进程
		for (unsigned i = 0; i < ans.size(); i++) {
			printf("P%d  %c", ans[i], i == ans.size() - 1 ? '\n' : ' ');
		}
		return;
	}
	                                                                   //k ！= N，没有找到N个进程。那么我就一直遍历找
	for (int i = 0; i < N; i++) {
		if (state.finished.find(i) == state.finished.end()) {          // 如果i进程没有运行完，那么可以考虑。运行完了，没必要考虑。 

			bool reqError = false;                                    //如果给进程i分配资源让它运行完，  会不会出错

			for (int j = 0; j < M; j++) {
				int req = state.claim[i][j] - state.alloc[i][j];       //req，某一类资源的需求
				if (req > state.available[j]) {
					reqError = true;
					break;
				}
			}
			if (reqError == false) {
				for (int j = 0; j < M; j++)
				{
					state.available[j] += state.alloc[i][j];
				}
				state.finished.insert(i);                        // 以下 --> 核心
				ans.push_back(i);

				findSecQue_dfs(k + 1);        //搜索下一层

				ans.pop_back();              //回溯，将第i个进程所做的改变恢复 
				state.finished.erase(i);
				for (int j = 0; j < M; j++)
				{
					state.available[j] -= state.alloc[i][j];
				}
			}
		}
	}
}


void printCondition(const State& S) {

	printf("\n---------Claim矩阵C---------- \n");
	printSymbolR();
	for (int i = 0; i < N; i++) {
		printf("-------P%d:  ", i);
		for (int j = 0; j < M; j++) {
			printf("%d  %c", S.claim[i][j], j == M - 1 ? '\n' : ' ');
		}
	}
	printf("-----------------------------\n\n");


	printf("---------Alloc矩阵A---------- \n");
	printSymbolR();
	for (int i = 0; i < N; i++) {
		printf("-------P%d:  ", i);
		for (int j = 0; j < M; j++) {
			printf("%d  %c", S.alloc[i][j], j == M - 1 ? '\n' : ' ');
		}
	}
	printf("-----------------------------\n\n");


	printf("---------Need矩阵N---------- \n");
	printSymbolR();
	for (int i = 0; i < N; i++) {
		printf("-------P%d:  ", i);
		for (int j = 0; j < M; j++) {
			printf("%d  %c", S.claim[i][j] - S.alloc[i][j], j == M - 1 ? '\n' : ' ');
		}
	}
	printf("-----------------------------\n\n");


	printf("---------Available矩阵V---------- \n");
	printSymbolR();
	printf("---------   ");
	for (int i = 0; i < M; i++) {
		printf("%d  %c", S.available[i], i == M - 1 ? '\n' : ' ');
	}
	printf("-----------------------------\n\n");


	printf("---------Resource矩阵R---------- \n");
	printSymbolR();
	printf("---------   ");
	for (int i = 0; i < M; i++) {
		printf("%d  %c", S.resources[i], i == M - 1 ? '\n' : ' ');
	}
	printf("-----------------------------\n\n");

	printf("--------Finished矩阵F---------- \n");
	for (set<int>::iterator it = state.finished.begin(); it != state.finished.end(); it++) {
		printf("%d", *it);
	}
	printf("\n-----------------------------\n\n");
}

void printSymbolR() {
	printf("---------   ");
	for (int i = 0; i < M; i++) {
		printf("R%d  ", i);
	}
	printf("\n");
}