#pragma once

#include "../Ref.h"

class CBoneSocket :
	public CRef
{
	friend class CSkeleton;
	friend class CAnimation;

private:
	CBoneSocket();
	~CBoneSocket();

private:
	std::string		m_strName;
	std::string		m_strBoneName;
	std::list<std::shared_ptr<class CComponent>>	m_ComponentList;
	Vector3	m_vOffset;
	Vector3	m_vRotation;
	int			m_iBoneIndex;

public:
	int GetBoneIndex()	const;

public:
	void SetInfo(const std::string& strName, const std::string& strBoneName,
		int iBoneIndex);
	void SetOffset(const Vector3& vOffset);
	void SetRotation(const Vector3& vRotation);
	void AddSceneComponent(const std::shared_ptr<class CComponent>& pCom);
	void AddSceneComponent(const std::shared_ptr<class CGameObject>& pObj);
	void DeleteObject(const std::shared_ptr<class CGameObject>& pObj);
	void DeleteObject(const std::shared_ptr<class CComponent>& pCom);
	void DeleteObject();
	virtual void ShowWindow();

public:
	bool Init();
	void Update(float fTime, const Matrix& matBone);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

