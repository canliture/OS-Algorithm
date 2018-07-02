#define _CRT_SECURE_NO_WARNINGS 

#include <stdio.h>
#include <vector>
#include <queue>
using namespace std;

#define MAX_N  10
#define MAX_M  10

struct State{
	int resources[MAX_M];             //系统各类资源总量
	int available[MAX_M];             //系统各类资源可用量
	int claim[MAX_N][MAX_M];            //进程对各类资源的总需求量
	int alloc[MAX_N][MAX_M];            //进程对各类资源的已分配量
	bool finished[MAX_N];
	vector<int> curQue;
	int curr;
} state;

int N;
int M;

void FindSecQue_bfs(State& );
void printCondition(const State& );
void printSymbolR();

int main() {

	//初始化进程个数和资源种类数   N, M
	printf("\n\nEnter the N and M ! N 表示进程的个数, M表示资源的种类数: ");
	scanf("%d%d", &N, &M);

	//初始化   各种资源总量:resources,  可用的资源总量:available
	printf("\n初始化%d类资源的总量Resources: ", M);
	for (int i = 0; i < M; i++) {
		int x;
		scanf("%d", &x);
		state.resources[i] = x;
		state.available[i] = x;
	}

	//初始化   各个进程对资源的需求情况:claim 
	printf("\n\n");
	for (int i = 0; i < N; i++) {
		printf("进程%d对各类资源的需求Claim[%d]: ", i, i);
		for (int j = 0; j < M; j++) {
			int req;
			scanf("%d", &req);
			state.claim[i][j] = req;
		}
	}

	//初始化   各个进程已经分配了多少：alloc
	printf("\n\n");
	for (int i = 0; i < N; i++) {
		printf("进程%d已经分配各类资源的量alloc[%d]: ", i, i);
		for (int j = 0; j < M; j++) {
			int req;
			scanf("%d", &req);
			state.alloc[i][j] = req;
			state.available[j] -= req;          //已经分配了，那么必须要减少可用量
		}
	}
	printf("\n\n");

	//finished = false;
	for (int i = 0; i < N; i++) {
		state.finished[i] = false;
	}

	//curr 当前序列数
	state.curr = 0;


	//查看整个系统，进程的情况
	printCondition(state);

	printf("---------安全序列S---------- \n");
	FindSecQue_bfs(state);
	printf("\n-------------------------- \n\n");

	system("pause");
}

void FindSecQue_bfs(State& S) {

	queue<State> q;

	for (int i = 0; i < N; i++) {                                   //先找第一组安全序列放队列里面

		bool reqError = false;
		for (int j = 0; j < M; j++) {
			if (S.claim[i][j] - S.alloc[i][j] > S.available[j]) {
				reqError = true;
				break;
			}
		}
		if (reqError == false) {

			for (int j = 0; j < M; j++) {
				S.available[j] += S.alloc[i][j];
			}
			S.finished[i] = true;
			S.curQue.push_back(i);
			S.curr = 1;
			
			q.push(S);

			S.curr = 0;
			S.curQue.pop_back();
			S.finished[i] = false;
			for (int j = 0; j < M; j++) {
				S.available[j] -= S.alloc[i][j];
			}
		}
	}

	while (!q.empty()) {
		
		State p = q.front();
		q.pop();
		
		if (p.curr == N) {                                 //如果已经找到了N个安全序列了
			for (unsigned i = 0; i < p.curQue.size(); i++) {
				printf("P%d  %c", p.curQue[i], i == p.curQue.size() - 1 ? '\n' : ' ');
			}
			printf("\n");
		}

		for (int i = 0; i < N; i++) {
			if (p.finished[i] == false) {

				bool reqError = false;
				for (int j = 0; j < M; j++) {
					if (p.claim[i][j] - p.alloc[i][j] > p.available[j]) {
						reqError = true;
						break;
					}
				}
				if (reqError == false) {

					for (int j = 0; j < M; j++) {
						p.available[j] += p.alloc[i][j];
					}
					p.finished[i] = true;
					p.curQue.push_back(i);
					p.curr++;

					q.push(p);

					p.curr--;
					p.curQue.pop_back();
					p.finished[i] = false;
					for (int j = 0; j < M; j++) {
						p.available[j] -= p.alloc[i][j];
					}
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

}

void printSymbolR() {
	printf("---------   ");
	for (int i = 0; i < M; i++) {
		printf("R%d  ", i);
	}
	printf("\n");
}