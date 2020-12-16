//index.js
const app = getApp()

Page({
  data: {
    switch1Checked: true,
    switch2Checked: false,
    switch1Style: '',
    switch2Style: '',
  },

  switch1Change:function (e) {
    console.log(`switch1发生change事件，携带值为`, e.detail.value)
    var obj = {}
    obj[`switch1Checked`] = e.detail.value
    this.setData(obj)
    obj = {}
    obj[`switch1Style`] = e.detail.value ? '' : 'text-decoration: line-through'
    this.setData(obj)
  },

  switch2Change:function (e) {
    console.log(`switch2发生change事件，携带值为`, e.detail.value)
    var obj = {}
    obj[`switch2Checked`] = e.detail.value
    this.setData(obj)
    obj = {}
    obj[`switch2Style`] = e.detail.value ? '' : 'text-decoration: line-through'
    this.setData(obj)
  },

  


})
