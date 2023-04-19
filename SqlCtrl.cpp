#include "StdAfx.h"
#include "SqlCtrl.h"
#define RCVBUF_SIZE		1024 * 4

CSqlCtrl::CSqlCtrl()
{
	m_pDB = NULL;
}

CSqlCtrl::~CSqlCtrl()
{
}

bool CSqlCtrl::Initialize(const char* lpDBName)
{
	int ret = 0;
	// �������ݿ�
	ret = sqlite3_open16(lpDBName,&m_pDB);
	if (ret != SQLITE_OK)
	{
		sqlite3_close(m_pDB);
		return FALSE;
	}
	//����sqllite�첽д���̣������ȴ�����д����ٷ���һ��д���󣬵��ڰ�ͬ��д���ƶ����ļ�ϵͳ����Ŀ�������Ч��
	sqlite3_exec16(m_pDB,L"PRAGMA synchronous = OFF;",0,0,0);
	WRITE_LOG(LogerSqliteCtrl,0,FALSE,L"Initialize OK");
	return TRUE;
}

void CSqlCtrl::Uninitilize()
{
	//�ر����ݿ�
	sqlite3_close(m_pDB);
	m_pDB = NULL;
}

bool CSqlCtrl::Excute(LPCTSTR lpSql,CString& strRet)
{
	wchar_t* pErrMsg = NULL;
	int ret = 0;
	ret = sqlite3_exec16(m_pDB, lpSql, 0, 0, &pErrMsg);
	if (ret != SQLITE_OK )
	{
		strRet = pErrMsg;
		sqlite3_free(pErrMsg);
		pErrMsg = NULL;
		return false;
	}
	return true;
}

bool CSqlCtrl::Query(LPCTSTR lpSql,CString& strRet)
{
	sqlite3_stmt* stmt = NULL; 
	int rc = 0,ncols = 0,i = 0;
	rc  = sqlite3_prepare16(m_pDB, lpSql, -1, &stmt, NULL);
	if (rc  !=  SQLITE_OK)
	{
		WRITE_LOG(LogerSqliteCtrl,0,FALSE,L"sqlite3_prepare16 failed!strsql=%s,reson:%s",lpSql,(wchar_t*)sqlite3_errmsg16(m_pDB));
		return false;
	}
	rc = sqlite3_step(stmt);
	ncols = sqlite3_column_count(stmt); 
	if (rc != SQLITE_ROW)
	{
		//�޼�¼
		strRet = WCHAR(0xFEFE);
	}
	else
	{
		CString strText;
		strText = WCHAR(0xEEEE);//�����־
		size_t nRecved = 0;
		CString strInfo;
		while (rc  ==  SQLITE_ROW) 
		{
			for(i = 0; i < ncols; i++)
			{
				if (sqlite3_column_type(stmt, i) != SQLITE_NULL)
				{
					strInfo.Format(L"%s%s",(wchar_t*)sqlite3_column_text16(stmt, i),strText);
				}
				else
				{
					strInfo.Format(L"%s",strText);
				}
				strRet += strInfo;
			}
			rc = sqlite3_step(stmt);
		}
		strRet += WCHAR(0xFFFF);//�����־;
	}
	sqlite3_finalize(stmt);
	return true;
}
