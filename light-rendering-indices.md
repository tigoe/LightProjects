# Light Rendering Indices

Here is a good [video](https://youtu.be/dIFIKTDDTtM)) introducing the [Sekonic C-800-U spectrometer](https://sekonic.com/sekonic-c-800-u-spectromaster-spectrometer/). It describes the various light rendering indices that the spectrometer can display. Some of those indices are measured relative to the human eye's sensitivity, while others are measured relative to a camera's sensitivity. 

**CRI - Color Rendering Index** Based on the evaluation of 15 colors using the human eye as observer. First 8 colors are what the CRI number is based on, and the rest are for further evaluating the quality of the source. **CRI-extended** is an evaluation using all 15. CRI is a measure of how the human eye can accurately perceive colors under daylight conditions. Was originally designed to judger fluoresecent lamps.

**TLCI - Television Lighting Consistency Index** The observer is a 3-chip broadcast camera. Uses 18 standard colors (from an X-Rite Color Checker classic card; leaves out the grays), and considers that some of the CRI colors (like R9) may be out of gamut for some cameras, so not a good index to use. 

**TLMF - Television Luminaire Matching Factor** A companion to TLCI, but based on using a reference luminant. Lets you evaluate two light sources relative to each other. Uses all colors from the Color Checker Classic. Based on a pre-digital camera standard (Rec. 709), so may be out of date for some modern cameras. 

**TM-30-18** - The final number is the version number. Uses 99 colors, and gives the ability to judge how saturated colors will look under a source. Color graph lets you know if specific colors are over or under saturated, and what the tendency to shift is. Is expected to replace CRI. TM-30 Rf measures color fidelity, 0-100. Rg is a 60-140 scale, measuring saturation. 100 is proper saturation relative to the reference color. Below 100 is less saturated, above 100 is more saturated.

**SSI - Spectral Similarity Index** compares spectra between two light sources. Eliminates the observer. Unlike TMLF, it looks at spectral distribution from 380 - 670nm, with 10nm bins. 

For more on light and color rendering, see the [chromaticity](chromaticity.md) description in this repository.  

