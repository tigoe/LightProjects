/*
  Spectrograph
  Written for serial input from AS7341, but could be adapted
  for other spectrometers, and for other transport mechanisms.

  Assumes a set of comma-separated values.

  uses p5.js and p5.serialport and chart.js. Requires p5.serialcontrol app 
  to be open as well.
  For more on chart.js, see https://www.chartjs.org/

  created 9 July 2021
  by Tom Igoe
*/

// variable to hold an instance of the serialport library:
let serial;
// HTML Select option object:
let portSelector;

// fill in wavelengths of your spectrometer here:
let wavelengths = [415, 445, 480, 515, 555, 590, 630, 680, 910];

// from https://ams.com/documents/20143/36005/AS7341_AN000633_1-00.pdf/fc552673-9800-8d60-372d-fc67cf075740 
// fig 10
// TODO: get this part correct.
let corrections = [3.20, 3.00, 2.07, 1.30, 1.07, 0.93, 0.78, 0.71];
let numBands = 9;

// array to put the readings from the sensors into:
let readings = new Array();

// instance of the chart:
let chart;

// data object for the chart:
const data = {
  labels: wavelengths,
  datasets: [{
    label: 'AS7341 Spectrometer',
    legend: false,
    data: new Array(),
    backgroundColor: new Array(),
    borderColor: new Array(),
    borderWidth: 1
  }]
};

const config = {
  type: 'bar',
  data: data,
  options: {
    plugins: {
      // this gets rid of the box by the data label:
      legend: {
        labels: {
          boxWidth: 0,
          boxHeight: 0
        }
      }
    },
    // change the animation from one reading to the next:
    animation: {
      easing: 'easeInOutSine',
      duration: 50
    },
    scales: {
      y: {
        beginAtZero: true
      }
    }
  }
};

function setup() {
  // set up the canvas:
  createCanvas(1024, 768);

  // new instance of the serialport library:
  serial = new p5.SerialPort();
  // callback function for serialport:
  serial.on('list', printList);
  serial.on('data', serialEvent);
  // list the serial ports:
  serial.list();

  // instantiate the chart:
  chart = new Chart(
    this,   // context: this sketch
    config  // config from the global variables
  );
  // push each wavelength onto the chart's
  // background color and borderColor:
  for (w of wavelengths) {
    chart.data.datasets[0].backgroundColor.push(wavelengthToColor(w));
    chart.data.datasets[0].borderColor.push(wavelengthToColor(w));
  }
}

function draw() {
  // update the chart:
  chart.update();
}

// make a serial port selector object:
function printList(portList) {
  // create a select object:
  portSelector = createSelect();
  portSelector.position(10, 10);
  // portList is an array of serial port names
  for (var i = 0; i < portList.length; i++) {
    // add this port name to the select object:
    portSelector.option(portList[i]);
  }
  // set an event listener for when the port is changed:
  portSelector.changed(mySelectEvent);
}

function mySelectEvent() {
  let item = portSelector.value();
  // give it an extra property for hiding later:
  portSelector.visible = true;
  // if there's a port open, close it:
  if (serial.serialport != null) {
    serial.close();
  }
  // open the new port:
  serial.open(item);
}

function keyPressed() {
  // if port selector is visible, hide it, else show it:
  if (portSelector) {
    if (portSelector.visible) {
      portSelector.hide();
      portSelector.visible = false;
    } else {
      portSelector.show();
      portSelector.visible = true;
    }
  }
}

// if new serial data comes in, put it in the readings array:
function serialEvent() {
  let inData = serial.readLine();
  let readings = float(split(inData, ','));
  // if you have all the readings, put them in the chart:
  if (readings.length >= numBands) {
    chart.data.datasets[0].data = readings;
  }
}

// takes wavelength in nm and returns an rgba value
// adapted from https://scienceprimer.com/javascript-code-convert-light-wavelength-color
function wavelengthToColor(wavelength) {
  var r, g, b, alpha, colorSpace,
    wl = wavelength,
    gamma = 1;
  // UV to indigo:
  if (wl >= 380 && wl < 440) {
    R = -1 * (wl - 440) / (440 - 380);
    G = 0;
    B = 1;
    // indigo to blue:
  } else if (wl >= 440 && wl < 490) {
    R = 0;
    G = (wl - 440) / (490 - 440);
    B = 1;
    // blue to green:
  } else if (wl >= 490 && wl < 510) {
    R = 0;
    G = 1;
    B = -1 * (wl - 510) / (510 - 490);
    // green to yellow:
  } else if (wl >= 510 && wl < 580) {
    R = (wl - 510) / (580 - 510);
    G = 1;
    B = 0;
    // yellow to orange:
  } else if (wl >= 580 && wl < 645) {
    R = 1;
    G = -1 * (wl - 645) / (645 - 580);
    B = 0.0;
    // orange to red:
  } else if (wl >= 645 && wl <= 780) {
    R = 1;
    G = 0;
    B = 0;
    // IR:
  } else {
    R = 0;
    G = 0;
    B = 0;
  }

  // intensity is lower at the edges of the visible spectrum.
  if (wl > 780 || wl < 380) {
    alpha = 0;
  } else if (wl > 700) {
    alpha = (780 - wl) / (780 - 700);
  } else if (wl < 420) {
    alpha = (wl - 380) / (420 - 380);
  } else {
    alpha = 1;
  }
  // combine it all:
  colorSpace = "rgba(" + (R * 100) + "%," + (G * 100) + "%," + (B * 100) + "%, " + alpha + ")";
  return colorSpace;
}