function Decoder(topic, payload) 
{
    // Transform incoming payload to JSON
    payload = JSON.parse(payload);
    payloadData = payload.payload;
    
    // Remember, data is: {"channel":0,"from":4201002620,"id":1242862792,"payload":{"air_util_tx":3.37072229385376,"battery_level":91,"channel_utilization":5.82999992370605,"voltage":4.06899976730347},"sender":"!fa66367c","timestamp":1710076900,"to":4294967295,"type":"telemetry"}
    
    // Extract from payload, do calculation
    var from = payload.from;
    var type = payload.type;
    var timestamp = payload.timestamp;
    var node;
    
    if      (from == 4201002620) 	{node = "Router Heltec LoRa32 V3";}
    else if (from == 2053978516) 	{node = "Client T-Beam";}
    else if (from == 808617300)  	{node = "Client Heltec LoRa32 V3 H01";}
    else if (from == 3662934188)  	{node = "Client Heltec LoRa32 V3 H02";}
    else if (from == 2692927950) 	{node = "Client RAK4631 RAK1";}
    else if (from == 3118357274) 	{node = "Client RAK4631 RAK2";}
    else                        	{node = "unknown";}
    
    if (from != 2692927950)
    {
        // Forward Data to Datacake Device API using Serial, Field-Identifier
        return [
            {
                device: "e9055050-af37-4836-a50a-6d90968f7500", // Serial Number or Device ID
                field: "from",
                value: from
            },
            {
                device: "e9055050-af37-4836-a50a-6d90968f7500", // Serial Number or Device ID
                field: "node",
                value: node
            },
            {
                device: "e9055050-af37-4836-a50a-6d90968f7500", // Serial Number or Device ID
                field: "type",
                value: type
            },
            {
                device: "e9055050-af37-4836-a50a-6d90968f7500", // Serial Number or Device ID
                field: "timestamp",
                value: timestamp
            }
        ];
    }
    else
    {
        if (type == "telemetry")
        {
            var voltage = payloadData.voltage;
        
            if (voltage != 0)
            {
                // Forward Data to Datacake Device API using Serial, Field-Identifier
                return [
                    {
                        device: "e9055050-af37-4836-a50a-6d90968f7500", // Serial Number or Device ID
                        field: "from",
                        value: from
                    },
                    {
                        device: "e9055050-af37-4836-a50a-6d90968f7500", // Serial Number or Device ID
                        field: "node",
                        value: node
                    },
                    {
                        device: "e9055050-af37-4836-a50a-6d90968f7500", // Serial Number or Device ID
                        field: "type",
                        value: type
                    },
                    {
                        device: "e9055050-af37-4836-a50a-6d90968f7500", // Serial Number or Device ID
                        field: "timestamp",
                        value: timestamp
                    },
                    {
                        device: "e9055050-af37-4836-a50a-6d90968f7500", // Serial Number or Device ID
                        field: "voltage",
                        value: voltage
                    }
                ]; 
            }
            else
            {
                var rh = payloadData.relative_humidity;
                var temp = payloadData.temperature;
                
                // Forward Data to Datacake Device API using Serial, Field-Identifier
                return [
                   {
                        device: "e9055050-af37-4836-a50a-6d90968f7500", // Serial Number or Device ID
                        field: "from",
                        value: from
                    },
                    {
                        device: "e9055050-af37-4836-a50a-6d90968f7500", // Serial Number or Device ID
                        field: "node",
                        value: node
                    },
                    {
                        device: "e9055050-af37-4836-a50a-6d90968f7500", // Serial Number or Device ID
                        field: "type",
                        value: type
                    },
                    {
                        device: "e9055050-af37-4836-a50a-6d90968f7500", // Serial Number or Device ID
                        field: "timestamp",
                        value: timestamp
                    },
                    {
                        device: "e9055050-af37-4836-a50a-6d90968f7500", // Serial Number or Device ID
                        field: "humidity",
                        value: rh
                    },
                    {
                        device: "e9055050-af37-4836-a50a-6d90968f7500", // Serial Number or Device ID
                        field: "temperature",
                        value: temp
                    }
                ];
            }   
        }
        else if (type == "position")
        {
            var altitude = payloadData.altitude;
            var latitude = payloadData.latitude_i/1e7;
            var longitude = payloadData.longitude_i/1e7;
            var sats = payloadData.sats_in_view;
            var loc = "(" + latitude + "," + longitude + ")"
        
            // Forward Data to Datacake Device API using Serial, Field-Identifier
            return [
                {
                    device: "e9055050-af37-4836-a50a-6d90968f7500", // Serial Number or Device ID
                    field: "from",
                    value: from
                },
                {
                    device: "e9055050-af37-4836-a50a-6d90968f7500", // Serial Number or Device ID
                    field: "node",
                    value: node
                },
                {
                    device: "e9055050-af37-4836-a50a-6d90968f7500", // Serial Number or Device ID
                    field: "type",
                    value: type
                },
                {
                    device: "e9055050-af37-4836-a50a-6d90968f7500", // Serial Number or Device ID
                    field: "timestamp",
                    value: timestamp
                },
                {
                    device: "e9055050-af37-4836-a50a-6d90968f7500", // Serial Number or Device ID
                    field: "altitude",
                    value: altitude
                },
                {
                    device: "e9055050-af37-4836-a50a-6d90968f7500", // Serial Number or Device ID
                    field: "location",
                    value: loc
                },
                {
                    device: "e9055050-af37-4836-a50a-6d90968f7500", // Serial Number or Device ID
                    field: "sats",
                    value: sats
                }            
            ];
        }
    }
}