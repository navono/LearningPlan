WORD(__stdcall *pGetModuleFileNameExW)(HANDLE hProcess,
	HMODULE hModule,
	LPTSTR lpFilename,
	DWORD nSize);

static std::wstring psapiDll = L"PSAPI.dll";

DWORD _stdcall GetExplorerToken(HANDLE* token)
{
	DWORD dwStatus = ERROR_FILE_NOT_FOUND;
	PROCESSENTRY32 pe32 = { 0 };
	HANDLE hProcessSnap = nullptr;
	HANDLE hProcess = nullptr;
	TCHAR szExplorerPath[MAX_PATH] = { 0 };
	TCHAR filename[MAX_PATH] = { 0 };
	HMODULE hPsapi = nullptr;
	__try
	{
		
		GetWindowsDirectory(szExplorerPath, MAX_PATH);
		_tcscat_s(szExplorerPath, MAX_PATH, L"\\Explorer.exe");
		hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hProcessSnap == INVALID_HANDLE_VALUE)
		{
			dwStatus = GetLastError();
			__leave;
		}

		hPsapi = LoadLibrary(psapiDll.c_str());
		if (hPsapi == nullptr)
		{
			dwStatus = GetLastError();
			__leave;
		}
		pGetModuleFileNameExW = (DWORD(__stdcall *) (HANDLE, HMODULE, LPTSTR, DWORD))
			GetProcAddress(hPsapi, "GetModuleFileNameExW");


		pe32.dwSize = sizeof(PROCESSENTRY32);
		if (!Process32First(hProcessSnap, &pe32))
		{
			dwStatus = GetLastError();
			__leave;
		}

		do 
		{
			hProcess = OpenProcess(
				PROCESS_ALL_ACCESS,
				FALSE,
				pe32.th32ProcessID);
			if (hProcess != nullptr)
			{
				if (pGetModuleFileNameExW(hProcess, nullptr, filename, MAX_PATH))
				{
					if (_tcsicmp(filename, szExplorerPath) == 0)
					{
						HANDLE hToken;
						if (OpenProcessToken(hProcess, TOKEN_ALL_ACCESS, &hToken))
						{
							*token = hToken;
							dwStatus = 0;
						}
						break;
					}
				}

				CloseHandle(hProcess);
				hProcess = nullptr;
			}
		} while (Process32Next(hProcessSnap, &pe32));

	}
	__finally
	{
		if (hPsapi != nullptr)
		{
			FreeLibrary(hPsapi);
			hPsapi = nullptr;
		}

		if (hProcess != nullptr)
		{
			CloseHandle(hProcess);
			hProcess = nullptr;
		}

		if (hProcessSnap != nullptr)
		{
			CloseHandle(hProcessSnap);
			hProcessSnap = nullptr;
		}
	}

	return dwStatus;
}

void CreateUserProcess()
{
	HANDLE hToken = nullptr;
	GetExplorerToken(&hToken);

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	STARTUPINFO si = { sizeof(STARTUPINFO),NULL
		,L"",NULL,0,0,0,0,0,0,0,STARTF_USESHOWWINDOW,0,0,NULL,0,0,0 };
	si.wShowWindow = SW_SHOW;
	si.lpDesktop = nullptr;

	CreateProcessAsUser(hToken, L"C:\\Windows\\System32\\notepad.exe",
		nullptr, nullptr, nullptr, FALSE,
		NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE,
		nullptr, nullptr, &si, &pi);

	WaitForSingleObject(pi.hProcess, INFINITE);
}