# ESP32 Preferences (模擬 EEPROM) 測試專案

這是一個使用 PlatformIO 開發的 ESP32 測試專案。主要目的是展示如何使用 ESP32 內建的 `Preferences.h` 函式庫來取代傳統的 `EEPROM.h`，實現資料在斷電或重啟後依然能保存的持久化儲存（非揮發性儲存）。

## 專案特色

- 示範如何初始化並讀寫 ESP32 的 Preferences（內部基於 NVS 系統）。
- 透過 Serial Monitor（序列埠監控視窗）與 ESP32 進行互動測試。
- 支援任意字串輸入：允許使用者在終端機中逐字元輸入文字，等待使用者按下 `Enter` 鍵後才會完整發送並儲存。
- 儲存後可以透過按下開發板上的 `EN` (Reset) 按鈕重啟，驗證剛才輸入的資料是否成功保留。

## 硬體需求

- 任意一款 ESP32 開發板 (如 NodeMCU-32S, ESP32-DevKitC 等)
- 一條用來連接電腦與開發板的 USB 傳輸線

## 軟體環境

- [Visual Studio Code](https://code.visualstudio.com/)
- [PlatformIO IDE 擴充套件](https://platformio.org/)

## 如何測試與使用

### 1. 編譯與上傳
1. 將 ESP32 開發板透過 USB 連接到電腦。
2. 在 VS Code 中開啟本專案資料夾。
3. 點擊 PlatformIO 底部狀態列的 **Upload** (向右的箭頭圖示 `→`) 按鈕，將程式碼編譯並燒錄至 ESP32。

### 2. 輸入與儲存文字
1. 燒錄完成後，點擊 PlatformIO 底部狀態列的 **Serial Monitor** (插頭圖示) 開啟終端機介面。
   > **注意：** 本專案的 Serial Baud Rate 設定為 `115200`，已設定在 `platformio.ini` 檔案中。
2. 開啟後，終端機會顯示目前的狀態。如果您以前有儲存過內容，它會優先讀取並顯示出來。
3. 在終端機介面中，直接使用鍵盤**輸入您想保存的文字**。程式設計了回顯功能，您會看到您輸入的字元即時顯示在畫面上。
4. 確定輸入完畢後，按下鍵盤的 **`Enter` 鍵** 送出。
5. 終端機會顯示收到輸入，並提示：「儲存成功！」。

### 3. 驗證資料是否保存 (重啟)
1. 找到您的 ESP32 開發板上標示為 **`EN`** (或 `RST` / `Reset`) 的按鈕。
2. 按下它讓 ESP32 重新啟動。
3. 觀察 Serial Monitor 的畫面，您應該會看到類似以下的訊息，證明重啟後資料依然存在：
   ```text
   重啟成功！上次保存的內容是: <您剛剛輸入的文字>
   ```

## 為什麼 ESP32 建議使用 Preferences 而非 EEPROM？

在傳統的 Arduino (例如 UNO) 上，EEPROM 是一塊獨立的實體記憶體。但 ESP32 硬體上**並沒有**實體的 EEPROM。
舊版的 `EEPROM.h` 在 ESP32 上是透過 Flash 記憶體模擬出來的，這會產生幾個缺點：
1. 需要手動呼叫 `commit()` 才會真正寫入。
2. **缺乏磨損平衡（Wear Leveling）**：頻繁讀寫相同的記憶體位址會加速 Flash 記憶體的損壞。
3. 需要自己計算與管理每一個 Byte 的位址與大小，很容易出錯。

官方強烈建議 ESP32 開發者改用 **`Preferences.h`**。它的優點是：
- **以鍵值對 (Key-Value) 的方式儲存**：例如 `putString("saved_text", "hello")`，靠自訂的名稱(Key)存取，完全不需要自己計算記憶體位址。
- **自動保存**：呼叫寫入函式後就會自動保存，不需要手動 `commit()`。
- **內建磨損平衡**：底層使用強大的 NVS (Non-Volatile Storage) 架構，能有效延長 Flash 記憶體的壽命。
