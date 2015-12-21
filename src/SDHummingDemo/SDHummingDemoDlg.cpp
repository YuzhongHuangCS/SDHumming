// SDHummingDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SDHummingDemo.h"
#include "SDHummingDemoDlg.h"
#include "Shlwapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

bool split( const char * line, vector<string> & parts, const char * pseg = 0 ){
	const char * seg = "\r \n\t";
	int nLen = strlen(line);
	char * newLine = new char [nLen+1];
	strcpy(newLine,line); newLine[nLen]=0;
	char * p=(char*)newLine, *q = 0 ;
	
	if(pseg!=0) seg = pseg;
	
	parts.clear();
	
	while(1==1){
		// trim left
		while(*p && strchr(seg,*p))p++;
		if(*p==0) break;
		
		// get info
		q = p;
		while(*q && !strchr(seg,*q))q++;
		char ch = *q; * q = 0;
		parts.push_back(p);
		*q = ch; p = q;
	}
	
	delete [] newLine; newLine=NULL;
	return parts.size()>0;
}
//读取文件
vector<string> ReadFromFile(char* filename){
	int i=0;
	vector<string> AllStrs;
	ifstream infile;
	infile.open(filename,NULL);
	string str; 
	if(infile != NULL){
		while(getline(infile,str,'\n')) {
			AllStrs.push_back(str);
		}
	}else{
		printf("Error: Could not open file: %s\n",filename);
	}
	infile.close();
	return AllStrs;
}

void NormalizeScore(FrameBasedResStru *res, int nLen){
    float Median=res[nLen/2].fScore;
	for (int i=0;i<nLen;i++){
		res[i].fScore=100-30.0f*res[i].fScore/Median;
	}
}


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSDHummingDemoDlg dialog

CSDHummingDemoDlg::CSDHummingDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSDHummingDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSDHummingDemoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_RecordStatus=false;
	fRecordTime=0.0f;
}

void CSDHummingDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSDHummingDemoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_WAVEEDITORCTRL1, m_Wav);
	DDX_Control(pDX, IDC_LIST1, m_ctrListResult);
}

BEGIN_MESSAGE_MAP(CSDHummingDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CSDHummingDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnRecord)
	ON_BN_CLICKED(IDC_BUTTON2, OnStopRecord)
	ON_BN_CLICKED(IDC_BUTTON3, OnSearch)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSDHummingDemoDlg message handlers

BOOL CSDHummingDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	GetModuleFileName(AfxGetInstanceHandle(),m_EXEfilename,_MAX_PATH);
	for(int x=strlen(m_EXEfilename);x>0;x--){   
		if(m_EXEfilename[x]!='\\'){
			m_EXEfilename[x]=0;   
		}else
			break;
	}


	m_ctrListResult.InsertColumn(0,_T("歌曲名"),LVCFMT_LEFT,125,-1);
	m_ctrListResult.InsertColumn(1,_T("歌手"),LVCFMT_LEFT,125,-1);
	//m_ctrListResult.InsertColumn(2,_T("专辑"),LVCFMT_LEFT,125,-1);
	m_ctrListResult.InsertColumn(3,_T("得分"),LVCFMT_LEFT,125,-1);
	//	m_ctrListSim.InsertColumn(0,_T("歌曲"),LVCFMT_LEFT,125,-1);
	//.	m_ctrListSim.InsertColumn(1,_T("歌手"),LVCFMT_LEFT,100,-1);
	//	m_ctrListSim.InsertColumn(2,_T("专辑"),LVCFMT_LEFT,75,-1);
	
	char szModelinfoPath[1024];
//	HIMAGELIST hScreens = ImageList_Create(THUMWIDTH, THUMHEIGHT, ILC_COLOR32, 0, 1);
//	m_imageList.Attach(hScreens);
//	m_ctrListResult.SetImageList(&m_imageList, LVSIL_SMALL);
	sprintf(szModelinfoPath,"%sQBHModel.info",m_EXEfilename);
	vector<string> ModelStrs=ReadFromFile(szModelinfoPath);
	for(int i=1;i<ModelStrs.size();i++){
		vector<string> parts;
		split(ModelStrs[i].c_str(),parts,";");
		ModelResStru tmp;
		tmp.Songname=parts[0];
		tmp.Singername=parts[1];
		tmp.Albumname=parts[2];
		tmp.Picpath=parts[3];
		
		char szTmp[1024];
		sprintf(szTmp,"%spic\\%s",m_EXEfilename,tmp.Picpath.c_str());
		////HBITMAP bitm=LoadPicture(szTmp);
		//if (bitm!=NULL){
		//	tmp.pImage = NULL;	
		//	tmp.pImage = new CBitmap();		 
		//	tmp.pImage->Attach(bitm);
	//	}
		m_ModelVec.push_back(tmp);
	}
	
	char szModelPath[1024];
	sprintf(szModelPath,"%sqbh.model",m_EXEfilename);
	SQBHModels=NULL;
	nModels=0;
	nTotalModel=SLoadModel(szModelPath, SQBHModels, nModels);

	
	m_SurvivalsI=20;
	m_nFrm4RawFilter = 80;
	
	return TRUE;  
	
}

void CSDHummingDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSDHummingDemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSDHummingDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSDHummingDemoDlg::OnRecord() 
{ 
	if(false==m_RecordStatus){
		m_Wav.DrawPitch("");
		m_ctrListResult.DeleteAllItems();
		m_Wav.StartRecord();
		m_RecordStatus=true;
		GetDlgItem(IDC_BUTTON1)->SetWindowText("已录制0秒");
		SetTimer(1,1000,0);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	}else{
		KillTimer(1);
		m_Wav.StopRecord();
		m_Wav.SaveWavFile("tmp.wav");
		char strFolderPath[1024];
		sprintf(strFolderPath,"%slog",m_EXEfilename);
		//   判断路径是否存在   
		if(!PathIsDirectory(strFolderPath)){   
			if(CreateDirectory(strFolderPath,NULL)){   
				char szTmp[1024];
				sprintf(szTmp,"%slog\\log_%d.wav",m_EXEfilename,GetTickCount());
				m_Wav.SaveWavFile(szTmp);
			}   
		}else{
			char szTmp[1024];
			sprintf(szTmp,"%slog\\log_%d.wav",m_EXEfilename,GetTickCount());
			m_Wav.SaveWavFile(szTmp);
		}
		m_RecordStatus=false;
		fRecordTime=0.0f;
		
		OnSearch();
		GetDlgItem(IDC_BUTTON1)->SetWindowText("开始录音");

		/*
		m_Wav.StopRecord();
		m_Wav.SaveWavFile("tmp.wav");
		
		char strFolderPath[1024];
		sprintf(strFolderPath,"%slog",m_EXEfilename);
		//   判断路径是否存在   
		if(!PathIsDirectory(strFolderPath)){   
			if(CreateDirectory(strFolderPath,NULL)){   
				char szTmp[1024];
				sprintf(szTmp,"%slog\\log_%d.wav",m_EXEfilename,GetTickCount());
				m_Wav.SaveWavFile(szTmp);
			}   
		}else{
			char szTmp[1024];
			sprintf(szTmp,"%slog\\log_%d.wav",m_EXEfilename,GetTickCount());
			m_Wav.SaveWavFile(szTmp);
		}

	
		m_RecordStatus=false;
		GetDlgItem(IDC_BUTTON1)->SetWindowText("开始录音");
		KillTimer(1);
		fRecordTime=0.0f;
		*/
	}
}

void CSDHummingDemoDlg::OnStopRecord() 
{
	
}

void CSDHummingDemoDlg::OnSearch() 
{
	int nStart=GetTickCount();
	if(true==m_RecordStatus){
		AfxMessageBox("请先结束录音后再检索");
		return ;
	}
	m_ctrListResult.DeleteAllItems();
	
	int i;
	float *pFeaBuf = NULL;
	int nFeaLen = 0;
	
	SPitchContourExtraction("tmp.wav",pFeaBuf,nFeaLen);	
	if(nFeaLen<20){
		AfxMessageBox("不能识别的语音，请重试");
		if(NULL!=pFeaBuf){
			delete[] pFeaBuf;
			pFeaBuf=NULL;
		}
		return ;
	}
	if(nFeaLen>5000)
		nFeaLen=5000;

	SMedianFilter(pFeaBuf,nFeaLen);
	SMedianFilter(pFeaBuf,nFeaLen);
	
	char szPitch[65536];
	strcpy(szPitch,"");
	for(i=0;i<nFeaLen;i++){
		char szTmp[1024];
		sprintf(szTmp,"%f;",pFeaBuf[i]);
		strcat(szPitch,szTmp);
	}
	m_Wav.DrawPitch(szPitch);
	SProcessQuery(pFeaBuf,nFeaLen);
	if(nFeaLen>1000)
		nFeaLen=1000;
	if(nFeaLen<20){
		AfxMessageBox("不能识别的语音，请重试");
		if(NULL!=pFeaBuf){
			delete[] pFeaBuf;
			pFeaBuf=NULL;
		}
		return ;
	}
	
	if(nFeaLen>200)
		nFeaLen=200;
	
	//MELODY_TEMPLATE m_MelodyNote=QueryNoteSegmentation(pFeaBuf,nFeaLen);
	SNote *QueryNotes=NULL;
	int nNoteLen=0;
	STranscribeQueryNote(pFeaBuf,nFeaLen, QueryNotes, nNoteLen);
	if(nFeaLen>1000)
		nFeaLen=1000;
	if(nFeaLen<20){
		AfxMessageBox("不能识别的语音，请重试");
		if(NULL!=pFeaBuf){
			delete[] pFeaBuf;
			pFeaBuf=NULL;
		}
		return ;
	}
	
	//SNote *Query=new SNote[m_MelodyNote.size()];
	//for(int x=0;x<m_MelodyNote.size();x++){
	//	Query[x].fNoteDuration=m_MelodyNote[x].Duration;
	//	Query[x].fNoteValue=m_MelodyNote[x].Pitch;
	//}
	
	NoteBasedResStru *myEMDResStru=new NoteBasedResStru[nTotalModel];
	//SNoteBasedMatch(SQBHModels, nModels, Query, m_MelodyNote.size(),myEMDResStru,m_nFrm4RawFilter);
	SNoteBasedMatch(SQBHModels, nModels, QueryNotes, nNoteLen,myEMDResStru,nFeaLen);
	
	FrameBasedResStru *res=new FrameBasedResStru[m_SurvivalsI];
	SFrameBasedMatch(SQBHModels, nModels, pFeaBuf, nFeaLen,myEMDResStru,m_SurvivalsI, res);
	
	int *nHasTag=new int[nModels];
	memset(nHasTag,0,sizeof(int)*nModels);
	int nCount=0;
	
	NormalizeScore(res, m_SurvivalsI);
	
	for( i=0;i<m_SurvivalsI;i++){
		if(nHasTag[res[i].nModelID]!=1){
			//int imgP=m_imageList.Add(m_ModelVec[res[i].nModelID].pImage,RGB(0,0,0));
			m_ctrListResult.InsertItem(nCount,m_ModelVec[res[i].nModelID].Songname.c_str());
			char tmp[1024];
			sprintf(tmp,"%3.2f",res[i].fScore);
			m_ctrListResult.SetItemText(nCount,1,m_ModelVec[res[i].nModelID].Singername.c_str());	
			//m_ctrListResult.SetItemText(nCount,2,m_ModelVec[res[i].nModelID].Albumname.c_str());
			m_ctrListResult.SetItemText(nCount,2,tmp);
			nHasTag[res[i].nModelID]=1;
			nCount++;
		}		
		if(nCount>20)
			break;
	}
	
	//Finalize
	int nEnd=GetTickCount();

	if(NULL!=nHasTag){
		delete[] nHasTag;
		nHasTag=NULL;
	}
	if(NULL!=myEMDResStru){
		delete[] myEMDResStru;
		myEMDResStru=NULL;
	}
	if(NULL!=res){
		delete[] res;
		res=NULL;
	}
	if(NULL!=QueryNotes){
		delete[] QueryNotes;
		QueryNotes=NULL;
	}
	if(NULL!=pFeaBuf){
		delete[] pFeaBuf;
		pFeaBuf=NULL;
	}
	return ;	
}

void CSDHummingDemoDlg::OnTimer(UINT nIDEvent){
	if(nIDEvent==1){
		fRecordTime+=1;
		char szTmp[1024];
		sprintf(szTmp,"已录制%d秒",(int)fRecordTime);
		//设置最短录音长度，八秒以上
		if(fRecordTime>8){
			GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON1)->SetWindowText(szTmp);
		}else{
			GetDlgItem(IDC_BUTTON1)->SetWindowText(szTmp);
		}
		
	}	
	CDialog::OnTimer(nIDEvent); 
}

HBITMAP CSDHummingDemoDlg::LoadPicture(CString mFile){
    WCHAR wpath[MAX_PATH];
    MultiByteToWideChar(CP_ACP, 0, mFile, -1, wpath, MAX_PATH);
	
    IPicture* pPic;
    OleLoadPicturePath(wpath, NULL, NULL, NULL, IID_IPicture,(LPVOID*)&pPic);
	
	if (pPic==NULL) return NULL;
    HBITMAP hPic = NULL;
    pPic->get_Handle((UINT*)&hPic);
	
	long nWidth=THUMWIDTH;
	long nHeight=THUMHEIGHT;
	
	long mWid,mHei;
	pPic->get_Height(&mHei);
	pPic->get_Width(&mWid);
	
	HBITMAP hPicRet = (HBITMAP)CopyImage(hPic, IMAGE_BITMAP, nWidth, nHeight , LR_COPYDELETEORG);	
	
	// Create Brushes for Border and BackGround
	HBRUSH hBrushBorder=::CreateSolidBrush(RGB(192, 192, 192));
	HBRUSH hBrushBk=::CreateSolidBrush(RGB(255, 255, 255));
	
	// Border Size
	RECT rcBorder;
	rcBorder.left=rcBorder.top=0;
	rcBorder.right=THUMWIDTH;
	rcBorder.bottom=THUMHEIGHT;
	
	const float fRatio=(float)THUMHEIGHT/THUMWIDTH;
	
	int XDest, YDest, nDestWidth, nDestHeight;
	// Calculate Rect to fit to canvas
	const float fImgRatio=(float)mHei/mWid;
	if(fImgRatio > fRatio){
		nDestWidth=(THUMHEIGHT/fImgRatio);
		XDest=(THUMWIDTH-nDestWidth)/2;
		YDest=0;
		nDestHeight=THUMHEIGHT;
	}else{
		XDest=0;
		nDestWidth=THUMWIDTH;
		nDestHeight=(THUMWIDTH*fImgRatio);
		YDest=(THUMHEIGHT-nDestHeight)/2;
	}
	
	CClientDC cdc(this);
	HDC hDC=::CreateCompatibleDC(cdc.m_hDC);
	HBITMAP bm = CreateCompatibleBitmap(cdc.m_hDC, THUMWIDTH, THUMHEIGHT);
	HBITMAP pOldBitmapImage = (HBITMAP)SelectObject(hDC,bm);
	// Draw Background
	::FillRect(hDC, &rcBorder, hBrushBk);
	
	// Draw Border
	::FrameRect(hDC, &rcBorder, hBrushBorder);
	
	HBITMAP hBmReturn= (HBITMAP)::SelectObject(hDC, pOldBitmapImage);
	
	CDC hdcSrc, hdcDst;
	
	hdcSrc.CreateCompatibleDC(NULL);
	hdcDst.CreateCompatibleDC(NULL);
	
	// Load the bitmaps into memory DC
	CBitmap* hbmSrcT = (CBitmap*) hdcSrc.SelectObject(hPicRet);
	CBitmap* hbmDstT = (CBitmap*) hdcDst.SelectObject(hBmReturn);
	
	// This call sets up the mask bitmap.
	hdcDst.BitBlt(XDest,YDest,nDestWidth, nDestHeight, &hdcSrc,0,0,SRCCOPY);	
	//hdcDst.StretchBlt(XDest,YDest,nDestWidth, nDestHeight, &hdcSrc,0,0,48,48,SRCCOPY);
	
	pOldBitmapImage = (HBITMAP)SelectObject(hdcDst.m_hDC,bm);
	
	// Release used DC and Object
	DeleteDC(hDC);
	DeleteObject(hBrushBorder); 
	DeleteObject(hBrushBk);
	
    return pOldBitmapImage;
	
}

void CSDHummingDemoDlg::OnClose(){
	int i=0;
	for( i=0;i<nModels;i++){
		if(NULL!=SQBHModels[i].PhrasePos){
			delete[] SQBHModels[i].PhrasePos; 
			SQBHModels[i].PhrasePos=NULL;
		}
		if(NULL!=SQBHModels[i].sNotes){
			delete[] SQBHModels[i].sNotes;
			SQBHModels[i].sNotes=NULL;
		}
	}
	delete[] SQBHModels;
	
	//for(i=0;i<m_ModelVec.size();i++){
//		delete m_ModelVec[i].pImage;
	//}
	
	CDialog::OnClose();
}
