/*
std::setw ：需要填充多少个字符,默认填充的字符为' '空格
std::setfill：设置std::setw将填充什么样的字符,如:std::setfill('*')

	//输出:    1
	std::cout<<std::setw(4)<<a<<std::endl;
	//输出: ***1
	std::cout<<std::setw(4)<<std::setfill('*')<<a<<std::endl;

*/

#define _CRT_SECURE_NO_WARNINGS

// 工程vmwalker 
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

	bool operator <(const Record &other)const   //升序排序    
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

// 以可读方式对用户显示保护的辅助方法。 
// 保护标记表示允许应用程序对内存进行访问的类型 
// 以及操作系统强制访问的类型 
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

// 遍历整个虚拟内存并对用户显示其属性的工作程序的方法 
void WalkVM(HANDLE hProcess)
{
	

	// 首先，获得系统信息 
	SYSTEM_INFO si;
	::ZeroMemory(&si, sizeof(si));
	::GetSystemInfo(&si);

	// 分配要存放信息的缓冲区 
	MEMORY_BASIC_INFORMATION mbi;
	::ZeroMemory(&mbi, sizeof(mbi));

	// 循环整个应用程序地址空间 
	LPCVOID pBlock = (LPVOID)si.lpMinimumApplicationAddress;     
	while (pBlock < si.lpMaximumApplicationAddress)
	{
		Record rec;                             

		// 获得下一个虚拟内存块的信息
		if (::VirtualQueryEx(
			hProcess,                           // 相关的进程 
			pBlock,                            // 开始位置    
			&mbi,						      // 缓冲区             
			sizeof(mbi))==sizeof(mbi) )      // 大小的确认 
		{
		    // 计算块的结尾及其大小 
			LPCVOID pEnd = (PBYTE)pBlock + mbi.RegionSize;
			TCHAR szSize[MAX_PATH];
			::StrFormatByteSize(mbi.RegionSize, szSize, MAX_PATH);
			
			// 1.显示块地址和大小      
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

			// 2.显示块的状态 
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


			// 3.显示保护    
			if(mbi.Protect==0 && mbi.State!=MEM_FREE) 
				{    mbi.Protect = PAGE_READONLY;     }
			char* tmp = ShowProtection(mbi.Protect);

			sprintf(rec.protect, "%-17s  ", tmp);

			// 4.显示类型             
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
					sprintf(rec.type, "%-7s", "无");
			}

			// 5.检验可执行的影像 
			TCHAR szFilename[MAX_PATH];             
			if (::GetModuleFileName(
				(HMODULE)pBlock,		 // 实际虚拟内存的模块句柄     
				szFilename,              //完全指定的文件名称 
				MAX_PATH) > 0)           //实际使用的缓冲区大小 
			{
				// 除去路径并显示 
				::PathStripPath(szFilename);                 
				std::cout << ", Module: " << szFilename;
				sprintf(rec.module_szFilename, "%s", szFilename);
			}
			else {
				sprintf(rec.module_szFilename, "%s", "无");
			}

			std::cout << std::endl;

			//printf("%s - %s = %s, %s , %s, %s, %s \n", rec.pBlock, rec.pEnd, rec.szSize, rec.state, rec.protect, rec.type, rec.module_szFilename);
			
			v.push_back(rec);

			// 移动块指针以获得下一下个块             
			pBlock = pEnd;
		}
	}
}

void ShowVirtualMemory()
{
	// 首先，让我们获得系统信息 
	SYSTEM_INFO si;
	::ZeroMemory(&si, sizeof(si));
	::GetSystemInfo(&si);

	// 使用外壳辅助程序对一些尺寸进行格式化 
	TCHAR szPageSize[MAX_PATH];
	TCHAR szMemSize[MAX_PATH];

	DWORD dwMemSize = (DWORD)si.lpMaximumApplicationAddress - (DWORD)si.lpMinimumApplicationAddress;     //内存大小

	::StrFormatByteSize(si.dwPageSize, szPageSize, MAX_PATH);     	                                                                      
	::StrFormatByteSize(dwMemSize, szMemSize, MAX_PATH);           

	// 将内存信息显示出来 
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
	//显示虚拟内存的基本信息 
	ShowVirtualMemory();

	// 遍历当前进程的虚拟内存 
	::WalkVM(::GetCurrentProcess());

	std::sort(v.begin(), v.end() );
	//std::sort(v.begin(), v.end(), cmp);

	printf("\n\n\n\n------------------------------------------------\n\n\n\n");
	printf("数据条数：%d\n\n", v.size());
	std::vector<Record>::iterator it;
	printf("      地   址      大  小 地址类型   访问权限            描  述\n");
	for (it = v.begin(); it != v.end(); it++) {
		printf("%s %s %s %s %s %s %s \n", (*it).pBlock, (*it).pEnd, (*it).szSize, (*it).state, (*it).protect, (*it).type, (*it).module_szFilename);
	}

	printf("\n\n");
	system("pause");
	return 0;
}
