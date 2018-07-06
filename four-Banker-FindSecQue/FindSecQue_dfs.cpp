/*
 Function: �����û������״̬��
							 ϵͳ������Դ����   resources[M],
							 ϵͳ������Դ������ available[M],
							 ϵͳ�������̶Ը�����Դ���������� claim[N][M],
							 ϵͳ���������Ѿ�ռ�ø�����Դ���� alloc[N][M]);
							 ϵͳ�н����ǹ����н������ͷ���ռ�õ�������Դ finished[N];
		   �ҵ����еİ�ȫ����
 Time: 2018-6-27;
*/

#define _CRT_SECURE_NO_WARNINGS              

#include <stdio.h>
#include <vector>
#include <set>
#include <queue>
using namespace std;

struct State {                       
	vector<int > resources;             //ϵͳ������Դ����
	vector<int > available;             //ϵͳ������Դ������
	vector< vector<int> > claim;        //���̶Ը�����Դ����������
	vector< vector<int> > alloc;        //���̶Ը�����Դ���ѷ�����
	set<int > finished;                 //ĳ�������Ƿ��Ѿ��������
} state;

int N;      //���̸���         N ������
int M;      //��Դ������       M ����Դ����

vector<int > ans;                      //��ȫ����

void findSecQue_dfs(int);                 //�ҵ���ȫ����
void printCondition(const State&);    //��ӡ��ǰϵͳ��������Դ����״̬
void printSymbolR();                  //UI����

int main() {
	
	//��ʼ�����̸�������Դ������   N, M
	printf("\n\nEnter the N and M ! N ��ʾ���̵ĸ���, M��ʾ��Դ��������: ");
	scanf("%d%d", &N, &M);

	//��ʼ��   ������Դ����:resources,  ���õ���Դ����:available
	printf("\n��ʼ��%d����Դ������Resources: ", M);
	for (int i = 0; i < M; i++) {
		int x;
		scanf("%d", &x);
		state.resources.push_back(x);
		state.available.push_back(x);
	}

	//��ʼ��   �������̶���Դ���������:claim 
	printf("\n\n");
	for (int i = 0; i < N; i++) {
		printf("����%d�Ը�����Դ������Claim[%d]: ", i, i);
		vector<int> line;                           //����һ��
		for (int j = 0; j < M; j++) {
			int req;
			scanf("%d", &req);
			line.push_back(req);
		}
		state.claim.push_back(line);       //��������
	}

	//��ʼ��   ���������Ѿ������˶��٣�alloc
	printf("\n\n");
	for (int i = 0; i < N; i++) {
		printf("����%d�Ѿ����������Դ����alloc[%d]: ", i, i);
		vector<int> line;
		for (int j = 0; j < M; j++) {
			int req;
			scanf("%d", &req);
			line.push_back(req);

			state.available[j] -= req;          //�Ѿ������ˣ���ô����Ҫ���ٿ�����
		}
		state.alloc.push_back(line);
	}
	printf("\n\n");

	//��ʼ��   �����Ѿ�������е�����Щ  finished
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

	//�鿴����ϵͳ�����̵����
	printCondition(state);

	//���Ұ�ȫ����
	printf("---------��ȫ����S---------- \n");
	findSecQue_dfs(0);
	printf("\n-------------------------- \n\n");


	system("pause");
	return 0;
}

void findSecQue_dfs(int k) {

	if (k == N) {                                                        //����ҵ�N������
		for (unsigned i = 0; i < ans.size(); i++) {
			printf("P%d  %c", ans[i], i == ans.size() - 1 ? '\n' : ' ');
		}
		return;
	}
	                                                                   //k ��= N��û���ҵ�N�����̡���ô�Ҿ�һֱ������
	for (int i = 0; i < N; i++) {
		if (state.finished.find(i) == state.finished.end()) {          // ���i����û�������꣬��ô���Կ��ǡ��������ˣ�û��Ҫ���ǡ� 

			bool reqError = false;                                    //���������i������Դ���������꣬  �᲻�����

			for (int j = 0; j < M; j++) {
				int req = state.claim[i][j] - state.alloc[i][j];       //req��ĳһ����Դ������
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
				state.finished.insert(i);                        // ���� --> ����
				ans.push_back(i);

				findSecQue_dfs(k + 1);        //������һ��

				ans.pop_back();              //���ݣ�����i�����������ĸı�ָ� 
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

	printf("--------Finished����F---------- \n");
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