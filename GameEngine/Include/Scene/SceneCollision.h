#pragma once

#include "../GameEngine.h"

enum class INDEX
{
	STARTX,
	ENDX,
	STARTY,
	ENDY,
	STARTZ,
	ENDZ,
	END
};

	typedef struct _tagSectionInfo
	{
		std::vector<class CCollisionSection*>	vecSection;
		Vector3							vSize;
		int										iNumX;
		int										iNumY;
		int										iNumZ;
		Vector3							vCenter;

		_tagSectionInfo() :
			iNumX(0),
			iNumY(0),
			iNumZ(0)
		{
		}
	}SectionInfo, * PSectionInfo;

	class CSceneCollision
	{
		friend class CScene;

	private:
		CSceneCollision();
		~CSceneCollision();

	private:
		PSectionInfo	m_p2DSection;
		PSectionInfo	m_p3DSection;
		PSectionInfo	m_pUISection;
		bool			m_bMouseCol;

	public:
		bool Init();
		PSectionInfo CreateSection(int iNumX, int iNumY, int iNumZ, const Vector3& vSize, const Vector3& vCenter);
		void SetCenter(const Vector3& vCenter);
		void AddCollider(class CCollider* pCol);
		void Collision(float fTime);
		int GetIdx(const Vector2& vPos, PSectionInfo pInfo);
		void Clear();
	};
