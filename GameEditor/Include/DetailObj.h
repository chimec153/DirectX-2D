#pragma once


// CDetailObj 보기
#include "Engine.h"

class CDetailObj : public CFormView
{
	DECLARE_DYNCREATE(CDetailObj)

protected:
	CDetailObj();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CDetailObj();

private:
	std::shared_ptr<CGameObject>		m_pSelectObj;
	CWnd			m_hWnd;

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OBJDETAIL };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	void SetObject(const std::string& strTag);
	void DeleteObject(const std::string& strTag);

private:
	std::vector<TreeItem>	m_vecItem;

protected:
	DECLARE_MESSAGE_MAP()
public:
	std::vector<CDialogEx*>	m_vecDialog;
	int						m_iOffset;
	HTREEITEM	m_RootItem;
	afx_msg void OnTvnSelchangedTreeComponent(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void DoDataExchange(CDataExchange* pDX);
	CTreeCtrl	m_Component;
	Vector3		m_vPos;
	Vector3		m_vScale;
	Vector3		m_vRot;
	virtual void OnInitialUpdate();
	afx_msg void OnEnChangeEditPosx();
	afx_msg void OnEnChangeEditScalex();
	afx_msg void OnEnChangeEditRotx();
	afx_msg void OnEnChangeEditPosy();
	afx_msg void OnEnChangeEditScaley();
	afx_msg void OnEnChangeEditRoty();
	afx_msg void OnEnChangeEditPosz();
	afx_msg void OnEnChangeEditScalez();
	afx_msg void OnEnChangeEditRotz();

	void Update(float fTime);
	afx_msg void OnNMClickTreeComponent(NMHDR* pNMHDR, LRESULT* pResult);
	float m_fRadius;
	afx_msg void OnEnChangeEditColRadius();
	float m_fExtentX;
	afx_msg void OnEnChangeEditColExtentX();
	float m_fExtentY;
	afx_msg void OnEnChangeEditColExtenty();
};


