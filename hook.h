#pragma once
#include "structure.h"

namespace pkodev
{
	namespace hook
	{
		// BOOL	SC_CHARSTR(LPRPACKET pk)
		void __cdecl SC_CHARSTR(void* pk);

		// bool CWorldScene::_Init()
		bool __fastcall CWorldScene___Init(void* This, void* NotUsed);

		// void CWorldScene::_FrameMove( DWORD dwTimeParam )
		void __fastcall CWorldScene___FrameMove(void* This, void* NotUsed, unsigned int dwTimeParam);
	}
}