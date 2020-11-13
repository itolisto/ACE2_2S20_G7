var express = require('express');
var router = express.Router();
const axios = require('axios');

router.get('/dashboard', function (req, res, next) {


    const selectFilter = {
        "fields": {
            "id": true,
            "idviaje": true,
            "totalobstaculos": true,
            "pesopromedio": true,
            "tiempopromedioida": true,
            "tiempopromedioregreso": true,
            "timestamp": true
        },
        "order": "timestamp DESC",
        "limit": 1
    };
    const url = process.env.URL + '/api/estadisticas?filter=' + encodeURIComponent(JSON.stringify(selectFilter));
    let dashboardData = {}
    axios({
            method: 'get',
            url: url,
            headers: {
                'Accept': 'application/json'
            },
        })
    .then(function (response) {
        dashboardData['peso_promedio'] = response.data[0]['pesopromedio'];
        dashboardData['tiempo_promedio_ida'] = response.data[0]['tiempopromedioida'];
        dashboardData['tiempo_promedio_regreso'] = response.data[0]['tiempopromedioregreso'];
        dashboardData['totalobstaculos'] = response.data[0]['totalobstaculos'];
        // handle success
        const selectFilter2 = {
            "fields": {
                "id": true,
                "estado": true,
                "ubicacion": true
            },
            "order": "timestamp DESC",
            "limit": 1
        };
        const url2 = process.env.URL + '/api/detalleviajes?filter=' + encodeURIComponent(JSON.stringify(selectFilter2));

        return axios({
            method: 'get',
            url: url2,
            headers: {
                'Content-Type': 'application/json',
                'Accept': 'application/json'
            }
        })
    }).then(function (response) {
        dashboardData['ubicacion'] = response.data[0]['ubicacion'];
        dashboardData['estado'] = response.data[0]['estado'];
        return axios({
            method: 'get',
            url: process.env.URL + '/api/viajes/count',
            headers: {
                'Content-Type': 'application/json',
                'Accept': 'application/json'
            }
        })
    }).then(function (response) {

        dashboardData['total_contador'] = response.data['count']
        res.status(200).json(dashboardData);

    }).catch(function (error) {
            next(error);
    });
});


module.exports = router;