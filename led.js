(function() {
  'use strict';

  class LED {

    /**
      * The LED class is a object used to represent the BLE service. Note for this project
      * the read code is not used, but I kept it there for you to see.
      *
      * customize your project here to reflect the uuid of your service and characteristics.
      */
    constructor() {
        this.deviceName = 'blink';
        this.serviceUUID =   '917649a3-d98e-11e5-9eec-0002a5d5c51b';
        this.ledUUID =       '917649a4-d98e-11e5-9eec-0002a5d5c51b';
        this.device = null;
        this.server = null;
        // The cache allows us to hold on to characeristics for access in response to user commands
        this._characteristics = new Map();
    }

    connect(){

      return navigator.bluetooth.requestDevice({
        filters:[{
          services:[this.serviceUUID],
        }]
      }).then(device => device.gatt.connect())
      .then(server => server.getPrimaryService(this.serviceUUID))
      .then(service => {
        return Promise.all([
          this._cacheCharacteristic(service, this.ledUUID),
        ]);
      })
    }

  _cacheCharacteristic(service, characteristicUuid){
    return service.getCharacteristic(characteristicUuid)
    .then(characteristic => {
      this._characteristics.set(characteristicUuid, characteristic).then(console.log('cache: '  + characteristicUuid));
    });
  }

 _readCharacteristic(characteristicUuid) {
   let characteristic = this._characteristics.get(characteristicUuid);
   console.log("read this char: " + characteristicUuid);
   console.log("this is thechar: " + characteristic);
   return characteristic.readValue()
   .then(value => {console.log('value: ' + value);
     return value;
   });

 }
 _writeCharacteristic(characteristicUuid, value){
   let characteristic = this._characteristics.get(characteristicUuid);
   console.log("the write char: " + characteristic);
   for(let k of this._characteristics.keys()) console.log('keys: ' + k);
   for(let v of this._characteristics.values()) console.log('values: ' + v);
   return characteristic.writeValue(value);
 }
}


// makes the led object available to therest of the program, remeber to change this to reflect your project
window.led = new LED();

})();
