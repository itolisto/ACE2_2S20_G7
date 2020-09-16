var express = require('express');
var router = express.Router();
const axios = require('axios');
const viaje = require('../../rest-api-loopback/server/models/detalleviaje');


router.post('/vehiculo/progreso', function(req, res, next) {
    // console.log(JSON.stringify(req.body));

        let vehiculoData = {};
        vehiculoData['estado'] = req.body.estado;


        if(req.body.estado=='reposo'){
            vehiculoData['ubicacion']='buzon';
        }else{
            vehiculoData['ubicacion']='cliente';
        }
        const selectFilter = {"fields": {"id": true},"order": "timestamp DESC","limit":1};
        const url = process.env.URL + '/api/viajes?filter=' + encodeURIComponent(JSON.stringify(selectFilter));
        axios({
            method: 'get',
            url: url,
            headers: {'Accept': 'application/json'},
        })
        .then(function (response) {
            // handle success
            vehiculoData['idviaje']= response.data[0]['id'];
            console.log('identificador'+vehiculoData['id']);
             //res.status(200).json(response.data[0]);

            axios({
                method: 'post',
                url: process.env.URL + '/api/detalleviajes',
                headers: {'Content-Type': 'application/json', 'Accept': 'application/json'},
                data: vehiculoData
            })
            .then(function (response) {
                res.status(200).json({ result: 'success' });
            })
            .catch(function (error) {
                console.log(vehiculoData);
                //console.log(error);
                next(error);
            });
       
        }).catch(function (error) {

            next(error);
        }); 
       
  
});


module.exports = router;
