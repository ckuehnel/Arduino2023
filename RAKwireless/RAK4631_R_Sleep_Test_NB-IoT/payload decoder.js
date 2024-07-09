function decodeUplink(input) 
{
  var value = (input.bytes[0]<<8 | input.bytes[1]);
  var Vsol = value/1000;  // Solar  Cell voltage, units:V
   
  value = input.bytes[2]<<8 | input.bytes[3];
  var Vbat= value/1000;   // LiPo battery voltage, units:V
  
  value = input.bytes[4];
  if(value===0)
    distance = "LiPo battery discharging";
  else if(value<20)
    distance = "Solar buffered - LiPo battery charged";
  
  return {
    data: {
      SolarCell: Vsol,
      LiPoBattery: Vbat,
      State: distance
    },
    warnings: [],
    errors: []
  };
}
