#include "../WebServer/WiFiScanner.h"

#include "../WebServer/WebServer.h"
#include "../WebServer/AccessControl.h"
#include "../WebServer/HTML_wrappers.h"

#include "../ESPEasyCore/ESPEasyWifi.h"
#include "../Helpers/StringGenerator_WiFi.h"


#ifdef WEBSERVER_NEW_UI

// ********************************************************************************
// Web Interface Wifi scanner
// ********************************************************************************
void handle_wifiscanner_json() {
  #ifndef BUILD_NO_RAM_TRACKER
  checkRAM(F("handle_wifiscanner"));
  #endif

  if (!isLoggedIn()) { return; }
  navMenuIndex = MENU_INDEX_TOOLS;
  TXBuffer.startJsonStream();
  addHtml(F("[{"));
  bool firstentry = true;
  int  n          = WiFi.scanNetworks(false, true);

  for (int i = 0; i < n; ++i)
  {
    if (firstentry) { firstentry = false; }
    else { addHtml(F(",{")); }
    String authType = WiFi_encryptionType(WiFi.encryptionType(i));
    if (authType.length() > 0) {
      stream_next_json_object_value(F("auth"), authType);
    }
    stream_next_json_object_value(getLabel(LabelType::SSID),      WiFi.SSID(i));
    stream_next_json_object_value(getLabel(LabelType::BSSID),     WiFi.BSSIDstr(i));
    stream_next_json_object_value(getLabel(LabelType::CHANNEL),   String(WiFi.channel(i)));
    stream_last_json_object_value(getLabel(LabelType::WIFI_RSSI), String(WiFi.RSSI(i)));
  }
  if (firstentry) {
    addHtml('}');
  }
  addHtml(']');
  TXBuffer.endStream();
}

#endif // WEBSERVER_NEW_UI

#ifdef WEBSERVER_WIFI_SCANNER

void handle_wifiscanner() {
  #ifndef BUILD_NO_RAM_TRACKER
  checkRAM(F("handle_wifiscanner"));
  #endif

  if (!isLoggedIn()) { return; }

  WiFiMode_t cur_wifimode = WiFi.getMode();
  WifiScan(false);
  setWifiMode(cur_wifimode);

  navMenuIndex = MENU_INDEX_TOOLS;
  TXBuffer.startStream();
  sendHeadandTail_stdtemplate(_HEAD);
  html_table_class_multirow();
  html_TR();
  html_table_header(getLabel(LabelType::SSID));
  html_table_header(getLabel(LabelType::BSSID));
  html_table_header(F("Network info"));
  html_table_header(F("RSSI"), 50);

  const int8_t scanCompleteStatus = WiFi.scanComplete();

  if (scanCompleteStatus <= 0) {
    addHtml(F("No Access Points found"));
  }
  else
  {
    for (int i = 0; i < scanCompleteStatus; ++i)
    {
      html_TR_TD();
      int32_t rssi = 0;
      addHtml(formatScanResult(i, "<TD>", rssi));
      html_TD();
      getWiFi_RSSI_icon(rssi, 45);
    }
  }

  html_end_table();
  sendHeadandTail_stdtemplate(_TAIL);
  TXBuffer.endStream();
}

#endif // ifdef WEBSERVER_WIFI_SCANNER