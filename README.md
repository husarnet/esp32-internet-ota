# esp32-internet-ota

ESP32 + GitHub Actions + Husarnet. 

A boilerplate project for ESP32 allowing in-field firmware update using GitHub Actions workflow.

> **Prerequisites** 
>
> Install platformio CLI:
>
> ```bash
> pip3 install -U platformio
> ```
>
> If you are working in [Visual Studio Code](https://code.visualstudio.com/) the [PlatformIO extension](https://platformio.org/install/ide?install=vscode) will be helpful.

## Quick start

### First setup

1. Click **[Use this template](https://github.com/husarnet/esp32-internet-ota/generate)** button to create your own copy of this repo. Clone the repo, and open it:

    ```bash
    git clone https://github.com/husarnet/esp32-internet-ota
    cd esp32-internet-ota/
    ```

2. Erase the ESP32 flash

    ```bash
    pio run --target erase
    # or 
    # esptool.py erase_flash
    ```

3. Prepare the `.env` file:

    ```bash
    cp .env.template .env
    ```

    And edit its content:

    ```bash
    export WIFI_SSID="<place-your-wifi-ssid-here>"
    export WIFI_PASS="<place-a-password-to-your-wifi-here>"
    export HUSARNET_JOINCODE="<place-your-husarnet-joincode-here"
    ```

    
> **Tip** 
>
> If your SSID or WLAN Password contains special characters (ex. ```$ & ( ) ? ; : , . < > | ` ```) you need to escape them like this::
>
> If WiFi password is ```$&()?``` then:
> ```bash
> export WIFI_PASS="\$\&\(\)\?"
> ```
> Some special characters (ex. ```' " \ ```) can't be escaped.
> We recommend to not use special characters and emojis in WiFi SSID/Password.
>

3. The first time you need to upload the firmware over the USB cable:

    ```bash
    source .env
    pio run -e serial_upload --target upload
    ```

4. Open the Platformio Device Monitor:

    ```bash
    pio device monitor
    ```

    If you will open a serial monitor you will see the similar output (the first time you may wait up to 2 minutes):

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

4. Visit http://my-esp32:8080/ from your laptop (that should be in the same Husarnet group) and if the website is avaialble you can test OTA upgrade from the level of your laptop:

    ```bash
    source .env
    pio run -e ota_upload --target upload
    ```

5. If it works you can configure your GitHub repository.

### Editing in Visual Studio Code with a PlatformIO extenstion

Source environment variables from `.env` file before opening VS Code.

```bash
cd esp32-internet-ota/
source .env
code .
```

### Internet OTA with GitHub Actions

> **warning!**
>
> Before launching an Internet OTA, you need to upload the [initial firmware](#first-setup) at first to make your board accessible over the Husarnet VPN.

1. Create the folowing GitHub repository secrets (`Settings` > `Secrets` > `New repository secret`):

    | Secret | Sample Value | Desription |
    | - | - | - |
    | `WIFI_SSID` | FreeWifi | just your WiFi network name |
    | `WIFI_PASS` | hardtoguess | ... and password |
    | `HUSARNET_JOINCODE` | fc94:...:932a/xhfqwPxxxetyCExsSPRPn9 | find your own **secret** Join Code at your user account at https://app/husarnet.com > `choosen network` >  `add element` button. Anyone with this Join Code can connect to your Husarnet network |
    | `HUSARNET_DASHBOARD_LOGIN` | me@acme.com | A login for your account at https://app.husarnet.com (needed by [Husarnet Action](https://github.com/husarnet/husarnet-action/)) |
    | `HUSARNET_DASHBOARD_PASSWORD` | hardtoguess | A password for your account at https://app.husarnet.com (needed by [Husarnet Action](https://github.com/husarnet/husarnet-action/)) |    

2. Push changes to your repo:

    ```bash
    git add *
    git commit -m "triggering the workflow"
    git push
    ```

    And trigger the workflow manually (`workflow_dispatch`) in your GitHub repository.

3. In ~3 minutes the GitHub workflow should finish its job. Visit: `http://my-esp32:8080` URL with a sample "Hello world" website hosted by your ESP32.


    Of course your laptop need to be connected to the same Husarnet network - you will find quick start guide showing how to do it here: https://husarnet.com/docs/


## Tips

### Monitoring network traffic on `hnet0` interface

```bash
sudo tcpflow -p -c -i hnet0
```

### Accesing a webserver hosted by ESP32 using a public domain

Here is a blog post showing how to configure Nginx Proxy Manager to **provide a public access to web servers hosted by Husarnet connected devices**: https://husarnet.com/blog/reverse-proxy-gui

It can be also used  o provide the access to a web server hosted by ESP32 using a nice looking link like: `https://my-awesome-esp32.mydomain.com`.

