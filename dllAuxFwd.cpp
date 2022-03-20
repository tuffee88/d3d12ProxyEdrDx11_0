#include <Windows.h>
#include <d3d12.h>
#include <d3dcommon.h>

extern FARPROC D3D12CoreCreateLayeredDeviceOrg;
extern FARPROC D3D12CoreGetLayeredDeviceSizeOrg;
extern FARPROC D3D12CoreRegisterLayersOrg;

extern FARPROC D3D12CreateRootSignatureDeserializerOrg;
extern FARPROC D3D12CreateVersionedRootSignatureDeserializerOrg;

extern FARPROC D3D12DeviceRemovedExtendedDataOrg;
extern FARPROC D3D12EnableExperimentalFeaturesOrg;
extern FARPROC D3D12GetDebugInterfaceOrg;

extern FARPROC D3D12GetInterfaceOrg;
extern FARPROC D3D12PIXEventsReplaceBlockOrg;
extern FARPROC D3D12PIXGetThreadInfoOrg;

extern FARPROC D3D12PIXNotifyWakeFromFenceSignalOrg;
extern FARPROC D3D12PIXReportCounterOrg;
extern FARPROC D3D12SerializeRootSignatureOrg;

extern FARPROC D3D12SerializeVersionedRootSignatureOrg;
extern FARPROC GetBehaviorValueOrg;
extern FARPROC SetAppCompatStringPointerOrg;

extern "C" void Proxy_CallOrgFcnAsm(void);

extern "C" FARPROC FcnPtrOrg = NULL;


/*Add simple forwarding proxies for other exported fcns for now*/

/*For functions with well-known public declarations: Define full function prototypes for forwarding (=plays nice with Debug builds, ecc)*/
typedef HRESULT(__cdecl* D3D12CreateRootSignatureDeserializerFnc)(LPCVOID, SIZE_T, REFIID, void**);
typedef HRESULT(__cdecl* D3D12CreateVersionedRootSignatureDeserializerFnc)(LPCVOID, SIZE_T, REFIID, void**);
typedef HRESULT(__cdecl* D3D12EnableExperimentalFeaturesFnc)(UINT, const IID*, void*, UINT*);
typedef HRESULT(__cdecl* D3D12GetDebugInterfaceFnc)(REFIID, void**);
typedef HRESULT(__cdecl* D3D12GetInterfaceFnc)(REFCLSID, REFIID, void**);
typedef HRESULT(__cdecl* D3D12SerializeRootSignatureFnc)(const D3D12_ROOT_SIGNATURE_DESC*, D3D_ROOT_SIGNATURE_VERSION, ID3DBlob**, ID3DBlob**);
typedef HRESULT(__cdecl* D3D12SerializeVersionedRootSignatureFnc)(const D3D12_VERSIONED_ROOT_SIGNATURE_DESC*, ID3DBlob**, ID3DBlob**);

HRESULT Proxy_D3D12CreateRootSignatureDeserializer(
    LPCVOID pSrcData,
    SIZE_T  SrcDataSizeInBytes,
    REFIID  pRootSignatureDeserializerInterface,
    void** ppRootSignatureDeserializer)
{
    return ((D3D12CreateRootSignatureDeserializerFnc)D3D12CreateRootSignatureDeserializerOrg)(pSrcData, SrcDataSizeInBytes, pRootSignatureDeserializerInterface, ppRootSignatureDeserializer);
}

HRESULT Proxy_D3D12CreateVersionedRootSignatureDeserializer(
    LPCVOID pSrcData,
    SIZE_T  SrcDataSizeInBytes,
    REFIID  pRootSignatureDeserializerInterface,
    void** ppRootSignatureDeserializer)
{
    return ((D3D12CreateVersionedRootSignatureDeserializerFnc)D3D12CreateVersionedRootSignatureDeserializerOrg)(pSrcData, SrcDataSizeInBytes, pRootSignatureDeserializerInterface, ppRootSignatureDeserializer);
}

HRESULT Proxy_D3D12EnableExperimentalFeatures(
    UINT      NumFeatures,
    const IID* pIIDs,
    void* pConfigurationStructs,
    UINT* pConfigurationStructSizes)
{
    return ((D3D12EnableExperimentalFeaturesFnc)D3D12EnableExperimentalFeaturesOrg)(NumFeatures, pIIDs, pConfigurationStructs, pConfigurationStructSizes);
}

HRESULT Proxy_D3D12GetDebugInterface(
    REFIID riid,
    void** ppvDebug)
{
    return ((D3D12GetDebugInterfaceFnc)D3D12GetDebugInterfaceOrg)(riid, ppvDebug);
}

HRESULT Proxy_D3D12GetInterface(
    REFCLSID rclsid,
    REFIID   riid,
    void** ppvDebug)
{
    return ((D3D12GetInterfaceFnc)D3D12GetInterfaceOrg)(rclsid, riid, ppvDebug);
}

HRESULT Proxy_D3D12SerializeRootSignature(
    const D3D12_ROOT_SIGNATURE_DESC* pRootSignature,
    D3D_ROOT_SIGNATURE_VERSION      Version,
    ID3DBlob** ppBlob,
    ID3DBlob** ppErrorBlob)
{
    return ((D3D12SerializeRootSignatureFnc)D3D12SerializeRootSignatureOrg)(pRootSignature, Version, ppBlob, ppErrorBlob);
}

HRESULT Proxy_D3D12SerializeVersionedRootSignature(
    const D3D12_VERSIONED_ROOT_SIGNATURE_DESC* pRootSignature,
    ID3DBlob** ppBlob,
    ID3DBlob** ppErrorBlob)
{
    return ((D3D12SerializeVersionedRootSignatureFnc)D3D12SerializeVersionedRootSignatureOrg)(pRootSignature, ppBlob, ppErrorBlob);
}


/*For functions with unkown public declarations: Rely on forwarding via ASM #jmp instruction (see dllAuxFwdAsm.asm) -> works for unkown prototypes AS LONG as the stack and registers remains untouched (=problematic for Debug builds)*/

extern "C" {
    void  Proxy_D3D12CoreCreateLayeredDevice() { FcnPtrOrg = D3D12CoreCreateLayeredDeviceOrg; Proxy_CallOrgFcnAsm(); }
    void  Proxy_D3D12CoreGetLayeredDeviceSize() { FcnPtrOrg = D3D12CoreGetLayeredDeviceSizeOrg; Proxy_CallOrgFcnAsm(); }
    void  Proxy_D3D12CoreRegisterLayers() { FcnPtrOrg = D3D12CoreRegisterLayersOrg; Proxy_CallOrgFcnAsm(); }

    //void  Proxy_D3D12CreateRootSignatureDeserializer() { FcnPtrOrg = D3D12CreateRootSignatureDeserializerOrg; Proxy_CallOrgFcnAsm(); }
    //void  Proxy_D3D12CreateVersionedRootSignatureDeserializer() { FcnPtrOrg = D3D12CreateVersionedRootSignatureDeserializerOrg; Proxy_CallOrgFcnAsm(); }
    void  Proxy_D3D12DeviceRemovedExtendedData() { FcnPtrOrg = D3D12DeviceRemovedExtendedDataOrg; Proxy_CallOrgFcnAsm(); }

    //void  Proxy_D3D12EnableExperimentalFeatures() { FcnPtrOrg = D3D12EnableExperimentalFeaturesOrg; Proxy_CallOrgFcnAsm(); }
    //void  Proxy_D3D12GetDebugInterface() { FcnPtrOrg = D3D12GetDebugInterfaceOrg; Proxy_CallOrgFcnAsm(); }
    //void  Proxy_D3D12GetInterface() { FcnPtrOrg = D3D12GetInterfaceOrg; Proxy_CallOrgFcnAsm(); }

    void  Proxy_D3D12PIXEventsReplaceBlock() { FcnPtrOrg = D3D12PIXEventsReplaceBlockOrg; Proxy_CallOrgFcnAsm(); }
    void  Proxy_D3D12PIXGetThreadInfo() { FcnPtrOrg = D3D12PIXGetThreadInfoOrg; Proxy_CallOrgFcnAsm(); }
    void  Proxy_D3D12PIXNotifyWakeFromFenceSignal() { FcnPtrOrg = D3D12PIXNotifyWakeFromFenceSignalOrg; Proxy_CallOrgFcnAsm(); }

    void  Proxy_D3D12PIXReportCounter() { FcnPtrOrg = D3D12PIXReportCounterOrg; Proxy_CallOrgFcnAsm(); }
    //void  Proxy_D3D12SerializeRootSignature() { FcnPtrOrg = D3D12SerializeRootSignatureOrg; Proxy_CallOrgFcnAsm(); }
    //void  Proxy_D3D12SerializeVersionedRootSignature() { FcnPtrOrg = D3D12SerializeVersionedRootSignatureOrg; Proxy_CallOrgFcnAsm(); }

    void  Proxy_GetBehaviorValue() { FcnPtrOrg = GetBehaviorValueOrg; Proxy_CallOrgFcnAsm(); }
    void  Proxy_SetAppCompatStringPointer() { FcnPtrOrg = SetAppCompatStringPointerOrg; Proxy_CallOrgFcnAsm(); }
}

