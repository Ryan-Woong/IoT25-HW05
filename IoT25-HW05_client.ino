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
