const credentials = {
   serverUrl : "tigoe.io",
   route : "/data",
	macAddress : "3c:71:bf:87:ce:4c",
	sessionKey : "1D132679-A0C9-4410-8C3C-46B87BB553E0"
};

document.getElementById('url').value = credentials.serverUrl;
// add the mac address from the macAddr input field om the HTML page:
document.getElementById('macAddr').value = credentials.macAddress;
// add the mac address from the sessKey input field om the HTML page:
document.getElementById('sessKey').value = credentials.sessionKey;