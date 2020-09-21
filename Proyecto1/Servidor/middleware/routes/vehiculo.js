var express = require('express');
var router = express.Router();
const axios = require('axios');

router.post('/vehiculo/progreso', function(req, res, next) {
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
        vehiculoData['idviaje']= response.data[0]['id'];
        return axios({
            method: 'post',
            url: process.env.URL + '/api/detalleviajes',
            headers: {'Content-Type': 'application/json', 'Accept': 'application/json'},
            data: vehiculoData
        });
    }).then(function () {
        res.status(200).json({ result: 'success' });
    }).catch(function (error) {
        next(error);
    }); 
       
  
});


router.post('/vehiculo/modo', function(req, res, next) {
    const selectFilter = {"fields": {"id": true},"order": "timestamp DESC","limit":1};
    const url = process.env.URL + '/api/viajes?filter=' + encodeURIComponent(JSON.stringify(selectFilter));
  
    axios({
        method: 'get',
        url: url,
        headers: {'Accept': 'application/json'},
    }).then(function (response) {
    
        let viajeData={
            modo:req.body.modo
        };
        return axios({
            method: 'patch',
            url: process.env.URL + '/api/viajes/'+response.data[0]['id'],
            headers: {'Content-Type': 'application/json', 'Accept': 'application/json'},
            data:viajeData
        });
    }).catch(function (error) {
        next(error);
    }); ;

});
module.exports = router;
