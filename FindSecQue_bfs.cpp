#define _CRT_SECURE_NO_WARNINGS 

#include <stdio.h>
#include <vector>
#include <queue>
using namespace std;

#define MAX_N  10
#define MAX_M  10

struct State{
	int resources[MAX_M];             //ϵͳ������Դ����
	int available[MAX_M];             //ϵͳ������Դ������
	int claim[MAX_N][MAX_M];            //���̶Ը�����Դ����������
	int alloc[MAX_N][MAX_M];            //���̶Ը�����Դ���ѷ�����
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

	//��ʼ�����̸�������Դ������   N, M
	printf("\n\nEnter the N and M ! N ��ʾ���̵ĸ���, M��ʾ��Դ��������: ");
	scanf("%d%d", &N, &M);

	//��ʼ��   ������Դ����:resources,  ���õ���Դ����:available
	printf("\n��ʼ��%d����Դ������Resources: ", M);
	for (int i = 0; i < M; i++) {
		int x;
		scanf("%d", &x);
		state.resources[i] = x;
		state.available[i] = x;
	}

	//��ʼ��   �������̶���Դ���������:claim 
	printf("\n\n");
	for (int i = 0; i < N; i++) {
		printf("����%d�Ը�����Դ������Claim[%d]: ", i, i);
		for (int j = 0; j < M; j++) {
			int req;
			scanf("%d", &req);
			state.claim[i][j] = req;
		}
	}

	//��ʼ��   ���������Ѿ������˶��٣�alloc
	printf("\n\n");
	for (int i = 0; i < N; i++) {
		printf("����%d�Ѿ����������Դ����alloc[%d]: ", i, i);
		for (int j = 0; j < M; j++) {
			int req;
			scanf("%d", &req);
			state.alloc[i][j] = req;
			state.available[j] -= req;          //�Ѿ������ˣ���ô����Ҫ���ٿ�����
		}
	}
	printf("\n\n");

	//finished = false;
	for (int i = 0; i < N; i++) {
		state.finished[i] = false;
	}

	//curr ��ǰ������
	state.curr = 0;


	//�鿴����ϵͳ�����̵����
	printCondition(state);

	printf("---------��ȫ����S---------- \n");
	FindSecQue_bfs(state);
	printf("\n-------------------------- \n\n");

	system("pause");
}

void FindSecQue_bfs(State& S) {

	queue<State> q;

	for (int i = 0; i < N; i++) {                                   //���ҵ�һ�鰲ȫ���зŶ�������

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
		
		if (p.curr == N) {                                 //����Ѿ��ҵ���N����ȫ������
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

	printf("\n---------Claim����C---------- \n");
	printSymbolR();
	for (int i = 0; i < N; i++) {
		printf("-------P%d:  ", i);
		for (int j = 0; j < M; j++) {
			printf("%d  %c", S.claim[i][j], j == M - 1 ? '\n' : ' ');
		}
	}
	printf("-----------------------------\n\n");


	printf("---------Alloc����A---------- \n");
	printSymbolR();
	for (int i = 0; i < N; i++) {
		printf("-------P%d:  ", i);
		for (int j = 0; j < M; j++) {
			printf("%d  %c", S.alloc[i][j], j == M - 1 ? '\n' : ' ');
		}
	}
	printf("-----------------------------\n\n");


	printf("---------Need����N---------- \n");
	printSymbolR();
	for (int i = 0; i < N; i++) {
		printf("-------P%d:  ", i);
		for (int j = 0; j < M; j++) {
			printf("%d  %c", S.claim[i][j] - S.alloc[i][j], j == M - 1 ? '\n' : ' ');
		}
	}
	printf("-----------------------------\n\n");


	printf("---------Available����V---------- \n");
	printSymbolR();
	printf("---------   ");
	for (int i = 0; i < M; i++) {
		printf("%d  %c", S.available[i], i == M - 1 ? '\n' : ' ');
	}
	printf("-----------------------------\n\n");


	printf("---------Resource����R---------- \n");
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