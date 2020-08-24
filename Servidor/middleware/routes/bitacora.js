var express = require('express');

//FCM
var FCM = require('fcm-node');
var serverKey = 'YOURSERVERKEYHERE'; //put your server key here
var fcm = new FCM(serverKey);


var message = { //this may vary according to the message type (single recipient, multicast, topic, et cetera)
    to: 'registration_token', 
    collapse_key: 'your_collapse_key',
    
    notification: {
        title: 'Objeto Depositado', 
        body: 'Se ha depositado un objeto en tu buzon, revisarlo' 
    },
    
    data: {  //you can send only notification or only data(or include both)
        my_key: 'my value',
        my_another_key: 'my another value'
    }
};



var router = express.Router();
const axios = require('axios');
var geoip = require('geoip-lite');

router.post('/bitacora', function(req, res, next) {
    // console.log(JSON.stringify(req.body));

    let bitacoraData = {};

    bitacoraData['existe'] = req.body.existe;
    bitacoraData['peso'] = req.body.peso;
    
    if(req.body.existe==1){
        fcm.send(message, function(err, response){
            if (err) {
                console.log("Something has gone wrong!");
            } else {
                console.log("Successfully sent with response: ", response);
            }
        });
    }


    const ip = req.clientIp;

    console.log(`Data recibida: ${JSON.stringify(bitacoraData)}`);
    console.log(`IP recivida ${ip}`);

    if (ip) {
        var geo = geoip.lookup(ip);

        console.log(`Geo ${JSON.stringify(geo)}`);

        if (geo) {
            console.log(`Location ${geo.ll[0]},${geo.ll[1]}`);

            bitacoraData['latitude'] = geo.ll[0];
            bitacoraData['longitude'] = geo.ll[1];
        }
    }

    axios({
        method: 'post',
        url: process.env.URL + '/api/bitacoras',
        headers: {'Content-Type': 'application/json', 'Accept': 'application/json'},
        data: bitacoraData
    })
    .then(function (response) {
        res.status(200).json({ result: 'success' });
    })
    .catch(function (error) {
        next(error);
    });
});

router.get('/bitacora', function(req, res, next) {
    const selectFilter = {"fields": {"existe": true,"peso": true ,"latitude": true,"longitude": true},"order": "timestamp DESC","limit":1};

    const url = process.env.URL + '/api/bitacoras?filter=' + encodeURIComponent(JSON.stringify(selectFilter));

    axios({
        method: 'get',
        url: url,
        headers: {'Accept': 'application/json'},
    })
    .then(function (response) {
        // handle success
        res.status(200).json(response.data[0]);
    })
    .catch(function (error) {
        next(error);
    });
});

module.exports = router;
