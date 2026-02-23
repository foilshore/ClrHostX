#include "HostCLR.h"
#include <metahost.h>
#include <iostream>
#pragma comment(lib, "mscoree.lib")

void ClrHost::RunAssemblyInMemory(const std::vector<unsigned char>& assemblyBytes) {
    ICLRMetaHost *pMetaHost = nullptr;
    ICLRRuntimeInfo *pRuntimeInfo = nullptr;
    ICLRRuntimeHost *pClrHost = nullptr;

    HRESULT hr = CLRCreateInstance(CLSID_CLRMetaHost, IID_ICLRMetaHost, (LPVOID*)&pMetaHost);
    if (FAILED(hr)) { std::cerr << "Failed to create CLR MetaHost.\n"; return; }

    hr = pMetaHost->GetRuntime(L"v4.0.30319", IID_ICLRRuntimeInfo, (LPVOID*)&pRuntimeInfo);
    if (FAILED(hr)) { std::cerr << "Failed to get CLR Runtime Info.\n"; return; }

    BOOL loadable;
    hr = pRuntimeInfo->IsLoadable(&loadable);
    if (!loadable) { std::cerr << "CLR not loadable.\n"; return; }

    hr = pRuntimeInfo->GetInterface(CLSID_CLRRuntimeHost, IID_ICLRRuntimeHost, (LPVOID*)&pClrHost);
    if (FAILED(hr)) { std::cerr << "Failed to get CLR Runtime Host.\n"; return; }

    hr = pClrHost->Start();
    if (FAILED(hr)) { std::cerr << "Failed to start CLR.\n"; return; }

    DWORD result;
    hr = pClrHost->ExecuteInDefaultAppDomain(
        L"ManagedStub.dll",
        L"StubNamespace.StubClass",
        L"Run",
        L"",
        &result
    );
    if (FAILED(hr)) { std::cerr << "Failed to execute managed assembly.\n"; }
}
