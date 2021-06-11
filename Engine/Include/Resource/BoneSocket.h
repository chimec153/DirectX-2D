#pragma once

#include "../Ref.h"

class CBoneSocket	:
	public CRef
{
	friend class CAnimation;

private:
	CBoneSocket();
	CBoneSocket(const CBoneSocket& socket);
	virtual ~CBoneSocket();

private:
	int												m_iBoneIndex;
	std::string										m_strBone;
	std::list<std::shared_ptr<class CComponent>>	m_ComponentList;
	Vector3											m_vOffset;
	Vector3											m_vRotation;

public:
	bool Init(const std::string& strKey, int iBoneIndex, const std::string& strBone);
	void SetOffset(const Vector3& vOffset);
	void SetRotation(const Vector3& vRotation);
	void AddComponent(const std::shared_ptr<class CComponent>& pCom);
	void DeleteComponent(const std::shared_ptr<class CComponent>& pCom);
	void Update(float fTime, const Matrix& matBone);
	CBoneSocket* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	virtual void ShowWindow() override;

};