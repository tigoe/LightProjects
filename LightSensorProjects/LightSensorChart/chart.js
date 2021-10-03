
/*
   Graph  page JavaScript
   Adapted from Don Coleman's  ITP conndev visualizer:
   https://github.com/don/itp-conndev/tree/main/public

*/

// Get  data from the server for the selected device
async function getChartData(device) {
   // assemble the URL from the URL input field on the HTML page:
   let serverUrl = 'https://' + document.getElementById('url').value + '/data';
   // add the mac address from the macAddr input field om the HTML page:
   let macAddress = document.getElementById('macAddr').value;
   // add the mac address from the sessKey input field om the HTML page:
   let sessionKey = document.getElementById('sessKey').value;
   // make an HTTPS call for the data:
   const response = await fetch(serverUrl + `?macAddress=`
      + macAddress + `&sessionKey=` + sessionKey);
   // When you get a response, put the result in a variable:
   const json = await response.json();
   // map the result into a big set of arrays that you can use to graph:
   const rows = json.map(row => [new Date(row.timestamp), JSON.parse(row.data).lux, JSON.parse(row.data).CCT]);
   // graphj the result:
   drawChart(rows);
}

// Draw the chart
function drawChart(rows) {
   // make an instance of the DataTable library:
   var data = new google.visualization.DataTable();
   // add columns to the table for date, Lux, and CCT:
   data.addColumn('date', 'Date');
   data.addColumn('number', 'Lux');
   data.addColumn('number', 'CCT');
   // add the rows from the server:
   data.addRows(rows);
   // make the chart:
   var chart = new google.visualization.AnnotationChart(document.getElementById('chart_div'));
   // include the annotations:
   var options = {
      displayAnnotations: true
   };
   // draw the chart:
   chart.draw(data, options);
}

// here's the only line that actually gives a command. When the page loads, 
// load the google charts library:
google.charts.load('current', { 'packages': ['annotationchart'] });