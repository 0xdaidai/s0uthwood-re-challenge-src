#include<stdio.h>
#include<windows.h>

void main() {
    //1.���Ҵ���
    HWND hWnd = ::FindWindow(NULL, TEXT("INIT"));
    if (NULL == hWnd)
    {
        return;
    }
    /*2.��ý��̵�PID,��Ȼͨ�õ�������ѽ���PID����Ҫע��ĳ���,����������
    �ڴ�����.����򵥱�д,����PID���Կ��ձ�����ȡ
    */
    DWORD dwPid = 0;
    DWORD dwTid = 0;
    dwTid = GetWindowThreadProcessId(hWnd, &dwPid);

    //3.�򿪽���
    HANDLE hProcess = NULL;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    if (NULL == hProcess)
    {
        return;
    }
    //4.�ɹ���,����Զ���ڴ�
    void* lpAddr = NULL;
    lpAddr = VirtualAllocEx(hProcess, 0, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (NULL == lpAddr)
    {
        return;
    }
    //5.д�����ǵ�DLL·��,������д�뵱ǰ��Ŀ¼�µ�·��
    char szBuf[] = "chall.dll";
    BOOL bRet = WriteProcessMemory(hProcess, lpAddr, szBuf, strlen(szBuf) + 1, NULL);
    if (!bRet)
    {
        return;
    }
    //6.�����߳�Tid,���߳̾��
    HANDLE hThread = NULL;
    hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, dwTid);
    if (NULL == hThread)
    {
        return;
    }
    //7.��APC�����в���ص�����
    QueueUserAPC((PAPCFUNC)LoadLibraryA, hThread, (ULONG_PTR)lpAddr);

    CloseHandle(hThread);
    CloseHandle(hProcess);
}