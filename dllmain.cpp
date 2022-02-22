#include <Windows.h>
#include <detours.h>

#include <cstdio>
#include <fstream>
#include <string>
#include <ctime>

#include "loader.h"

#include "address.h"
#include "pointer.h"
#include "hook.h"
#include "structure.h"

namespace pkodev
{
    namespace global
    {
        // Server time at the time of connection
        std::time_t start = 0;

        // Client ticks count at the time of connection
        unsigned __int64 tick = 0;
   
        // Clock update timer
        unsigned int timer = 0;

        // Pointer to the clock label
        void* label = nullptr;

        // Clock format
        std::string format{ "%H:%M:%S %d.%m.%y" };
    }
}

// Entry point
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    // Nothing to do . . .
    return TRUE;
}

// Get mod information
void GetModInformation(mod_info& info)
{
    strcpy_s(info.name, TOSTRING(MOD_NAME));
    strcpy_s(info.version, TOSTRING(MOD_VERSION));
    strcpy_s(info.author, TOSTRING(MOD_AUTHOR));
    info.exe_version = MOD_EXE_VERSION;
}

// Start the mod
void Start(const char* path)
{
    // Build path to the config file
    char buf[MAX_PATH]{ 0x00 };
    sprintf_s(buf, sizeof(buf), "%s\\%s.cfg", path, TOSTRING(MOD_NAME));

    // Open the file
    std::ifstream file(buf, std::ifstream::in);

    // Check that file is open
    if (file.is_open() == true)
    {
        // Read the clock format
        std::getline(file, pkodev::global::format);

        // Close the file
        file.close();
    }

    // Enable hooks
    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)pkodev::pointer::SC_CHARSTR, pkodev::hook::SC_CHARSTR);
    DetourAttach(&(PVOID&)pkodev::pointer::CWorldScene___Init, pkodev::hook::CWorldScene___Init);
    DetourAttach(&(PVOID&)pkodev::pointer::CWorldScene___FrameMove, pkodev::hook::CWorldScene___FrameMove);
    DetourTransactionCommit();
}

// Stop the mod
void Stop()
{
    // Disable hooks
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)pkodev::pointer::SC_CHARSTR, pkodev::hook::SC_CHARSTR);
    DetourDetach(&(PVOID&)pkodev::pointer::CWorldScene___Init, pkodev::hook::CWorldScene___Init);
    DetourDetach(&(PVOID&)pkodev::pointer::CWorldScene___FrameMove, pkodev::hook::CWorldScene___FrameMove);
    DetourTransactionCommit();
}

// BOOL	SC_CHARSTR(LPRPACKET pk)
void __cdecl pkodev::hook::SC_CHARSTR(void* pk)
{
    // Call the original function BOOL	SC_CHARSTR(LPRPACKET pk)
    pkodev::pointer::SC_CHARSTR(pk);

    // Get chap string
    const char* chap_str = reinterpret_cast<const char*>(
        reinterpret_cast<void *>(
            *reinterpret_cast<unsigned int*>(
                reinterpret_cast<void*>(pkodev::address::MOD_EXE_VERSION::g_NetIF)
            ) + 0x0348
        )
    );

    // Get some data from chap string [01-05 16:18:59:572]
    unsigned int month, day, hour, minute, second, millisecond;
    sscanf_s(chap_str, "[%d-%d %d:%d:%d:%d]", &month, &day, &hour,
        &minute, &second, &millisecond);

    // Create the structure std::tm
    std::time_t t = std::time(nullptr);
    std::tm tm;
    localtime_s(&tm, &t);

    // Fill in the structure
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = second;
    tm.tm_isdst = -1;

    // Save current server time
    pkodev::global::start = std::mktime(&tm);
    
    // Save current tick count
    pkodev::global::tick = GetTickCount64();
}

// bool CWorldScene::_Init()
bool __fastcall pkodev::hook::CWorldScene___Init(void* This, void* NotUsed)
{
    // Call the original method bool CWorldScene::_Init()
    bool ret = pkodev::pointer::CWorldScene___Init(This);

    // Check the result
    if (ret == true)
    {
        // Get the pointer to the clock label
        if (pkodev::global::label == nullptr)
        {
            // Search minimap form
            void* frmMinimap = pkodev::pointer::CUIInterface___FindForm(
                reinterpret_cast<void*>(pkodev::address::MOD_EXE_VERSION::CUIInterface),
                "frmMinimap"
            );

            // Check that the form is found
            if (frmMinimap != nullptr)
            {
                // Search the label
                pkodev::global::label = pkodev::pointer::CForm__Find(frmMinimap, "labClock");
            }
        }
    }

    return ret;
}

// void CWorldScene::_FrameMove( DWORD dwTimeParam )
void __fastcall pkodev::hook::CWorldScene___FrameMove(void* This, void* NotUsed, 
    unsigned int dwTimeParam)
{
    // Call the original method void CWorldScene::_FrameMove( DWORD dwTimeParam )
    pkodev::pointer::CWorldScene___FrameMove(This, dwTimeParam);

    // Timer 1000 ms
    if ( (dwTimeParam - pkodev::global::timer) >= 1000 )
    {
        // Update timer
        pkodev::global::timer = dwTimeParam;

        // Update the label
        if (pkodev::global::label != nullptr)
        {
            // Calculate current server time
            // T = ts + dt + 1 = ts + (t1 - t0) + 1
            const __int64 T = pkodev::global::start + ( (GetTickCount64() - pkodev::global::tick) / 1000 ) + 1;

            // Create the structure std::tm
            static std::tm tm;
            localtime_s(&tm, &T);

            // Build the string with server time
            static char buf[128]{ 0x00 };
            std::strftime(buf, sizeof(buf), pkodev::global::format.c_str(), &tm);

            // Set the label text
            pkodev::pointer::CLabel__SetCaption(pkodev::global::label, buf);
        }
    }
}