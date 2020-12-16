const express = require('express')
const app = express()
const request = require('request')

const bodyParser = require('body-parser')
app.use(bodyParser.urlencoded({extended: false}))
app.use(bodyParser.json())
/*
const mongo = require('./mongo')
const UserModel = require('./mongo')

app.post('/login', async (req, res) => {
    let status = await UserModel.find(req.body)
    console.log(status)
    if(!status.length) res.send("用户不存在或密码错误")
    else res.send("登陆成功")
    console.log(status)
    res.end()
}) //登录

app.post('/register',  async (req, res) => {
    let status = await UserModel.find(req.body)
    if(status.length) res.send("账号已存在")
    else {
        await UserModel.add(req.body)
        console.log("注册成功")
        res.send("注册成功")
    }
    res.end()
}) //注册

app.post('/update', async (req, res) => {
    await UserModel.update(req.body)
    console.log("更新成功")
    res.send("更新成功")
    res.end()
}) //更新偏好设置
*/
app.get('/query', (req, res) => {
    res.send('fuck')
}) //获取天气数据

app.get('/control', (req, res) => {
    console.log(req.query.value)
    let url = 'http://api.heclouds.com/devices/643814844/datapoints'
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
              value: req.query.value
            }]
          }]
        },
        method: 'POST',
        json : true
      }, (err, res, body) => {
        //console.log(body)
      })
})

app.listen(3000)