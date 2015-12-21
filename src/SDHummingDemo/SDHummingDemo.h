// SDHummingDemo.h : main header file for the SDHUMMINGDEMO application
//

#if !defined(AFX_SDHUMMINGDEMO_H__43C1AB9B_7BF7_4127_BA34_F96B6DABC51B__INCLUDED_)
#define AFX_SDHUMMINGDEMO_H__43C1AB9B_7BF7_4127_BA34_F96B6DABC51B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSDHummingDemoApp:
// See SDHummingDemo.cpp for the implementation of this class
//

class CSDHummingDemoApp : public CWinApp
{
public:
	CSDHummingDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSDHummingDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSDHummingDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SDHUMMINGDEMO_H__43C1AB9B_7BF7_4127_BA34_F96B6DABC51B__INCLUDED_)
