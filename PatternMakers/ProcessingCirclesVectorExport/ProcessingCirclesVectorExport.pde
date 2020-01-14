/**
 Circle packing sketch that exports a vector (DXF or SVG) file
 when you press d or s. Every time you press one of these keys,
 the current image is saved and a new image is generated.
 
 created 13 Jan 2020
 by Tom Igoe
 */

// import SVG and DXF libraries:
import processing.dxf.*;
import processing.svg.*;


ArrayList<Circle> circles = new ArrayList();  // arrayList to hold objects to be drawn
int objectCount = 500;                        // how many circles to draw
boolean recording = false;                    // whether you're recording a file or not
int fileCount = 0;                            // count of files recorded so far
String fileName;                              // file name and extension
String extension = "";

void setup() {
  // set up window and smooth drawing:
  size(800, 600, P3D);
  smooth();
}

void draw() {
  // generate objects to be drawn:
  while (circles.size() < objectCount) {
    makeCircles();
  } 

  // start recording if s or d key has been pressed:
  if (recording) {
    // assemble fileName:
    fileName = "circles" + fileCount + "." + extension;

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

  // draw the actual objects in the arrayList:
  for (int c=0; c < circles.size(); c++) {
    Circle thisCircle = circles.get(c);
    circle(thisCircle.x, thisCircle.y, thisCircle.d);
  }

  if (recording) {
    endRaw();            // end the recording:
    fileCount++;         // increment file counter:
    circles.clear();     // clear the list of objects:
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

// generate objects to be drawn:
void makeCircles() {
  // generate a random position and diameter:
  Circle thisCircle = new Circle(random(width), random(height), random(10, 50));

  // check to see if new circle overlaps any of the existing ones:
  for (int c = 0; c < circles.size(); c++) {
    Circle thatCircle = circles.get(c);
    // don't add a circle if it overlaps an existing one:
    if (overlaps(thatCircle, thisCircle)) {
      return;
    }
  }
  // add to the list of objects to be drawn:
  circles.add(thisCircle);
}

// check to see if two circles overlap
// (actually checks if they are less than a number of pixels from each other)
boolean overlaps(Circle obj, Circle newObj) {
  boolean result = true;
  // safe margin of distance between objects:
  float margin = 5.0;
  // get the distance between the objects' centersL
  float distance = dist(newObj.x, newObj.y, obj.x, obj.y);
  // generate an overall safe distance between them:
  float safeDistance = newObj.d / 2 + obj.d / 2 + margin;
  if (distance >= safeDistance) {
    // if the distance > safeDistance, then they don't overlap:
    result = false;
  }
  return result;
}

// generates a new circle object. Used so we have all the circles' characteristics in memory
class Circle {
  float x;  // horizontal position
  float y;  // vertical position
  float d;  // diameter

  // constructor. Used to make new objects:
  Circle(float _x, float _y, float _d) {
    x = _x;
    y = _y;
    d = _d;
  }
}
