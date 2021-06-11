#include "pch.h"
#include "GameEditor.h"
#include "SpriteDlg.h"
#include "afxdialogex.h"
#include "Component/SpriteComponent.h"

IMPLEMENT_DYNAMIC(CSpriteDlg, CDialogEx)

CSpriteDlg::CSpriteDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SPRITE, pParent)
	, m_bStop(FALSE)
	, m_bLoop(FALSE)
	, m_bArrayTex(FALSE)
	, m_strCurrent(_T(""))
	, m_iFrame(0)
	, m_fTime(0.f)
{

}

CSpriteDlg::~CSpriteDlg()
{
}

void CSpriteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_STOP, m_bStop);
	DDX_Check(pDX, IDC_CHECK_LOOP, m_bLoop);
	DDX_Check(pDX, IDC_CHECK_ARRAYTEX, m_bArrayTex);
	DDX_Text(pDX, IDC_EDIT_CURRENT, m_strCurrent);
	DDX_Text(pDX, IDC_EDIT_FRAME, m_iFrame);
	DDX_Text(pDX, IDC_EDIT_FRAME, m_iFrame);
	DDX_Text(pDX, IDC_EDIT_SEQ_TIME, m_fTime);
	DDX_Text(pDX, IDC_EDIT_FRAME_MAX, m_iMaxFrame);
	DDX_Control(pDX, IDC_LIST_SEQ, m_SeqList);
}


BEGIN_MESSAGE_MAP(CSpriteDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_STOP, &CSpriteDlg::OnBnClickedCheckStop)
	ON_BN_CLICKED(IDC_CHECK_LOOP, &CSpriteDlg::OnBnClickedCheckLoop)
	ON_BN_CLICKED(IDC_CHECK_ARRAYTEX, &CSpriteDlg::OnBnClickedCheckArraytex)
	ON_LBN_SELCHANGE(IDC_LIST_SEQ, &CSpriteDlg::OnLbnSelchangeListSeq)
	ON_EN_CHANGE(IDC_EDIT_CURRENT, &CSpriteDlg::OnEnChangeEditCurrent)
	ON_EN_CHANGE(IDC_EDIT_FRAME, &CSpriteDlg::OnEnChangeEditFrame)
	ON_EN_CHANGE(IDC_EDIT_SEQ_TIME, &CSpriteDlg::OnEnChangeEditSeqTime)
	ON_EN_CHANGE(IDC_EDIT_FRAME_MAX, &CSpriteDlg::OnEnChangeEditFrameMax)
END_MESSAGE_MAP()


void CSpriteDlg::OnBnClickedCheckStop()
{
	UpdateData(TRUE);
}


void CSpriteDlg::OnBnClickedCheckLoop()
{
	UpdateData(TRUE);
}


void CSpriteDlg::OnBnClickedCheckArraytex()
{
	UpdateData(TRUE);
}

void CSpriteDlg::OnLbnSelchangeListSeq()
{
}

int CSpriteDlg::GetWidth() const
{
	return m_iWidth;
}

int CSpriteDlg::GetHeight() const
{
	return m_iHeight;
}

void CSpriteDlg::SetComponent(std::shared_ptr<class CComponent> pCom)
{
	m_pCom = pCom;

	CSpriteComponent* pSprite = (CSpriteComponent*)m_pCom.get();

	PAni2DInfo pInfo = pSprite->GetCurrentSeq();

	m_iFrame = pInfo->iFrame;
	m_fTime = pInfo->fTime;

	m_bArrayTex = pSprite->IsArrayTex();
	m_bStop = pSprite->IsStopped();
	m_bLoop = pInfo->bLoop;

	m_iMaxFrame = pInfo->pSequence->GetFrameCount();

	const std::unordered_map<std::string, PAni2DInfo> mapSeq = pSprite->GetAniSequences();

	std::unordered_map<std::string, PAni2DInfo>::const_iterator iter = mapSeq.begin();
	std::unordered_map<std::string, PAni2DInfo>::const_iterator iterEnd = mapSeq.end();

	for (; iter != iterEnd; ++iter)
	{
		std::string strAni = iter->second->pSequence->GetName();

		CString pAni = CA2CT(strAni.c_str());

		m_SeqList.AddString(pAni);

	}

	UpdateData(FALSE);
}


BOOL CSpriteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	RECT tRC = {};

	GetClientRect(&tRC);

	m_iWidth = tRC.right - tRC.left;
	m_iHeight = tRC.bottom - tRC.top;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CSpriteDlg::OnEnChangeEditCurrent()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CSpriteDlg::OnEnChangeEditFrame()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CSpriteDlg::OnEnChangeEditSeqTime()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}



void CSpriteDlg::OnEnChangeEditFrameMax()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
