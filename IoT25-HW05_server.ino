//서버
#include <BLEDevice.h>
#include <BLEServer.h>

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) override {
    Serial.println("✅ 클라이언트 연결됨!");
    for (int i = 0; i < 5; i++) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(2000);
      digitalWrite(LED_BUILTIN, LOW);
      delay(2000);
    }
    Serial.println("LED 깜빡임 완료");
  }

  void onDisconnect(BLEServer* pServer) override {
    Serial.println("🔌 클라이언트 연결 해제됨.");
  }
};

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  Serial.println("BLE 서버 시작 중...");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  BLEDevice::init("ESP32_BLE_SERVER");

  BLEServer* pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService* pService = pServer->createService("12345678-1234-1234-1234-1234567890ab");
  pService->start();

  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID("12345678-1234-1234-1234-1234567890ab");
  pAdvertising->start();

  Serial.println("BLE 광고 시작 완료");
}

void loop() {
  // nothing here; BLE event driven
}
