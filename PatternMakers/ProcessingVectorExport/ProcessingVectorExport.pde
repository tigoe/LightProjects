/**
 Simple sketch that exports a vector (DXF or SVG) file
 when you press d or s. Every time you press one of these keys,
 the current image is saved and a new image is generated.
 
 created 13 Jan 2020
 by Tom Igoe
 */

// import SVG and DXF libraries:
import processing.dxf.*;
import processing.svg.*;

boolean recording = false;                    // whether you're recording a file or not
int fileCount = 0;                            // count of files recorded so far
String fileName;                              // file name and extension
String extension = "";

void setup() {
  // set up window and smooth drawing:
  size(800, 600, P3D);
  smooth();
  // new pattern every 2 seconds:
  frameRate(0.5);
}

void draw() {
  // start recording if s or d key has been pressed:
  if (recording) {
    // assemble fileName:
    fileName = "pattern" + fileCount + "." + extension;

    // recording format depends on key pressed:
    if (extension.equals("svg")) {
      beginRaw(SVG, fileName);
    } else if (extension.equals("dxf")) {
      beginRaw(DXF, fileName);
    }
  }

  // you've got enough things to draw, now draw them:
  background(255);
  stroke(0);
  noFill();

  // draw the pattern:
   circle(random(width), random(height), random(20, 80));
   rect(random(width), random(height), random(20, 80), random(20, 80));

  if (recording) {
    endRaw();            // end the recording:
    fileCount++;         // increment file counter
    recording = false;   // clear recording flag:
  }
}

// determine what file format, SVG (press s) or DXF (press D):
void keyPressed() {
  // record SVG
  if (key == 's' || key == 'S') {
    recording = true;
    extension = "svg";
  }
  // record DXF
  if (key == 'd' || key == 'D') {
    recording = true;
    extension = "dxf";
  }
}
