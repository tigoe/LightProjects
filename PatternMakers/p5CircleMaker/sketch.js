/**
 Circle packing sketch that exports an SVG file
 when you click the button.
 
 created 13 Jan 2020
 by Tom Igoe
 */

let circles = new Array;	// array to hold the circles
let objectCount = 500;     // how many circles to draw
let h = 600;
let w = 800;
let button;
let img;

function setup() {
	// set up the SVG canvas:
	createCanvas(w, h, SVG);
	// create an image object to hold the  pattern:
	img = createGraphics(w, h, SVG);
	// set image parameters:
	img.strokeWeight(1);
	img.ellipseMode(CENTER);
	img.noFill();
	// // create the file save button and give it properties:
	button = createButton('save');
	button.position(10, 10);
	button.mouseReleased(makeFile);
	noLoop();
}

function draw() {
	// generate pattern until there are enough circles:
	while (circles.length < objectCount) {
		makeCircles();
	}
	// // draw the image on the screen:
	image(img, 0, 0, w, h);
}

// make a new file with the SVG element:
function makeFile() {
	// get the SVG string:
	let input = img.elt.svg.outerHTML;
	// put the string in an array:
	let output = [input];
	// save it out to a file:
	saveStrings(output, 'image', 'svg');
}

// generate objects to be drawn:
function makeCircles() {
	let thisCircle = {
		x: random(w),
		y: random(h),
		d: random(10, 50)
	};

	// check to see if new circle overlaps any of the existing ones:
	for (let c = 0; c < circles.length; c++) {
		if (overlaps(circles[c], thisCircle)) {
			// don't add a circle if it overlaps an existing one:
			return;
		}
	}
	// add to the list of objects to be drawn:
	circles.push(thisCircle);
	// add the circle to the SVG image
	img.ellipse(thisCircle.x, thisCircle.y, thisCircle.d, thisCircle.d);
}

// check to see if two circles overlap
// (actually checks if they are less than a number of pixels from each other)
function overlaps(obj, newObj) {
	let result = true;
	// safe margin of distance between objects:
	let margin = 5.0;
	// get the distance between the objects' centers:
	let distance = dist(newObj.x, newObj.y, obj.x, obj.y);
	// generate an overall safe distance between them:
	let safeDistance = newObj.d / 2 + obj.d / 2 + margin;
	if (distance >= safeDistance) {
		// if the distance > safeDistance, then they don't overlap:
		result = false;
	}

	return result;
}