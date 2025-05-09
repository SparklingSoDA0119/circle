#if defined(_WIN64)

#include "libsoda/foundation/string_util.h"
#include "libsoda/foundation/system_util.h"
#include "libsoda/foundation/time.h"

#include <windows.h>
#include <TlHelp32.h>
#include <DbgHelp.h>

#pragma optimize("y", off)        //generate stack frame pointers for all functions - same as /Oy- in the project
#pragma warning(disable: 4200)    //nonstandard extension used : zero-sized array in struct/union
#pragma warning(disable: 4100)    //unreferenced formal parameter
#pragma warning(disable: 4996)

namespace {
    typedef    BOOL(WINAPI* MINIDUMP_WRITE_DUMP)(
        IN HANDLE           hProcess,
        IN DWORD            ProcessId,
        IN HANDLE           hFile,
        IN MINIDUMP_TYPE    DumpType,
        IN CONST PMINIDUMP_EXCEPTION_INFORMATION    ExceptionParam, OPTIONAL
        IN PMINIDUMP_USER_STREAM_INFORMATION        UserStreamParam, OPTIONAL
        IN PMINIDUMP_CALLBACK_INFORMATION           CallbackParam OPTIONAL
        );

    // Tool Help functions.
    typedef    HANDLE(WINAPI* CREATE_TOOL_HELP32_SNAPSHOT)(DWORD dwFlags, DWORD th32ProcessID);
    typedef    BOOL(WINAPI* MODULE32_FIRST)(HANDLE hSnapshot, LPMODULEENTRY32 lpme);
    typedef    BOOL(WINAPI* MODULE32_NEST)(HANDLE hSnapshot, LPMODULEENTRY32 lpme);

#define    DUMP_SIZE_MAX    8000    //max size of our dump
#define    CALL_TRACE_MAX   ((DUMP_SIZE_MAX - 2000) / (MAX_PATH + 40))    //max number of traced calls
#define    NL               L"\r\n"    //new line

    class MiniDump {
    private:
        int32_t _dumpNumber;

        CREATE_TOOL_HELP32_SNAPSHOT    _createToolhelp32Snapshot;
        MODULE32_FIRST _module32First;
        MODULE32_NEST _module32Next;

        HMODULE _hDbgHelp;
        MINIDUMP_WRITE_DUMP _writeDump;

    public:
        MiniDump() : _dumpNumber(0)
        {
            // Try to get MiniDumpWriteDump() address.
            _hDbgHelp = ::LoadLibrary(L"DBGHELP.DLL");
            _writeDump = (MINIDUMP_WRITE_DUMP)::GetProcAddress(_hDbgHelp, "MiniDumpWriteDump");
            printf("_hDbgHelp=%X, _writeDump=%X", _hDbgHelp, _writeDump);

            // Try to get Tool Help library functions.
            HMODULE    hKernel32 = GetModuleHandle(L"KERNEL32");
            _createToolhelp32Snapshot = (CREATE_TOOL_HELP32_SNAPSHOT)::GetProcAddress(hKernel32, "CreateToolhelp32Snapshot");
            _module32First = (MODULE32_FIRST)GetProcAddress(hKernel32, "Module32First");
            _module32Next = (MODULE32_NEST)GetProcAddress(hKernel32, "Module32Next");
        }

        ~MiniDump()
        {
            ::FreeLibrary(_hDbgHelp);
        }

        //****************************************************************************************
        BOOL WINAPI Get_Module_By_Ret_Addr(PBYTE Ret_Addr, TCHAR* Module_Name, PBYTE& Module_Addr)
            //****************************************************************************************
            // Find module by Ret_Addr (address in the module).
            // Return Module_Name (full path) and Module_Addr (start address).
            // Return TRUE if found.
        {
            MODULEENTRY32 M = { sizeof(M) };
            HANDLE hSnapshot;

            Module_Name[0] = 0;

            if (_createToolhelp32Snapshot) {
                hSnapshot = _createToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);
                if ((hSnapshot != INVALID_HANDLE_VALUE) &&
                    _module32First(hSnapshot, &M)) {
                    do {
                        if (DWORD(Ret_Addr - M.modBaseAddr) < M.modBaseSize) {
                            lstrcpyn(Module_Name, M.szExePath, MAX_PATH);
                            Module_Addr = M.modBaseAddr;
                            break;
                        }
                    } while (_module32Next(hSnapshot, &M));
                }

                CloseHandle(hSnapshot);
            }
            return Module_Name[0] != 0;
        } //Get_Module_By_Ret_Addr

        //******************************************************************
        int WINAPI Get_Call_Stack(PEXCEPTION_POINTERS pException, TCHAR* Str)
            //******************************************************************
            // Fill Str with call stack info.
            // pException can be either GetExceptionInformation() or NULL.
            // If pException = NULL - get current call stack.
        {
            TCHAR   Module_Name[MAX_PATH];
            PBYTE   Module_Addr = 0;
            PBYTE   Module_Addr_1;
            int     Str_Len;

            typedef struct STACK
            {
                STACK* Ebp;
                PBYTE   Ret_Addr;
                DWORD   Param[0];
            } STACK, * PSTACK;

            STACK   Stack = { 0, 0 };
            PSTACK  Ebp;

            if (pException) {       //fake frame for exception address
#if !defined(_WIN64)
                Stack.Ebp = (PSTACK)pException->ContextRecord->Ebp;
#else
                Stack.Ebp = (PSTACK)pException->ContextRecord->Rbp;
#endif
                Stack.Ret_Addr = (PBYTE)pException->ExceptionRecord->ExceptionAddress;
                Ebp = &Stack;
            }
            else {
                Ebp = (PSTACK)&pException - 1;    //frame addr of Get_Call_Stack()

                // Skip frame of Get_Call_Stack().
                if (!IsBadReadPtr(Ebp, sizeof(PSTACK)))
                    Ebp = Ebp->Ebp;        //caller ebp
            }

            Str[0] = 0;
            Str_Len = 0;

            // Trace CALL_TRACE_MAX calls maximum - not to exceed DUMP_SIZE_MAX.
            // Break trace on wrong stack frame.
            for (int Ret_Addr_I = 0;
                (Ret_Addr_I < CALL_TRACE_MAX) && !IsBadReadPtr(Ebp, sizeof(PSTACK)) && !IsBadCodePtr(FARPROC(Ebp->Ret_Addr));
                Ret_Addr_I++, Ebp = Ebp->Ebp) {
                // If module with Ebp->Ret_Addr found.
                if (Get_Module_By_Ret_Addr(Ebp->Ret_Addr, Module_Name, Module_Addr_1)) {
                    if (Module_Addr_1 != Module_Addr) {   //new module
                        // Save module's address and full path.
                        Module_Addr = Module_Addr_1;
                        Str_Len += wsprintf(Str + Str_Len, NL L"%08X  %s", Module_Addr, Module_Name);
                    }

                    // Save call offset.
                    Str_Len += wsprintf(Str + Str_Len,
                        NL L"  +%08X", Ebp->Ret_Addr - Module_Addr);

                    // Save 5 params of the call. We don't know the real number of params.
                    if (pException && !Ret_Addr_I) {   //fake frame for exception address
                        Str_Len += wsprintf(Str + Str_Len, L"  Exception Offset");
                    }
                    else if (!IsBadReadPtr(Ebp, sizeof(PSTACK) + 5 * sizeof(DWORD))) {
                        Str_Len += wsprintf(Str + Str_Len, L"  (%X, %X, %X, %X, %X)",
                            Ebp->Param[0], Ebp->Param[1], Ebp->Param[2], Ebp->Param[3], Ebp->Param[4]);
                    }
                }
                else {
                    Str_Len += wsprintf(Str + Str_Len, NL L"%08X", Ebp->Ret_Addr);
                }
            }

            return Str_Len;
        } //Get_Call_Stack

        //***********************************
        int WINAPI Get_Version_Str(TCHAR* Str)
            //***********************************
            // Fill Str with Windows version.
        {
            OSVERSIONINFOEX    V = { sizeof(OSVERSIONINFOEX) };    //EX for NT 5.0 and later

            if (!GetVersionEx((POSVERSIONINFO)&V))
            {
                ZeroMemory(&V, sizeof(V));
                V.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
                GetVersionEx((POSVERSIONINFO)&V);
            }

            if (V.dwPlatformId != VER_PLATFORM_WIN32_NT)
                V.dwBuildNumber = LOWORD(V.dwBuildNumber);    //for 9x HIWORD(dwBuildNumber) = 0x04xx

            return wsprintf(Str,
                NL L"Windows:  %d.%d.%d, SP %d.%d, Product Type %d",    //SP - service pack, Product Type - VER_NT_WORKSTATION,...
                V.dwMajorVersion, V.dwMinorVersion, V.dwBuildNumber, V.wServicePackMajor, V.wServicePackMinor, V.wProductType);
        } //Get_Version_Str

        //*************************************************************
        TCHAR* WINAPI Get_Exception_Info(PEXCEPTION_POINTERS pException)
            //*************************************************************
            // Allocate Str[DUMP_SIZE_MAX] and return Str with dump, if !pException - just return call stack in Str.
        {
            TCHAR* Str;
            int         Str_Len;
            TCHAR       Module_Name[MAX_PATH];
            PBYTE       Module_Addr;
            HANDLE      hFile;
            FILETIME    Last_Write_Time;
            FILETIME    Local_File_Time;
            SYSTEMTIME  T;

            Str = new TCHAR[DUMP_SIZE_MAX];

            if (!Str)
                return NULL;

            Str_Len = 0;
            Str_Len += Get_Version_Str(Str + Str_Len);

            Str_Len += wsprintf(Str + Str_Len, NL L"Process:  ");
            GetModuleFileName(NULL, Str + Str_Len, MAX_PATH);
            Str_Len = lstrlen(Str);

            // If exception occurred.
            if (pException)
            {
                EXCEPTION_RECORD& E = *pException->ExceptionRecord;
                CONTEXT& C = *pException->ContextRecord;

                // If module with E.ExceptionAddress found - save its path and date.
                if (E.ExceptionAddress) {
                    if (Get_Module_By_Ret_Addr((PBYTE)E.ExceptionAddress, Module_Name, Module_Addr))
                    {
                        Str_Len += wsprintf(Str + Str_Len,
                            NL L"Module:  %s", Module_Name);

                        if ((hFile = CreateFile(Module_Name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE)
                        {
                            if (GetFileTime(hFile, NULL, NULL, &Last_Write_Time))
                            {
                                FileTimeToLocalFileTime(&Last_Write_Time, &Local_File_Time);
                                FileTimeToSystemTime(&Local_File_Time, &T);

                                Str_Len += wsprintf(Str + Str_Len,
                                    NL L"Date Modified:  %02d/%02d/%d",
                                    T.wMonth, T.wDay, T.wYear);
                            }
                            CloseHandle(hFile);
                        }
                    }
                    else
                    {
                        if (E.ExceptionAddress) {
                            Str_Len += wsprintf(Str + Str_Len,
                                NL L"Exception Addr:  %08X", E.ExceptionAddress);
                        }
                    }
                }

                Str_Len += wsprintf(Str + Str_Len,
                    NL L"Exception Code:  %08X", E.ExceptionCode);

                if (E.ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
                {
                    // Access violation type - Write/Read.
                    Str_Len += wsprintf(Str + Str_Len,
                        NL L"%s Address:  %08X",
                        (E.ExceptionInformation[0]) ? L"Write" : L"Read", E.ExceptionInformation[1]);
                }

                // Save instruction that caused exception.
                Str_Len += wsprintf(Str + Str_Len, NL L"Instruction: ");
                if (E.ExceptionAddress) {
                    for (int i = 0; i < 16; i++) {
                        Str_Len += wsprintf(Str + Str_Len, L" %02X", PBYTE(E.ExceptionAddress)[i]);
                    }
                }

                // Save registers at exception.
                Str_Len += wsprintf(Str + Str_Len, NL L"Registers:");
#if !defined(_WIN64)
                Str_Len += wsprintf(Str + Str_Len, NL L"EAX: %08X  EBX: %08X  ECX: %08X  EDX: %08X", C.Eax, C.Ebx, C.Ecx, C.Edx);
                Str_Len += wsprintf(Str + Str_Len, NL L"ESI: %08X  EDI: %08X  ESP: %08X  EBP: %08X", C.Esi, C.Edi, C.Esp, C.Ebp);
                Str_Len += wsprintf(Str + Str_Len, NL L"EIP: %08X  EFlags: %08X", C.Eip, C.EFlags);
#else
                Str_Len += wsprintf(Str + Str_Len, NL L"RAX: %08X  RBX: %08X  RCX: %08X  RDX: %08X", C.Rax, C.Rbx, C.Rcx, C.Rdx);
                Str_Len += wsprintf(Str + Str_Len, NL L"RSI: %08X  RDI: %08X  RSP: %08X  RBP: %08X", C.Rsi, C.Rdi, C.Rsp, C.Rbp);
                Str_Len += wsprintf(Str + Str_Len, NL L"RIP: %08X  EFlags: %08X", C.Rip, C.EFlags);
#endif
            } //if (pException)

            // Save call stack info.
            Str_Len += wsprintf(Str + Str_Len, NL L"Call Stack:");
            Get_Call_Stack(pException, Str + Str_Len);

            if (Str[0] == NL[0])
                lstrcpy(Str, Str + sizeof(NL) - 1);

            return Str;
        } //Get_Exception_Info

        //*************************************************************************************
        int32_t WINAPI createDump(PEXCEPTION_POINTERS pException, const wchar_t* append)
            //*************************************************************************************
            // Create dump.
            // pException can be either GetExceptionInformation() or NULL.
            // If File_Flag = TRUE - write dump files (.dmz and .dmp) with the name of the current process.
            // If Show_Flag = TRUE - show message with Get_Exception_Info() dump.
        {
            TCHAR tname[MAX_PATH];
            ::GetModuleFileName(NULL, tname, MAX_PATH);    //path of current process

            soda::String path(tname);

            soda::String dumpPath;
            soda::Time t = soda::Time::currentTime();
            soda::String appendStr(append);
            dumpPath.format(L"%@.%@.%04d%02d%02d%02d%02d%02d_%d", &path, &appendStr, t.year(), t.month(), t.day(),
                                                                  t.hour(), t.min(), t.sec(), _dumpNumber++);

            // If MiniDumpWriteDump() of DbgHelp.dll available.
            if (_writeDump) {
                MINIDUMP_EXCEPTION_INFORMATION M;
                M.ThreadId = ::GetCurrentThreadId();
                M.ExceptionPointers = pException;
                M.ClientPointers = 0;
                soda::String path = dumpPath + L".dmp";

                HANDLE hFile = ::CreateFile(path.cString(),
                    GENERIC_WRITE, 0, NULL,
                    CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                if (hFile) {
                    _writeDump(::GetCurrentProcess(), ::GetCurrentProcessId(),
                        hFile, MiniDumpNormal, (pException) ? &M : NULL, NULL, NULL);
                    ::CloseHandle(hFile);
                }
            }
            return EXCEPTION_EXECUTE_HANDLER;
        }
    };

    MiniDump _miniDump;
}   // unnamed namespace


_namespace_soda_begin

int32_t WINAPI soda_dump(void* p, const wchar_t* fname)
{
    return ::_miniDump.createDump((PEXCEPTION_POINTERS)p, fname);
}

_namespace_soda_end

#endif