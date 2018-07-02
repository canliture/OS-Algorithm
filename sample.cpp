#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <vector>
#include <set>
using namespace std;

struct State {
	vector<int > resources;
	vector<int > available;
	vector< vector<int> > claim;
	vector< vector<int> > alloc;
} state;

int N;      //���̸���         N ������
int M;      //��Դ������       M ����Դ����

bool safe(const State& );               //�ж�״̬�Ƿ�ȫ
void printCondition(const State& );     //��ӡ��ǰϵͳ��������Դ����״̬
void printSymbolR();                    //UI����

int main() {

	printf("\n\nEnter the N and M ! N ��ʾ���̵ĸ���, M��ʾ��Դ��������: ");

	//��ʼ�����̸�������Դ������   N, M
	scanf("%d%d", &N, &M);         

	set<int> st;
	for (int i = 0; i < N; i++) {
		st.insert(i);
	}
	

	//��ʼ��   ������Դ����:resources,  ���õ���Դ����:available
	printf("��ʼ��%d����Դ������\n", M);
	for (int i = 0; i < M; i++) {
		int x;
		scanf("%d", &x);
		state.resources.push_back(x);
		state.available.push_back(x);
	}

	//��ʼ��   �������̶���Դ���������:claim,   ��Դ�Խ��̵ķ������: alloc;
	for (int i = 0; i < N; i++) {                         
		printf("����%d�Ը�����Դ������: \n", i);
		vector<int> line;                       //����һ��
		vector<int> zero;
 		for (int j = 0; j < M; j++) {
			int req;
			scanf("%d", &req);
			line.push_back(req);
			zero.push_back(0);
		}
		state.claim.push_back(line);       //��������
		state.alloc.push_back(zero);       //�ѷ���
	}
	


	//������������������������������������������������������������������������������ʼ���ȳ��� - - - - --> ���з�����Դ ����������������������������������������������������������������������������������������������������

	//ÿ�����̿�ʼ���ζ���Դ��������                            ���̵��� --- ���������˳���������
	vector<int> req;
	for (int i = 0; i < N; i = (i + 1) % N ) {                     //Ŀǰ��i������

		req.clear();

		printCondition(state);

		if (st.empty())
			break;
		if (st.find(i) == st.end())
			continue;

		printf("\n\n---------------------------------------------\n");

		printf("��%d�������������%d����Դ��Ŀ: ", i, M);
		for (int j = 0; j < M; j++) {
			int x;
			scanf("%d", &x);
			req.push_back(x);
		}

		bool reqError = false;                    //������������

		//������Դ����������ì�� ���� ���ڿ�����:    �ѷ��� + ���� > ���� ,  ���� > ϵͳ������
		for (int k = 0; k < M; k++) {

			if (state.alloc[i][k] + req[k] > state.claim[i][k]) {       //��i�����̵�k����Դ�ѷ������Դ + ����ĵ�k����Դ > ��i�����̵�k����Դ�������� ERROR
				printf("\n\n��������ì�ܣ����������Դ������������������Դ����������ǰ���̡�\n\n");
				reqError = true;
				break;                                         
			} else if(req[k] > state.available[k]){                     //����ĵ�k����Դ���ڿ��õĵ�k����Դ  ERROR
				printf("����ĵ�%d����Դ�������������õ�����, ֱ��������ǰ����!!\n", k);
				//������ǰ����
				reqError = true;
				break;
			}
		}

		//���req����û�д���Ļ�, ������������Ƿ�ȫ��        ����д���Ļ������������Ƚ�����һ������(������һ��ѭ��)��   
		if (reqError == false) {

			//����һ���µ�״̬, �ȿ���֮ǰ��״̬    
			struct State newState;       
			newState.resources.assign(state.resources.begin(), state.resources.end() );
			newState.available.assign(state.available.begin(), state.available.end() );
			for (int k = 0; k < N; k++) {
				vector<int> tmp;
			
				tmp.assign(state.claim[k].begin(), state.claim[k].end());
				newState.claim.push_back( tmp );

				tmp.assign(state.alloc[k].begin(), state.alloc[k].end());
				newState.alloc.push_back(tmp);
			}

			//�µ�״̬
			for (int k = 0; k < M; k++) {
				newState.alloc[i][k] += req[k];
				newState.available[k] -= req[k];
			}

			//�ж��Ƿ����ڰ�ȫ״̬ safe()
			if ( safe( newState) ) {

				//newState Ϊ���ڵ�״̬  state = newState
				for (int k = 0; k < M; k++) {
					state.alloc[i][k] += req[k];
					state.available[k] -= req[k];
				}

				//������ֽ���i ������Դ�Ѿ���ȫ�����Լ��������ˣ� Ҳ����˵���н����ˡ�   �ͷ�������Դ
				bool endProcessI = true;
				for (int k = 0; k < M; k++) {
					if (state.alloc[i][k] != state.claim[i][k]) {
						endProcessI = false;
						break;
					}
				}
				if (endProcessI) {
					for (int k = 0; k < M; k++) {
						state.available[k] += state.claim[i][k];
						state.claim[i][k] = 0;
						state.alloc[i][k] = 0;
					}
					st.erase(i);
				}

				//printCondition(state);

			} else {
				//state Ϊ���ڵ�״̬   state != newState
				printf("unsafe!\n");
			}

		}
	}
	
	system("pause");
	return 0;
}


bool safe(const State& S) {

	vector<int > currentAvail;
	currentAvail.assign( S.available.begin(), S.available.end() );       //��ǰ����

	//û�������꣬  ��Ȼ���ڴ��еĽ��� 
	vector<int > restProcess;
	for (int i = 0; i < N; i++) {          //��ʼ����Ȼ�����еĳ���
		restProcess.push_back(i);
	}

	bool possible = true;
	
	while (possible) {

		//�ҵ�һ������k�� ʹ��������̶��κ�������Դ������С�ڵ�ǰ��Դ������
		bool found = true;
		int foundProcess = -1;
		for (int i = 0; i < restProcess.size(); i++) {         //����Ȼ���ڴ��еĽ�����ѡ
			found = true;
			for (int j = 0; j < M; j++) {
				if ( S.claim[ restProcess[i] ][j] - S.alloc[ restProcess[i] ][j] > currentAvail[j]) {           //�˽��̲�����������Ѱ����һ������
					found = false;
					break;
				}
			}
			if (found) {
				foundProcess = i;
				break;
			}
		}
		//����ҵ����������
		if (found && foundProcess != -1) {
			for (int k = 0; k < M; k++) {                              //����ҵ�����ʼ������Դ
				currentAvail[k] += S.alloc[ restProcess[foundProcess] ][k];
			}
			restProcess.erase(restProcess.begin() + foundProcess);
		} else {
			possible = false;
		}
	}

	//return false /* rest == null   ��Ȼ�����ڴ��еĽ������Ƿ�Ϊ0 */;
	return (restProcess.size() == 0);
}

void printCondition(const State& S) {

	printf("\n---------Claim����C---------- \n");
	printSymbolR();
	for (int i = 0; i < N; i++) {
		printf("-------P%d:  ", i);
		for (int j = 0; j < M; j++) {
			printf("%d  %c" , S.claim[i][j], j==M-1?'\n':' ');
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