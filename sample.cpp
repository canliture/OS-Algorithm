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

int N;      //进程个数         N 个进程
int M;      //资源种类数       M 种资源种类

bool safe(const State& );               //判断状态是否安全
void printCondition(const State& );     //打印当前系统，进程资源分配状态
void printSymbolR();                    //UI杂项

int main() {

	printf("\n\nEnter the N and M ! N 表示进程的个数, M表示资源的种类数: ");

	//初始化进程个数和资源种类数   N, M
	scanf("%d%d", &N, &M);         

	set<int> st;
	for (int i = 0; i < N; i++) {
		st.insert(i);
	}
	

	//初始化   各种资源总量:resources,  可用的资源总量:available
	printf("初始化%d类资源的总量\n", M);
	for (int i = 0; i < M; i++) {
		int x;
		scanf("%d", &x);
		state.resources.push_back(x);
		state.available.push_back(x);
	}

	//初始化   各个进程对资源的需求情况:claim,   资源对进程的分配情况: alloc;
	for (int i = 0; i < N; i++) {                         
		printf("进程%d对各类资源的需求: \n", i);
		vector<int> line;                       //输入一行
		vector<int> zero;
 		for (int j = 0; j < M; j++) {
			int req;
			scanf("%d", &req);
			line.push_back(req);
			zero.push_back(0);
		}
		state.claim.push_back(line);       //进程声明
		state.alloc.push_back(zero);       //已分配
	}
	


	//——————————————————————————————————————开始调度程序 - - - - --> 进行分配资源 ——————————————————————————————————————————————————

	//每个进程开始依次对资源进行请求                            进程调度 --- 按进程序号顺序进行请求
	vector<int> req;
	for (int i = 0; i < N; i = (i + 1) % N ) {                     //目前第i个进程

		req.clear();

		printCondition(state);

		if (st.empty())
			break;
		if (st.find(i) == st.end())
			continue;

		printf("\n\n---------------------------------------------\n");

		printf("第%d个进程请求分配%d类资源数目: ", i, M);
		for (int j = 0; j < M; j++) {
			int x;
			scanf("%d", &x);
			req.push_back(x);
		}

		bool reqError = false;                    //请求数量错误

		//请求资源数不能自相矛盾 或者 大于可利用:    已分配 + 请求 > 声明 ,  请求 > 系统可利用
		for (int k = 0; k < M; k++) {

			if (state.alloc[i][k] + req[k] > state.claim[i][k]) {       //第i个进程第k类资源已分配的资源 + 请求的第k类资源 > 第i个进程第k类资源的声明量 ERROR
				printf("\n\n出错，自相矛盾：你所求的资源总量大于你声明的资源量。阻塞当前进程。\n\n");
				reqError = true;
				break;                                         
			} else if(req[k] > state.available[k]){                     //请求的第k类资源大于可用的第k类资源  ERROR
				printf("请求的第%d类资源数量超过可利用的数量, 直接阻塞当前进程!!\n", k);
				//阻塞当前进程
				reqError = true;
				break;
			}
		}

		//如果req数量没有错误的话, 进行请求分配是否安全，        如果有错误的话，不处理，调度进入下一个进程(进入下一次循环)。   
		if (reqError == false) {

			//定义一个新的状态, 先拷贝之前的状态    
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

			//新的状态
			for (int k = 0; k < M; k++) {
				newState.alloc[i][k] += req[k];
				newState.available[k] -= req[k];
			}

			//判断是否属于安全状态 safe()
			if ( safe( newState) ) {

				//newState 为现在的状态  state = newState
				for (int k = 0; k < M; k++) {
					state.alloc[i][k] += req[k];
					state.available[k] -= req[k];
				}

				//如果发现进程i 分配资源已经完全满足自己的需求了， 也就是说运行结束了。   释放所有资源
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
				//state 为现在的状态   state != newState
				printf("unsafe!\n");
			}

		}
	}
	
	system("pause");
	return 0;
}


bool safe(const State& S) {

	vector<int > currentAvail;
	currentAvail.assign( S.available.begin(), S.available.end() );       //当前可用

	//没有运行完，  仍然在内存中的进程 
	vector<int > restProcess;
	for (int i = 0; i < N; i++) {          //初始化仍然在运行的程序
		restProcess.push_back(i);
	}

	bool possible = true;
	
	while (possible) {

		//找到一个进程k， 使得这个进程对任何种类资源的需求都小于当前资源可用量
		bool found = true;
		int foundProcess = -1;
		for (int i = 0; i < restProcess.size(); i++) {         //在仍然在内存中的进程中选
			found = true;
			for (int j = 0; j < M; j++) {
				if ( S.claim[ restProcess[i] ][j] - S.alloc[ restProcess[i] ][j] > currentAvail[j]) {           //此进程不符合条件，寻找下一个进程
					found = false;
					break;
				}
			}
			if (found) {
				foundProcess = i;
				break;
			}
		}
		//如果找到了这个进程
		if (found && foundProcess != -1) {
			for (int k = 0; k < M; k++) {                              //如果找到，开始分配资源
				currentAvail[k] += S.alloc[ restProcess[foundProcess] ][k];
			}
			restProcess.erase(restProcess.begin() + foundProcess);
		} else {
			possible = false;
		}
	}

	//return false /* rest == null   仍然留在内存中的进程数是否为0 */;
	return (restProcess.size() == 0);
}

void printCondition(const State& S) {

	printf("\n---------Claim矩阵C---------- \n");
	printSymbolR();
	for (int i = 0; i < N; i++) {
		printf("-------P%d:  ", i);
		for (int j = 0; j < M; j++) {
			printf("%d  %c" , S.claim[i][j], j==M-1?'\n':' ');
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