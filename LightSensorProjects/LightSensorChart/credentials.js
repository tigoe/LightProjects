const credentials = {
   serverUrl : "",
   route : "/data",
	macAddress : "",
	sessionKey : ""
};

document.getElementById('url').value = credentials.serverUrl;
// add the mac address from the macAddr input field om the HTML page:
document.getElementById('macAddr').value = credentials.macAddress;
// add the mac address from the sessKey input field om the HTML page:
document.getElementById('sessKey').value = credentials.sessionKey;