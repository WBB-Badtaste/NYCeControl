#include "StdAfx.h"
#include "Axis_Motion.h"



Axis_Motion::Axis_Motion(uint32_t num, const char* const names[], BOOL bSim):Axis_Base(num, names, bSim), NYCe_Base(bSim)
{
	m_pAxHandle = new BOOL[m_axNum];
}

Axis_Motion::~Axis_Motion(void)
{
	delete[] m_pAxHandle;
}

void Axis_Motion::TermAxis(void)
{
	uint32_t ax;
	SAC_STATE sacState = SAC_NO_STATE;
	SAC_SPG_STATE sacSpgState;
	nyceStatus = NYCE_OK;
	for (ax = 0; ax < m_axNum; ax++ )
	{

		if (m_pAxHandle[ax])
		{
			nyceStatus = NyceError(nyceStatus) ? nyceStatus : SacReadState(m_pAxId[ ax ], &sacState, &sacSpgState);
			if(NyceSuccess(nyceStatus) && sacState == SAC_MOVING)
			{
				nyceStatus = NyceError(nyceStatus) ? nyceStatus : SacQuickStop(m_pAxId[ ax ]);
				if (NyceSuccess(nyceStatus))
				{
					nyceStatus = NyceError(nyceStatus) ? nyceStatus : SacSynchronize( m_pAxId[ ax ], SAC_REQ_MOTION_STOPPED, 10 );
					if (NyceError(nyceStatus))
					{
						EHer.HandleError(nyceStatus, m_pAxNames[ ax ]);
						nyceStatus = NyceError(nyceStatus) ? nyceStatus : SacReset(m_pAxId[ ax ]);
						nyceStatus = NyceError(nyceStatus) ? nyceStatus : SacSynchronize( m_pAxId[ ax ], SAC_REQ_RESET, 10 );
					}
				}
			}
			nyceStatus = NyceError(nyceStatus) ? nyceStatus : SacShutdown(m_pAxId[ ax ]);
			nyceStatus = NyceError(nyceStatus) ? nyceStatus : SacSynchronize( m_pAxId[ ax ], SAC_REQ_SHUTDOWN, 10 );
		}

		nyceStatus = NyceError(nyceStatus) ? nyceStatus : SacDisconnect(m_pAxId[ ax ]);
		if(NyceError(nyceStatus)) EHer.HandleError(nyceStatus, m_pAxNames[ ax ]);
	}
}

BOOL Axis_Motion::InitAxis(void)
{
	uint32_t ax;
	SAC_SPG_STATE sacSpgState;
	SAC_STATE sacState;
	SAC_CONFIGURE_AXIS_PARS axisPars;
	nyceStatus = NYCE_OK;
	for (ax = 0; ax < m_axNum; ax++ )
	{

		nyceStatus =  NyceError(nyceStatus) ? nyceStatus : SacReadState( m_pAxId[ ax ], &sacState, &sacSpgState);
		if(NyceSuccess(nyceStatus))
		{
			switch (sacState)
			{
			case SAC_IDLE:
				nyceStatus =  NyceError(nyceStatus) ? nyceStatus : SacInitialize( m_pAxId[ ax ], SAC_USE_FLASH );
				nyceStatus =  NyceError(nyceStatus) ? nyceStatus : SacSynchronize( m_pAxId[ ax ], SAC_REQ_INITIALIZE, 10 );
				m_pAxHandle[ax] = NyceSuccess(nyceStatus) ? TRUE :FALSE;
				nyceStatus =  NyceError(nyceStatus) ? nyceStatus : SacGetAxisConfiguration( m_pAxId[ ax ], &axisPars );
				if ( NyceSuccess(nyceStatus) && axisPars.motorType == SAC_BRUSHLESS_AC_MOTOR )
				{
					nyceStatus =  NyceError(nyceStatus) ? nyceStatus : SacAlignMotor( m_pAxId[ ax ] );
					nyceStatus =  NyceError(nyceStatus) ? nyceStatus : SacSynchronize( m_pAxId[ ax ], SAC_REQ_ALIGN_MOTOR, 10 );
				}
			case SAC_FREE:
				nyceStatus =  NyceError(nyceStatus) ? nyceStatus : SacLock( m_pAxId[ ax ] );
				nyceStatus =  NyceError(nyceStatus) ? nyceStatus : SacSynchronize( m_pAxId[ ax ], SAC_REQ_LOCK, 10 );
				nyceStatus =  NyceError(nyceStatus) ? nyceStatus : SacHome( m_pAxId[ ax ] );
				nyceStatus =  NyceError(nyceStatus) ? nyceStatus : SacSynchronize( m_pAxId[ ax ], SAC_REQ_HOMING_COMPLETED, 10 );
				break;
			case SAC_MOVING:
				printf("Waiting the motion stop...");
				nyceStatus =  NyceError(nyceStatus) ? nyceStatus : SacSynchronize( m_pAxId[ ax ], SAC_REQ_MOTION_STOPPED, 30 );
				break;
			default:
				nyceStatus =  NyceError(nyceStatus) ? nyceStatus : SacShutdown( m_pAxId[ ax ]);
				nyceStatus =  NyceError(nyceStatus) ? nyceStatus : SacSynchronize( m_pAxId[ ax ], SAC_REQ_SHUTDOWN, 10 );
				break;
			}
		}

		if(NyceError(nyceStatus))
		{
			EHer.HandleError(nyceStatus, m_pAxNames[ ax ]);
			TermAxis();
			return FALSE;
		}
	}

	return TRUE;
}

