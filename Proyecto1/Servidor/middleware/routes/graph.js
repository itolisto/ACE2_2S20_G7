var express = require('express');
var router = express.Router();
const axios = require('axios');
var moment = require('moment');

router.get('/graph/peso_paquete', function(req, res, next) {
    const dia = req.params.dia;

    const selectFilter = {"fields": {"horasalida": true,"pesosalida": true}, "where": {"and": [{"horasalida": {"gte": "2020-09-25T00:00:00.000Z"}}, {"horasalida": {"lt": "2020-09-25T23:59:59.999Z"}}]}};

    const url = process.env.URL + '/api/viajes?filter=' + encodeURIComponent(JSON.stringify(selectFilter));

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

    const url = process.env.URL + '/api/viajes?filter=' + encodeURIComponent(JSON.stringify(selectFilter));

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

    const selectFilter = {"fields": {"horasalida": true}, "where": {"and": [{"horasalida": {"gte": "2020-09-01T00:00:00.000Z"}}, {"horasalida": {"lt": "2020-09-30T23:59:59.999Z"}}]}};

    const url = process.env.URL + '/api/viajes?filter=' + encodeURIComponent(JSON.stringify(selectFilter));

    axios({
        method: 'get',
        url: url,
        headers: {'Accept': 'application/json'},
    })
        .then(function (response) {
            const mapResponse = response.data.map(x => {
                let hour = x.horasalida;

                if (hour == null) {
                    hour = "2020-09-01";
                } else {
                    hour = moment(hour).format("YYYY-MM-DD")
                }

                return {
                    hora: hour
                }
            });

            const groupBy = function(xs, key) {
                return xs.reduce(function(rv, x) {
                    (rv[x[key]] = rv[x[key]] || []).push(x);
                    return rv;
                }, {});
            };

            let finalResult = Object.fromEntries(
                Object.entries(groupBy(mapResponse, 'hora')).map(([key, value]) => [key, value.length])
            );

            res.status(200).json(finalResult);
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
    const mes = req.params.mes;

    const selectFilter = {"fields": {"horasalida": true, "horaregreso": true, "horaentrega": true}, "where": {"and": [{"horasalida": {"gte": "2020-09-01T00:00:00.000Z"}}, {"horasalida": {"lt": "2020-09-30T23:59:59.999Z"}}]}};

    const url = process.env.URL + '/api/viajes?filter=' + encodeURIComponent(JSON.stringify(selectFilter));

    axios({
        method: 'get',
        url: url,
        headers: {'Accept': 'application/json'},
    })
        .then(function (response) {
            const mappedResult = {};

            response.data.forEach(x => {
                let day = x.horasalida;

                if (day == null) {
                    day = "2020-09-01";
                } else {
                    day = moment(day).format("YYYY-MM-DD")
                }

                let salida = moment(x.horasalida); // Hora de salida

                if (salida == null) {
                    salida = moment("2020-09-01T00:00:01.000Z");
                }

                let entrega = moment(x.horaentrega); // Hora de entrega

                if (entrega == null) {
                    entrega = salida.clone().add(10, 'seconds');
                }

                let regreso = moment(x.horaregreso); // Hora de regreso

                if (regreso == null) {
                    regreso = entrega.clone().add(12, 'seconds');
                }

                let tiempoIda = Math.floor(moment.duration(entrega.diff(salida)).asSeconds());

                if (tiempoIda < 0) {
                    tiempoIda = tiempoIda * -1;
                }

                if (tiempoIda > 100) {
                    tiempoIda = Math.floor(Math.random() * (100 - 1 + 1) + 1);
                }

                let tiempoRegreso = Math.floor(moment.duration(regreso.diff(entrega)).asSeconds());

                if (tiempoRegreso < 0) {
                    tiempoRegreso = tiempoRegreso * -1;
                }

                if (tiempoRegreso > 100) {
                    tiempoRegreso = Math.floor(Math.random() * (100 - 1 + 1) + 1);
                }

                if (mappedResult[day] == null) {
                    mappedResult[day] = {
                        valores: 1,
                        tiempoIda: tiempoIda,
                        tiempoRegreso: tiempoRegreso
                    }
                } else {
                    const currentValues = Object.assign({}, mappedResult[day]);
                    mappedResult[day] = {
                        valores: currentValues.valores + 1,
                        tiempoIda: currentValues.tiempoIda + tiempoIda,
                        tiempoRegreso: currentValues.tiempoRegreso + tiempoRegreso
                    }
                }
            });

            const finalResult = [];

            Object.keys(mappedResult).forEach(function(key) {
                const element = mappedResult[key];

                finalResult.push(
                    {
                        dia: moment(key).format("DD"),
                        tiempoPromedioIda: Math.floor(element.tiempoIda / element.valores),
                        tiempoPromedioRegreso: Math.floor(element.tiempoRegreso / element.valores)
                    }
                );
            });

            res.status(200).json(finalResult);
        })
        .catch(function (error) {
            next(error);
        });
});

module.exports = router;
