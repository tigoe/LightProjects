/**
 hexagon packing sketch that exports an SVG file
 when you click the button. Unlike the p5CircleMaker, this 
 sketch will check to make sure all shapes are fully within
 the frame of the image.
 
 created 13 Jan 2020
 modified 18 July 2021
 by Tom Igoe
 */

let shapes = new Array;	// array to hold the shapes
let objectCount = 500;     // how many shapes to draw
let h = 600;
let w = 800;
let button;
let img;
// change this for other numbers of sides to the shape:
const SIDE_COUNT = 6;

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
	// generate pattern until there are enough shapes:
	while (shapes.length < objectCount) {
		makeShapes();
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
function makeShapes() {
	// set the diameter of the shape:
	let thisSize = random(10, 50);
	// set the x and y such that it's always inside the canvas:
	let thisShape = {
		x: random(w - thisSize * 2) + thisSize,
		y: random(h - thisSize * 2) + thisSize,
		d: thisSize
	};

	// check to see if new shape overlaps any of the existing ones:
	for (let c = 0; c < shapes.length; c++) {
		// don't add a shape if it overlaps an existing one:
		if (overlaps(shapes[c], thisShape)) return;
	}
	// add to the list of objects to be drawn:
	shapes.push(thisShape);
	// add the shape to the SVG image
	polygon(thisShape.x, thisShape.y, thisShape.d / 2, SIDE_COUNT);
}

// check to see if two shapes overlap
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

// from https://p5js.org/examples/form-regular-polygon.html
function polygon(x, y, radius, npoints) {
	let angle = TWO_PI / npoints;
	img.beginShape();
	for (let a = 0; a < TWO_PI; a += angle) {
		let sx = x + cos(a) * radius;
		let sy = y + sin(a) * radius;
		img.vertex(sx, sy);
	}
	img.endShape(CLOSE);
}
