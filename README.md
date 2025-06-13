# Touch LCD Project

Skeleton ESP-IDF project targeting Waveshare ESP32-S3 touch LCD boards:

- **7-inch (800x480) GT911**
- **5B 5-inch (1024x600) FT5436**

The project uses LVGL **9.2.2** as a component and aims to autodetect the display
and touch controller so the same firmware binary supports both models.

## Directory Layout

```
.
├── CMakeLists.txt                - Top level build script
├── sdkconfig.defaults            - Base configuration
├── main/                         - Application source files
│   ├── CMakeLists.txt
│   ├── main.c                    - app_main entry point
│   ├── ui.c/h                    - LVGL UI setup
│   ├── touch_manager.c/h         - Touch controller handling
│   ├── lcd_manager.c/h           - LCD display handling
│   ├── wifi_manager.c/h          - Wi-Fi connectivity
│   ├── ble_manager.c/h           - BLE services
│   ├── rs485_manager.c/h         - RS485 interface
│   ├── can_manager.c/h           - CAN bus interface
│   ├── gpio_manager.c/h          - GPIO helpers
│   └── storage_manager.c/h       - NVS/filesystem helpers
├── components/
│   └── lvgl/                     - LVGL component (add library here)
├── assets/                       - Images, fonts, icons
└── README.md                     - This file
```

## Quick Start

1. Install **ESP-IDF >= 5.4.1** and set up the environment variables.
2. Add the LVGL 9.2.2 source inside `components/lvgl/lvgl` (e.g., as a submodule).
3. Configure the project using `idf.py menuconfig` if needed.
4. Build and flash:

```bash
idf.py build
idf.py flash monitor
```

Further documentation and hardware schematics should be added as the project
evolves.
