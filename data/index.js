$(document).ready(function () {
  $('switch').on('click', function (event) {
    switchClick(event.target)
  })
  $('.slider').on('input', function (event) {
    changeValue(event.target)
  })

  colorR = $('#colorR').val()
  colorG = $('#colorG').val()
  colorB = $('#colorB').val()
  colorString = 'rgb(' + colorR + ',' + colorG + ',' + colorB + ')'
  var colorPicker = new iro.ColorPicker('#picker', {
    width: 200,
    color: colorString
  })
  var design = $('#designField').val()
  console.log(design)
  $('#design').val(design)
  $('#colorR').val(colorPicker.color.rgb.r)
  $('#colorG').val(colorPicker.color.rgb.g)
  $('#colorB').val(colorPicker.color.rgb.b)

  colorPicker.on('color:change', function (color) {
    console.log(color.rgb)
    $('#colorR').val(color.rgb.r)
    $('#colorG').val(color.rgb.g)
    $('#colorB').val(color.rgb.b)
  })

  window.setInterval(()=>{
    d = new Date();
    $("#timeNow").html(d.toLocaleTimeString());
    $("#timeUnix").val(parseInt(d.getTime()/1000)-d.getTimezoneOffset()*60);
  },1000)

  $('.toggle-password').click(function () {
    $(this).toggleClass('fa-eye fa-eye-slash')
    var input = $($(this).attr('toggle'))
    if (input.attr('type') == 'password') {
      input.attr('type', 'text')
    } else {
      input.attr('type', 'password')
    }
  })
})
function switchClick (elem) {
  if (elem.attributes.checked.value == 'true') {
    elem.attributes.checked.value = 'false'
  }else {
    elem.attributes.checked.value = 'true'
  }
}
function changeValue (elem) {
  $('.slidervalue .' + elem.id).html(elem.value)
  console.log($('.slidervalue .' + elem.id))
}
