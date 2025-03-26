# Getting A Reading of Melanopic EDI From A Handheld Spectrometer

Here's how to calculate melanopic equivalent daylight illuminance from a spectral reading using [Luox](https://luox.app/). To make this work, you will need a spectrometer that can deliver a spectral power distribution in normalized (relative) values. We used a Sekonic C-7000. 

First, make sure your meter is set to measure illuminance in lux, and to give you the spectral power distribution (SPD) in normalized values (a range from 0 to 1). Next, export the data from your meter, and look for the lux value and the SPD readings. Here's an [example file from the C-7000](SPD_001_02°_5791K.csv). You'll notice it gives you a lot of extra data. Take note of the illuminance reading (line 8 in that file, 1400lx). You'll also notice that there are two sets of spectral readings: the ones from lines 47-127 give readings in 5-nanometer ranges, and the ones in lines 129-529 give readings in 1-nanometer ranges. Whatever ranges your meter gives, you want the higher resolution ones. In this case, you would take lines 125-529 and delete the rest of the lines in the file. Then change the first column of each line so that it contains only the wavelength number. In this case, you'd change ``Spectral Data 780[nm]`` to ``780``, and do the same with all the other lines. Note that the readings in the second column are all between 0 and 1. 

Once you've got your file with just the spectral readings, upload them to the [Luox upload page](https://luox.app/upload). When prompted by Luox, respond that you want to "Use all rows and columns". On the next screen, you'll be asked to describe your data, with several optional pull-down boxes, like so:
````
My data contains \[absolute/relative\]  spectra with wavelength in nm. Label my measurements as 
\[Observation 1\].
Each measurement column contains 
\[irradiances/radiances\]
 in W per m².
 ````

Fill in the appropriate values for each option. Using the sample file, the answers are:
````
My data contains 
relative spectra with wavelength in nm. Label my measurements as Observation 1. I have separately measured 
illuminance for Observation 1 at 1400 (lx).
 ````

Finally, scroll down and Luox will give you all the melanopic readings (in this case, Melanopic EDI (lx) = 1263.0338) and let you download the full data as a file.
