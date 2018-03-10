# Light up your Space - Workshop guide

# About AllThingsTalk

AllThingsTalk is building [an Internet of Things cloud platform](https://maker.allthingstalk.com/), used by makers and businesses who are getting into IoT all around the world.

# Goal of the workshop

Your goal is to use AllThingsTalk Maker web application to control the LED strips connected to a WiFi device.

# Hardware and Software setup

Hardware for the workshop includes:


- NodeMCU (ESP8266) board
- 1 meter of LED strip (60 WS2812B LEDs)
- A regulated power supply
- Micro USB cable


## NodeMCU board

For this workshop we’ll be using [NodeMCU](https://en.wikipedia.org/wiki/NodeMCU) running on [Arduino](https://github.com/esp8266/Arduino) platform, as it can be easily programmed using the on-board USB port.
It has an ESP8266 — an inexpensive WiFi micro-controller which you can use to quickly connect to wireless networks, send and receive data over various protocols, and read and write data from its IO pins.


## LED Strip

We’ll be using a strip of 60 WS2812B LEDs. These LEDs are individually-addressable, meaning you can set each to any color you want, and they support Full RGB color spectrum. 

Since these are basically the same as Adafruit NeoPixel, we’ll be using [NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) library to control the LEDs so we don’t have to worry about protocol details.


## **Step 1: Connect NodeMCU board and LED strip**
- You’ll notice that the NodeMCU is mounted to a breadboard, which enables easier I/O connecting
- Following the diagram, connect the input pins of the prepared LED strip to the corresponding holes in the breadboard:
![](https://d2mxuefqeaa7sj.cloudfront.net/s_7B1C4C32122E6D3FECDA886585079810F932CEED067020328DC5F76B25D71F23_1520608522421_image.png)

- Connect the power supply jack to the LED strip jack:
![](https://d2mxuefqeaa7sj.cloudfront.net/s_7B1C4C32122E6D3FECDA886585079810F932CEED067020328DC5F76B25D71F23_1520622408181_Untitled-2.png)

- Connect the power supply to the AC

 

## **Step** **2****:** **Install Arduino IDE** **and** **connect** **your board**


- Download and install [Arduino IDE](https://www.arduino.cc/en/Main/Software) to your computer
- Download and install [USB to UART](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers) bridge drivers

Board configuration needs to be added into Arduino IDE so we can choose the correct one from `Tools` menu:


- Connect the board with your computer using a micro USB cable
- Open Arduino IDE, go to `Preferences` and type `http://arduino.esp8266.com/stable/package_esp8266com_index.json` into the field for `Additional Boards Manager URL`.


![](https://d2mxuefqeaa7sj.cloudfront.net/s_7B1C4C32122E6D3FECDA886585079810F932CEED067020328DC5F76B25D71F23_1519900542258_image.png)

- Select `Tools` > `Board` > `Boards Manage``r…`
- Search for **ESP8266**, choose version **2.3.0** and install
![](https://d2mxuefqeaa7sj.cloudfront.net/s_7B1C4C32122E6D3FECDA886585079810F932CEED067020328DC5F76B25D71F23_1519900625316_image.png)

- Select `Tools` > `Boards` > `NodeMCU 1.0 (ESP-12E Module)`
- In `Tools`  > `Port` select the port to which the board is connected, you can find this in `Device Manager` > `Ports`
![](https://d2mxuefqeaa7sj.cloudfront.net/s_7B1C4C32122E6D3FECDA886585079810F932CEED067020328DC5F76B25D71F23_1519901296036_image.png)

## **Step** **3****:** **Add A****ll****T****hings****Talk** **Arduino** **SDK** **and libraries**


- Download [Arduino](https://github.com/allthingstalk/arduino-sdk/) [SDK](https://github.com/allthingstalk/arduino-sdk/) and follow the instructions for installation in the **readme****.md** file.
- Download and install **Adafruit NeoPixel** library, for controlling LED strips:
  - In Arduino IDE, go to `Sketch` > `Include library` > `Manage libraries`
![](https://d2mxuefqeaa7sj.cloudfront.net/s_7B1C4C32122E6D3FECDA886585079810F932CEED067020328DC5F76B25D71F23_1519901716060_image.png)

  
  - Search for **Adafruit NeoPixel** by **Adafruit** and install the latest version
![](https://d2mxuefqeaa7sj.cloudfront.net/s_7B1C4C32122E6D3FECDA886585079810F932CEED067020328DC5F76B25D71F23_1519901803357_image.png)



- Download and install **ArduinoJson** by **Benoit Blanchon**, following the previous steps 
![](https://d2mxuefqeaa7sj.cloudfront.net/s_7B1C4C32122E6D3FECDA886585079810F932CEED067020328DC5F76B25D71F23_1519901872687_image.png)



## **Step** **4****: Create** **a** **device in** **AllThingsTalk** **Maker**

Create a device in [Maker](https://maker.allthingstalk.com) to represent your NodeMCU board:


- Open [Maker](https://maker.allthingstalk.com/) in your browser
- Sign up to create an account
- Go to `Playground`
- Create new device via `Device` tab > `+C``ONNECT A DEVICE` > `WIFI/Lan devices` > `Your own`
![](https://d2mxuefqeaa7sj.cloudfront.net/s_7B1C4C32122E6D3FECDA886585079810F932CEED067020328DC5F76B25D71F23_1519903304871_image.png)

- Name your device however you like, and click `Connect`
- Go to  `Settings` > `Authentication` to find your `Device ID` and `Device Token`; you’ll need to include these in your sketch later, so the cloud can route the traffic to / from your board
- Create a **c****olor** asset, which you’ll use to control the color of LED strips
  - `+NEW ASSET`
  - Select `Actuator`
  - Name the asset `color` (watch out, name is case sensitive)
  - For setting the **Profile**, select `Advanced` tab
  - Paste the Profile type code:
    {
        "type": "object",
        "properties": {
            "r": {
                "type": "integer",
                "minimum": 0,
                "maximum": 255
            },
            "g": {
                "type": "integer",
                "minimum": 0,
                "maximum": 255
            },
            "b": {
                "type": "integer",
                "minimum": 0,
                "maximum": 255
            }
        }
    }


  - Click `Create asset`
![](https://d2mxuefqeaa7sj.cloudfront.net/s_7B1C4C32122E6D3FECDA886585079810F932CEED067020328DC5F76B25D71F23_1519915304591_image.png)



- To control the LED strip using a graphical interface, you can pin the **color** control to the pinboard
  - Choose `+Create pinboard` icon from the top right of the Device page
![](https://d2mxuefqeaa7sj.cloudfront.net/s_7B1C4C32122E6D3FECDA886585079810F932CEED067020328DC5F76B25D71F23_1519913419646_image.png)



  - Now you can see the `color` asset in your pinboard
  - Click on the context menu of the `color` control (three dots)
  - Select `Edit control`
  - Select `Color picker` control and hit OK
  - Select `Save` icon in the top right part of the Pinboard to save the changes
![](https://d2mxuefqeaa7sj.cloudfront.net/s_7B1C4C32122E6D3FECDA886585079810F932CEED067020328DC5F76B25D71F23_1519913685900_image.png)

# Run the example sketch

Now that you’re all set, you should try out the example sketch and see those LEDs actually glowing!


- Download the [example sketch](https://github.com/allthingstalk/startit-workshop/tree/master/examples/maker_with_leds) from GitHub
- Edit the **credentials.h** file with your `Device ID` and `Device Token` which you previously got from the Device Settings in Maker application, and **WiFi SSID** and **Password**
  
![](https://d2mxuefqeaa7sj.cloudfront.net/s_7B1C4C32122E6D3FECDA886585079810F932CEED067020328DC5F76B25D71F23_1519914009959_image.png)

  - **SSID** = “startit”
  - **PASW** = “StartitCentar”


- Upload the sketch to the board
![](https://d2mxuefqeaa7sj.cloudfront.net/s_7B1C4C32122E6D3FECDA886585079810F932CEED067020328DC5F76B25D71F23_1520522135416_image.png)



- From the Pinboard page in Maker use the `color` control to test your sketch
  - Click on the color picker
  - Select a color you like
![](https://d2mxuefqeaa7sj.cloudfront.net/s_7B1C4C32122E6D3FECDA886585079810F932CEED067020328DC5F76B25D71F23_1519915209704_image.png)

  - Watch the LED strip change the color 🌈 
## 
# What do I do now?

That’s up to you and your imagination! You can check out [NeoPixel Examples](https://github.com/adafruit/Adafruit_NeoPixel/tree/master/examples) for some inspiration, though.

