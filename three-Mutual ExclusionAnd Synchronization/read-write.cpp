#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <stdio.h>

unsigned int readCount = 0;

HANDLE ReaderCountMutex;               //读者数量   信号量
HANDLE WriterMutex;                   //写者互斥量
HANDLE Control;

DWORD WINAPI Writer(LPVOID);         //写者线程
DWORD WINAPI Reader(LPVOID);         //读者线程

bool p_ccontinue = true;           //控制程序结束

const unsigned short READER_COUNT = 2;            //读者数量
const unsigned short WRITER_COUNT = 5;            //写者数量

//线程相关
const unsigned short THREADS_COUNT = READER_COUNT + WRITER_COUNT;
HANDLE hThreads[THREADS_COUNT];      //各线程的handle
DWORD readerID[READER_COUNT];      //读者线程的标识符
DWORD writerID[WRITER_COUNT];     //写者线程的标识符

int main() {

	ReaderCountMutex = CreateMutex(NULL, FALSE, NULL);    //读者优先   读者数量互斥
	WriterMutex = CreateMutex(NULL, FALSE, NULL);         //写者互斥， 一次只能一个写者进行写操作

	//创建读线程
	for (int i = 0; i < READER_COUNT; i++) {
		hThreads[i] = CreateThread(NULL, 0, Reader, NULL, 0, &readerID[i]);
		if (hThreads[i] == NULL)
			return -1;
	}

	//创建写线程
	for (int i = 0; i < WRITER_COUNT; i++) {
		hThreads[i + READER_COUNT] = CreateThread(NULL, 0, Writer, NULL, 0, &writerID[i]);
		if (hThreads[i] == NULL)
			return -1;
	}

	while (p_ccontinue) {                //控制程序结束
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

DWORD WINAPI Reader(LPVOID) {        //读者线程

	while (p_ccontinue) {

		WaitForSingleObject(ReaderCountMutex, INFINITE);    //p(mutex);     
			readCount++;
			if (readCount == 1) {                     //如果我是第一个读者，那么我必须等待写者写完（与写者互斥） 
				WaitForSingleObject(WriterMutex, INFINITE);    
			}
		ReleaseMutex(ReaderCountMutex);                     //V(mutex); 

		READUNIT();                //Read Unit
		Sleep(1500);
		
		WaitForSingleObject(ReaderCountMutex, INFINITE);    //p(mutex);     
			readCount--;
			if (readCount == 0) {               //如果我是目前唯一一个读者并且我已经读完了， 那么我可以通知写者了 
				ReleaseMutex(WriterMutex);
			}
		ReleaseMutex(ReaderCountMutex);                     //V(mutex);
	}

	return 0;
}

DWORD WINAPI Writer(LPVOID) {         //写者线程

	while (p_ccontinue) {
		WaitForSingleObject(WriterMutex, INFINITE);    //p(mutex);      //临界区互斥
		  WRITEUNIT();                                //Write　Unit 	
		  Sleep(1500);
		ReleaseMutex(WriterMutex);                    //V(mutex); 
	}

	return 0;
}