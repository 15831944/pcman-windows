#include <afxwin.h>

#include <windows.h>
#include "AppConfig.h"
#include "SiteSettings.h"

/*
int WINAPI WinMain( HINSTANCE hinst, HINSTANCE hprev, LPSTR lpcmd, int showcmd )
{

	return 0;
}
*/

class CApp : public CWinApp
{
public:
	BOOL InitInstance();
}theApp;

CString ConfigPath;

BOOL CApp::InitInstance()
{
	CString AppPath;

	::GetModuleFileName(AfxGetInstanceHandle(),AppPath.GetBuffer(MAX_PATH),_MAX_PATH);
	AppPath.ReleaseBuffer();
	AppPath=AppPath.Left(AppPath.ReverseFind('\\')+1);
	ConfigPath = AppPath + CONFIG_DIR;

	CAppConfig cfg;
	CString OldConfigPath = AppPath + "Config.2004\\";

	cfg.BackupConfig( ConfigPath, OldConfigPath );
	cfg.Load( OldConfigPath + CONFIG_FILENAME );

	cfg.Save( ConfigPath + "Config.ini" );

	CFile f;
	if(  f.Open( OldConfigPath + HISTORY_FILENAME, CFile::modeRead )  )
	{
		cfg.LoadHistory(f);
		f.Close();
	}


#if 0	// import old UI

	if(  f.Open(OldConfigPath + "UI", CFile::modeRead)  )
	{
		WORD c;
		f.Read( &c, 2 );
		ACCEL* acc = new ACCEL[c];
		f.Read( acc, c * sizeof(ACCEL) );
		f.Close();

		CArray<ACCEL, ACCEL&> accels;
		accels.SetSize( c );
		int i;
		for( i = 0; i < c; ++i )
			accels[i] = acc[i];
		delete acc;
		acc = NULL;

		BYTE* data = NULL;
		DWORD rest = 0;
		if( f.Open( ConfigPath + "UI2", CFile::modeRead ) )
		{
			f.Read( &c, sizeof(c) );
			acc = new ACCEL[c];
			f.Read( acc, c * sizeof(ACCEL) );
			rest = f.GetLength() - (c * sizeof(ACCEL));
			data = new BYTE[rest];
			f.Read( data, rest * sizeof(BYTE) );
			f.Close();
		}

		if( acc && data )
		{
			for( i = 0; i < c; ++i )
			{
				int j;
				for( j = 0; j < accels.GetSize(); ++j )
				{
					// �b�ª� UI �̭����]�w���R�O������
					if(	accels[j].cmd == acc[i].cmd )
						break;
				}
				if( j >= accels.GetSize() )
				{
					// �ª� UI ���S�����R�O�A��ܬ� 2007 �s�W���w�]����
					ACCEL new_acc = acc[i];
					for( i = 0; i < accels.GetSize(); ++i )
					{
						// �b�ª� UI �̭�������w�Q�ϥ�
						if(	accels[j].fVirt == acc[i].fVirt 
							&& accels[j].key == acc[i].key )
							break;
					}

					// �ª� UI ���å��ϥΦ�����
					if( i >= accels.GetSize() )
						accels.Add( new_acc );	// �[�J������
				}
			}
			if( f.Open( ConfigPath + "UI", CFile::modeWrite|CFile::modeCreate ) )
			{
				f.Write( accels.GetData(), accels.GetSize() * sizeof(ACCEL) );
				f.Write( data, rest );
				f.Close();
			}
			delete []acc;
			delete []data;
		}
	}
#endif	// import old UI

	// per-site settings
	CFileFind finder;
	BOOL found = finder.FindFile( OldConfigPath + "*;*" );
	while( found )
	{
		found = finder.FindNextFile();
		CString src = finder.GetFilePath();
		CString dest = finder.GetFileTitle();
		dest.Replace(";", "__");
		dest = ConfigPath + dest;
		CSiteSettings site;
		if( site.Load( src ) )
			site.Save( dest );
	}

#if 0
#if defined _COMBO_
	SaveWebPageFilter();
#endif
#endif

	return CWinApp::InitInstance();
}
