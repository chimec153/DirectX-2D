#include "UITransform.h"
#include "../Device.h"
#include "../Component/Camera.h"
#include "../CameraManager.h"
#include "../Resource/ShaderManager.h"
#include "UIManager.h"

CUITransform::CUITransform() :
	m_pViewPort(nullptr),
	m_pOwner(nullptr),
	m_pParent(nullptr),
	m_bInheritScale(true),
	m_bInheritRotX(true),
	m_bInheritRotY(true),
	m_bInheritRotZ(true),
	m_bUpdateScale(true),
	m_bUpdateRot(true),
	m_bUpdatePos(true)
{
	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i];
		m_vWorldAxis[i] = Vector3::Axis[i];
	}

	m_vMeshSize = Vector3(1.f, 1.f, 1.f);
}

CUITransform::CUITransform(const CUITransform& trans)
{
	*this = trans;

	m_vecChild.clear();
	m_pViewPort = nullptr;
	m_pOwner = nullptr;
	m_pParent = nullptr;

	m_bUpdateScale = true;
	m_bUpdateRot = true;
	m_bUpdatePos = true;
}

CUITransform::~CUITransform()
{
}


void CUITransform::InheritScale()
{
	if (m_bInheritScale)
		m_vWorldScale = m_vRelativeScale * m_pParent->GetWorldScale();

	m_bUpdateScale = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritScale();
	}
}

void CUITransform::InheritRot()
{
	if (m_bInheritRotX)
		m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;

	if (m_bInheritRotY)
		m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;

	if (m_bInheritRotZ)
		m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	if (m_pParent)
		InheritPos();

	m_bUpdateRot = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::InheritPos()
{
	Matrix tMat;

	tMat.Rotation(m_pParent->GetWorldRot());

	memcpy(&tMat._41, &m_pParent->GetWorldPos(), sizeof(float) * 3);

	m_vWorldPos = m_vRelativePos.TranslationCoor(tMat);

	m_bUpdatePos = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritPos();
	}
}

void CUITransform::SetRelativeScale(const Vector3& v)
{
	m_vVelocityScale += v - m_vRelativeScale;

	m_vRelativeScale = v;

	m_vWorldScale = m_vRelativeScale;

	if (m_pParent && m_bInheritScale)
		m_vWorldScale = m_vRelativeScale * m_pParent->GetWorldScale();

	m_bUpdateScale = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritScale();
	}
}

void CUITransform::SetRelativeScale(float x, float y, float z)
{
	m_vVelocityScale += Vector3(x, y, z) - m_vRelativeScale;

	m_vRelativeScale = Vector3(x, y, z);

	m_vWorldScale = m_vRelativeScale;

	if (m_pParent && m_bInheritScale)
		m_vWorldScale = m_vRelativeScale * m_pParent->GetWorldScale();

	m_bUpdateScale = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritScale();
	}
}

void CUITransform::SetRelativeRot(const Vector3& v)
{
	m_vVelocityRot += v - m_vRelativeRot;

	m_vRelativeRot = v;

	m_vWorldRot = m_vRelativeRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::SetRelativeRot(float x, float y, float z)
{
	m_vVelocityRot += Vector3(x, y, z) - m_vRelativeRot;

	m_vRelativeRot = Vector3(x, y, z);

	m_vWorldRot = m_vRelativeRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::SetRelativeRotX(float x)
{
	m_vVelocityRot.x += x - m_vRelativeRot.x;

	m_vRelativeRot.x = x;

	m_vWorldRot.x = m_vRelativeRot.x;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::SetRelativeRotY(float y)
{
	m_vVelocityRot.y += y - m_vRelativeRot.y;

	m_vRelativeRot.y = y;

	m_vWorldRot.y = m_vRelativeRot.y;

	if (m_pParent)
	{
		if (m_bInheritRotY)
			m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::SetRelativeRotZ(float z)
{
	m_vVelocityRot.z += z - m_vRelativeRot.z;

	m_vRelativeRot.z = z;

	m_vWorldRot.z = m_vRelativeRot.z;

	if (m_pParent)
	{
		if (m_bInheritRotZ)
			m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::SetRelativePos(const Vector3& v)
{
	m_vVelocity += v - m_vRelativePos;

	m_vRelativePos = v;

	m_vWorldPos = m_vRelativePos;

	if (m_pParent)
	{
		Matrix tMat;

		tMat.Rotation(m_pParent->GetWorldRot());

		memcpy(&tMat._41, &m_pParent->GetWorldPos(), sizeof(float) * 3);

		m_vWorldPos = m_vRelativePos.TranslationCoor(tMat);
	}

	m_bUpdatePos = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritPos();
	}
}

void CUITransform::SetRelativePos(float x, float y, float z)
{
	m_vVelocity += Vector3(x, y, z) - m_vRelativePos;

	m_vRelativePos = Vector3(x, y, z);

	m_vWorldPos = m_vRelativePos;

	if (m_pParent)
	{
		Matrix tMat;

		tMat.Rotation(m_pParent->GetWorldRot());

		memcpy(&tMat._41, &m_pParent->GetWorldPos(), sizeof(float) * 3);

		m_vWorldPos = m_vRelativePos.TranslationCoor(tMat);
	}

	m_bUpdatePos = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritPos();
	}
}

void CUITransform::AddRelativeScale(const Vector3& v)
{
	m_vVelocityScale += v;

	m_vRelativeScale += v;

	m_vWorldScale = m_vRelativeScale;

	if (m_pParent && m_bInheritScale)
		m_vWorldScale = m_vRelativeScale * m_pParent->GetWorldScale();

	m_bUpdateScale = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritScale();
	}
}

void CUITransform::AddRelativeScale(float x, float y, float z)
{
	m_vVelocityScale += Vector3(x, y, z);

	m_vRelativeScale += Vector3(x, y, z);

	m_vWorldScale = m_vRelativeScale;

	if (m_pParent && m_bInheritScale)
		m_vWorldScale = m_vRelativeScale * m_pParent->GetWorldScale();

	m_bUpdateScale = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritScale();
	}
}

void CUITransform::AddRelativeRot(const Vector3& v)
{
	m_vVelocityRot += v;

	m_vRelativeRot += v;

	m_vWorldRot = m_vRelativeRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;

		if (m_bInheritRotX)
			m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::AddRelativeRot(float x, float y, float z)
{
	m_vVelocityRot += Vector3(x, y, z);

	m_vRelativeRot += Vector3(x, y, z);

	m_vWorldRot = m_vRelativeRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;

		if (m_bInheritRotX)
			m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::AddRelativeRotX(float x)
{
	m_vVelocityRot.x += x;

	m_vRelativeRot.x += x;

	m_vWorldRot = m_vRelativeRot;

	if (m_pParent && m_bInheritRotX)
		m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::AddRelativeRotY(float y)
{
	m_vVelocityRot.y += y;

	m_vRelativeRot.y += y;

	m_vWorldRot = m_vRelativeRot;

	if (m_pParent && m_bInheritRotY)
		m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::AddRelativeRotZ(float z)
{
	m_vVelocityRot.z += z;

	m_vRelativeRot.z += z;

	m_vWorldRot = m_vRelativeRot;

	if (m_pParent && m_bInheritRotZ)
		m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::AddRelativePos(const Vector3& v)
{
	m_vVelocity += v;

	m_vRelativePos += v;

	m_vWorldPos = m_vRelativePos;

	if (m_pParent)
	{
		Matrix tMat;

		tMat.Rotation(m_pParent->GetWorldRot());

		memcpy(&tMat._41, &m_pParent->GetWorldPos(), sizeof(float) * 3);

		m_vWorldPos = m_vRelativePos.TranslationCoor(tMat);
	}

	m_bUpdatePos = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritPos();
	}
}

void CUITransform::AddRelativePos(float x, float y, float z)
{
	m_vVelocity += Vector3(x, y, z);

	m_vRelativePos += Vector3(x, y, z);

	m_vWorldPos = m_vRelativePos;

	if (m_pParent)
	{
		Matrix tMat;

		tMat.Rotation(m_pParent->GetWorldRot());

		memcpy(&tMat._41, &m_pParent->GetWorldPos(), sizeof(float) * 3);

		m_vWorldPos = m_vRelativePos.TranslationCoor(tMat);
	}

	m_bUpdatePos = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritPos();
	}
}

Vector3 CUITransform::GetRelativeScale() const
{
	return m_vRelativeScale;
}

Vector3 CUITransform::GetRelativeRot() const
{
	return m_vRelativeRot;
}

Vector3 CUITransform::GetRelativePos() const
{
	return m_vRelativePos;
}

Vector3 CUITransform::GetRelativeAxis(AXIS axis) const
{
	return m_vRelativeAxis[axis];
}

Vector3 CUITransform::GetWorldScale() const
{
	return m_vWorldScale;
}

Vector3 CUITransform::GetWorldRot() const
{
	return m_vWorldRot;
}

Vector3 CUITransform::GetWorldPos() const
{
	return m_vWorldPos;
}

Vector3 CUITransform::GetPivot() const
{
	return m_vPivot;
}

Vector3 CUITransform::GetWorldAxis(AXIS axis) const
{
	return m_vWorldAxis[axis];
}

void CUITransform::SetWorldScale(const Vector3& v)
{
	m_vVelocityScale += v - m_vWorldScale;

	m_vWorldScale = v;

	m_vRelativeScale = m_vWorldScale;

	if (m_pParent && m_bInheritScale)
		m_vRelativeScale = m_vWorldScale / m_pParent->GetWorldScale();

	m_bUpdateScale = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritScale();
	}
}

void CUITransform::SetWorldScale(float x, float y, float z)
{
	m_vVelocityScale += Vector3(x, y, z) - m_vWorldScale;

	m_vWorldScale = Vector3(x, y, z);

	m_vRelativeScale = m_vWorldScale;

	if (m_pParent && m_bInheritScale)
		m_vRelativeScale = m_vWorldScale / m_pParent->GetWorldScale();

	m_bUpdateScale = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritScale();
	}
}

void CUITransform::SetWorldRot(const Vector3& v)
{
	m_vVelocityRot += v - m_vWorldRot;

	m_vWorldRot = v;

	m_vRelativeRot = m_vWorldRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::SetWorldRot(float x, float y, float z)
{
	m_vVelocityRot += Vector3(x, y, z) - m_vWorldRot;

	m_vWorldRot = Vector3(x, y, z);

	m_vRelativeRot = m_vWorldRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::SetWorldRotX(float x)
{
	m_vVelocityRot.x += x - m_vWorldRot.x;

	m_vWorldRot.x = x;

	m_vRelativeRot.x = m_vWorldRot.x;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::SetWorldRotY(float y)
{
	m_vVelocityRot.y += y - m_vWorldRot.y;

	m_vWorldRot.y = y;

	m_vRelativeRot.y = m_vWorldRot.y;

	if (m_pParent)
	{
		if (m_bInheritRotY)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::SetWorldRotZ(float z)
{
	m_vVelocityRot.z += z - m_vWorldRot.z;

	m_vWorldRot.z = z;

	m_vRelativeRot.z = m_vWorldRot.z;

	if (m_pParent)
	{
		if (m_bInheritRotZ)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::SetWorldPos(const Vector3& v)
{
	m_vVelocity += v - m_vWorldPos;

	m_vWorldPos = v;

	m_vRelativePos = m_vWorldPos;

	if (m_pParent)
	{
		Matrix tMat;

		tMat.Rotation(m_pParent->GetWorldRot());

		memcpy(&tMat._41, &m_pParent->GetWorldPos(), sizeof(float) * 3);

		tMat.Inverse();

		m_vRelativePos = m_vWorldPos.TranslationCoor(tMat);
	}

	m_bUpdatePos = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritPos();
	}
}

void CUITransform::SetWorldPos(float x, float y, float z)
{
	m_vVelocity += Vector3(x, y, z) - m_vWorldPos;

	m_vWorldPos = Vector3(x, y, z);

	m_vRelativePos = m_vWorldPos;

	if (m_pParent)
	{
		Matrix tMat;

		tMat.Rotation(m_pParent->GetWorldRot());

		memcpy(&tMat._41, &m_pParent->GetWorldPos(), sizeof(float) * 3);

		tMat.Inverse();

		m_vRelativePos = m_vWorldPos.TranslationCoor(tMat);
	}

	m_bUpdatePos = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritPos();
	}
}

void CUITransform::AddWorldScale(const Vector3& v)
{
	m_vVelocityScale += v;

	m_vWorldScale += v;

	m_vRelativeScale = m_vWorldScale;

	if (m_pParent && m_bInheritScale)
		m_vRelativeScale = m_vWorldScale / m_pParent->GetWorldScale();

	m_bUpdateScale = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritScale();
	}
}

void CUITransform::AddWorldScale(float x, float y, float z)
{
	m_vVelocityScale += Vector3(x, y, z);

	m_vWorldScale += Vector3(x, y, z);

	m_vRelativeScale = m_vWorldScale;

	if (m_pParent && m_bInheritScale)
		m_vRelativeScale = m_vWorldScale / m_pParent->GetWorldScale();

	m_bUpdateScale = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritScale();
	}
}

void CUITransform::AddWorldRot(const Vector3& v)
{
	m_vVelocityRot += v;

	m_vWorldRot += v;

	m_vRelativeRot = m_vWorldRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::AddWorldRot(float x, float y, float z)
{
	m_vVelocityRot += Vector3(x, y, z);

	m_vWorldRot += Vector3(x, y, z);

	m_vRelativeRot = m_vWorldRot;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;

		if (m_bInheritRotY)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;

		if (m_bInheritRotZ)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::AddWorldRotX(float x)
{
	m_vVelocityRot.x += x;

	m_vWorldRot.x += x;

	m_vRelativeRot.x = m_vWorldRot.x;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::AddWorldRotY(float y)
{
	m_vVelocityRot.y += y;

	m_vWorldRot.y += y;

	m_vRelativeRot.y = m_vWorldRot.y;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::AddWorldRotZ(float z)
{
	m_vVelocityRot.z += z;

	m_vWorldRot.z += z;

	m_vRelativeRot.z = m_vWorldRot.z;

	if (m_pParent)
	{
		if (m_bInheritRotX)
			m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
	}

	m_bUpdateRot = true;

	Matrix matRot;

	matRot.Rotation(m_vRelativeRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vRelativeAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vRelativeAxis[i].Normalize();
	}

	matRot.Rotation(m_vWorldRot);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TranslationNorm(matRot);
		m_vWorldAxis[i].Normalize();
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritRot();
	}
}

void CUITransform::AddWorldRot(const Vector3& v, float fAngle)
{
	Matrix matRot;

	matRot.RotationAxis(v, fAngle);
}

void CUITransform::AddWorldPos(const Vector3& v)
{
	m_vVelocity += v;

	m_vWorldPos += v;

	m_vRelativePos = m_vWorldPos;

	if (m_pParent)
	{
		Matrix tMat;

		tMat.Rotation(m_pParent->GetWorldRot());

		memcpy(&tMat._41, &m_pParent->GetWorldPos(), sizeof(float) * 3);

		tMat.Inverse();

		m_vRelativePos = m_vWorldPos.TranslationCoor(tMat);
	}

	m_bUpdatePos = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritPos();
	}
}

void CUITransform::AddWorldPos(float x, float y, float z)
{
	m_vVelocity += Vector3(x, y, z);

	m_vWorldPos += Vector3(x, y, z);

	m_vRelativePos = m_vWorldPos;

	if (m_pParent)
	{
		Matrix tMat;

		tMat.Rotation(m_pParent->GetWorldRot());

		memcpy(&tMat._41, &m_pParent->GetWorldPos(), sizeof(float) * 3);

		tMat.Inverse();

		m_vRelativePos = m_vWorldPos.TranslationCoor(tMat);
	}

	m_bUpdatePos = true;

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecChild[i]->InheritPos();
	}
}

void CUITransform::SetPivot(const Vector3& v)
{
	m_vPivot = v;
}

void CUITransform::SetPivot(float x, float y, float z)
{
	SetPivot(Vector3(x, y, z));
}

void CUITransform::SetMeshSize(const Vector3& v)
{
	m_vMeshSize = v;
}

void CUITransform::LookAt(const Vector3& v)
{
	Vector3 vVec = v - m_vWorldPos;

	vVec.Normalize();

	Vector3 vAxis = vVec.Cross(m_vWorldAxis[AXIS_Y]);

	float fAngle = DirectX::XMConvertToDegrees(acosf(vVec.Dot(m_vWorldAxis[AXIS_Y])));


	if (vVec.x < 0.f)
		fAngle *= -1.f;

	if (m_vWorldRot.z > 360.f)
		m_vWorldRot.z -= 360.f;

	else if (m_vWorldRot.z < -360.f)
		m_vWorldRot.z += 360.f;

	AddWorldRotZ(fAngle);
}

void CUITransform::SetInheritScale(bool bInherit)
{
	m_bInheritScale = bInherit;
}
void CUITransform::SetInheritRotX(bool bInherit)
{
	m_bInheritRotX = bInherit;
}

void CUITransform::SetInheritRotY(bool bInherit)
{
	m_bInheritRotY = bInherit;
}

void CUITransform::SetInheritRotZ(bool bInherit)
{
	m_bInheritRotZ = bInherit;
}

Vector3 CUITransform::GetVelocityScale()	const
{
	return m_vVelocityScale;
}

Vector3 CUITransform::GetVelocityRot()	const
{
	return m_vVelocityRot;
}

Vector3 CUITransform::GetVelocity()	const
{
	return m_vVelocity;
}

float CUITransform::GetVelocityAmount()	const
{
	return m_vVelocity.Length();
}

void CUITransform::Start()
{
	m_vVelocityScale = Vector3::Zero;
	m_vVelocityRot = Vector3::Zero;
	m_vVelocity = Vector3::Zero;
}

void CUITransform::Update(float fTime)
{
}

void CUITransform::PostUpdate(float fTime)
{
	if (m_bUpdateScale)
		m_matScale.Scaling(m_vWorldScale);

	if (m_bUpdateRot)
		m_matRot.Rotation(m_vWorldRot);

	if (m_bUpdatePos)
		m_matPos.Translate(m_vWorldPos);

	m_matWorld = m_matScale * m_matRot * m_matPos;
}

void CUITransform::SetTransform()
{
	Resolution tRS = RESOLUTION;

	m_tCBuffer.matWorld = m_matWorld;
	m_tCBuffer.matView.Identity();
	m_tCBuffer.matProj = GET_SINGLE(CUIManager)->GetProjMat();

	m_tCBuffer.matWV = m_matWorld * m_tCBuffer.matView;
	m_tCBuffer.matWVP = m_tCBuffer.matWV * m_tCBuffer.matProj;
	m_tCBuffer.vPivot = m_vPivot;
	m_tCBuffer.vMeshSize = m_vMeshSize;

	m_tCBuffer.matWorld.Transpose();
	m_tCBuffer.matProj.Transpose();
	m_tCBuffer.matWV.Transpose();
	m_tCBuffer.matWVP.Transpose();

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform", &m_tCBuffer);

	m_vVelocityScale = Vector3::Zero;
	m_vVelocityRot = Vector3::Zero;
	m_vVelocity = Vector3::Zero;
}

CUITransform* CUITransform::Clone()
{
	return new CUITransform(*this);
}

void CUITransform::Save(FILE* pFile)
{
	fwrite(&m_vVelocityScale, sizeof(Vector3), 1, pFile);
	fwrite(&m_vVelocityRot, sizeof(Vector3), 1, pFile);
	fwrite(&m_vVelocity, sizeof(Vector3), 1, pFile);
	fwrite(&m_vRelativeScale, sizeof(Vector3), 1, pFile);
	fwrite(&m_vRelativeRot, sizeof(Vector3), 1, pFile);
	fwrite(&m_vRelativePos, sizeof(Vector3), 1, pFile);
	fwrite(m_vRelativeAxis, sizeof(Vector3), AXIS_END, pFile);
	fwrite(&m_bInheritScale, sizeof(bool), 1, pFile);
	fwrite(&m_bInheritRotX, sizeof(bool), 1, pFile);
	fwrite(&m_bInheritRotY, sizeof(bool), 1, pFile);
	fwrite(&m_bInheritRotZ, sizeof(bool), 1, pFile);

	fwrite(&m_vWorldScale, sizeof(Vector3), 1, pFile);
	fwrite(&m_vWorldRot, sizeof(Vector3), 1, pFile);
	fwrite(&m_vWorldPos, sizeof(Vector3), 1, pFile);
	fwrite(m_vWorldAxis, sizeof(Vector3), AXIS_END, pFile);
	fwrite(&m_vPivot, sizeof(Vector3), 1, pFile);
	fwrite(&m_vMeshSize, sizeof(Vector3), 1, pFile);
}

void CUITransform::Load(FILE* pFile)
{
	fread(&m_vVelocityScale, sizeof(Vector3), 1, pFile);
	fread(&m_vVelocityRot, sizeof(Vector3), 1, pFile);
	fread(&m_vVelocity, sizeof(Vector3), 1, pFile);
	fread(&m_vRelativeScale, sizeof(Vector3), 1, pFile);
	fread(&m_vRelativeRot, sizeof(Vector3), 1, pFile);
	fread(&m_vRelativePos, sizeof(Vector3), 1, pFile);
	fread(m_vRelativeAxis, sizeof(Vector3), AXIS_END, pFile);
	fread(&m_bInheritScale, sizeof(bool), 1, pFile);
	fread(&m_bInheritRotX, sizeof(bool), 1, pFile);
	fread(&m_bInheritRotY, sizeof(bool), 1, pFile);
	fread(&m_bInheritRotZ, sizeof(bool), 1, pFile);

	fread(&m_vWorldScale, sizeof(Vector3), 1, pFile);
	fread(&m_vWorldRot, sizeof(Vector3), 1, pFile);
	fread(&m_vWorldPos, sizeof(Vector3), 1, pFile);
	fread(m_vWorldAxis, sizeof(Vector3), AXIS_END, pFile);
	fread(&m_vPivot, sizeof(Vector3), 1, pFile);
	fread(&m_vMeshSize, sizeof(Vector3), 1, pFile);

	m_bUpdatePos = true;
	m_bUpdateRot = true;
	m_bUpdateScale = true;
}
