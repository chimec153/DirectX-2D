#pragma once
#include "SceneComponent.h"
class CParticle :
	public CSceneComponent
{
public:
	CParticle();
	CParticle(const CParticle& com);
	virtual ~CParticle();

private:
	PARTICLECBUFFER					m_tCBuffer;
	std::shared_ptr<class CShader>	m_pUpdateShader;
	ID3D11UnorderedAccessView*		m_pUAV;
	ID3D11UnorderedAccessView*		m_pUAVShare;
	ID3D11ShaderResourceView*		m_pSRV;
	ID3D11ShaderResourceView*		m_pSRVShare;
	float							m_fMaxSpawnTime;
	float							m_fTime;
	int								m_iSpawnCount;
	ID3D11Buffer* m_pSrcBuffer;
	ID3D11Buffer* m_pSrcShareBuffer;
#ifdef _DEBUG
	ID3D11Buffer* m_pBuffer;
	ID3D11Buffer* m_pShareBuffer;
#endif
	int								m_iThreadGroupSize;

public:
	bool CreateBuffer(int iCount,int iSize, ID3D11UnorderedAccessView** pUAV, ID3D11ShaderResourceView** pSRV, 
		bool bUAV = true, ID3D11Buffer** pBuffer = nullptr);
	void SetMaxSpawnTime(float fTime);
	void SetAccel(const Vector3& vAccel);
	void SetMaxTime(float fTime);
	void SetMinTime(float fTime);
	void SetMaxColor(const Vector4& vColor);
	void SetMinColor(const Vector4& vColor);
	void SetAngleAccel(float fAccel);
	void SetMaxScale(const Vector3& vScale);
	void SetMinScale(const Vector3& vScale);
	void SetMove(bool bMove);
	void SetRange(const Vector3& vRange);
	void SetMaxSpeed(float fSpeed);
	void SetMinSpeed(float fSpeed);
	void SetMaxCount(int iCount);
	void SetMaxFrame(int iFrame);
	void SetImageSize(const Vector2& vSize);
	void SetCountX(int iCount);
	void SetCountY(int iCount);
	void SetSpawnCount(int iCount);
	void SetUpdateShader(const std::string& strKey);
	void SetThreadGroupSize(int iSize);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual std::shared_ptr<CComponent> Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	virtual void ShowWindow() override;
};

