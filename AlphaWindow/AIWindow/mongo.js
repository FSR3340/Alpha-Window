const mongoose = require('mongoose')
var Schema = mongoose.Schema
mongoose.connect('mongodb://127.0.0.1:27017/data', {useNewUrlParser: true, useUnifiedTopology: true})
var db = mongoose.connection

var userSchema = new Schema({
    username: String,
    password: String
})

class UserModel {
    constructor() {
        this.UserModel = db.model('users', userSchema)
    }
    add(data) {
        let users = new this.UserModel(data)
        console.log("HHHH")
        console.log(data)
        return users.save()
    }
    find(data) {
        return this.UserModel.find(data)
    }
    update(data) {
        return this.UserModel.update({username: data.username}, data)
    }
}

module.exports = new UserModel()