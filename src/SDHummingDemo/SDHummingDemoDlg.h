// SDHummingDemoDlg.h : header file
//

#if !defined(AFX_SDHUMMINGDEMODLG_H__1F17A1A0_8BDA_4B5E_A2CE_EC57A0394854__INCLUDED_)
#define AFX_SDHUMMINGDEMODLG_H__1F17A1A0_8BDA_4B5E_A2CE_EC57A0394854__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "WaveEditor.h"
/////////////////////////////////////////////////////////////////////////////
// CSDHummingDemoDlg dialog
#define THUMWIDTH 35
#define THUMHEIGHT 35
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "..\\SDHumming\\SModel.h"
#include "..\\SDHumming\\SMelody.h"
#include "..\\SDHumming\\SDFuzzySearch.h"
#pragma comment(lib, "SDFuzzySearch.lib")
using namespace std;

typedef struct ModelResStru{
	string Songname;
	string Albumname;
	string Singername;
	string Picpath;
	CBitmap* pImage;
}ModelResStru;

class CSDHummingDemoDlg : public CDialog
{
// Construction
public:
	CSDHummingDemoDlg(CWnd* pParent = NULL);	// standard constructor
	CWaveEditor	m_Wav;
	CListCtrl	m_ctrListResult;
	CString m_LoadedFile;
	CImageList m_imageList;
	char m_EXEfilename[_MAX_PATH];
	vector<ModelResStru> m_ModelVec;
	int nTotalModel;
	SModel *SQBHModels;
	int nModels;
	int m_SurvivalsI;
	int m_IsPrintSearchingTime;
	int m_nFrm4RawFilter;
	float fRecordTime;
	bool m_RecordStatus;
	HBITMAP LoadPicture(CString mFile);
// Dialog Data
	//{{AFX_DATA(CSDHummingDemoDlg)
	enum { IDD = IDD_SDHUMMINGDEMO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSDHummingDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSDHummingDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRecord();
	afx_msg void OnStopRecord();
	afx_msg void OnSearch();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SDHUMMINGDEMODLG_H__1F17A1A0_8BDA_4B5E_A2CE_EC57A0394854__INCLUDED_)
