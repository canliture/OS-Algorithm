#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <stdio.h>

unsigned int readCount = 0;

HANDLE ReaderCountMutex;               //��������   �ź���
HANDLE WriterMutex;                   //д�߻�����
HANDLE Control;

DWORD WINAPI Writer(LPVOID);         //д���߳�
DWORD WINAPI Reader(LPVOID);         //�����߳�

bool p_ccontinue = true;           //���Ƴ������

const unsigned short READER_COUNT = 2;            //��������
const unsigned short WRITER_COUNT = 5;            //д������

//�߳����
const unsigned short THREADS_COUNT = READER_COUNT + WRITER_COUNT;
HANDLE hThreads[THREADS_COUNT];      //���̵߳�handle
DWORD readerID[READER_COUNT];      //�����̵߳ı�ʶ��
DWORD writerID[WRITER_COUNT];     //д���̵߳ı�ʶ��

int main() {

	ReaderCountMutex = CreateMutex(NULL, FALSE, NULL);    //��������   ������������
	WriterMutex = CreateMutex(NULL, FALSE, NULL);         //д�߻��⣬ һ��ֻ��һ��д�߽���д����

	//�������߳�
	for (int i = 0; i < READER_COUNT; i++) {
		hThreads[i] = CreateThread(NULL, 0, Reader, NULL, 0, &readerID[i]);
		if (hThreads[i] == NULL)
			return -1;
	}

	//����д�߳�
	for (int i = 0; i < WRITER_COUNT; i++) {
		hThreads[i + READER_COUNT] = CreateThread(NULL, 0, Writer, NULL, 0, &writerID[i]);
		if (hThreads[i] == NULL)
			return -1;
	}

	while (p_ccontinue) {                //���Ƴ������
		if ( getchar() )
			p_ccontinue = false;
	}

	return 0;
}

void READUNIT() {
	printf("%d readers is reading and now, %d is reading\n", readCount, GetCurrentThreadId());
}

void WRITEUNIT() {
	printf("I am writing, and I am %d \n", GetCurrentThreadId());
}

DWORD WINAPI Reader(LPVOID) {        //�����߳�

	while (p_ccontinue) {

		WaitForSingleObject(ReaderCountMutex, INFINITE);    //p(mutex);     
			readCount++;
			if (readCount == 1) {                     //������ǵ�һ�����ߣ���ô�ұ���ȴ�д��д�꣨��д�߻��⣩ 
				WaitForSingleObject(WriterMutex, INFINITE);    
			}
		ReleaseMutex(ReaderCountMutex);                     //V(mutex); 

		READUNIT();                //Read Unit
		Sleep(1500);
		
		WaitForSingleObject(ReaderCountMutex, INFINITE);    //p(mutex);     
			readCount--;
			if (readCount == 0) {               //�������ĿǰΨһһ�����߲������Ѿ������ˣ� ��ô�ҿ���֪ͨд���� 
				ReleaseMutex(WriterMutex);
			}
		ReleaseMutex(ReaderCountMutex);                     //V(mutex);
	}

	return 0;
}

DWORD WINAPI Writer(LPVOID) {         //д���߳�

	while (p_ccontinue) {
		WaitForSingleObject(WriterMutex, INFINITE);    //p(mutex);      //�ٽ�������
		  WRITEUNIT();                                //Write��Unit 	
		  Sleep(1500);
		ReleaseMutex(WriterMutex);                    //V(mutex); 
	}

	return 0;
}