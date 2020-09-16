var express = require('express');
var router = express.Router();
const axios = require('axios');
const viaje = require('../../rest-api-loopback/server/models/viaje');



router.post('/envio/salida', function(req, res, next) {
    // console.log(JSON.stringify(req.body));

        let viajeData = {};
        viajeData['pesosalida'] = req.body.peso;
        viajeData['horasalida']=  new Date().toISOString();
        viajeData['estadoviaje']='progreso';
    axios({
        method: 'post',
        url: process.env.URL + '/api/viajes',
        headers: {'Content-Type': 'application/json', 'Accept': 'application/json'},
        data: viajeData
    })
    .then(function (response) {
        res.status(200).json({ result: 'success' });
    })
    .catch(function (error) {
        console.log(viajeData);
        //console.log(error);
        next(error);
    });
});

router.post('/envio/entrega', function(req, res, next) {
    let viajeData = {};
        viajeData['pesoentrega'] = req.body.peso;
        viajeData['obstaculoentrega'] = req.body.obstaculos;
        viajeData['horaentrega']= new Date().toISOString();

      //  viajeData['horaentrega']='now';
        const selectFilter = {"fields": {"id": true},"order": "timestamp DESC","limit":1};
        const url = process.env.URL + '/api/viajes?filter=' + encodeURIComponent(JSON.stringify(selectFilter));
        axios({
            method: 'get',
            url: url,
            headers: {'Accept': 'application/json'},
        })
        .then(function (response) {
            // handle success
            let id = response.data[0]['id'];
             //res.status(200).json(response.data[0]);

             axios({
                method: 'patch',
                url: process.env.URL + '/api/viajes/'+id,
                headers: {'Content-Type': 'application/json', 'Accept': 'application/json'},
                data:viajeData
            }).then(function(response){
                res.status(200).json({ result: 'success' });

            }).catch(function (error){
                next(error);
            });
       
        }).catch(function (error) {

            next(error);
        });
});


router.post('/envio/regreso', function(req, res, next) {
        let viajeData = {};
        viajeData['obstaculoregreso'] = req.body.obstaculos;
        viajeData['horaregreso']= new Date().toISOString();
        viajeData['estadoviaje']='finalizado';

      //  viajeData['horaentrega']='now';
        const selectFilter = {"fields": {"id": true},"order": "timestamp DESC","limit":1};
        const url = process.env.URL + '/api/viajes?filter=' + encodeURIComponent(JSON.stringify(selectFilter));
        axios({
            method: 'get',
            url: url,
            headers: {'Accept': 'application/json'},
        })
        .then(function (response) {
            // handle success
            let id = response.data[0]['id'];
             //res.status(200).json(response.data[0]);

             axios({
                method: 'patch',
                url: process.env.URL + '/api/viajes/'+id,
                headers: {'Content-Type': 'application/json', 'Accept': 'application/json'},
                data:viajeData
            }).then(function(response){
                res.status(200).json({ result: 'success' });

            }).catch(function (error){
                next(error);
            });
       
        }).catch(function (error) {

            next(error);
        });
});





module.exports = router;
