# Projet ESP32-S3 LVGL

Ce projet fournit une interface utilisateur complète basée sur LVGL pour le microcontrôleur ESP32-S3, avec prise en charge des écrans tactiles Waveshare 7 pouces et 5B, connectivité Wi-Fi et Bluetooth, stockage SPIFFS/SD, mises à jour OTA et un clavier virtuel AZERTY.

## Table des matières

- [Arborescence du projet](#arborescence-du-projet)
- [Prérequis](#prérequis)
- [Configuration du matériel](#configuration-du-matériel)
- [Compilation et Flash](#compilation-et-flash)
- [Structure du code](#structure-du-code)
- [Composants](#composants)
- [Utilisation](#utilisation)
- [Logs](#logs)

## Arborescence du projet

```
esp32_s3_lvgl_project/
├── CMakeLists.txt
├── sdkconfig.defaults
├── partition.csv
├── main/
│   ├── CMakeLists.txt
│   └── main.c
└── components/
    ├── LovyanGFX/
    │   ├── CMakeLists.txt
    │   ├── LGFX_Config.hpp
    │   └── LovyanGFX.cpp
    ├── gt911_touch/
    │   ├── CMakeLists.txt
    │   ├── gt911.h
    │   └── gt911.c
    ├── lvgl_port/
    │   ├── CMakeLists.txt
    │   ├── lvgl_port.h
    │   └── lvgl_port.c
    ├── ui_pages/
    │   ├── home_page/
    │   │   ├── CMakeLists.txt
    │   │   ├── home_page.h
    │   │   └── home_page.c
    │   ├── wifi_bt_page/
    │   │   ├── CMakeLists.txt
    │   │   ├── wifi_bt_page.h
    │   │   └── wifi_bt_page.c
    │   ├── file_explorer_page/
    │   │   ├── CMakeLists.txt
    │   │   ├── file_explorer_page.h
    │   │   └── file_explorer_page.c
    │   ├── settings_page/
    │   │   ├── CMakeLists.txt
    │   │   ├── settings_page.h
    │   │   └── settings_page.c
    │   └── console_debug_page/
    │       ├── CMakeLists.txt
    │       ├── console_debug_page.h
    │       └── console_debug_page.c
    ├── ui_manager/
    │   ├── CMakeLists.txt
    │   ├── ui_manager.h
    │   └── ui_manager.c
    ├── lvgl_keyboard/
    │   ├── CMakeLists.txt
    │   ├── lvgl_keyboard.h
    │   └── lvgl_keyboard.c
    ├── wifi_service/
    │   ├── CMakeLists.txt
    │   ├── wifi_service.h
    │   └── wifi_service.c
    ├── bluetooth_service/
    │   ├── CMakeLists.txt
    │   ├── bluetooth_service.h
    │   └── bluetooth_service.c
    ├── ota_service/
    │   ├── CMakeLists.txt
    │   ├── ota_service.h
    │   └── ota_service.c
    ├── storage_service/
    │   ├── CMakeLists.txt
    │   ├── storage_service.h
    │   └── storage_service.c
    ├── i18n_service/
    │   ├── CMakeLists.txt
    │   ├── i18n_service.h
    │   └── i18n_service.c
    └── lvgl_theme/
        ├── CMakeLists.txt
        ├── lvgl_theme.h
        └── lvgl_theme.c
```

## Prérequis

- **ESP-IDF v5.x** (dernière version stable compatible avec LVGL 9). Suivez les instructions d'installation officielles : [https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/index.html](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/index.html)
- **LVGL 9** : La bibliothèque LVGL est récupérée automatiquement depuis le registre des composants grâce au fichier `idf_component.yml`. Exécutez `idf.py add-dependency` lors de la première compilation pour télécharger cette dépendance.
- **Waveshare ESP32-S3-Touch-LCD 7 pouces & 5B** : Ce projet est spécifiquement conçu pour ces écrans. Les configurations des broches dans `LovyanGFX/LGFX_Config.hpp` et `gt911_touch/gt911.c` devront être vérifiées et ajustées si vous utilisez une autre carte ou un autre écran.

## Configuration du matériel

Les broches GPIO pour l'écran LCD (ST7262) et le contrôleur tactile (GT911) sont définies dans :

- `components/LovyanGFX/LGFX_Config.hpp` : Configuration du bus parallèle RGB et des broches de contrôle pour le ST7262.
- `components/gt911_touch/gt911.c` : Configuration des broches I2C pour le GT911.

**Il est crucial de vérifier et d'ajuster ces broches en fonction du schéma de votre carte Waveshare ESP32-S3-Touch-LCD.**

## Compilation et Flash

1.  **Naviguez vers le répertoire du projet** :
    ```bash
    cd esp32_s3_lvgl_project
    ```

2.  **Configurez le projet** (optionnel, mais recommandé pour vérifier les options) :
    ```bash
    idf.py menuconfig
    ```
    Assurez-vous que les options LVGL, Wi-Fi, Bluetooth, NVS et SPIFFS sont activées comme spécifié dans `sdkconfig.defaults`.

3.  **Compilez le projet** :
    ```bash
    idf.py build
    ```

4.  **Flashez le firmware sur l'ESP32-S3** :
    ```bash
    idf.py -p /dev/ttyUSB0 flash
    ```
    Remplacez `/dev/ttyUSB0` par le port série de votre ESP32-S3.

5.  **Monitorez les logs** :
    ```bash
    idf.py -p /dev/ttyUSB0 monitor
    ```

## Structure du code

Le projet suit une architecture modulaire basée sur les composants ESP-IDF :

-   `main/` : Contient le point d'entrée de l'application (`app_main`).
-   `components/` : Contient tous les modules et services du projet, organisés par fonctionnalité.

## Composants

### `LovyanGFX`

-   **Description** : Backend graphique pour LVGL, configuré pour le contrôleur LCD ST7262 via un bus parallèle RGB. Les broches sont à vérifier et ajuster.
-   **Fichiers clés** : `LGFX_Config.hpp`, `LovyanGFX.cpp`

### `gt911_touch`

-   **Description** : Driver pour le contrôleur tactile GT911 via I2C, avec gestion de la lecture des points tactiles.
-   **Fichiers clés** : `gt911.h`, `gt911.c`

### `lvgl_port`

-   **Description** : Intégration de LVGL avec LovyanGFX et le driver tactile GT911. Initialise LVGL et gère les callbacks d'affichage et d'entrée.
-   **Fichiers clés** : `lvgl_port.h`, `lvgl_port.c`

### `ui_pages`

-   **Description** : Contient les implémentations des différentes pages de l'interface utilisateur LVGL.
    -   `home_page` : Page d'accueil avec titre, heure et un placeholder pour le logo.
    -   `wifi_bt_page` : Page pour le scan et la connexion Wi-Fi/Bluetooth.
    -   `file_explorer_page` : Page pour l'exploration des fichiers SPIFFS et SD.
    -   `settings_page` : Page pour les paramètres système (thème, langue, etc.).
    -   `console_debug_page` : Page pour l'affichage des logs et informations système.

### `ui_manager`

-   **Description** : Gère la navigation entre les différentes pages de l'interface utilisateur LVGL.
-   **Fichiers clés** : `ui_manager.h`, `ui_manager.c`

### `lvgl_keyboard`

-   **Description** : Implémentation d'un clavier virtuel AZERTY pour LVGL, avec prise en charge des majuscules/minuscules et des caractères spéciaux.
-   **Fichiers clés** : `lvgl_keyboard.h`, `lvgl_keyboard.c`

### `wifi_service`

-   **Description** : Service pour la gestion de la connectivité Wi-Fi, y compris le scan des réseaux, la connexion et la sauvegarde des identifiants dans la NVS.
-   **Fichiers clés** : `wifi_service.h`, `wifi_service.c`

### `bluetooth_service`

-   **Description** : Service pour la gestion de la connectivité Bluetooth (BLE). L'implémentation SPP est un ajout futur potentiel.
-   **Fichiers clés** : `bluetooth_service.h`, `bluetooth_service.c`

### `ota_service`

-   **Description** : Service pour les mises à jour Over-The-Air (OTA) du firmware via Wi-Fi.
-   **Fichiers clés** : `ota_service.h`, `ota_service.c`

### `storage_service`

-   **Description** : Service pour la gestion du stockage SPIFFS et de la carte SD (FATFS), incluant la liste des répertoires et la lecture/écriture de fichiers.
-   **Fichiers clés** : `storage_service.h`, `storage_service.c`

### `i18n_service`

-   **Description** : Service d'internationalisation pour la gestion des chaînes de caractères multilingues (Français et Anglais) via un dictionnaire intégré.
-   **Fichiers clés** : `i18n_service.h`, `i18n_service.c`

### `lvgl_theme`

-   **Description** : Thème LVGL personnalisé avec un style moderne, sombre et fluide, incluant des animations de transitions et des styles pour les widgets courants.
-   **Fichiers clés** : `lvgl_theme.h`, `lvgl_theme.c`

## Utilisation

Après avoir flashé le firmware, l'application démarrera sur la page d'accueil. Vous pouvez naviguer entre les pages en utilisant le `ui_manager`. Le clavier virtuel AZERTY s'affichera automatiquement lorsque vous interagirez avec un champ de texte LVGL.

## Logs

Les logs système sont activés par défaut et peuvent être visualisés via le moniteur série de l'ESP-IDF (`idf.py monitor`).

-   **Logs ESP_LOG*** : Logs généraux du système.
-   **Logs internes LVGL** : Les logs de LVGL peuvent être activés via `idf.py menuconfig` sous `Component config -> LVGL`.



