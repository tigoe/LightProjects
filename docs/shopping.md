# Shopping for Fixture Parts

For any fixture, there are a few essentials you need:

* light source
* power supply
* base or structure
* shade or diffuser
* knob, switch, or other control

## Light Sources

LED strips are very flat and thin, but they don't always provide a ton of light. Look for how many lumens per meter they produce. Addressable LEDs are generally dimmer than non-addressable ones, and more expensive per LED, so if you don't need full addressing, don't spend on it. 

These [LED sheets](https://www.superbrightleds.com/moreinfo/flexible-led-sheet-lights/tunable-white-led-back-lighting-sheet-20-x-10-dual-led-segments-24v-ip20/6179/13906/#tab/overview) look interesting. They're tunable white (2700K to 6500K), non-addressable, 24V LEDs, and they can be cut into different shapes from the sheet. They can be bent as well. The full sheet (20" x 11") is rated at 5280 lumens. Probably good for light boxes, sconces, and any situation where you need a low profile fixture with a lot of surface area. They'd pair well with a sheet of diffuser. Polystyrene is often used in lamp shades, so I ordered [these sheets in 1/16" and 1/8" thickness](https://www.mcmaster.com/catalog/126/3831) from McMaster-Carr to see how well they work. The 1/16" sheet will probably cut well with a mat knife or a vinyl cutter. 

At 24V and 2.5A, you'd need a 60W power supply for the sheet. Something like this [Meanwell supply](https://www.superbrightleds.com/moreinfo/power-supplies/mean-well-led-switching-power-supply-lpv-series-20-100w-single-output-led-power-supply-24v-dc/1534/5199/) would work. Don't forget the power cord and some [wire nuts](https://www.superbrightleds.com/moreinfo/wire-connectors/18-16-awg-orange-wire-nut/4250/8751/) to attach it with. This lower power [Meanwell constant current driver](https://www.superbrightleds.com/moreinfo/mean-well-constant-current-led-drivers/mean-well-constant-current-led-driver-1400ma-24-48-vdc/6354/5307/) would power about 160 of the 288 LEDs on the sheet.

## Bulbs (lamps)

These [12V A19 bulbs](https://www.superbrightleds.com/moreinfo/standard-led-light-bulbs/12v-low-voltage-a19-led-light-bulb-60w-equivalent-700-lumens-off-grid-lighting/3183/) are great. They fit in a standard E26 socket, and they run on only 12V, so they're easy to power from a low-voltage DC power supply, and controllable from a potentiometer or microcontroller with only a transistor. Reated at 700 lumens, they're plenty bright enough for any application that would otherwise use a 60W incandescent bulb.

## Sockets

When making pendants or sconces, it's worth looking at lamp socket clusters like these. With multiple lamps, you get more brightness. You also get potentially more interactivity if you're looking to create animated effects with larger lamps. Here are a few [socket clusters for E26 sockets](https://www.grandbrass.com/category/lamp_sockets-e_26_medium_base-multi_socket_clusters/lamp_sockets-e_26_medium_base_lamp_sockets/) from Grandbrass.com. Here are some [clusters](https://vintagewireandsupply.com/4-light-socket-cluster/) from Vintage Wire & Supply. 

Clusters aren't usually made to control each lamp separately, but many can be re-wired to do so.

## Power Supplies

Here's a nice [intro to picking a power supply](https://www.superbrightleds.com/blog/how-to-choose-led-drivers-led-power-supplies/1760/) from superbrightleds. 

You need to match your power supply's voltage with the rated voltage of your lamps, and you need to make sure the power supply can provide enough current. Remember:
````
Watts = Volts * Amps
````

So, if you know your lamp is rated at 24V and 2500mA like the LED sheet above, you need a 24V supply that can supply 2500mA. That's a 60-watt supply.

[DC power supplies that plug in the wall](https://www.superbrightleds.com/moreinfo/constant-voltage-power-supplies/wall-mounted-ac-adapter-12-vdc-power-supply-12-36w/4273/9569/) are often good enough (with the right [connectors](https://www.superbrightleds.com/moreinfo/bar-strip-connectors/cps-f2st-female-standard-barrel-connector-to-screw-terminal-adapter/856/2134/)), but for larger LED projects, check out dedicated supplies like those made by [Meanwell](https://www.meanwell.com/productSeries.aspx?i=24&c=6#tag-6-24). They're good supplies that are not too expensive. You can get them from many different suppliers. 

### Constant Current and Constant Voltage Drivers

If you're powering bare LEDs, you need to limit the current to them. That's where a constant current power driver comes in. These power supplies have built-in circuitry to limit the current that the load draws. They prevent the LEDs from having to consume too much current. The disadvantage to these is that if you know what you're doing and want to drive a little more current into the LEDs to get them brighter (and therefore shorten their life), you generally can't do it with this kind of power supply. 

By contrast, constant voltage drivers have circuitry that attempts to maintain a constant voltage, even as you add more to the load, requiring more current. These are useful if you know your light source already has resistors built in to limit the current. Here's a useful [intro](https://www.superbrightleds.com/blog/constant-current-vs-constant-voltage/5649/) to both if you want to know more. 

## Lamp Parts

Lamp pipe is generally 1/8" diameter, and designed to screw into the base of an E26 socket or candleabra socket.

These [kits](http://www.profhdwr.com/08812L.htm#nickel) are great, because they include most of the parts you need to make a desk lamp. Even if you don't buy the kit, they're a useful reminder of what parts you need. 


## Shades

Ilikethatlamp.com has some [good tutorials](https://makely.shop/pages/how-to-make-a-lamp) on DIY lamp making. They're written with their own products in mind, but you can order the parts elsewhere for cheaper as well. Most of their tutorials assume a 120V lamp, but could be modified to 12V pretty simply.  I like their [drum shade tutorial](https://makely.shop/pages/diy-drum-lampshade). 

Wikihow on [how to make lampshades](https://www.wikihow.com/Make-Lampshades).

[3 Chooks lampshade making kits](https://3chooks.com.au/products/lampshade-making-kit). Probably too far away to order from for this semester, but some good ideas and tutorials about shade making. 

## Lamps for Inspiration

I love [this lamp](https://www.ambientedirect.com/en/lzf-lamps/nut-suspension-lamp_vid_29031_1261690.html). The knot pattern is pretty simple to duplicate using construction paper, but that wood veneer is just beautiful. 

Idea inspired by [this lamp](https://www.mikodesigns.com.au/shop-lighting/nightworks-studio-code-lighting-system): multiple lamps could be controlled individually to create a subtle change over time. 

This [sconce](https://www.dhgate.com/product/umeiluce-led-wall-lamp-wall-sconces-designer/412976784.html#seo=WAP) could be executed entirely with LED strips.  Could be dimmed all at once, could be a two-channel warm-cool control, could be multiple channels, depending on the design you wanted. 

Love the way [this pendant](https://www.ebay.com/c/1442732036) combines several panels around a single source to create the illusion of a much more complex light than it actually is. 

[This](https://www.pinterest.com/pin/90916486197333020/) is a variation on a classic pendant lampshade pattern, using strips of opaque material, some sort of wood veneer, to create a shade that lets out lots of light. 

Love the way [these pendants](https://www.indiamart.com/proddetail/designer-lamps-15052972991.html) get a lot of warmth out of the lamps by coating the inside of the shade with gold, not unlike what Louise was doing with her makeshift brass shade. 

This [corner sconce](http://apartmentgeeks.net/wp-content/uploads/2013/06/Greenshade-Wireshades-Designer-Lamps-by-Marc-Trotereau-1.jpg) is made from wire, paper, and probably a bunch of small bulbs [like this](https://www.1000bulbs.com/category/led-decorative-2in-globes-candelabra-base/). Neat design, not expensive in terms of materials. This comes from a [page with some other good lamps](http://apartmentgeeks.net/designer-lamps-that-will-enlighten-your-decor/). 
