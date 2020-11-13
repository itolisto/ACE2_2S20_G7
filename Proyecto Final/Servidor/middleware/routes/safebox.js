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
module.exports = router;