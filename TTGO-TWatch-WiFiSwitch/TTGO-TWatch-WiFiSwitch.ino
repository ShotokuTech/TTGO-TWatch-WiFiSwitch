#include "config.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

const char *ssid = "TTGOWAP";
const char *password = "11111111";
WiFiServer server(80);


typedef struct {
    lv_obj_t *hour;
    lv_obj_t *minute;
    lv_obj_t *second;
} str_datetime_t;


static str_datetime_t g_data;
TTGOClass *watch = nullptr;
PCF8563_Class *rtc;
LV_IMG_DECLARE(channel_jpg);
LV_IMG_DECLARE(shotoku_jpg);
LV_FONT_DECLARE(morgnite_bold_64);

void setup()
{
    Serial.begin(115200);
//
  Serial.println();
  Serial.println("Configuring access point...");
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();
  Serial.println("Server started");
//


    watch = TTGOClass::getWatch();
    watch->begin();
    watch->lvgl_begin();
    rtc = watch->rtc;

    // Use compile time
    rtc->check();

    watch->openBL();

    //Lower the brightness
    watch->bl->adjust(150);


    lv_obj_t *img1 = lv_img_create(lv_scr_act(), NULL);
/*    lv_img_set_src(img1, &batman_png);
    lv_obj_align(img1, NULL, LV_ALIGN_CENTER, 0, 0);
*/

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_color(&style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&style, LV_STATE_DEFAULT, &morgnite_bold_64);

    g_data.hour = lv_label_create(img1, nullptr);
    lv_obj_add_style(g_data.hour, LV_OBJ_PART_MAIN, &style);

    lv_label_set_text(g_data.hour, "00");
    lv_obj_align(g_data.hour, img1, LV_ALIGN_IN_TOP_MID, 10, 30);

    g_data.minute = lv_label_create(img1, nullptr);
    lv_obj_add_style(g_data.minute, LV_OBJ_PART_MAIN, &style);
    lv_label_set_text(g_data.minute, "00");
    lv_obj_align(g_data.minute, g_data.hour, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

    g_data.second = lv_label_create(img1, nullptr);
    lv_obj_add_style(g_data.second, LV_OBJ_PART_MAIN, &style);
    lv_label_set_text(g_data.second, "00");
    lv_obj_align(g_data.second, g_data.minute, LV_ALIGN_OUT_RIGHT_MID, 9, 0);

    lv_task_create([](lv_task_t *t) {

  WiFiClient client = server.available();   // listen for incoming clients
  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.print("<br><br>");            
            client.print("Click <a href=\"/1\">here</a> to RUN.<br>");
            client.print("<br><br>");
            client.print("Click <a href=\"/2\">here</a> to STOP.<br>");
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

if (currentLine.endsWith("GET /1")) {
    lv_obj_t *img1 = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img1, &channel_jpg);
    lv_obj_align(img1, NULL, LV_ALIGN_CENTER, 0, 0);
}

if (currentLine.endsWith("GET /2")) {
    lv_obj_t *img1 = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img1, &shotoku_jpg);
    lv_obj_align(img1, NULL, LV_ALIGN_CENTER, 0, 0);
}
}
}

client.stop();
    Serial.println("Client Disconnected.");
  }



    RTC_Date curr_datetime = rtc->getDateTime();
        lv_label_set_text_fmt(g_data.second, "%02u", curr_datetime.second);
        lv_label_set_text_fmt(g_data.minute, "%02u", curr_datetime.minute);
        lv_label_set_text_fmt(g_data.hour, "%02u", curr_datetime.hour);


//set below 50 from 1000
    }, 50, LV_TASK_PRIO_MID, nullptr);

    // Set 20MHz operating speed to reduce power consumption
 //   setCpuFrequencyMhz(20);

}

void loop()
{

    lv_task_handler();
//Serial.print(".");
}
