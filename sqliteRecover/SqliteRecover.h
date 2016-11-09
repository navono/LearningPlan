#pragma once
#include <string>

static const std::string RecoverDirName = "HisDataRecover";
static const std::string SqliteExeName = "sqlite3.exe";
static const std::string BackupBatName = "backup.bat";
static const std::string RecoverBatName = "recover.bat";
static const std::string BackupSql = "backup.sql";


class SqliteRecover
{
public:
	SqliteRecover(void);
	~SqliteRecover(void);

	void BackupDB(const std::string &corruptDBFilepath);
	void RecoverDB(const std::string &dstDBFilepath);

private:
	enum BatType
	{
		Backup = 0,
		Recover
	};

	std::string _GetFullParameters(const std::string &dbFilepath, BatType batType);
	void		_ExecuteBatFile(const std::string &params);

private:
	std::string		_currentModulePath;
	std::string		_sqlite3ExePath;

};

