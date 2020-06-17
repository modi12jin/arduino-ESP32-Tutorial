
#pragma GCC optimize ("O3")

#include <TFT_eSPI.h>  

TFT_eSPI Tft;

#include <WiFi.h>
#include <esp_wifi.h>

#include "TCPReceiver.h"
#include "DMADrawer.h"

TCPReceiver recv;

void setup(void)
{
  Serial.begin(115200);
  Serial.flush();
  Tft.begin();

  Tft.setRotation(0);
  if (Tft.width() < Tft.height())
    Tft.setRotation(1);

  int width  = Tft.width();
  int height = Tft.height();
  if (width  > 320) width  = 320;
  if (height > 240) height = 240;

  Tft.setTextFont(2);

  Serial.println("WiFi begin.");
  Tft.println("WiFi begin.");
  //尝试连接到存储的AP
  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin();

//等待10秒钟再连接
  for (int i = 0; WiFi.status() != WL_CONNECTED && i < 100; i++) { delay(100); }

  //如果无法连接，请启动SmartConfig
  // https://itunes.apple.com/app/id1071176700
  // https://play.google.com/store/apps/details?id=com.cmmakerclub.iot.esptouch
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("SmartConfig start.");
    Tft.println("SmartConfig start.");
    WiFi.mode(WIFI_MODE_APSTA);
    WiFi.beginSmartConfig();

    while (WiFi.status() != WL_CONNECTED) {
      delay(100);
    }
    WiFi.stopSmartConfig();
    WiFi.mode(WIFI_MODE_STA);
  }

  Serial.println(String("IP:") + WiFi.localIP().toString());
  Tft.println(WiFi.localIP().toString());

  setup_t s;
  Tft.getSetup(s);

  int spi_freq = SPI_FREQUENCY;
//如果您进入80MHz的重启循环，请尝试降低到40MHz。
//如果（spi_freq> 40000000）spi_freq = 40000000;

  recv.setup( s.r0_x_offset
            , s.r0_y_offset
            , width
            , height
            , spi_freq
            , TFT_MOSI
            , TFT_MISO
            , TFT_SCLK
            , TFT_CS
            , TFT_DC
            );
}

void loop(void)
{
  recv.loop();
}
