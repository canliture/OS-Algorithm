#define _CRT_SECURE_NO_WARNINGS

/*
char* �滻: LPSTR
const char* �滻: LPCSTR
WCHAR* �滻: LPWSTR
const WCHAR* �滻: LPCWSTR (C��W֮ǰ, ��Ϊ const �� WCHAR֮ǰ)
TCHAR* �滻: LPTSTR
const TCHAR* �滻: LPCTSTR
*/

/*
//------------------------------1.2
#include <windows.h>
#include <iostream>
#include <stdio.h>

// �������ݹ����Ľ��̵Ŀ�¡���̲������� ID ֵ
void StartClone(int nCloneID)
{
	// ��ȡ���ڵ�ǰ��ִ���ļ����ļ���
	TCHAR szFilename[MAX_PATH];
	GetModuleFileName(NULL, szFilename, MAX_PATH);

	// ��ʽ�������ӽ��̵������в�֪ͨ�� EXE �ļ����Ϳ�¡ ID
	TCHAR szCmdLine[MAX_PATH];
	sprintf(szCmdLine, "\"%s\" %d", szFilename, nCloneID);

	// �����ӽ��̵� STARTUPINFO �ṹ
	STARTUPINFO si;                                                   //���ڲ���ֵ���������̺����ñ��ı�
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si); // �����Ǳ��ṹ�Ĵ�С
						// ���ص������ӽ��̵Ľ�����Ϣ
	
	PROCESS_INFORMATION pi;                                           //���ڲ���ֵ���������̺����ñ��ı�
	// ����ͬ���Ŀ�ִ���ļ��������д������̣����������ӽ��̵�����

	BOOL bCreateOK = ::CreateProcess(
		szFilename, // ������� EXE ��Ӧ�ó��������             
		szCmdLine, // ��������Ϊ��һ���ӽ��̵ı�־           
		NULL, // ȱʡ�Ľ��̰�ȫ��                      
		NULL, // ȱʡ���̰߳�ȫ��                    
		FALSE, // ���̳о��                        
		CREATE_NEW_CONSOLE, // ʹ���µĿ���̨       
		NULL, // �µĻ���                         
		NULL, // ��ǰĿ¼                      
		&si, // ������Ϣ                       
		&pi); // ���صĽ�����Ϣ               
			  // ���ӽ����ͷ�����               

	if (bCreateOK)
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}

	//CloseHandleֻ�ǵݼ���������ü����������ǹرս��̣�CloseHandleֻ�Ǹ��߲���ϵͳ���ǶԽ��̵�ͳ�Ƽ���������Ȥ���ѡ�
	//���ӻ�(����) ��  ң����(���)  �Ұ� ң����(���)��ذ���, ���Ӳ���Ӱ���
}

int main(int argc, char* argv[])
{
	// ȷ���������������̣������������ڽ����б��е�λ��
	int nClone = 0;
	//�޸���䣺int nClone;
	//��һ���޸ģ�nClone=0;
	
	if (argc > 1)
	{
		// �ӵڶ�����������ȡ��¡ ID
		::sscanf(argv[1], "%d", &nClone);
	}
	
	//�ڶ����޸ģ�nClone = 0;
	// ��ʾ����λ��
	std::cout << "Process ID:" << ::GetCurrentProcessId()
		<< ", Clone ID:" << nClone
		<< std::endl;
	// ����Ƿ��д����ӽ��̵���Ҫ
	const int c_nCloneMax = 5;
	if (nClone < c_nCloneMax){
		// �����½��̵������кͿ�¡��
		StartClone(++nClone);
	}
	// �ȴ���Ӧ���������������
	getchar();
	return 0;
}

*/
//-------------------------1.3                                         
// procterm ��Ŀ

#include <windows.h>
#include <iostream>
#include <stdio.h>

static LPCTSTR g_szMutexName = "w2kdg.ProcTerm.mutex.Suicide";

// ������ǰ���̵Ŀ�¡���̵ļ򵥷���
void StartClone()
{
	// ��ȡ��ǰ��ִ���ļ����ļ���
	TCHAR szFilename[MAX_PATH];
	GetModuleFileName(NULL, szFilename, MAX_PATH);          // one.exe child

	// ��ʽ�������ӽ��̵������У��ַ�����child������Ϊ�βδ��ݸ��ӽ��̵� main ����
	TCHAR szCmdLine[MAX_PATH];
	//ʵ�� 1-3 ���� 3�����¾��е��ַ��� child ��Ϊ����ַ��������±���ִ�У�ִ��ǰ���ȱ����Ѿ���ɵĹ���
	sprintf(szCmdLine, "\"%s\" child", szFilename);

	printf("\n Cloned process excuse cmdLine: %s\n", szCmdLine);

	// �ӽ��̵�������Ϣ�ṹ
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si); // Ӧ���Ǵ˽ṹ�Ĵ�С
						// ���ص������ӽ��̵Ľ�����Ϣ

	PROCESS_INFORMATION pi;
	// ��ͬ���Ŀ�ִ���ļ����������д������̣���ָ������һ���ӽ���

	BOOL bCreateOK = CreateProcess(
		szFilename, // ������Ӧ�ó�������� (�� EXE �ļ�)
		szCmdLine, // ������������һ���ӽ��̵ı�־
		NULL, // ���ڽ��̵�ȱʡ�İ�ȫ��
		NULL, // �����̵߳�ȱʡ��ȫ��
		FALSE, // ���̳о��
		CREATE_NEW_CONSOLE, //�����´���
		NULL, // �»���
		NULL, // ��ǰĿ¼
		&si, // ������Ϣ�ṹ
		&pi); // ���صĽ�����Ϣ
			  // �ͷ�ָ���ӽ��̵�����

	if (bCreateOK)
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}

void Parent()
{
	// ��������ɱ�����������
	HANDLE hMutexSuicide = CreateMutex(
		NULL, // ȱʡ�İ�ȫ��
		TRUE, // ���ӵ�е�
		g_szMutexName); // ����������

	if (hMutexSuicide != NULL)
	{
		// �����ӽ���
		std::cout << "Creating the child process." << std::endl;
		StartClone();
		// ָ���ӽ��̡�ɱ��������
		std::cout << "Telling the child process to quit. " << std::endl;
		//�ȴ������̵ļ�����Ӧ
		getchar();
		//�ͷŻ����������Ȩ������źŻᷢ�͸��ӽ��̵� WaitForSingleObject ����
		ReleaseMutex(hMutexSuicide);
		// �������
		CloseHandle(hMutexSuicide);
	}
}

void Child()
{
	// �򿪡���ɱ��������
	HANDLE hMutexSuicide = OpenMutex(
		SYNCHRONIZE,                      // ������ͬ��
		FALSE,                            // ����Ҫ���´���
		g_szMutexName);                   // ����

	if (hMutexSuicide != NULL)
	{
		// �����������ڵȴ�ָ��
		std::cout << "Child waiting for suicide instructions. " << std::endl;

		//�ӽ��̽�������״̬���ȴ�������ͨ�������巢�����ź�
		WaitForSingleObject(hMutexSuicide, INFINITE);
		//ʵ�� 1-3 ���� 4�����Ͼ��Ϊ WaitForSingleObject(hMutexSuicide, 0) �����±���ִ��
		//WaitForSingleObject(hMutexSuicide, 0);
		// ׼������ֹ��������
		std::cout << "Child quiting." << std::endl;
		CloseHandle(hMutexSuicide);

	}
}


int main(int argc, char* argv[])       // argv  [ one.exe�� 123�� 1312 ] 
{	
	                                   // sub   [ one.exe, child ]
	// ��������Ϊ�Ǹ����̻����ӽ���
	if (argc>1 && ::strcmp(argv[1], "child") == 0){
		printf("Call Child Function !\n");
		Child();
	} else {
		printf("Call Parent Function !\n");
		Parent();
	}
	return 0;
}


