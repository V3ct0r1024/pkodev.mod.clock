#pragma once
#include "address.h"
#include "structure.h"

namespace pkodev
{
	namespace pointer
	{
		// BOOL	SC_CHARSTR(LPRPACKET pk)
		typedef bool(__cdecl* SC_CHARSTR__Ptr)(void*);
		SC_CHARSTR__Ptr SC_CHARSTR = (SC_CHARSTR__Ptr)(void*)(pkodev::address::MOD_EXE_VERSION::SC_CHARSTR);
		
		// bool CWorldScene::_Init()
		typedef bool(__thiscall* CWorldScene___Init__Ptr)(void*);
		CWorldScene___Init__Ptr CWorldScene___Init = (CWorldScene___Init__Ptr)(void*)(pkodev::address::MOD_EXE_VERSION::CWorldScene___Init);

		// void CWorldScene::_FrameMove( DWORD dwTimeParam )
		typedef void(__thiscall* CWorldScene___FrameMove__Ptr)(void*, unsigned int);
		CWorldScene___FrameMove__Ptr CWorldScene___FrameMove = (CWorldScene___FrameMove__Ptr)(void*)(pkodev::address::MOD_EXE_VERSION::CWorldScene___FrameMove);

		// CForm* CUIInterface::_FindForm(const char * frmName)
		typedef void* (__thiscall* CUIInterface___FindForm__Ptr)(void*, const char*);
		CUIInterface___FindForm__Ptr CUIInterface___FindForm = (CUIInterface___FindForm__Ptr)(void*)(address::MOD_EXE_VERSION::CUIInterface___FindForm);

		// CCompent* CForm::Find( const char* str )
		typedef void* (__thiscall* CForm__Find__Ptr)(void*, const char*);
		CForm__Find__Ptr CForm__Find = (CForm__Find__Ptr)(void*)(address::MOD_EXE_VERSION::CForm__Find);

		// void CLabel::SetCaption( const char * str )
		typedef void(__thiscall* CLabel__SetCaption__Ptr)(void*, const char*);
		CLabel__SetCaption__Ptr CLabel__SetCaption = (CLabel__SetCaption__Ptr)(void*)(address::MOD_EXE_VERSION::CLabel__SetCaption);
	}
}