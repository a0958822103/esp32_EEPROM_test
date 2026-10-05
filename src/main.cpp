#include <Arduino.h>
#include <Preferences.h>

Preferences preferences;
String inputBuffer = ""; // 用來暫存每次輸入的字元

void setup() {
  Serial.begin(115200);
  delay(1000); // 給 Serial 一點時間初始化
  
  Serial.println("\n--- ESP32 Preferences 測試 ---");
  
  // 1. 初始化 Preferences，打開命名空間 "my-app"
  preferences.begin("my-app", false);

  // 2. 讀取名為 "saved_text" 的字串，如果不存在則回傳預設的字串 (這裡設為空字串)
  String savedText = preferences.getString("saved_text", "");
  
  if (savedText == "") {
    Serial.println("目前沒有保存的內容。");
  } else {
    Serial.print("重啟成功！上次保存的內容是: ");
    Serial.println(savedText);
  }

  Serial.println("\n請在終端機輸入文字 (按下 Enter 後發送並儲存)...");
}

void loop() {
  // 檢查是否從序列埠接收到資料
  while (Serial.available() > 0) {
    char c = Serial.read(); // 一次讀取一個字元
    
    // 判斷是否為換行符號 (\n 或 \r 代表按下 Enter 鍵)
    if (c == '\n' || c == '\r') {
      
      inputBuffer.trim(); // 清除字串前後的空白
      
      // 確保輸入的內容不是空的才處理
      if (inputBuffer.length() > 0) {
        Serial.print("\n收到輸入: \"");
        Serial.print(inputBuffer);
        Serial.println("\"");
        
        // 3. 儲存字串到 Preferences 的 "saved_text" 鍵
        preferences.putString("saved_text", inputBuffer);
        
        Serial.println("儲存成功。");
        Serial.println("--------------------------------------------------");
        
        // 儲存完後清空緩衝區，準備下一次輸入
        inputBuffer = "";
      }
    } else {
      // 還沒按下 Enter，將字元存入緩衝區
      inputBuffer += c;
      
      // (選擇性) 回顯輸入的字元到終端機上，讓你能看到你正在打什麼字
      Serial.print(c); 
    }
  }
}