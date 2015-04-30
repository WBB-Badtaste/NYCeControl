#include "StdAfx.h"
#include "Axis_Base.h"

Axis_Base::Axis_Base(uint32_t num, const char* const names[], BOOL bSim) : m_axNum(num), NYCe_Base(bSim)
{
	m_pAxIds	= new SAC_AXIS[m_axNum];
	m_pAxCons	= new BOOL[m_axNum];
	m_pAxNames	= new char*[m_axNum];

	ZeroMemory(m_pAxIds,	sizeof(SAC_AXIS) * m_axNum);
	ZeroMemory(m_pAxCons,	sizeof(BOOL)	 * m_axNum);
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
	DisconnectAxes();

	for (uint32_t ax = 0; ax < m_axNum; ++ax) delete[] m_pAxNames[ax];
	
	delete[] m_pAxNames;
	delete[] m_pAxIds;
	delete[] m_pAxCons;

	
}

BOOL Axis_Base::ConnectAxes()
{
	for (uint32_t ax = 0; ax < m_axNum; ax++ )
	{
		if (m_pAxCons[ax]) continue;

		nyceStatus = SacConnect( m_pAxNames[ ax ], &(m_pAxIds[ ax ]) );
		if (NyceError(nyceStatus))
		{
			EHer.HandleError(nyceStatus, m_pAxNames[ ax ]);
			return FALSE;
		}
		m_pAxCons[ax] = TRUE;
	}
	return TRUE;
}

BOOL Axis_Base::DisconnectAxes()
{
	for (uint32_t ax = 0; ax < m_axNum; ax++ )
	{
		if (!m_pAxCons[ax]) continue;

		nyceStatus = SacDisconnect( m_pAxIds[ ax ] );
		if (NyceError(nyceStatus))
		{
			EHer.HandleError(nyceStatus, m_pAxNames[ ax ]);
			return FALSE;
		}
		m_pAxCons[ax] = FALSE;
	}
	return TRUE;
}

BOOL Axis_Base::Reconnect()
{
	return DisconnectAxes() && ConnectAxes();
}