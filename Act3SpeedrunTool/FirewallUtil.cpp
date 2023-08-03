#include "FirewallUtil.h"
#include "GlobalData.h"
#include "LogUtil.h"
#include <QDebug>

bool FirewallUtil::inited = false;
bool FirewallUtil::isEnabled = false;
HRESULT FirewallUtil::hrComInit = S_OK;
INetFwPolicy2* FirewallUtil::pNetFwPolicy2 = nullptr;
long FirewallUtil::CurrentProfilesBitMask = 0;
INetFwRules* FirewallUtil::pFwRules = nullptr;
BSTR FirewallUtil::bstrRuleName;
BSTR FirewallUtil::bstrRuleLPorts;
BSTR FirewallUtil::bstrRuleGroup;

FirewallUtil::FirewallUtil()
{
}

// Instantiate INetFwPolicy2
HRESULT WFCOMInitialize(INetFwPolicy2** ppNetFwPolicy2)
{
    HRESULT hr = S_OK;

    hr = CoCreateInstance(
        __uuidof(NetFwPolicy2),
        NULL,
        CLSCTX_INPROC_SERVER,
        __uuidof(INetFwPolicy2),
        (void**)ppNetFwPolicy2);

    if (FAILED(hr)) {
        qDebug() << "CoCreateInstance for INetFwPolicy2 failed: "
                 << hr;
        LogUtil::addLog("CoCreateInstance for INetFwPolicy2 failed: " + QString::number(hr));
        goto Cleanup;
    }

Cleanup:
    return hr;
}

INetFwRule* FirewallUtil::getNetFwRule()
{
    if (!inited)
        return nullptr;

    HRESULT hr = S_OK;

    INetFwRule* pFwRule = NULL;

    hr = pFwRules->Item(bstrRuleName, (INetFwRule**)&pFwRule);
    if (SUCCEEDED(hr)) {
        LogUtil::addLog("Found NetFwRule by pFwRules->Item()");
        HRESULT h;
        if (globalData->firewallAppPath().isEmpty()) {
            h = pFwRule->put_ApplicationName(NULL);
        } else {
            h = pFwRule->put_ApplicationName(SysAllocString(
                QString(globalData->firewallAppPath())
                    .replace("/", "\\")
                    .toStdWString()
                    .c_str()));
        }
        if (SUCCEEDED(h)) {
            return pFwRule;
        } else {
            LogUtil::addLog("put_ApplicationName failed!");
            qDebug() << h;
        }
    }

    // Create a new Firewall Rule object.
    hr = CoCreateInstance(
        __uuidof(NetFwRule),
        NULL,
        CLSCTX_INPROC_SERVER,
        __uuidof(INetFwRule),
        (void**)&pFwRule);
    if (FAILED(hr)) {
        qDebug() << "CoCreateInstance for Firewall Rule failed: "
                 << hr;
        LogUtil::addLog("CoCreateInstance for Firewall Rule failed: " + QString::number(hr));
        goto Cleanup;
    }

    // Populate the Firewall Rule object
    pFwRule->put_Name(bstrRuleName);
    if (!globalData->firewallAppPath().isEmpty()) {
        pFwRule->put_ApplicationName(SysAllocString(
            QString(globalData->firewallAppPath())
                .replace("/", "\\")
                .toStdWString()
                .c_str()));
    }
    pFwRule->put_Protocol(NET_FW_IP_PROTOCOL_TCP);
    //    pFwRule->put_LocalPorts(bstrRuleLPorts);
    pFwRule->put_Grouping(bstrRuleGroup);
    pFwRule->put_Direction(NET_FW_RULE_DIR_OUT);
    //    pFwRule->put_Profiles(CurrentProfilesBitMask);
    pFwRule->put_Action(NET_FW_ACTION_BLOCK);
    pFwRule->put_Enabled(VARIANT_TRUE);

    // Add the Firewall Rule
    hr = pFwRules->Add(pFwRule);
    if (FAILED(hr)) {
        qDebug() << "Firewall Rule Add failed: "
                 << hr;
        LogUtil::addLog("Firewall Rule Add failed: " + QString::number(hr));
        goto Cleanup;
    }

    return pFwRule;

Cleanup:
    if (pFwRule != NULL) {
        pFwRule->Release();
    }

    return nullptr;
}

bool FirewallUtil::setNetFwRuleEnabled(bool enabled)
{
    if (enabled == isEnabled)
        return true;
    INetFwRule* fwRule = getNetFwRule();
    if (!fwRule) {
        LogUtil::addLog("getNetFwRule() return null!");
        return false;
    }
    if (FAILED(fwRule->put_Enabled(enabled ? VARIANT_TRUE : VARIANT_FALSE))) {
        LogUtil::addLog(QString("setNetFwRuleEnabled fwRule->put_Enabled(")
            + (enabled ? "VARIANT_TRUE" : "VARIANT_FALSE") + ") failed!");
        return false;
    }
    isEnabled = enabled;
    return true;
}

void FirewallUtil::init()
{
    if (inited)
        return;

    LogUtil::addLog("Initializing the firewall...");
    hrComInit = S_OK;
    bstrRuleName = SysAllocString(L"AutoFirewall");
    //    bstrRuleLPorts = SysAllocString(L"80");
    bstrRuleGroup = SysAllocString(L"AutoFirewall");

    HRESULT hr = S_OK;

    // Initialize COM.
    hrComInit = CoInitializeEx(0, COINIT_APARTMENTTHREADED);

    // Ignore RPC_E_CHANGED_MODE; this just means that COM has already been
    // initialized with a different mode. Since we don't care what the mode is,
    // we'll just use the existing mode.
    if (hrComInit != RPC_E_CHANGED_MODE) {
        if (FAILED(hrComInit)) {
            qDebug() << "CoInitializeEx failed: "
                     << hrComInit;
            LogUtil::addLog("CoInitializeEx failed: " + QString::number(hrComInit));
            release();
            return;
        }
    }

    // Retrieve INetFwPolicy2
    hr = WFCOMInitialize(&pNetFwPolicy2);
    if (FAILED(hr)) {
        LogUtil::addLog("WFCOMInitialize failed!");
        release();
        return;
    }

    // Retrieve INetFwRules
    hr = pNetFwPolicy2->get_Rules(&pFwRules);
    if (FAILED(hr)) {
        qDebug() << "get_Rules failed: "
                 << hr;
        LogUtil::addLog("get_Rules failed: " + QString::number(hr));
        release();
        return;
    }

    // Retrieve Current Profiles bitmask
    hr = pNetFwPolicy2->get_CurrentProfileTypes(&CurrentProfilesBitMask);
    if (FAILED(hr)) {
        qDebug() << "get_CurrentProfileTypes failed: "
                 << hr;
        LogUtil::addLog("get_CurrentProfileTypes failed: " + QString::number(hr));
        release();
        return;
    }

    // When possible we avoid adding firewall rules to the Public profile.
    // If Public is currently active and it is not the only active profile, we remove it from the bitmask
    if ((CurrentProfilesBitMask & NET_FW_PROFILE2_PUBLIC)
        && (CurrentProfilesBitMask != NET_FW_PROFILE2_PUBLIC)) {
        CurrentProfilesBitMask ^= NET_FW_PROFILE2_PUBLIC;
    }

    auto fwRule = getNetFwRule();
    if (fwRule) {
        VARIANT_BOOL enabled;
        fwRule->get_Enabled(&enabled);
        isEnabled = (enabled == VARIANT_TRUE);
    } else {
        LogUtil::addLog("auto fwRule = getNetFwRule(); fwRule is null");
    }

    inited = true;

    LogUtil::addLog("Initializing the firewall successfully");
}

void FirewallUtil::release()
{
    if (!inited)
        return;

    // Free BSTR's
    SysFreeString(bstrRuleName);
    SysFreeString(bstrRuleLPorts);

    INetFwRule* fwRule = getNetFwRule();
    if (fwRule != NULL) {
        BSTR name = nullptr;
        fwRule->get_Name(&name);
        pFwRules->Remove(name);
        fwRule->Release();
    }

    // Release the INetFwRules object
    if (pFwRules != NULL) {
        pFwRules->Release();
    }

    // Release the INetFwPolicy2 object
    if (pNetFwPolicy2 != NULL) {
        pNetFwPolicy2->Release();
    }

    // Uninitialize COM.
    if (SUCCEEDED(hrComInit)) {
        CoUninitialize();
    }
}

bool FirewallUtil::getIsEnabled()
{
    return isEnabled;
}
