/*
  Spectrograph
  Written for serial input from AS7341, but could be adapted
  for other spectrometers, and for other transport mechanisms.

  Assumes a set of comma-separated values.

  uses p5.js, p5BLE.js and chart.js. 
  For more on chart.js, see https://www.chartjs.org/
  For more on the p5BLE.js: https://itpnyu.github.io/p5ble-website/
  
  created 4 Jan 2021
  by Tom Igoe
*/

// HTML div for messages
let textDiv;

// fill in wavelengths of your spectrometer here:
let wavelengths = [415, 445, 480, 515, 555, 590, 630, 680, 910, 'clear'];
let numBands = 10;

// array to put the readings from the sensors into:
let readings = new Array();

// instance of the chart:
let chart;

// advertised service UUID of the  to search for:
const serviceUuid = "9af01fc3-0000-44b8-8acc-f3ed7a225431";
// characteristic that you plan to read:
let myCharacteristic;
// instance of p5.ble:
let myBLE;
// DOM elements to interact with:
let connectButton;

// data object for the chart:
const data = {
  labels: wavelengths,
  datasets: [{
    label: 'AS7341 Spectrometer',
    data: new Array(),
    backgroundColor: new Array()
  }]
};

const config = {
  type: 'bar',
  data: data,
  options: {
    responsive: true,
    plugins: {
      // this gets rid of the box by the data label:
      legend: {
        labels: {
          boxWidth: 0
        }
      }
    },
    // change the animation from one reading to the next:
    animation: {
      duration: 200 // in ms
    }
  }
};

function setup() {
  // set up the canvas:
  createCanvas(windowWidth, windowHeight);

  // Create a p5ble instance:
  myBLE = new p5ble();

  // Create a 'Connect' button
  connectButton = createButton('Connect')
  connectButton.mousePressed(connectToBle);
  connectButton.position(10, 30);

  // create a div for local messages:
  textDiv = createDiv('Waiting for BLE connection');
  textDiv.position(20, 10);

  // instantiate the chart:
  chart = new Chart(
    this,   // context: this sketch
    config  // config from the global variables
  );
  // push each wavelength onto the chart's
  // background color and borderColor:
  for (w of wavelengths) {
    // if the band name  in the visible spectrum, calculate its color:
    if (w > 400 && w < 700) {
      chart.data.datasets[0].backgroundColor.push(wavelengthToColor(w));
    }
    else {
      if (w > 900) { // for near IR, use deep red:
        chart.data.datasets[0].backgroundColor.push('#770000');
      } else { // for 'clear', use black as its color:
        chart.data.datasets[0].backgroundColor.push('#000000');
      }
    }
  }
}

function draw() {
  // update the chart:
  chart.update();
}

function windowResized() {
  textDiv.html('resize');
  resizeCanvas(windowWidth, windowHeight);
  chart.resize(windowWidth, windowHeight);
  chart.update();
}

function connectToBle() {
  // Connect to a device by passing the service UUID
  myBLE.connect(serviceUuid, gotCharacteristics);
}

// A function that will be called once got characteristics
function gotCharacteristics(error, characteristics) {
  // if there's an error, 
  // notify the user and quit the function:
  if (error) {
    console.log(error);
    textDiv.innerHTML = 'error: ' + error;
    return;
  }

  if (myBLE.isConnected()) {
    connectButton.html('Disconnect');
    connectButton.mousePressed(disconnectBle);
  } else {
    connectButton.html('Connect');
    connectButton.mousePressed(connectToBle);
  }

  myBLE.onDisconnected(connectionChanged);
  console.log('characteristics: ', characteristics);
  myCharacteristic = characteristics[0];
  // start notifications:
  myBLE.startNotifications(myCharacteristic, handleNotifications, 'string');
}


// A function that will be called once got characteristics
function handleNotifications(data) {
  textDiv.html(data);
  // Split the message into an array:
  let readings = float(split(data, ','));
  // if you have all the readings, put them in the chart:
  if (readings.length >= numBands) {
    chart.data.datasets[0].data = readings;
    // update the timestamp:
    textDiv.html('last reading: ' + new Date().toLocaleString());
  }
}

function disconnectBle() {
  // Disonnect  the device
  myBLE.disconnect();
  // Check if myBLE is connected
  isConnected = myBLE.isConnected();
}

function connectionChanged() {
  if (myBLE.isConnected()) {
    connectButton.html('Disconnect');
    connectButton.mousePressed(disconnectBle);
  } else {
    connectButton.html('Connect');
    connectButton.mousePressed(connectToBle);
  }

  console.log('Device state:');
  isConnected = myBLE.isConnected();
  console.log(isConnected);
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