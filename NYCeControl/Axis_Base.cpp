#include "StdAfx.h"
#include "Axis_Base.h"

Axis_Base::Axis_Base(uint32_t num, const char* const names[], BOOL bSim) : m_axNum(num), NYCe_Base(bSim)
{
	m_pAxId		= new SAC_AXIS[m_axNum];
	m_pAxCon	= new BOOL[m_axNum];
	m_pAxNames	= new char*[m_axNum];

	ZeroMemory(m_pAxId,		sizeof(SAC_AXIS) * m_axNum);
	ZeroMemory(m_pAxCon,	sizeof(BOOL)	 * m_axNum);
	ZeroMemory(m_pAxNames,	sizeof(char*)	 * m_axNum);

	for(uint32_t ax = 0; ax < m_axNum; ++ax) 
	{
		size_t len = strlen(names[ax]);
		m_pAxNames[ax] = new char[len + 1];
		uint32_t i(0);
		while(m_pAxNames[ax][i] = names[ax][i]) ++i;
	}

	ConnectAxes();
}

Axis_Base::~Axis_Base(void)
{
	for (uint32_t ax = 0; ax < m_axNum; ++ax) delete[] m_pAxNames[ax];
	
	delete[] m_pAxNames;
	delete[] m_pAxId;
	delete[] m_pAxCon;

	DisconnectAxes();
}

BOOL Axis_Base::ConnectAxes()
{
	for (uint32_t ax = 0; ax < m_axNum; ax++ )
	{
		if (m_pAxCon[ax]) continue;

		nyceStatus = SacConnect( m_pAxNames[ ax ], &m_pAxId[ ax ] );
		if (NyceError(nyceStatus))
		{
			EHer.HandleError(nyceStatus, m_pAxNames[ ax ]);
			return FALSE;
		}
		m_pAxCon[ax] = TRUE;
	}
	return TRUE;
}

BOOL Axis_Base::DisconnectAxes()
{
	for (uint32_t ax = 0; ax < m_axNum; ax++ )
	{
		if (!m_pAxCon[ax]) continue;

		nyceStatus = SacDisconnect( m_pAxId[ ax ] );
		if (NyceError(nyceStatus))
		{
			EHer.HandleError(nyceStatus, m_pAxNames[ ax ]);
			return FALSE;
		}
		m_pAxCon[ax] = FALSE;
	}
	return TRUE;
}

BOOL Axis_Base::Reconnect()
{
	return DisconnectAxes() && ConnectAxes();
}