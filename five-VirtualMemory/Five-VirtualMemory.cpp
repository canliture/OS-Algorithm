/*
std::setw ����Ҫ�����ٸ��ַ�,Ĭ�������ַ�Ϊ' '�ո�
std::setfill������std::setw�����ʲô�����ַ�,��:std::setfill('*')

	//���:    1
	std::cout<<std::setw(4)<<a<<std::endl;
	//���: ***1
	std::cout<<std::setw(4)<<std::setfill('*')<<a<<std::endl;

*/

#define _CRT_SECURE_NO_WARNINGS

// ����vmwalker 
#include <windows.h> 
#include <iostream> 
#include <vector>
#include <algorithm>
#include <shlwapi.h> 
#include <iomanip> 
#pragma comment(lib, "Shlwapi.lib") 

struct Record{
	char* pBlock;
	char* pEnd;
	char* szSize;
	char* state;
	char* protect;
	char* type;
	char* module_szFilename;


	Record() {
		pBlock = (char*)malloc(sizeof(char) * 32);
		pEnd = (char*)malloc(sizeof(char) * 32);
		szSize = (char*)malloc(sizeof(char) * 32);
		state = (char*)malloc(sizeof(char) * 32);
		protect = (char*)malloc(sizeof(char) * 32);
		type = (char*)malloc(sizeof(char) * 32);
		module_szFilename = (char*)malloc(sizeof(char) * 32);
	}

	~Record() {
		pBlock = NULL;
		pEnd = NULL;
		szSize = NULL;
		state = NULL;
		protect = NULL;
		type = NULL;
		module_szFilename = NULL;
	}

	bool operator <(const Record &other)const   //��������    
	{
		if (strcmp(state, other.state) == 0) {
			if (strcmp(pBlock, other.pBlock) > 0)
				return false;
			else
				return true;
		}
		else if (strcmp(state, other.state) > 0) {
			return false;
		}
		else {
			return true;
		}
	}

};



bool cmp(const Record& p, const Record& q) {
	if (strcmp(p.state, q.state) == 0) {
		if (strcmp(p.pBlock, q.pBlock) > 0)
			return false;
		else
			return true;
	} else if (strcmp(p.state, q.state) > 0) {
		return false;
	}
	else {
		return true;
	}
}

std::vector<Record> v;

// �Կɶ���ʽ���û���ʾ�����ĸ��������� 
// ������Ǳ�ʾ����Ӧ�ó�����ڴ���з��ʵ����� 
// �Լ�����ϵͳǿ�Ʒ��ʵ����� 
inline bool TestSet(DWORD dwTarget, DWORD dwMask)
{
	return ((dwTarget &dwMask) == dwMask);
}

#define SHOWMASK(dwTarget, type) \
if (TestSet(dwTarget, PAGE_##type) ) \
 {std :: cout << ", " << #type; /*printf("__%s__", #type);*/ return (char*)#type; }


char* ShowProtection(DWORD dwTarget)
{
	SHOWMASK(dwTarget, READONLY);
	SHOWMASK(dwTarget, GUARD);
	SHOWMASK(dwTarget, NOCACHE);
	SHOWMASK(dwTarget, READWRITE);
	SHOWMASK(dwTarget, WRITECOPY);
	SHOWMASK(dwTarget, EXECUTE);
	SHOWMASK(dwTarget, EXECUTE_READ);
	SHOWMASK(dwTarget, EXECUTE_READWRITE);
	SHOWMASK(dwTarget, EXECUTE_WRITECOPY);
	SHOWMASK(dwTarget, NOACCESS);
}

// �������������ڴ沢���û���ʾ�����ԵĹ�������ķ��� 
void WalkVM(HANDLE hProcess)
{
	

	// ���ȣ����ϵͳ��Ϣ 
	SYSTEM_INFO si;
	::ZeroMemory(&si, sizeof(si));
	::GetSystemInfo(&si);

	// ����Ҫ�����Ϣ�Ļ����� 
	MEMORY_BASIC_INFORMATION mbi;
	::ZeroMemory(&mbi, sizeof(mbi));

	// ѭ������Ӧ�ó����ַ�ռ� 
	LPCVOID pBlock = (LPVOID)si.lpMinimumApplicationAddress;     
	while (pBlock < si.lpMaximumApplicationAddress)
	{
		Record rec;                             

		// �����һ�������ڴ�����Ϣ
		if (::VirtualQueryEx(
			hProcess,                           // ��صĽ��� 
			pBlock,                            // ��ʼλ��    
			&mbi,						      // ������             
			sizeof(mbi))==sizeof(mbi) )      // ��С��ȷ�� 
		{
		    // �����Ľ�β�����С 
			LPCVOID pEnd = (PBYTE)pBlock + mbi.RegionSize;
			TCHAR szSize[MAX_PATH];
			::StrFormatByteSize(mbi.RegionSize, szSize, MAX_PATH);
			
			// 1.��ʾ���ַ�ʹ�С      
			std::cout.fill('0');             
			std::cout
				<< std::hex << std::setw(8) << (DWORD)pBlock
				<< "-"
				<< std::hex << std::setw(8) << (DWORD)pEnd
				<< (::strlen(szSize) == 7 ? " (" : " (") << szSize
				<< ") ";
			
			sprintf(rec.pBlock, "%08x", (DWORD)pBlock);
			sprintf(rec.pEnd, "%08x", (DWORD)pEnd);
			sprintf(rec.szSize, "%7s", szSize);

			// 2.��ʾ���״̬ 
			switch (mbi.State)
			{
				case MEM_COMMIT:
					std::cout << "Committed";  
					sprintf(rec.state, "%-9s", "Committed");
					break;                 
				case MEM_FREE:
					std::cout << "Free";  
					sprintf(rec.state, "%-9s", "Free");
					break;
				case MEM_RESERVE:
					std::cout << "Reserved";  
					sprintf(rec.state, "%-9s", "Reserved");
					break;
			}


			// 3.��ʾ����    
			if(mbi.Protect==0 && mbi.State!=MEM_FREE) 
				{    mbi.Protect = PAGE_READONLY;     }
			char* tmp = ShowProtection(mbi.Protect);

			sprintf(rec.protect, "%-17s  ", tmp);

			// 4.��ʾ����             
			switch(mbi.Type){                 
				case MEM_IMAGE : 
					std::cout << ", Image";
					sprintf(rec.type, "%-7s", "Image");
					break;
				case MEM_MAPPED:      
					std::cout << ", Mapped"; 
					sprintf(rec.type, "%-7s", "Mapped");
					break;
				case MEM_PRIVATE:
					std::cout << ", Private"; 
					sprintf(rec.type, "%-7s", "Private");
					break;
				default:
					sprintf(rec.type, "%-7s", "��");
			}

			// 5.�����ִ�е�Ӱ�� 
			TCHAR szFilename[MAX_PATH];             
			if (::GetModuleFileName(
				(HMODULE)pBlock,		 // ʵ�������ڴ��ģ����     
				szFilename,              //��ȫָ�����ļ����� 
				MAX_PATH) > 0)           //ʵ��ʹ�õĻ�������С 
			{
				// ��ȥ·������ʾ 
				::PathStripPath(szFilename);                 
				std::cout << ", Module: " << szFilename;
				sprintf(rec.module_szFilename, "%s", szFilename);
			}
			else {
				sprintf(rec.module_szFilename, "%s", "��");
			}

			std::cout << std::endl;

			//printf("%s - %s = %s, %s , %s, %s, %s \n", rec.pBlock, rec.pEnd, rec.szSize, rec.state, rec.protect, rec.type, rec.module_szFilename);
			
			v.push_back(rec);

			// �ƶ���ָ���Ի����һ�¸���             
			pBlock = pEnd;
		}
	}
}

void ShowVirtualMemory()
{
	// ���ȣ������ǻ��ϵͳ��Ϣ 
	SYSTEM_INFO si;
	::ZeroMemory(&si, sizeof(si));
	::GetSystemInfo(&si);

	// ʹ����Ǹ��������һЩ�ߴ���и�ʽ�� 
	TCHAR szPageSize[MAX_PATH];
	TCHAR szMemSize[MAX_PATH];

	DWORD dwMemSize = (DWORD)si.lpMaximumApplicationAddress - (DWORD)si.lpMinimumApplicationAddress;     //�ڴ��С

	::StrFormatByteSize(si.dwPageSize, szPageSize, MAX_PATH);     	                                                                      
	::StrFormatByteSize(dwMemSize, szMemSize, MAX_PATH);           

	// ���ڴ���Ϣ��ʾ���� 
	std::cout << "Virtual memory page size: " << szPageSize << std::endl;                                 
	std::cout.fill('0');     
	std::cout << "Minimum application address: 0x" << std::hex << std::setw(8)
			  << (DWORD)si.lpMinimumApplicationAddress
			  << std::endl;
	std::cout << "Maximum application address: 0x" << std::hex << std::setw(8)
			  << (DWORD)si.lpMaximumApplicationAddress
			  << std::endl;
	std::cout << "Total available virtual memory: "  << szMemSize << std::endl;
}

int main()
{
	//��ʾ�����ڴ�Ļ�����Ϣ 
	ShowVirtualMemory();

	// ������ǰ���̵������ڴ� 
	::WalkVM(::GetCurrentProcess());

	std::sort(v.begin(), v.end() );
	//std::sort(v.begin(), v.end(), cmp);

	printf("\n\n\n\n------------------------------------------------\n\n\n\n");
	printf("����������%d\n\n", v.size());
	std::vector<Record>::iterator it;
	printf("      ��   ַ      ��  С ��ַ����   ����Ȩ��            ��  ��\n");
	for (it = v.begin(); it != v.end(); it++) {
		printf("%s %s %s %s %s %s %s \n", (*it).pBlock, (*it).pEnd, (*it).szSize, (*it).state, (*it).protect, (*it).type, (*it).module_szFilename);
	}

	printf("\n\n");
	system("pause");
	return 0;
}
