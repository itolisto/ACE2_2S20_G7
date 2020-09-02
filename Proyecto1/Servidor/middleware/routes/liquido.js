var express = require('express');
var router = express.Router();
const axios = require('axios');
var geoip = require('geoip-lite');


router.post('/liquido', function(req, res, next) {
    // console.log(JSON.stringify(req.body));

    let liquidoData = {};

    liquidoData['nivel'] = req.body.liquido;
   

    const ip = req.clientIp;

    console.log(`Data recibida: ${JSON.stringify(liquidoData)}`);
    console.log(`IP recivida ${ip}`);

    if (ip) {
        var geo = geoip.lookup(ip);

        console.log(`Geo ${JSON.stringify(geo)}`);

        if (geo) {
            console.log(`Location ${geo.ll[0]},${geo.ll[1]}`);

            liquidoData['latitude'] = geo.ll[0];
            liquidoData['longitude'] = geo.ll[1];
        }
    }

    axios({
        method: 'post',
        url: process.env.URL + '/api/liquidos',
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

router.get('/liquido', function(req, res, next) {
    const selectFilter = {"fields": {"nivel": true,"latitude": true,"longitude": true},"order": "timestamp DESC","limit":1};

    const url = process.env.URL + '/api/liquidos?filter=' + encodeURIComponent(JSON.stringify(selectFilter));

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
