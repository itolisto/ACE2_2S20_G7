var express = require('express');
var path = require('path');
var bitacora = require('./routes/bitacora');
var liquido = require('./routes/liquido');
var viaje = require('./routes/viaje');
var vehiculo = require('./routes/vehiculo');
var estadistica = require('./routes/estadistica');
var graph = require('./routes/graph');
var safebox = require('./routes/safebox');
var app = express();
const requestIp = require('request-ip');
const bodyParser = require('body-parser');
var cors = require('cors');

// Enable CORS
app.use(cors());

// set path for static assets
app.use(express.static(path.join(__dirname, 'public')));

// parse application/x-www-form-urlencoded
app.use(bodyParser.urlencoded({ extended: false }))

// Handles post requests
app.use(bodyParser.json());

// Add IP middleware
app.use(requestIp.mw());

// routes <agregar o cambiar dependiendo de tu modelo>
app.use('/', liquido);
app.use('/', bitacora);
app.use('/', viaje);
app.use('/', vehiculo);
app.use('/', estadistica);
app.use('/', graph);
app.use('/',safebox);

// catch 404 and forward to error handler
app.use(function(req, res, next) {
  var err = new Error('Not Found');
  err.status = 404;
  next(err);
});

// error handler
app.use(function(err, req, res, next) {
  // render the error page
  res.status(err.status || 500).send({status:err.status, message:err.message});
});

module.exports = app;
