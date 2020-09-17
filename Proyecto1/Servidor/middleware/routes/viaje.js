var express = require('express');
var router = express.Router();
const axios = require('axios');
const viaje = require('../../rest-api-loopback/server/models/viaje');
var moment = require('moment');

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
   
        let dataEstadista= {};
        dataEstadista['idviaje']=response['data']['id'];
        axios({ 
            method:'post',
            url:process.env.URL+'/api/estadisticas',
            headers:{'Content-Type': 'application/json', 'Accept': 'application/json'},
            data:dataEstadista
        })
        .then(function(response){
                //debo a?adir un detalle viaje que va camino al cliente.
            res.status(200).json({ result: 'success' });
        })
        .catch(function(error){
            next(error);
        });

         
    })
    .catch(function (error) {
        console.log(viajeData);
 
        next(error);
    });

});

router.post('/envio/entrega', function(req, res, next) {
    let viajeData = {};
        viajeData['pesoentrega'] = req.body.peso;
        viajeData['obstaculoentrega'] = req.body.obstaculos;
        viajeData['horaentrega']= new Date().toISOString();

      //  viajeData['horaentrega']='now';
        const selectFilter = {"fields": {"id": true,"horasalida":true},"order": "timestamp DESC","limit":1};
        const url = process.env.URL + '/api/viajes?filter=' + encodeURIComponent(JSON.stringify(selectFilter));
        axios({
            method: 'get',
            url: url,
            headers: {'Accept': 'application/json'},
        })
        .then(function (response) {
            // handle success
            let id = response.data[0]['id'];
      
            const  horasalida = moment(response.data[0]['horasalida']);
            const horaentrega = moment(viajeData['horaentrega']);
            const tiempoProm = horaentrega-horasalida;
   

            
             //res.status(200).json(response.data[0]);

             axios({
                method: 'patch',
                url: process.env.URL + '/api/viajes/'+id,
                headers: {'Content-Type': 'application/json', 'Accept': 'application/json'},
                data:viajeData
            }).then(function(response){
                    /**
                        * dentro del patch para estadistica se debe enviar pesopromedio,tiempopromedioida
                    */
                   const selectFilter4 = {"fields": {"id": true},"order": "timestamp DESC","limit":1};
                   const url4 = process.env.URL + '/api/estadisticas?filter=' + encodeURIComponent(JSON.stringify(selectFilter4));
                   axios({
                        method: 'get',
                        url: url4,
                        headers: {'Accept': 'application/json'},
                    }).then(function(response4){

                        let idest = response4.data[0]['id'];
                        let dataEstadistico={}
                        
                        dataEstadistico['pesopromedio']=viajeData['pesoentrega'];
                        dataEstadistico ['tiempopromedioida']  =tiempoProm;                     

                        
                        axios({
                            method: 'patch',
                            url: process.env.URL + '/api/estadisticas/'+idest,
                            headers: {'Content-Type': 'application/json', 'Accept': 'application/json'},
                            data:dataEstadistico
                        }).then(function(response5){

  
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
