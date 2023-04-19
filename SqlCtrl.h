#pragma once

class CSqlCtrl
{
public:
	CSqlCtrl();
	virtual ~CSqlCtrl();
	bool Initialize(const char* lpDBName);
	void Uninitilize();
	bool Excute(LPCTSTR lpSql,CString& strRet);
	bool Query(LPCTSTR lpSql,CString& strRet);
	bool IsBusy();
protected:
	sqlite3* m_pDB;
};