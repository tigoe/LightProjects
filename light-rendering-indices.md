# Light Rendering Indices

Here is a good [video](https://youtu.be/dIFIKTDDTtM)) introducing the [Sekonic C-800-U spectrometer](https://sekonic.com/sekonic-c-800-u-spectromaster-spectrometer/). It describes the various light rendering indices that the spectrometer can display. Some of those indices are measured relative to the human eye's sensitivity, while others are measured relative to a camera's sensitivity. 

**CRI - Color Rendering Index** Based on the evaluation of 15 colors using the human eye as observer. First 8 colors are what the CRI number is based on, and the rest are for further evaluating the quality of the source. **CRI-extended** is an evaluation using all 15. CRI is a measure of how the human eye can accurately perceive colors under daylight conditions. Was originally designed to judger fluoresecent lamps.

**TLCI - Television Lighting Consistency Index** The observer is a 3-chip broadcast camera. Uses 18 standard colors (from an X-Rite [Color Checker classic](https://www.xrite.com/categories/calibration-profiling/colorchecker-classic) card; leaves out the grays), and considers that some of the CRI colors (like R9) may be out of gamut for some cameras, so not a good index to use. 

**TLMF - Television Luminaire Matching Factor** A companion to TLCI, but based on using a reference luminant. Lets you evaluate two light sources relative to each other. Uses all colors from the [Color Checker Classic](https://www.xrite.com/categories/calibration-profiling/colorchecker-classic). Based on a pre-digital camera standard (Rec. 709), so may be out of date for some modern cameras. 

**TM-30-18** - The final number is the version number. Uses 99 colors, and gives the ability to judge how saturated colors will look under a source. Color graph lets you know if specific colors are over or under saturated, and what the tendency to shift is. Is expected to replace CRI. TM-30 Rf measures color fidelity, 0-100. R<sub>g</sub> is a 60-140 scale, measuring saturation. 100 is proper saturation relative to the reference color. Below 100 is less saturated, above 100 is more saturated.

More on TMC-30 from [Using TM-30 to Improve Your Lighting Design](https://www.ies.org/fires/using-tm-30-to-improve-your-lighting-design/):

TM-30 is an attempt to improve upon the color rendering index (CRI) standard for evaluating how well a light source renders the color of an object lit by that source. It’s a standard for lighting manufacturers to use to describe and test their sources, and for designers to use in evaluating those sources in use.  The major properties, or design intents of TM-30 are Preference, Vividness, and Fidelity. 

From the IES document above:

“The Preference design intent captures subjective evaluations of pleasantness, naturalness, acceptability, and related qualities, and may be the dominant color rendition design intent in retail, office, hospitality, and residential lighting applications.”

“The Vividness design intent captures subjective evaluations of color vividness, saturation, or vibrancy and may be the dominant color rendition design intent in some entertainment, display, or retail applications.”

“The Fidelity design intent captures the match between a test light source and its reference. Note that fidelity should not be considered equivalent to naturalness, a common misconception.”

There are multiple rendering indices in TM-30, each labeled R<sub>subscript</sub> (e.g. R<sub>f</sub> for fidelity index, R<sub>g</sub> for gamut index, etc). Each index offers a different measurement of the light source. R<sub>f</sub>, for example, measures the average similarity between a source and the reference light source for all colors (this is where you may want to read up on [standard illuminants](https://sensing.konicaminolta.us/us/blog/understanding-standard-illuminants-in-color-measurement/)). Each design intent uses a subset of the rendering indices to articulate how well a light source meets the design intent. 

**SSI - Spectral Similarity Index** compares spectra between two light sources. Eliminates the observer. Unlike TMLF, it looks at spectral distribution from 380 - 670nm, with 10nm bins. 

For more on light and color rendering, see the [chromaticity](chromaticity.md) description in this repository.  

