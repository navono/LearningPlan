#include "StdAfx.h"
#include "SqliteRecover.h"
#include <ShellAPI.h>

#pragma comment (lib, "Shell32.lib")

SqliteRecover::SqliteRecover(void)
{
}


SqliteRecover::~SqliteRecover(void)
{
}

void SqliteRecover::BackupDB( const std::string &corruptDBFilepath )
{
	if (corruptDBFilepath.empty())
	{
		return;
	}

	auto parameters = _GetFullParameters(corruptDBFilepath, Backup);
	_ExecuteBatFile(parameters);
}

void SqliteRecover::RecoverDB( const std::string &dstDBFilepath )
{
	if (dstDBFilepath.empty())
	{
		return;
	}

	auto parameters = _GetFullParameters(dstDBFilepath, Recover);
	_ExecuteBatFile(parameters);
}

std::string SqliteRecover::_GetFullParameters( const std::string &dbFilepath, BatType batType )
{
	if (dbFilepath.empty())
	{
		return "";
	}

	DWORD len = 0, ret = 0;
	len = GetCurrentDirectory(0, nullptr);
	if (!len)
	{
		return "";
	}

	char *pBuffer = new char[len];
	ret = GetCurrentDirectory(len, pBuffer);
	if (ret == 0 || ret >= len)
	{
		return "";
	}

	std::string dir(pBuffer);
	dir.append("\\");
	dir.append(RecoverDirName);
	dir.append("\\");

	std::string batFullpath, exeFullpath, backupSqlFullpath, fullParameters;

	if (batType == Backup)
	{
		batFullpath = dir + BackupBatName;
	}
	else if (batType == Recover)
	{
		batFullpath = dir + RecoverBatName;
	}
	else
	{
		return "";
	}
	
	exeFullpath = dir + SqliteExeName;
	backupSqlFullpath = dir + BackupSql;

	fullParameters.append("/c ");
	fullParameters += batFullpath;
	fullParameters.append(" ");
	fullParameters += exeFullpath;
	fullParameters.append(" ");
	fullParameters += dbFilepath;
	fullParameters.append(" ");
	fullParameters += backupSqlFullpath;

	return fullParameters;
}

void SqliteRecover::_ExecuteBatFile( const std::string &params )
{
	if (params.empty())
	{
		return;
	}

	TCHAR szPath[MAX_PATH] = {0};
	GetSystemDirectory(szPath, MAX_PATH);
	std::string cmdPath = szPath;
	cmdPath.append("\\cmd.exe");

	SHELLEXECUTEINFO info;
	info.cbSize = sizeof(SHELLEXECUTEINFO);
	info.fMask = SEE_MASK_NOCLOSEPROCESS;
	info.lpFile = cmdPath.c_str();
	info.lpParameters = params.c_str();
	info.lpDirectory = nullptr;
	info.nShow = SW_HIDE;
	info.hwnd = nullptr;
	info.lpVerb = nullptr;
	info.hInstApp = nullptr;

	ShellExecuteEx(&info);
	WaitForSingleObject(info.hProcess, INFINITE);
}
