var express = require('express');
var router = express.Router();
const axios = require('axios');
var moment = require('moment');

router.post('/safebox/llegada',function(req,res,netxt){
    const safeboxData = {
        temperatura: req.body.temperatura,
        horallegada: new Date().toISOString(),
        acceso:0
    };
    if(req.body.temperatura<=37.5){
        safeboxData['acceso']=1;
    }
    axios({
        method: 'post',
        url: process.env.URL + '/api/safeboxes',
        headers: {'Content-Type': 'application/json', 'Accept': 'application/json'},
        data: safeboxData
    }).then(function(response){
        res.status(200).json({ result: 'success' });
    }).catch(function (error) {
        next(error);
    });
    

});

router.get('/safebox/dashboard',function(req,res,netxt){
    
    let dashboardData ={   };
 

    axios({
        method: 'get',
        url: process.env.URL + '/api/safeboxes/count',
        headers: {'Content-Type': 'application/json', 'Accept': 'application/json'},
    }).then(function(response){
        dashboardData['contador'] = response.data['count'];
        const selectFilter1 = {"fields": {"acceso": true}, "where": {"acceso": {"gte": "1"}}};
        const url1 = process.env.URL + '/api/safeboxes?filter=' + encodeURIComponent(JSON.stringify(selectFilter1));
        return axios({
            method:'get',
            url: url1,
            headers: {'Accept': 'application/json'},
        })
    }).then(function(response){

        dashboardData['entradas'] = response.data.length;

        const selectFilter2 = {"fields": {"acceso": true}, "where": {"acceso": {"lte": "0"}}};
        const url2 = process.env.URL + '/api/safeboxes?filter=' + encodeURIComponent(JSON.stringify(selectFilter2));
        return axios({
            method:'get',
            url: url2,
            headers: {'Accept': 'application/json'},
        })
    }).then(function(response){
        dashboardData['denegados'] = response.data.length;
        const selectFilter3 = {"fields": {"temperatura": true,"order": "timestamp DESC"}};
        const url3 = process.env.URL + '/api/safeboxes?filter=' + encodeURIComponent(JSON.stringify(selectFilter3));
        return axios({
            method:'get',
            url: url3,
            headers: {'Accept': 'application/json'},
        })
    }).then(function(response){
        console.log(response.data);
       let total = response.data.reduce(function(tot,element){
            return tot+element.temperatura;
       },0);
       console.log(total);
       dashboardData['promedio']= total/ dashboardData['contador'];
        res.status(200).json(dashboardData);
    }).catch(function (error) {
        next(error);
        console('error');
    });
    

});
module.exports = router;