#pragma once


// CMesh2DDlg 대화 상자

class CMesh2DDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMesh2DDlg)

public:
	CMesh2DDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMesh2DDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MESH2D };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

private:
	int		m_iHeight;
	int		m_iWidth;
	std::shared_ptr<class CComponent>	m_pCom;

public:
	int GetWidth()	const;
	int GetHeight()	const;

	void SetComponent(std::shared_ptr<class CComponent> pCom);

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
};
