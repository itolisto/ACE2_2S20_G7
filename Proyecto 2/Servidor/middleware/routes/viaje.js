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
    var horaparse = moment(viajeData['horasalida']);
    const message = {
        notification: {
            title: 'Buzon inteligente',
            body: 'saliendo \npeso:'+viajeData['pesosalida']+' \nhora:'+horaparse.format('MMMM Do YYYY, h:mm:ss a')
        },
        topic: 'grupoVehiculo'
    };

    admin.messaging().send(message)
    .then((response) => {
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
    var horaparse = moment(viajeData['horaentrega']);

    const selectFilter = {"fields": {"id": true,"horasalida":true},"order": "timestamp DESC","limit":1};
    const url = process.env.URL + '/api/viajes?filter=' + encodeURIComponent(JSON.stringify(selectFilter));
    
    const message = {
        notification: {
            title: 'Buzon inteligente',
            body: 'entrega \npeso:'+viajeData['pesoentrega']+' \nhora:'+horaparse.format('MMMM Do YYYY, h:mm:ss a')+'\nobstaculos:'+viajeData['obstaculoentrega']
        },
        topic: 'grupoVehiculo'
    };
    admin.messaging().send(message)
    .then((response) => {
        console.log('Successfully sent message:', response);
    return axios({
        method: 'get',
        url: url,
        headers: {'Accept': 'application/json'},
    });
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
    let viajeData = {
        obstaculoregreso:req.body.obstaculos,
        horaregreso:new Date().toISOString(),
        estadoviaje:'finalizado'
    };
    const selectFilter = {"fields": {"id": true,'horaentrega':true,'obstaculoentrega':true},"order": "timestamp DESC","limit":1};
    const url = process.env.URL + '/api/viajes?filter=' + encodeURIComponent(JSON.stringify(selectFilter));
    var horaparse = moment(viajeData['horaregreso']);
    const message = {
        notification: {
            title: 'Buzon inteligente',
            body: 'regreso \nhora:'+horaparse.format('MMMM Do YYYY, h:mm:ss a')+'\nobstaculos:'+viajeData['obstaculoregreso']
        },
        topic: 'grupoVehiculo'
    };
    let dataEstadistico ={};
    admin.messaging().send(message)
    .then((response) => {
        console.log('Successfully sent message:', response);
        return axios({
            method: 'get',
            url: url,
            headers: {'Accept': 'application/json'},
        });
    }).then(function (response) {
        dataEstadistico={
            totalobstaculos:viajeData['obstaculoregreso']+response.data[0]['obstaculoentrega'],
            tiempopromedioregreso: moment( viajeData['horaregreso'])-moment(response.data[0]['horaentrega'])
        }
        // handle success
        return axios({
            method: 'patch',
            url: process.env.URL + '/api/viajes/'+response.data[0]['id'],
            headers: {'Content-Type': 'application/json', 'Accept': 'application/json'},
            data:viajeData
        });
    }).then(function(){

      
        const selectFilter4 = {"fields": {"id": true},"order": "timestamp DESC","limit":1};
        const url4 = process.env.URL + '/api/estadisticas?filter=' + encodeURIComponent(JSON.stringify(selectFilter4));


        return axios({
            method: 'get',
            url: url4,
            headers: {'Accept': 'application/json'},
        });
    }).then(function(response4){
     
    
        return axios({
            method: 'patch',
            url: process.env.URL + '/api/estadisticas/'+ response4.data[0]['id'],
            headers: {'Content-Type': 'application/json', 'Accept': 'application/json'},
            data:dataEstadistico
        });              
    }).then(function(){
        res.status(200).json({ result: 'success' });
    }).catch(function (error) {
        next(error);
    });
});
module.exports = router;
