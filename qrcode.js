// add a QR code of the URL when the page loads
function getQrCode() {
  // make the QR code:
  let qr = qrcode(0, 'L');
  qr.addData(document.URL);
  qr.make();
  // create an image from it:
  let qrImg = qr.createImgTag(2, 8, "qr code of " + document.URL);
 // get the image and label:
  let label = document.getElementById('qrcode');
  label.innerHTML = qrImg;
}

// on page load, call the QR code function:
document.addEventListener('DOMContentLoaded', getQrCode);