//--------------------------------------------------------------------------------------
// SymbolHelper.cpp
//
// Microsoft Game Technology Group.
// Copyright (C) Microsoft Corporation. All rights reserved.
//
// NCsoft - modified for use on PC
//--------------------------------------------------------------------------------------
#include "stdafx.h"
#include "SymbolHelper.h"
#include <string>
#include <dbghelp.h>

// We link with dbghelp.lib to make it convenient to call functions in dbghelp.dll,
// however we specify delay loading of dbghelp.dll so that we can load a specific
// version in LoadDbgHelp. This is crucial since otherwise we may get the version in
// the system directory, and then dbghelp.dll will not load symsrv.dll.
#pragma comment(lib, "dbghelp.lib")

// Name: LoadDbgHelp
// Desc: Forcibly loads DbgHelp.dll from %XEDK%\bin\win32. This is done by calling
//       LoadLibrary with a fully specified path, since otherwise the version in the
//       system directory will take precedence.
//--------------------------------------------------------------------------------------
BOOL LoadDbgHelp()
{
    // Get the app path
    std::string path = ""; // assume there is an appropriate dbghelp.dll in working directory

    // Create a fully specified path to the desired version of dbghelp.dll
    // This is necessary because symsrv.dll must be in the same directory
    // as dbghelp.dll, and only a fully specified path can guarantee which
    // version of dbghelp.dll we load.
    std::string dbgHelpPath = path + "dbghelp.dll";

    // Call LoadLibrary on DbgHelp.DLL with our fully specified path.
    HMODULE hDbgHelp = LoadLibrary(dbgHelpPath.c_str());

    // Print an error message and return FALSE if DbgHelp.DLL didn't load.
    if (!hDbgHelp)
    {
        printf("ERROR: Couldn't load DbgHelp.dll from %s\n", dbgHelpPath.c_str());
        return FALSE;
    }

    // DbgHelp.DLL loaded.
    return TRUE;
}

//--------------------------------------------------------------------------------------
// Name: cbSymbol
// Desc: DbgHelp callback which can be used for printing DbgHelp diagnostics.
//--------------------------------------------------------------------------------------
static BOOL CALLBACK cbSymbol(HANDLE /*hProcess*/, ULONG ActionCode, PVOID CallbackData, PVOID /*UserContext*/)
{
    switch (ActionCode)
    {
        case CBA_DEBUG_INFO:
            printf("%s", (PSTR)CallbackData);
            break;

        default:
            return false;
    }

    return true;
}

//--------------------------------------------------------------------------------------
// Name: SymbolHelper constructor
// Desc: Do necessary setup for locating and loading symbols. This includes initializing
//       COM, ensuring that the necessary DLLs are in the correct location, initializing
//       DbgHelp and passing the XEDK symbol server path on to DbgHelp.
//--------------------------------------------------------------------------------------
SymbolHelper::SymbolHelper()
{
    // Initialize COM.
    if (FAILED(CoInitialize(NULL)))
    {
        printf("CoInitialize failed.\n");
        exit(10);
    }

    // Load DbgHelp. This must be done before any DbgHelp functions are used,
    // and it only works if DbgHelp.dll is delay loaded.
    if (!LoadDbgHelp())
        exit(10);

    // Most DbgHelp functions use a 'process' handle to identify their context.
    // This can be virtually any number, except zero.
    m_DebugProcess = (HANDLE)1;

    // Enable DbgHelp debug messages, make sure that DbgHelp only loads symbols that
    // exactly match, and do deferred symbol loading for greater efficiency.
    SymSetOptions(SYMOPT_DEBUG | SYMOPT_EXACT_SYMBOLS | SYMOPT_DEFERRED_LOADS);

    // Now build up a complete symbol search path to give to DbgHelp.
    std::string fullSearchPath;

    // Add the current directory to the search path.
    fullSearchPath += std::string(";.");

    CHAR* ntSymbolPath;
    size_t symbolPathSize;
    errno_t err = _dupenv_s(&ntSymbolPath, &symbolPathSize, "_NT_SYMBOL_PATH");
    if (!err && ntSymbolPath)
    {
        fullSearchPath += ";" + std::string(ntSymbolPath);
        free(ntSymbolPath);
    }

    // Pass the symbol search path on to DbgHelp.
    SymInitialize(m_DebugProcess, const_cast<char*>(fullSearchPath.c_str()), FALSE);

    // Set up a callback to help debug symbol loading problems. If symsrv.dll can't be loaded
    // then this will print a message to that effect.
    // SymRegisterCallback( m_DebugProcess, cbSymbol, NULL );
}

//--------------------------------------------------------------------------------------
// Name: SymbolHelper destructor
// Desc: Cleanup the symbol helper class.
//--------------------------------------------------------------------------------------
SymbolHelper::~SymbolHelper()
{
    // Free our DIA2 objects prior to shutting down COM.
    m_ModuleList.clear();

    // Shut down COM.
    CoUninitialize();
}

// Walk module list and try to retrieve symbol information for the address in the
// address block, including the address, symbol name, offset, source file, line-number, etc.
bool SymbolHelper::ResolveAddress(AddressSymbol& as)
{
    bool success = false;
    CHAR symbolName[1000] = {0};
    ULONG displacement = 0;
    CHAR filename[500] = {0};
    ULONG lineNumber = 0;
    DWORD address = as.address;

    // Scan through the list of loaded modules to find the one that contains the
    // requested address.
    for (size_t i = 0; i < m_ModuleList.size(); ++i)
    {
        // Find what module's address range the address falls in.
        if (address > m_ModuleList[i].m_Address && address < m_ModuleList[i].m_Address + m_ModuleList[i].m_Size)
        {
            CComPtr<IDiaSession>& pSession = m_ModuleList[i].m_psession;

            // Find the symbol using the virtual address--the raw address. This
            // only works if you have previously told DIA where the module was
            // loaded, using put_loadAddress.
            // Specify SymTagPublicSymbol instead of SymTagFunction if you want
            // the full decorated names.
            IDiaSymbol* pFunc;
            HRESULT result = pSession->findSymbolByVA(address, SymTagFunction, &pFunc);
            if (SUCCEEDED(result) && pFunc)
            {
                // Get the name of the function.
                CComBSTR functionName = 0;
                pFunc->get_name(&functionName);
                if (functionName)
                {
                    // Convert the function name from wide characters to char.
                    sprintf_s(symbolName, "%S", functionName.m_str);

                    // Get the offset of the address from the symbol's address.
                    ULONGLONG symbolBaseAddress;
                    pFunc->get_virtualAddress(&symbolBaseAddress);
                    displacement = address - (ULONG)symbolBaseAddress;
                    success = true;

                    // Now try to get the filename and line number.
                    // Get an enumerator that corresponds to this instruction.
                    CComPtr<IDiaEnumLineNumbers> pLines;
                    const DWORD instructionSize = 4;
                    if (SUCCEEDED(pSession->findLinesByVA(address, instructionSize, &pLines)))
                    {
                        // We could loop over all of the source lines that map to this instruction,
                        // but there is probably at most one, and if there are multiple source
                        // lines we still only want one.
                        CComPtr<IDiaLineNumber> pLine;
                        DWORD celt;
                        if (SUCCEEDED(pLines->Next(1, &pLine, &celt)) && celt == 1)
                        {
                            // Get the line number.
                            pLine->get_lineNumber(&lineNumber);

                            // Get the source file object, and then its name.
                            CComPtr<IDiaSourceFile> pSrc;
                            pLine->get_sourceFile(&pSrc);
                            CComBSTR sourceName = 0;
                            pSrc->get_fileName(&sourceName);
                            // Convert from wide characters to ASCII.
                            sprintf_s(filename, "%S", sourceName.m_str);
                        }
                    }
                }
            }
        }
    }

    if (success)
    {
        // fill the resolve block for the caller
        as.displacement = displacement;
        as.lineNumber = lineNumber;
        strcpy_s(as.symbolName, sizeof(as.symbolName), symbolName);
        strcpy_s(as.filename, sizeof(as.filename), filename);
    }

    return success;
}

//--------------------------------------------------------------------------------------
// Name: PrintSymbolSummary
// Desc: Lookup the specified address in the previously loaded symbol files, and print
//       out a readable description of information about that address. The symbol
//       lookup will only succeed if the PDB files associated with the code modules are
//       still available.
//--------------------------------------------------------------------------------------
VOID SymbolHelper::PrintSymbolSummary(DWORD address)
{
    AddressSymbol as(address);

    bool success = ResolveAddress(as);
    if (success)
    {
        // print out human readable symbol information
        printf(as.str().c_str());
    }
    else
        printf("    %8X: Symbol lookup failed.\n", address);
}

//--------------------------------------------------------------------------------------
// Name: LoadSymbolsForModule
// Desc: Load the specified symbols using DbgHelp and DIA2. Return true for success.
//--------------------------------------------------------------------------------------
bool SymbolHelper::LoadSymbolsForModule(const VOID* baseAddress, size_t size, DWORD /*timeStamp*/, const char* aModulePath)
{
    // Create a DIA2 data source
    CComPtr<IDiaDataSource> pSource;
    HRESULT hr = CoCreateInstance(__uuidof(DiaSource), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiaDataSource), (void**)&pSource);

    if (FAILED(hr))
        return false;

    // Convert the filename to wide characters for use with DIA2.
    wchar_t wPdbPath[_MAX_PATH];
    size_t convertedChars;
    mbstowcs_s(&convertedChars, wPdbPath, aModulePath, _TRUNCATE);

    pSource->loadDataForExe(wPdbPath, NULL, NULL);
    if (FAILED(hr))
    {
        // Check the error code for details on why the load failed, which
        // could be because the file doesn't exist, signature doesn't match,
        // etc.
        return false;
    }

    // Create a session for the just loaded PDB file.
    CComPtr<IDiaSession> psession;
    if (FAILED(pSource->openSession(&psession)))
    {
        return false;
    }

    // Tell DIA2 where the module was loaded.
    psession->put_loadAddress((ULONG_PTR)baseAddress);

    // Add this session to a list of loaded modules.
    m_ModuleList.push_back(Module((ULONG_PTR)baseAddress, size, psession));
    return true;
}
