#pragma once

#include "OEMBase.h"
#include "matrix.h"
#include "Bounds3f.h"

class COMMON_BASE_EXPORT IRenderInterface
{
public:
	IRenderInterface()
		: m_nForceLODLvl(0)
		, m_nDestrLvl(0)
		, m_Flags(0)
	{
	}

	inline_ Matrix			GetRWTM() const { return m_WorldMatrixTransform; }
	inline_ void			SetRWTM(const Matrix &matrix) { m_WorldMatrixTransform = matrix; }

	virtual void			SetRBounds_(const Bounds3f &bound) { m_Bounds = bound; }
	virtual const Bounds3f&	GetRBounds_() const { return m_Bounds; }

	virtual void			SetWBounds(const Bounds3f &bound) { m_WorldBounds = bound; }
	virtual const Bounds3f&	GetWBounds() const { return m_WorldBounds; }

	virtual unsigned int	GetForceLODLvl() const { return m_nForceLODLvl; }
	virtual void			SetForceLODLvl(unsigned int Value) { m_nForceLODLvl = Value; }

	virtual unsigned int	GetDestructLvl() const { return m_nDestrLvl; }
	virtual void			SetDestructLvl(unsigned int Value) { m_nDestrLvl = Value; }

	unsigned int			GetFlags() const { return m_Flags; }
	void					SetFlags(unsigned int Value) { m_Flags = Value; }

protected:
	/** precomputed world matrix */
	mutable Matrix		m_WorldMatrixTransform;

	Bounds3f			m_Bounds;
	Bounds3f			m_WorldBounds;

	unsigned int		m_nForceLODLvl;
	unsigned int		m_nDestrLvl;

	unsigned int		m_Flags;
};