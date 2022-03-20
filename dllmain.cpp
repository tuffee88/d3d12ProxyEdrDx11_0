
/*Dumpbin /exports d3d12.dll:
104    0 00004910 D3D12CoreCreateLayeredDevice
105    1 000048D0 D3D12CoreGetLayeredDeviceSize
106    2 000047E0 D3D12CoreRegisterLayers

101    3 00006AF0 D3D12CreateDevice
107    4 00010150 D3D12CreateRootSignatureDeserializer
108    5 000101B0 D3D12CreateVersionedRootSignatureDeserializer

109    6 00019218 D3D12DeviceRemovedExtendedData
110    7 00010210 D3D12EnableExperimentalFeatures
102    8 00010270 D3D12GetDebugInterface

111    9 00008F40 D3D12GetInterface
112    A 00010940 D3D12PIXEventsReplaceBlock
113    B 00010950 D3D12PIXGetThreadInfo

114    C 00010960 D3D12PIXNotifyWakeFromFenceSignal
115    D 00010990 D3D12PIXReportCounter
116    E 000102B0 D3D12SerializeRootSignature

117    F 00010310 D3D12SerializeVersionedRootSignature
100   10 00006ED0 GetBehaviorValue
103   11 0000FF40 SetAppCompatStringPointer
*/

#include <Windows.h>
#include <string>
#include <stdio.h>
#include <d3d12.h>
#include <d3dcommon.h>

extern void enablePixRuntimeAttach(void);

/*This is probably the noob way to do it, but wtf...*/
typedef HRESULT(__cdecl* D3D12CreateDeviceFnc)(IUnknown*, D3D_FEATURE_LEVEL, REFIID, void**);

/*Global stuff...*/
HMODULE hL = 0;
D3D12CreateDeviceFnc D3D12CreateDeviceOrg;

/*Functions that are just forwarded...at least for now -> See dllAuxFwd.cpp for details*/
FARPROC D3D12CoreCreateLayeredDeviceOrg;
FARPROC D3D12CoreGetLayeredDeviceSizeOrg;
FARPROC D3D12CoreRegisterLayersOrg; 

FARPROC D3D12CreateRootSignatureDeserializerOrg;
FARPROC D3D12CreateVersionedRootSignatureDeserializerOrg;

FARPROC D3D12DeviceRemovedExtendedDataOrg;
FARPROC D3D12EnableExperimentalFeaturesOrg;
FARPROC D3D12GetDebugInterfaceOrg;

FARPROC D3D12GetInterfaceOrg;
FARPROC D3D12PIXEventsReplaceBlockOrg; 
FARPROC D3D12PIXGetThreadInfoOrg; 

FARPROC D3D12PIXNotifyWakeFromFenceSignalOrg;
FARPROC D3D12PIXReportCounterOrg; 
FARPROC D3D12SerializeRootSignatureOrg;

FARPROC D3D12SerializeVersionedRootSignatureOrg;
FARPROC GetBehaviorValueOrg; 
FARPROC SetAppCompatStringPointerOrg; 



/*Main Entry point*/
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        OutputDebugString(L"Proxy Dll process attach\n");

        /*Load original function dll: Try application directory first - fall back and try to load the system32 d3d12.dll if unsuccessfull*/
        hL = LoadLibraryW(L".\\d3d12_original.dll"); 
        if (hL != NULL)
        {
            OutputDebugString(L"D3D12CreateDevice - Loaded d3d12_original.dll\n");
        }
        else 
        {
            hL = LoadLibraryEx(L".\\d3d12.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
            OutputDebugString(L"D3D12CreateDevice - Couldn't load d3d12_original.dll attempt to load system d3d12.dll instead\n");
        }

        /*Provide feedback*/
        wchar_t fB[100];
        swprintf(fB, 100, L"D3D12CreateDevice - LoadLibrary Result: %llx\n", (unsigned long long)hL);
        OutputDebugString(fB);            

        if (hL == NULL)return FALSE; /*Exit on Error*/

        /*Attempt to insert Pix runtime if avilable - see https://devblogs.microsoft.com/pix/taking-a-capture/ for extended debugging purposes, have a look at dllAuxDbg.cpp for details*/
        OutputDebugString(L"D3D12CreateDevice - DEBUG: Checking Pix support\n");
        enablePixRuntimeAttach();       

        /*Get function addrs from original dll*/
        D3D12CreateDeviceOrg = (D3D12CreateDeviceFnc)GetProcAddress(hL, "D3D12CreateDevice");
        
        D3D12CoreCreateLayeredDeviceOrg = GetProcAddress(hL, "D3D12CoreCreateLayeredDevice");              
        D3D12CoreGetLayeredDeviceSizeOrg = GetProcAddress(hL, "D3D12CoreGetLayeredDeviceSize");
        D3D12CoreRegisterLayersOrg = GetProcAddress(hL, "D3D12CoreRegisterLayers");

        D3D12CreateRootSignatureDeserializerOrg = GetProcAddress(hL, "D3D12CreateRootSignatureDeserializer");
        D3D12CreateVersionedRootSignatureDeserializerOrg = GetProcAddress(hL, "D3D12CreateVersionedRootSignatureDeserializer");

        D3D12DeviceRemovedExtendedDataOrg = GetProcAddress(hL, "D3D12DeviceRemovedExtendedData");
        D3D12EnableExperimentalFeaturesOrg = GetProcAddress(hL, "D3D12EnableExperimentalFeatures");
        D3D12GetDebugInterfaceOrg = GetProcAddress(hL, "D3D12GetDebugInterface");

        D3D12GetInterfaceOrg = GetProcAddress(hL, "D3D12GetInterface");
        D3D12PIXEventsReplaceBlockOrg = GetProcAddress(hL, "D3D12PIXEventsReplaceBlock");
        D3D12PIXGetThreadInfoOrg = GetProcAddress(hL, "D3D12PIXGetThreadInfo");

        D3D12PIXNotifyWakeFromFenceSignalOrg = GetProcAddress(hL, "D3D12PIXNotifyWakeFromFenceSignal");
        D3D12PIXReportCounterOrg = GetProcAddress(hL, "D3D12PIXReportCounter");
        D3D12SerializeRootSignatureOrg = GetProcAddress(hL, "D3D12SerializeRootSignature");

        D3D12SerializeVersionedRootSignatureOrg = GetProcAddress(hL, "D3D12SerializeVersionedRootSignature");
        GetBehaviorValueOrg = GetProcAddress(hL, "GetBehaviorValue");
        SetAppCompatStringPointerOrg = GetProcAddress(hL, "SetAppCompatStringPointer");
                
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        OutputDebugString(L"Proxy Dll process detach\n");
        FreeLibrary(hL);
        break;
    }
    return TRUE;
}

/*Proxy D3D12CreateDevice function*/
HRESULT Proxy_D3D12CreateDevice(
    IUnknown* pAdapter,
    D3D_FEATURE_LEVEL MinimumFeatureLevel,
    REFIID            riid,
    void** ppDevice)
{

    HRESULT hD = 0;
    wchar_t debugBuffer[100];
    D3D_FEATURE_LEVEL ModifiedFeatureLevel = D3D_FEATURE_LEVEL_11_0;

    OutputDebugString(L"D3D12CreateDevice - Proxy function called!\n");

    /*Check function addr from original dll and call D3D12CreateDevice with modified MinimumFeatureLevel parameter*/
    if(D3D12CreateDeviceOrg != NULL) OutputDebugString(L"D3D12CreateDevice - Original function address available");
    else { OutputDebugString(L"D3D12CreateDevice - Original function address unavailable, quitting!"); return S_FALSE; }

    hD = D3D12CreateDeviceOrg(
        pAdapter,
        ModifiedFeatureLevel,
        riid,
        ppDevice);
    
    /*Feedback via DebugString*/
    swprintf(debugBuffer,100, L"D3D12CreateDevice - CallResult: %llx\n", (unsigned long long)hD);
    OutputDebugString(debugBuffer);

    /*Extract some additonal useful information from the return code*/
    if(hD == S_OK) swprintf(debugBuffer, 100, L"D3D12CreateDevice - CallResult indicates SUCCESS (S_OK)\n");
    if(hD == S_FALSE) swprintf(debugBuffer, 100, L"D3D12CreateDevice - CallResult indicates FAILURE (S_FALSE)\n");
    OutputDebugString(debugBuffer);

    if (hD >= 0x887A0000)
    {
        swprintf(debugBuffer, 100, L"Possible DXGI_ERROR code, check MSDN for more information on: %llx\n", (unsigned long long)hD);    
        OutputDebugString(debugBuffer);
    }

    return hD;
}



