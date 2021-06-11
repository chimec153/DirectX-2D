#pragma once


// CRendererDlg 대화 상자

class CRendererDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRendererDlg)

public:
	CRendererDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CRendererDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RENDERER };
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
	BOOL m_bInst;
	afx_msg void OnBnClickedCheckInst();
	Vector3		m_vMin;
	Vector3		m_vMax;
	afx_msg void OnEnChangeEditMinX();
	afx_msg void OnEnChangeEditMinY();
	afx_msg void OnEnChangeEditMinZ();
	afx_msg void OnEnChangeEditMaxX();
	afx_msg void OnEnChangeEditMaxY();
	afx_msg void OnEnChangeEditMaxZ();
	int m_iInstCount;
	afx_msg void OnEnChangeEditInstCount();
	CComboBox m_MeshMatTexInfo;
	afx_msg void OnCbnSelchangeComboMeshTextureinfo();
	int m_MeshMatReg;
	afx_msg void OnEnChangeEditMeshMatRegister();
	int m_iMeshLinkType;
	afx_msg void OnEnChangeEditMeshMatLinkType();
	float m_fMeshShaderType;
	afx_msg void OnEnChangeEditMeshMatShaderType();
	int m_iMeshDescSize;
	afx_msg void OnEnChangeEditMeshMatShaderDescSize();
	int m_iMeshInstSize;
	afx_msg void OnEnChangeEditMeshMatShaderInstSize();
	int m_iMeshShaderType;
	afx_msg void OnEnChangeEditMeshMatShadertype();
	CComboBox m_MatTexInfo;
	afx_msg void OnCbnSelchangeComboMatTexInfo();
	int m_iMatReg;
	afx_msg void OnEnChangeEditMatRegister();
	int m_iMatLinkType;
	afx_msg void OnEnChangeEditMatLineType();
	int m_iMatTexType;
	afx_msg void OnEnChangeEditMatShaderType();
	int m_iMatDescSize;
	afx_msg void OnEnChangeEditMatShaderDescSize();
	int m_iMatInstSize;
	afx_msg void OnEnChangeEditMatShaderInstSize();
	int m_iShaderType;
	afx_msg void OnEnChangeEditMatShadertype();
	virtual BOOL OnInitDialog();
	CString m_strMeshMatTex;
	afx_msg void OnEnChangeEditMeshMatTex();
	afx_msg void OnEnChangeEditMatTex();
	afx_msg void OnBnClickedButtonLoadMeshTex();
	afx_msg void OnBnClickedButtonLoadTex();
	CString m_strMeshTag;
	afx_msg void OnEnChangeEditMeshName();
	afx_msg void OnEnChangeEditMeshMatName();
	CString m_strMeshMatTag;
	CString m_strMeshShaderTag;
	afx_msg void OnEnChangeEditMeshShaderName();
	CString m_strMatTag;
	afx_msg void OnEnChangeEditMatName();
	CString m_strShaderTag;
	afx_msg void OnEnChangeEditShaderName();
	CString m_strMatTex;
	CString m_strLayer;
	afx_msg void OnEnChangeEditLayer();
	int m_iScene;
	afx_msg void OnEnChangeEditScene();
};
