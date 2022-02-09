/*
  Spectrograph
  Written for serial input from AS7341, but could be adapted
  for other spectrometers, and for other transport mechanisms.

  Assumes a set of comma-separated values.

  uses p5.js and chart.js and webSerial. 
  For more on chart.js, see https://www.chartjs.org/

  created 9 July 2021
  modified 8 Feb 2022
  by Tom Igoe
*/

// Serial port elements:
let port;               // the serial port
let reader;             // port reader object
let writer;             // port writer object
let keepReading = true; // keeps the listenForSerial loop going
let serialReadPromise;  // Promise for the listenForSerial function

// the DOM elements that might be changed by various functions:
let portButton;   // the open/close port button

let timestampDiv;
// fill in wavelengths of your spectrometer here:
let wavelengths = [415, 445, 480, 515, 555, 590, 630, 680, 910, 'clear'];
let numBands = 10;

// array to put the readings from the sensors into:
let readings = new Array();

// instance of the chart:
let chart;

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
  createCanvas(800, 600);

  // port open/close button:
  portButton = createButton("Open port");
  portButton.position(10, 10);
  portButton.mousePressed(openClosePort);

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
  // make a text div for the timestamp of each reading:
  timestampDiv = createDiv('last reading at: ');
  timestampDiv.position(windowWidth - 300, 10);
}

function draw() {
  // update the chart:
  chart.update();
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

////////////////////

async function openClosePort() {
  // if the browser supports serial:
  if ("serial" in navigator) {
    // if the port exists, it's likely open. Close it:
    if (port) {
      // set keepReading false to stop the listenForSerial loop:
      keepReading = false;
      // stop the reader, so you can close the port:
      reader.cancel();
      // wait for the listenForSerial function to stop:
      await serialReadPromise;
      // close the serial port itself:
      await port.close();
      // change the button label:
      portButton.html("open port");
      // clear the port variable:
      port = null;
    } else {
      // if the port is null, try to open it:
      try {
        // pop up window to select port:
        port = await navigator.serial.requestPort();
        // set port settings and open it:
        await port.open({ baudRate: 9600 });
        // enable the listenForSerial loop:
        keepReading = true;
        // start the listenForSerial function:
        serialReadPromise = listenForSerial();
        // change the button label:
        portButton.html("Close port");
      } catch (err) {
        // if there's an error opening the port:
        console.error("There was an error opening the serial port:", err);
      }
    }
  }
}

async function sendData(data) {
  // if the port's open and readable:
  if (port) {
    if (port.readable) {
      // initialize the writer:
      writer = port.writable.getWriter();
      // convert the data to be sent to an array:
      var output = new TextEncoder().encode(data);
      // send it, then release the writer:
      writer.write(output).then(writer.releaseLock());
    }
  }
}

async function listenForSerial() {
  // while the port is open and keepReading is true:
  while (port.readable && keepReading) {
    // initialize the reader:
    reader = port.readable.getReader();
    try {
      // read incoming serial buffer:
      const { value, done } = await reader.read();
      // convert the input to a text string:
      let inString = new TextDecoder().decode(value);
      // use the serial data:
      let readings = float(split(inString, ','));
      // if you have all the readings, put them in the chart:
      if (readings.length >= numBands) {
        chart.data.datasets[0].data = readings;
        // update the timestamp:
        timestampDiv.html('last reading at: ' + new Date().toLocaleString());
      }
    } catch (error) {
      // if there's an error reading the port:
      console.log(error);
    } finally {
      // once the read is done, release the reader.
      // this enables other functions to run port.close():
      reader.releaseLock();
    }
  }
}