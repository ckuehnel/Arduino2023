function Decoder(topic, payload) 
{
    var decodedPayload = JSON.parse(payload);

    var node = decodedPayload.node;
    var gps = decodedPayload.GPS;
    var Vsolar = parseFloat(decodedPayload.Vsolar).toFixed(3);
    var Vbat = parseFloat(decodedPayload.Vbat).toFixed(3);
    var loading;
    if (Vsolar > Vbat) loading = true;
    else loading = false;
    
    console.log(Vsolar, Vbat, loading);
    
    return [
        {
            device: "8534e445-99b3-4e13-a42c-3697f4c97b9d",
            field: "VSOLAR",
            value: Vsolar
        },
        {
            device: "8534e445-99b3-4e13-a42c-3697f4c97b9d",
            field: "VBAT",
            value: Vbat
        },
        {
            device: "8534e445-99b3-4e13-a42c-3697f4c97b9d",
            field: "LOADING",
            value: loading
        }

    ]                               
}