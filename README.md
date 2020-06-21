# ESP32 Weather Station Node

Heavily inspired by/based on [https://github.com/theoretick/esp8266_prometheus_exporter_for_ds18b20_temperature_sensors]()


### Uploading via OTA
```
export PLATFORMIO_UPLOAD_FLAGS=--auth=<ota_password>
platformio run --target upload --upload-port <remote_ip>
```
