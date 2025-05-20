//IoT25-HW05_client

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEClient.h>

#define SERVICE_UUID "12345678-1234-1234-1234-1234567890ab"

BLEAdvertisedDevice* myDevice = nullptr;
BLEClient* pClient;
bool doConnect = false;
bool connected = false;

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) override {
    Serial.print("스캔 중: ");
    Serial.println(advertisedDevice.toString().c_str());

    if (advertisedDevice.haveServiceUUID() &&
        advertisedDevice.isAdvertisingService(BLEUUID(SERVICE_UUID))) {
      Serial.println("✅ 서버 발견!");
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      BLEDevice::getScan()->stop();
    }
  }
};

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  BLEDevice::init("");

  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  Serial.println("🔎 서버 스캔 시작...");
  pBLEScan->start(10, false);  // 10초 스캔
}

void loop() {
  if (doConnect && !connected) {
    Serial.println("🔗 서버에 연결 시도...");
    pClient = BLEDevice::createClient();
    if (pClient->connect(myDevice)) {
      Serial.println("✅ 서버에 연결 성공!");
      connected = true;
    } else {
      Serial.println("❌ 서버 연결 실패!");
    }
    doConnect = false;
  }

  delay(1000);
}

//IoT25-HW05_server

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


https://github.com/user-attachments/assets/f631672b-d62b-4e15-bc14-3697f24a6dc7


