var express = require('express');
var router = express.Router();
const axios = require('axios');
var moment = require('moment');

router.get('/graph/peso_paquete', function(req, res, next) {
    const dia = req.params.dia;

    const selectFilter = {"fields": {"horasalida": true,"pesosalida": true}, "where": {"and": [{"horasalida": {"gte": "2020-09-25T00:00:00.000Z"}}, {"horasalida": {"lt": "2020-09-25T23:59:59.999Z"}}]}};

    const url = process.env.URL + 'http://localhost:3000/api/viajes?filter=' + encodeURIComponent(JSON.stringify(selectFilter));

    axios({
        method: 'get',
        url: url,
        headers: {'Accept': 'application/json'},
    })
        .then(function (response) {
            const mapResponse = response.data.map(x => {
                let weight = x.pesosalida;

                if (weight == null) {
                    weight = 0;
                }

                let hour = x.horasalida;

                if (hour == null) {
                    hour = "00:00";
                } else {
                    hour = moment(hour).format("hh:mm")
                }

                return {
                    peso: weight,
                    hora: hour
                }
            });

            res.status(200).json(mapResponse);
        })
        .catch(function (error) {
            next(error);
        });
});

router.get('/graph/peso_paquete_2', function(req, res, next) {
    const dia = req.params.dia;

    const selectFilter = {"fields": {"horasalida": true,"pesosalida": true}, "where": {"and": [{"horasalida": {"gte": "2020-09-25T00:00:00.000Z"}}, {"horasalida": {"lt": "2020-09-25T23:59:59.999Z"}}]}};

    const url = process.env.URL + 'http://localhost:3000/api/viajes?filter=' + encodeURIComponent(JSON.stringify(selectFilter));

    axios({
        method: 'get',
        url: url,
        headers: {'Accept': 'application/json'},
    })
        .then(function (response) {
            const mapResponse = response.data.map(x => {
                let weight = x.pesosalida;

                if (weight == null) {
                    weight = 0;
                }

                let hour = x.horasalida;

                if (hour == null) {
                    hour = "00";
                } else {
                    hour = moment(hour).format("hh")
                }

                return {
                    peso: weight,
                    hora: hour
                }
            });

            res.status(200).json(mapResponse);
        })
        .catch(function (error) {
            next(error);
        });
});

router.get('/graph/paquete_entrega', function(req, res, next) {
    const dia = req.params.dia;

    const selectFilter = {"fields": {"horasalida": true,"pesosalida": true}, "where": {"and": [{"horasalida": {"gte": "2020-09-25T00:00:00.000Z"}}, {"horasalida": {"lt": "2020-09-25T23:59:59.999Z"}}]}};

    const url = process.env.URL + 'http://localhost:3000/api/viajes?filter=' + encodeURIComponent(JSON.stringify(selectFilter));

    axios({
        method: 'get',
        url: url,
        headers: {'Accept': 'application/json'},
    })
        .then(function (response) {
            const mapResponse = response.data.map(x => {
                let weight = x.pesosalida;

                if (weight == null) {
                    weight = 0;
                }

                let hour = x.horasalida;

                if (hour == null) {
                    hour = "00";
                } else {
                    hour = moment(hour).format("hh")
                }

                return {
                    peso: weight,
                    hora: hour
                }
            });

            res.status(200).json(mapResponse);
        })
        .catch(function (error) {
            next(error);
        });
});

router.get('/graph/obstaculo_encontrado', function(req, res, next) {

});

router.get('/graph/tiempo_ida_vuelta', function(req, res, next) {

});

router.get('/graph/tiempo_promedio', function(req, res, next) {
    
});

module.exports = router;
