# Telephone Dimmer

We have a Sputnik chandelier above our dining table that's my wife's favorite (Figure 1). A year or so ago, she came home with a rotary  wall phone and said, "Can you make this into a dimmer for the Sputnik?" Seemed like a good challenge, so we took it on.

![Figure 1. Sputnik chandelier](/LightProjects/img/rotary-dimmer/sputnik.jpg)

_Figure 1. The Sputnik chandelier_ 

![Figure 2. Rotary telephone](img/rotary-dimmer/rotary-phone.jpg)

_Figure 2. The rotary phone_ 

## Dimming the Chandelier

Reading the rotary phone from a microcontroller seemed pretty straightforward. Nathan Seidle published a [tutorial on the Sparkfun blog](https://www.sparkfun.com/tutorials/51) fifteen years ago that would do the trick. Building an A/C dimmer in the wall was the harder part. I have yet to be satisfied with any of the DIY A/C dimmer circuits I've tested, and in-wall construction is beyond my comfort zone. But we have a [Philips Hue hub](https://www2.meethue.com/en-us), and a lot of Hue lamps, and I have examples of [how to control the Hue system from a WiFi-enabled Arduino](https://github.com/tigoe/hue-control), so that seemed like a good approach.

Next problem: the Hue system communicates from the hub to the bulbs directly, and the bulbs for the sputnik are specialty bulbs, a G16 half-silvered bulb. So I started researching third-party Hue-compatible dimmers. Most of those are not actual in-wall dimmers, but I did find one that seemed to work, the [Quotra Wireless Smart Dimmer Zigbee Light Switch](https://smile.amazon.com/gp/product/B07CVL9SZF/ref=ppx_yo_dt_b_asin_title_o03_s00?ie=UTF8&psc=1). 

I got of these dimmers, put it in the wall in place of the existing dimmer, and fired it up. It worked well for about an hour. Trouble was, I hadn't properly calculated the load of the chandelier. Even though the lamps were only 15 watts each, with 12 of them it pushed up against the dimmer's 180-watt limit. The dimmer overheated and stopped dimming, would only go on and off, and stopped responding to the Hue.

After looking for other dimmers, I decided the next best thing to do was to see if I could reduce the Sputnik's load by switching to LED lamps. Seemed like a good idea for the power savings anyway. It took a few months, but I found dimmable [G14 Candelabra LED Bulb, Half-Silvered](https://www.superbrightleds.com/moreinfo/globe-bulbs/g14-candelabra-led-bulb-silver-tipped-led-filament-bulb-40-watt-equivalent-dimmable-275-lumens/3108/7241/), and as the lamps in the Sputnik died (the incandescent ones tend to run hot), I swapped them out for LED. Now the chandelier's only 50 watts, well in the range for a Quotra dimmer. So I got another one and tried again, successfully.

## Reading the Rotary Phone

With the dimmer in place, it was time to visit [Nate's tutorial](https://www.sparkfun.com/tutorials/51). Sure enough, it was pretty easy to convert to modern Arduino code. The harder part was deciphering the wires of my phone. Mine is not a standard AT&T phone, so the wiring isn't the same. But again, Nate's tutorial was helpful there too. The dial worked just like his. Figure 3 shows a video of it in action. A little time with a multimeter, and I had the wiring worked out. Figures 4 and 5 show the two terminal blocks inside the phone where all the wires go to. One is on the side of the phone and the other is in the front, under the dial. 

![Figure 3. Rotary telephone dial from behind, in motion](img/rotary-dimmer/dial-movement-sm.mp4)

_Figure 3. Video of rotary telephone dial from behind, in motion. There are basically two switches, one that switches at the beginning and end of dialing, and another that switches once for each number._ 

The relevant terminals were as follows:

* Front terminal block, terminal 5: cradle hook terminal
* Front 6: end rotary terminal
* Side terminal block, terminal 5: ground terminal
* Side 8: rotary terminal

![Figure 4. Side terminal block in the rotary phone](img/rotary-dimmer/side-terminal-block.jpg)

_Figure 4. Side terminal block in the rotary phone._ 

![Figure 5. Front terminal block in the rotary phone](img/rotary-dimmer/front-terminal-block.jpg)

_Figure 5. Front terminal block in the rotary phone._ 

## Circuit

The circuit for this project is quite simple. The rotary phone's dial, end rotary dial, and on/off hook terminals go to three of the Arduino's interrupt pins (pins 2, 3, and 9, since I am using a Nano 33 IoT). The phone's  speaker terminal goes to pin A0 through a 10-kilohm resistor for generating tones, and the phone's ground terminal goes to the Nano's ground. 

![Figure 6. Breadboard view of the Arduino Nano 33 IoT wired to the rotary phone dialer](img/rotary-dimmer/nano-rotary-phone_bb.png)

_Figure 6. Breadboard view of the Arduino Nano 33 IoT wired to the rotary phone dialer._ 

## User Interaction

With the wiring worked out, I started outlining a sketch for the WiFi boards. Since I had Hue examples already, I knew I'd use the ArduinoHttpClient to do the HTTP requests, and also the Arduino_JSON library to make the necessary JSON objects for the body of the requests.  The first thing was to outline the behavior:

* Participant has to pick up the phone to dial
* Dialing any digit fades the light
* Hanging up the phone puts the processor to sleep 

I decided to put the processor to sleep when not in action because I didn't want to run in-wall power. With a decent phone charger, I could run the processor in sleep mode for a long time. But when the participant picks up, the processor will need time to re-connect to the network. How should I indicate that? The obvious answer was: dialtone means you have a network. No dialtone means you don't. 

After looking into generating DTMF tones from an Arduino, I decided I didn't want to add the extra circuitry to generate the tones. All the examples I looked at were too hardware-specific to the Uno, and required 8 pins to work, so I faked it by generating the tones with the `tone()` command.  It doesn't sound quite right, but it's good enough.

## The Code

Here's a pseudocode outline of the Arduino sketch for this. 

````
// setup function:
  // Initialize serial
  // configure the inputs for the rotary phone
  // Initialize builtin LED for network status
  // attach interrupts for phone inputs
  // Attach interrrupt to hook to wake up processor

// loop function:
   // LED shows whether the WiFi is connected or not

  // if phone is hung up, 
    // turn off dialtone
    // set a timer until sleep (now + 1 minute)
   
  // if phone is not hung up and not on the network,
    // try to connect
 

  // if the sleep timer has passed,
    // disconnect the WiFi 
    // turn off the status LED
    // sleep

  // if dial is at rest and there is a recent dial count,
      // 1 is off, 10 is brightest.
      // map to a 0-255 range

      // Make a JSON object for the HTTP request
      // if there's a network connection and you're not hung up,
        // make the HTTP request
````

That's the main logic of the program. The full sketch can be found at [this link](https://github.com/tigoe/LightProjects/tree/master/TelephoneDimmer/RotaryDialDimmer0003). 
