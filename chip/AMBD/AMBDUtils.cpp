/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
 *    Copyright (c) 2018 Nest Labs, Inc.
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

/**
 *    @file
 *          General utility methods for the AmebaD platform.
 */
/* this file behaves like a config.h, comes first */
#include <platform/internal/CHIPDeviceLayerInternal.h>

#include <platform/AMBD/AMBDUtils.h>
#include <support/CodeUtils.h>
#include <support/ErrorStr.h>
#include <support/logging/CHIPLogging.h>

//#include "wifi_conf.h"
//#include "wlan_intf.h"
//#include "wifi_constants.h"
//#include "lwip_netconf.h"
//#include <wifi/wifi_conf.h>
//#include <wifi/wifi_util.h>
//#include <device_lock.h>

using namespace ::chip::DeviceLayer::Internal;
using chip::DeviceLayer::Internal::DeviceNetworkInfo;

CHIP_ERROR AMBDUtils::IsAPEnabled(bool & apEnabled)
{
#if 1
    // TODO
    return CHIP_NO_ERROR;
#else
    int mode = 0;
    char *ifname = "wlan0";

    if(wext_get_mode(ifname, &mode) < 0)
        return;

    switch(mode) {
        case IW_MODE_MASTER:
            apEnabled = 1;  // RTW_MODE_AP;
            break;
        case IW_MODE_INFRA:
        default:
            apEnabled = 0; // RTW_MODE_STA;
            break;
    }
    return CHIP_NO_ERROR;
#endif
}

bool AMBDUtils::IsStationProvisioned(void)
{
#if 1
    // TODO
    return CHIP_NO_ERROR;
#else
    char *ifname[2] = {(u8*)WLAN0_NAME,(u8*)WLAN1_NAME};
    rtw_wifi_setting_t setting;
    return (0 == wifi_get_setting((const char*)ifname[0],&setting) && setting.ssid[0] != 0);
#endif
}

CHIP_ERROR AMBDUtils::IsStationConnected(bool & connected)
{
#if 1
    // TODO
    return CHIP_NO_ERROR;
#else
    connected = (wifi_is_connected_to_ap() == RTW_SUCCESS);
    return CHIP_NO_ERROR;
#endif
}

CHIP_ERROR AMBDUtils::StartWiFiLayer(void)
{
#if 1
    // TODO
    return CHIP_NO_ERROR;
#else
    bool wifiStarted;

    device_mutex_lock(RT_DEV_LOCK_WLAN);
    if(rltk_wlan_running(WLAN0_IDX))
    {
        wifiStarted = true;
    }
    else
    {
        wifiStarted = false;
    }
    device_mutex_unlock(RT_DEV_LOCK_WLAN);

    if (!wifiStarted)
    {
        printf("Starting Ameba WiFi layer");
        // TODO : need to match behavior with ConfigurationManagerImpl.cpp,
        //        maybe no need to call wifi_on here.
        if (wifi_on(RTW_MODE_STA) < 0)
        {
            printf("wifi_on() failed\n");
        }
    }
    return CHIP_NO_ERROR;
#endif
}

CHIP_ERROR AMBDUtils::EnableStationMode(void)
{
#if 1
    // TODO
    return CHIP_NO_ERROR;
#else
    int curWiFiMode;
    char *ifname = "wlan0";

    if(wext_get_mode(ifname, &curWiFiMode) < 0)
        return;

    if (curWiFiMode == RTW_MODE_AP)
    {
        ChipLogProgress(DeviceLayer, "Changing WiFi mode: %s -> %s", WiFiModeToStr(RTW_MODE_AP),
                        WiFiModeToStr(RTW_MODE_STA));

        if(wifi_set_mode(RTW_MODE_STA) < 0)
        {
            printf("wifi_set_mode() failed\n");
        }
    }
    return CHIP_NO_ERROR;
#endif
}

CHIP_ERROR AMBDUtils::SetAPMode(bool enabled)
{
#if 1
    // TODO
    return CHIP_NO_ERROR;
#else
    int curWiFiMode, targetWiFiMode;
    char *ifname = "wlan0";

    targetWiFiMode = (enabled) ? RTW_MODE_AP : RTW_MODE_STA;

    if(wext_get_mode(ifname, &curWiFiMode) < 0)
        return;

    if (curWiFiMode != targetWiFiMode)
    {
        ChipLogProgress(DeviceLayer, "Changing WiFi mode: %s -> %s", WiFiModeToStr(curWiFiMode),
                        WiFiModeToStr(targetWiFiMode));

        if(wifi_set_mode(targetWiFiMode) < 0)
        {
            printf("wifi_set_mode() failed\n");
        }
    }
    return CHIP_NO_ERROR;
#endif
}

int AMBDUtils::OrderScanResultsByRSSI(const void * _res1, const void * _res2)
{
#if 1
    // TODO
    return CHIP_NO_ERROR;
#else
    const wifi_ap_record_t * res1 = (const wifi_ap_record_t *) _res1;
    const wifi_ap_record_t * res2 = (const wifi_ap_record_t *) _res2;

    if (res1->rssi > res2->rssi)
    {
        return -1;
    }
    if (res1->rssi < res2->rssi)
    {
        return 1;
    }
    return 0;
#endif
}


const char * AMBDUtils::WiFiModeToStr(int wifiMode)
{
#if 1
    // TODO
    return "(unknown)";
#else
    switch (wifiMode)
    {
    case RTW_MODE_NONE:
        return "NONE";
    case RTW_MODE_STA:
        return "STA";
    case RTW_MODE_AP:
        return "AP";
    case RTW_MODE_STA_AP:
        return "STA+AP";
    default:
        return "(unknown)";
    }
#endif
}


struct netif * AMBDUtils::GetStationNetif(void)
{
#if 1
    // TODO
    return CHIP_NO_ERROR;
#else
    return GetNetif("WIFI_STA_DEF");
#endif
}

struct netif * AMBDUtils::GetNetif(const char * ifKey)
{
#if 1
    // TODO
    return CHIP_NO_ERROR;
#else
    struct netif * netif       = NULL;
    esp_netif_t * netif_handle = NULL;
    netif_handle               = esp_netif_get_handle_from_ifkey(ifKey);
    netif                      = (struct netif *) esp_netif_get_netif_impl(netif_handle);
    return netif;
#endif
}

bool AMBDUtils::IsInterfaceUp(const char * ifKey)
{
#if 1
    // TODO
    return CHIP_NO_ERROR;
#else
    struct netif * netif = GetNetif(ifKey);
    return netif != NULL && netif_is_up(netif);
#endif
}

bool AMBDUtils::HasIPv6LinkLocalAddress(const char * ifKey)
{
#if 1
    // TODO
    return CHIP_NO_ERROR;
#else
    struct esp_ip6_addr if_ip6_unused;
    return esp_netif_get_ip6_linklocal(esp_netif_get_handle_from_ifkey(ifKey), &if_ip6_unused) == ESP_OK;
#endif
}

CHIP_ERROR AMBDUtils::GetWiFiStationProvision(Internal::DeviceNetworkInfo & netInfo, bool includeCredentials)
{
#if 1
    // TODO
    return CHIP_NO_ERROR;
#else
    CHIP_ERROR err = CHIP_NO_ERROR;
    wifi_config_t stationConfig;

    err = esp_wifi_get_config(ESP_IF_WIFI_STA, &stationConfig);
    SuccessOrExit(err);

    VerifyOrExit(stationConfig.sta.ssid[0] != 0, err = CHIP_ERROR_INCORRECT_STATE);

    netInfo.NetworkId              = kWiFiStationNetworkId;
    netInfo.FieldPresent.NetworkId = true;
    memcpy(netInfo.WiFiSSID, stationConfig.sta.ssid,
           min(strlen(reinterpret_cast<char *>(stationConfig.sta.ssid)) + 1, sizeof(netInfo.WiFiSSID)));

    // Enforce that netInfo wifiSSID is null terminated
    netInfo.WiFiSSID[kMaxWiFiSSIDLength] = '\0';

    if (includeCredentials)
    {
        static_assert(sizeof(netInfo.WiFiKey) < 255, "Our min might not fit in netInfo.WiFiKeyLen");
        netInfo.WiFiKeyLen = static_cast<uint8_t>(min(strlen((char *) stationConfig.sta.password), sizeof(netInfo.WiFiKey)));
        memcpy(netInfo.WiFiKey, stationConfig.sta.password, netInfo.WiFiKeyLen);
    }

exit:
    return err;
#endif
}

CHIP_ERROR AMBDUtils::SetWiFiStationProvision(const Internal::DeviceNetworkInfo & netInfo)
{
#if 1
    // TODO
    return CHIP_NO_ERROR;
#else
    CHIP_ERROR err = CHIP_NO_ERROR;
    wifi_config_t wifiConfig;

    char wifiSSID[kMaxWiFiSSIDLength + 1];
    size_t netInfoSSIDLen = strlen(netInfo.WiFiSSID);

    // Ensure that ESP station mode is enabled.  This is required before esp_wifi_set_config(ESP_IF_WIFI_STA,...)
    // can be called.
    err = AMBDUtils::EnableStationMode();
    SuccessOrExit(err);

    // Enforce that wifiSSID is null terminated before copying it
    memcpy(wifiSSID, netInfo.WiFiSSID, min(netInfoSSIDLen + 1, sizeof(wifiSSID)));
    if (netInfoSSIDLen + 1 < sizeof(wifiSSID))
    {
        wifiSSID[netInfoSSIDLen] = '\0';
    }
    else
    {
        wifiSSID[kMaxWiFiSSIDLength] = '\0';
    }

    // Initialize an ESP wifi_config_t structure based on the new provision information.
    memset(&wifiConfig, 0, sizeof(wifiConfig));
    memcpy(wifiConfig.sta.ssid, wifiSSID, min(strlen(wifiSSID) + 1, sizeof(wifiConfig.sta.ssid)));
    memcpy(wifiConfig.sta.password, netInfo.WiFiKey, min((size_t) netInfo.WiFiKeyLen, sizeof(wifiConfig.sta.password)));
    wifiConfig.sta.scan_method = WIFI_ALL_CHANNEL_SCAN;
    wifiConfig.sta.sort_method = WIFI_CONNECT_AP_BY_SIGNAL;

    // Configure the ESP WiFi interface.
    err = esp_wifi_set_config(ESP_IF_WIFI_STA, &wifiConfig);
    if (err != ESP_OK)
    {
        ChipLogError(DeviceLayer, "esp_wifi_set_config() failed: %s", chip::ErrorStr(err));
    }
    SuccessOrExit(err);

    ChipLogProgress(DeviceLayer, "WiFi station provision set (SSID: %s)", netInfo.WiFiSSID);

exit:
    return err;
#endif
}

CHIP_ERROR AMBDUtils::ClearWiFiStationProvision(void)
{
#if 1
    // TODO
    return CHIP_NO_ERROR;
#else
    CHIP_ERROR err = CHIP_NO_ERROR;
    wifi_config_t stationConfig;

    // Clear the ESP WiFi station configuration.
    memset(&stationConfig, 0, sizeof(stationConfig));
    esp_wifi_set_config(ESP_IF_WIFI_STA, &stationConfig);

    return err;
#endif
}
