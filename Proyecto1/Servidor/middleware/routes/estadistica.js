var express = require('express');
var router = express.Router();
const axios = require('axios');

router.get('/dashboard', function(req, res, next) {

    let dashboardData={}
    const selectFilter = {"fields": {"id": true,"idviaje":true,"totalobstaculos":true,"pesopromedio":true,"tiempopromedioida":true,"tiempopromedioregreso":true,"timestamp":true},"order": "timestamp DESC","limit":1};
    const url = process.env.URL + '/api/estadisticas?filter=' + encodeURIComponent(JSON.stringify(selectFilter));
    axios({
        method: 'get',
        url: url,
        headers: {'Accept': 'application/json'},
    })
    .then(function (response) {
        // handle success
        const selectFilter2 = {"fields": {"id": true,"estado":true,"ubicacion":true},"order": "timestamp DESC","limit":1};
        const url2 = process.env.URL + '/api/detalleviajes?filter=' + encodeURIComponent(JSON.stringify(selectFilter2));

        axios({
            method: 'get',
            url: url2,
            headers: {'Content-Type': 'application/json', 'Accept': 'application/json'}
        })
        .then(function (response2) {
           //  res.status(200).json({ result: 'success' });
          

            axios({
                method: 'get',
                url: process.env.URL +'/api/viajes/count',
                headers: {'Content-Type': 'application/json', 'Accept': 'application/json'}
            })
            .then(function (response3) {
                console.log(response3.data);
          
                dashboardData['ubicacion']=response2.data[0]['ubicacion']; //obtenerla detalle viaje
                dashboardData['estado']=response2.data[0]['estado'];//obtenerla detalle viaje
                dashboardData['total_contador']= response3.data['count']; //obtenerla desde loopback
                dashboardData['total_obstaculos']=response.data[0]['totalobstaculos'];
                dashboardData['peso_promedio']=response.data[0]['pesopromedio'];
                dashboardData['tiempo_promedio_ida']=response.data[0]['tiempopromedioida'];
                dashboardData['tiempo_promedio_regreso']=response.data[0]['tiempopromedioregreso'];
                res.status(200).json(dashboardData);

            })
            .catch(function (error3) {
                console.log(dashboardData);
                next(error3);
            });

        })
        .catch(function (error2) {
            console.log(dashboardData);
            //console.log(error);
            next(error2);
        });
   
    }).catch(function (error) {

        next(error);
    }); 
});


module.exports = router;