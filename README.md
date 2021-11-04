# esp32-internet-ota

Project template allowing to update the ESP32 firmware over the Internet using GitHub Actions and Husarnet VPN Client for Arduino.

> **Prerequisites** 
>
> Install [Visual Studio Code](https://code.visualstudio.com/) with [PlatformIO extension](https://platformio.org/install/ide?install=vscode).

## Quick start

### First setup

1. Click **[Use this template](https://github.com/husarnet/esp32-internet-ota/generate)** button to create your own copy of this repo.

2. Clone the repo you have just created and open it in Visual Studio Code. Platformio should automatically install all project dependencies.

3. Rename `credentials-template.h` to `credentials.h` and type your WiFi an Husarnet credentials there (you will find you Husarnet Join Code at https://app.husarnet.com).

4. Click "PlatformIO: upload" button to flash your ESP32 board connected to your laptop. You will find the following log in the serial monitor:

    ```bash
    **************************************
    GitHub Actions OTA example
    **************************************
    
    📻 1. Connecting to: FreeWifi Wi-Fi network .. done

    ⌛ 2. Waiting for Husarnet to be ready ... done

    🚀 HTTP server started

    Visit:
    http://my-esp32:8080/

    Known hosts:
    my-laptop (fc94:a4c1:1f22:ab3b:b04a:1a3b:ba15:84bc)
    my-esp32 (fc94:f632:c8d9:d2a6:ad18:ed16:ed7e:9f3f)
    ```

### Internet OTA with GitHub Actions

1. Create the folowing GitHub repository secrets (`Settings` > `Secrets` > `New repository secret`):

    | Secret | Sample Value | Desription |
    | - | - | - |
    | `WIFI_SSID` | `FreeWifi` | just your WiFi network name |
    | `WIFI_PASS` | `hardtoguess` | ... and password |
    | `HUSARNET_HOSTNAME` | `my-esp32` | hostname under which you want your ESP32 to be available by other peers |
    | `HUSARNET_JOINCODE` | `fc94:b01d:1803:8dd8:b293:5c7d:7639:932a/xhfqwPxxxetyCExsSPRPn9` | find your own **secret** Join Code at your user account at https://app/husarnet.com > `choosen network` >  `add element` button. Anyone with this Join Code can connect to your Husarnet network |

2. Push changes to your repo:

    ```bash
    git add *
    git commit -m "triggering the workflow"
    git push
    ```

3. After ~X minutes, after the GitHub workflow does it job, visit: `http://my-esp32:8080` URL.


    Of course your laptop need to be connected to the same Husarnet network - you will find quick start guide here: https://husarnet.com/docs/


## Troubleshooting

### Erasing flash memory of ESP32

1. Connect ESP32 to your laptop

2. Install platformio CLI

    ```bash
    pip install -U platformio
    ```

3. Make flash erase:

    ```bash
    pio run --target erase
    ```

### Monitoring network traffic on `hnet0` interface

```bash
sudo tcpflow -p -c -i hnet0
```