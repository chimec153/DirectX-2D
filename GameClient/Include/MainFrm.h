
// MainFrm.h: CMainFrame 클래스의 인터페이스
//

#pragma once

class CMainFrame : public CFrameWnd
{
	
protected: // serialization에서만 만들어집니다.
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

protected:
	CSplitterWnd		m_hSplit;
	CSplitterWnd		m_hSplitChild;
	CSplitterWnd		m_hSplitChild1;
	class CAniEditor*	m_pAniEditDlg;
	class CObjCreate*	m_pObjCreateDlg;

public:

public:
	void ClearCreateDlg();

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

public:
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
	afx_msg void OnAniEdit();
	afx_msg void OnObjCreate();
};


