/**
* app.js is the file connecting elements and actions
* 10/18/2016
*/


/**
 * Toggles the opacity of the selected circle representing an LED, then
 * passes off to toggleLight() to change the LED state on the actual
 * arduino101
 */
function lightIt(circleID)
{
  var circle = document.getElementById(circleID);
  var currentOpacity = circle.getAttribute("fill-opacity");
  console.log("current 0 opacity = " + currentOpacity);
  var curOp = parseFloat(currentOpacity);
  console.log("curOp: " + curOp);
 if ( currentOpacity < 0.8 ) {
  // currentOpacity = currentOpacity + 0.2;
   curOp = curOp + 0.2;
  circle.setAttribute("fill-opacity", curOp);
 }
 else {
  circle.setAttribute("fill-opacity", 0.2);
 }
 toggleLight(circleID);
};

function getBrightness()
{
    lightReading = exp3._readCharacteristic(exp3.cds);
    console.log('light: ' + lightReading);
}

/**
  * takes the chosen "circle" and writes the appropriate data
  * to the ledservice characteristics.
  */
var toggleLight = function( circleID )
{
  if (circleID == "circle1" ) {
   led._writeCharacteristic(led.ledUUID, new Uint8Array([4]))
   .then(() => console.log('wrote 4'))
   .catch(error => {console.log('write error');
   });
  }
  else if (circleID == "circle2" ){
    led._writeCharacteristic(led.ledUUID, new Uint8Array([3]))
   .then(() => console.log('wrote 3'))
   .catch(error => {console.log('write error');
   });
  }
  else if (circleID == "circle3") {
    led._writeCharacteristic(led.ledUUID, new Uint8Array([2]))
   .then(() => console.log('wrote 2'))
   .catch(error => {console.log('write error');
   })
  }
  else{
    led._writeCharacteristic(led.ledUUID, new Uint8Array([1]))
   .then(() => console.log('wrote 1'))
   .catch(error => {console.log('write error');
   });
  }
};

/**
  * The Web Bluetooth API specifies that a user action like a click is needed to
  * initiate a connection.
  */
function bleConnect(buttonID) {
  var thisButton = document.getElementById(buttonID);
  thisButton.setAttribute("fill-opacity",0.9);
  led.connect()
      .then(() => console.log('connected'))
      .catch(error => { console.log('connect error!'); });

};
