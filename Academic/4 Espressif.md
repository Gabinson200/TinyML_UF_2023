
### Background
Espressif Systems is a multinational, fabless semiconductor company that specializes in producing highly integrated Wi-Fi and Bluetooth SoCs (System on Chips) primarily used for wireless communication in a wide range of electronic devices. One of Espressif's most well-known products is the ESP8266, a low-cost Wi-Fi microchip with full TCP/IP stack and microcontroller capability, which gained significant popularity among hobbyists and developers for its ease of use and affordability. Following the success of the ESP8266, Espressif released the ESP32, a more powerful SoC with both Wi-Fi and Bluetooth capabilities, enhanced security features, and a wide range of other functionalities including ample GPIOs, support for a variety of sensors, and options for hardware acceleration for encryption and decryption processes.

## Hardware
### ESP8266 Series

- **ESP8266EX**: The initial Wi-Fi SoC that became extremely popular for its low cost and ease of use, featuring a Tensilica L106 32-bit microcontroller and integrated Wi-Fi capabilities.
- **ESP8285**: A variant of the ESP8266 with integrated 1MB flash memory, facilitating designs with minimal external components.

### ESP32 Series

- **ESP32**: The successor to the ESP8266, featuring a dual-core processor, Wi-Fi and Bluetooth 4.2/5.0 (BR/EDR & BLE) capabilities, more GPIOs, and improved security features.
- **ESP32-S2**: A single-core processor variant with no Bluetooth, focused on a more robust and secure Wi-Fi functionality.
- **ESP32-S3**: Featuring dual-core processing, Wi-Fi and Bluetooth 5 (LE), and enhanced support for AI-based applications with vector instructions for faster computation.
- **ESP32-C3**: A single-core, RISC-V architecture processor with Wi-Fi and Bluetooth 5 (LE) support, aimed at low-power and cost-sensitive IoT applications.
- **ESP32-C6**: Espressif’s first Wi-Fi 6 SoC with a RISC-V single-core processor and Bluetooth 5 (LE) capabilities, designed for low-power IoT devices.
- **ESP32-S3**: Enhanced for AI and security features, supports Wi-Fi + Bluetooth 5 (LE), and includes neural network acceleration.
- **ESP32-H2**: Targeted for smart home applications with IEEE 802.15.4 (Thread & Zigbee) and Bluetooth 5 (LE) capabilities, based on a RISC-V single-core processor.

### ESP32-C Series (Cost-Effective)

- **ESP32-C2**: Aimed at providing a cost-effective Wi-Fi 4 and Bluetooth LE solution, particularly focusing on low-power IoT devices.

### Development Boards

Espressif also produces development boards for these SoCs to facilitate easy prototyping and development. Some of the popular development boards include:

- **ESP8266 NodeMCU**  
- **ESP8266 ESP-12E**
- **ESP32 DevKitC**
- **ESP32-WROOM** 
- **ESP32-S2 Saola**
- **ESP32-C3 DevKitM-1**

## Software

[Main Espressif Github](https://github.com/espressif)

Espressif AI resources:
[Espressif Deep Learning(DL) Github](https://github.com/espressif/esp-dl) 
[Espressif DL Tutorial](https://docs.espressif.com/projects/esp-dl/en/latest/esp32/introduction.html)
[Espressif-WHO image recognition Gihub](https://github.com/espressif/esp-who?tab=readme-ov-file) 
[Espressif TFLM](https://github.com/espressif/esp-tflite-micro) ==this is the main repository we will be working with more specifically== the
[micro speech](https://github.com/espressif/esp-tflite-micro/tree/6a91f2c4b297744489f3ff12405cbcfe32ff7fd6/examples/micro_speech)example based off of the [TFLM code](https://github.com/tensorflow/tflite-micro/tree/main/tensorflow/lite/micro/examples/micro_speech) 