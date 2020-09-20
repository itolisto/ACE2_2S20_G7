var express = require('express');
var router = express.Router();
const axios = require('axios');
const viaje = require('../../rest-api-loopback/server/models/viaje');
var moment = require('moment');
//FCM
var admin = require('firebase-admin');
var serviceAccount = require("./tokenfirebase.json");
admin.initializeApp({
    credential: admin.credential.cert(serviceAccount),
    databaseURL: "https://proyecto1-e5661.firebaseio.com"
});

router.post('/envio/salida', function(req, res, next) {
    const viajeData = {
        pesosalida: req.body.peso,
        horasalida: new Date().toISOString(),
        estadoviaje: 'progreso'
    };

    const message = {
        data: {
            title: 'Hola Mundo',
            body: 'Hola Body'
        },
        topic: 'grupoVehiculo'
    };

    admin.messaging().send(message)
    .then((response) => {
        // Response is a message ID string.
        console.log('Successfully sent message:', response);
        
        return axios({
            method: 'post',
            url: process.env.URL + '/api/viajes',
            headers: {'Content-Type': 'application/json', 'Accept': 'application/json'},
            data: viajeData
        });
    }).then((response) => {
        const dataEstadista = {
            idviaje: response['data']['id']
        };
        
        return axios({ 
            method:'post',
            url:process.env.URL+'/api/estadisticas',
            headers:{'Content-Type': 'application/json', 'Accept': 'application/json'},
            data:dataEstadista
        });
    }).then(() => {
        res.status(200).json({ result: 'success' });
    }).catch((error) => {
        console.log('Error:', error);
    });
});

router.post('/envio/entrega', function(req, res, next) {
    let viajeData = {
        pesoentrega: req.body.peso,
        obstaculoentrega: req.body.obstaculos,
        horaentrega: new Date().toISOString()
    };

    let tiempoProm;

    const selectFilter = {"fields": {"id": true,"horasalida":true},"order": "timestamp DESC","limit":1};
    const url = process.env.URL + '/api/viajes?filter=' + encodeURIComponent(JSON.stringify(selectFilter));

    axios({
        method: 'get',
        url: url,
        headers: {'Accept': 'application/json'},
    }).then(function (response) {
        // handle success
        let id = response.data[0]['id'];
    
        const horasalida = moment(response.data[0]['horasalida']);
        const horaentrega = moment(viajeData['horaentrega']);
        tiempoProm = horaentrega-horasalida;

        return axios({
            method: 'patch',
            url: process.env.URL + '/api/viajes/' + id,
            headers: {'Content-Type': 'application/json', 'Accept': 'application/json'},
            data:viajeData
        });
    }).then(function() {
        /**
         * Dentro del patch para estadistica se debe enviar pesopromedio,tiempopromedioida
         */
        const selectFilter4 = {"fields": {"id": true},"order": "timestamp DESC","limit":1};
        const url4 = process.env.URL + '/api/estadisticas?filter=' + encodeURIComponent(JSON.stringify(selectFilter4));

        return axios({
            method: 'get',
            url: url4,
            headers: {'Accept': 'application/json'}
        });
    }).then(function(response4) {
        let idest = response4.data[0]['id'];
        
        let dataEstadistico = {
            pesopromedio: viajeData['pesoentrega'],
            tiempopromedioida: tiempoProm
        };
        
        return axios({
            method: 'patch',
            url: process.env.URL + '/api/estadisticas/' + idest,
            headers: {'Content-Type': 'application/json', 'Accept': 'application/json'},
            data: dataEstadistico
        });
    }).then(function() {
        res.status(200).json({ result: 'success' });
    }).catch(function (error) {
        console.log('Error:', error);
    });
});


router.post('/envio/regreso', function(req, res, next) {
        let viajeData = {};
        viajeData['obstaculoregreso'] = req.body.obstaculos;
        viajeData['horaregreso']= new Date().toISOString();
        viajeData['estadoviaje']='finalizado';

      //  viajeData['horaentrega']='now';
        const selectFilter = {"fields": {"id": true,'horaentrega':true,'obstaculoentrega':true},"order": "timestamp DESC","limit":1};
        const url = process.env.URL + '/api/viajes?filter=' + encodeURIComponent(JSON.stringify(selectFilter));
        axios({
            method: 'get',
            url: url,
            headers: {'Accept': 'application/json'},
        })
        .then(function (response) {
            // handle success
            let id = response.data[0]['id'];
            let promedioRegreso = moment( viajeData['horaregreso'])-moment(response.data[0]['horaentrega']);
            let totalObstaculos =viajeData['obstaculoregreso']+response.data[0]['obstaculoentrega'];

           
             //res.status(200).json(response.data[0]);

             axios({
                method: 'patch',
                url: process.env.URL + '/api/viajes/'+id,
                headers: {'Content-Type': 'application/json', 'Accept': 'application/json'},
                data:viajeData
            }).then(function(response){

                //res.status(200).json({ result: 'success' });
                const selectFilter4 = {"fields": {"id": true},"order": "timestamp DESC","limit":1};
                const url4 = process.env.URL + '/api/estadisticas?filter=' + encodeURIComponent(JSON.stringify(selectFilter4));
                axios({
                     method: 'get',
                     url: url4,
                     headers: {'Accept': 'application/json'},
                 }).then(function(response4){

                     let idest = response4.data[0]['id'];
                     let dataEstadistico={}
                     
                     dataEstadistico['totalobstaculos']=totalObstaculos;
                     dataEstadistico ['tiempopromedioregreso']  =promedioRegreso;                     

                     
                     axios({
                         method: 'patch',
                         url: process.env.URL + '/api/estadisticas/'+idest,
                         headers: {'Content-Type': 'application/json', 'Accept': 'application/json'},
                         data:dataEstadistico
                     }).then(function(response5){

                            console.log(dataEstadistico);
                              res.status(200).json({ result: 'success' });

                     }).catch(function(error){
                         next(error);
                     });


                 }).catch(function(error){
                     next(error);
                 })

            }).catch(function (error){
                next(error);
            });
       
        }).catch(function (error) {

            next(error);
        });
});






module.exports = router;
