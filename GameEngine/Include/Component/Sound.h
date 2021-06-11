#pragma once
#include "SceneComponent.h"
#include "../fmod.hpp"
class CSound :
	public CSceneComponent
{
	friend class CGameObject;

public:
	CSound();
	CSound(const CSound& snd);
	virtual ~CSound();

private:
	FMOD::Channel* m_pChannel;
	FMOD::ChannelGroup* m_pGroup;
	FMOD::System* m_pSystem;
	FMOD::Sound* m_pSound;
	bool	m_bLoop;
	bool	m_bPause;

public:
	void SetSound(const std::string& strKey);
	void Play();
	void Stop();

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

};