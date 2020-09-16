#pragma once
class CGameMode
{
	friend class CScene;

protected:
	CGameMode();
	~CGameMode();

protected:
	class CScene*		m_pScene;
	class CGameObject* m_pPlayer;

public:
	class CGameObject* GetPlayer()	const;
	void SetPlayer(class CGameObject* pPlayer);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
};

