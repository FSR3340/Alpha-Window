var request = require('request')
var url = 'http://api.heclouds.com/devices/643814844/datapoints'

request({
  url : url,
  headers: {
    "content-type": "application/json",
    'api-key': '1A75=FYuV1f9edF=M1yoXPwYQgE='
  },
  body: {
    datastreams: [{
      id: 'Switch',
      datapoints: [{
        value: 1 
      }]
    }]
  },
  method: 'POST',
  json : true
}, (err, res, body) => {
  console.log(body)
})