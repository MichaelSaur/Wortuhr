$(document).ready(function () {
  $('switch').on('click', function (event) {
    switchClick(event.target)
  })
  $('.slider').on('input', function (event) {
    changeValue(event.target)
  })

  // daymode
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

  // Nightmode
  if($("switch")[0].attributes.checked.value == 'true'){
    $(".OnNightModeActive").removeClass("hidden")
  }else{
    $(".OnNightModeActive").addClass("hidden")
  }
  $("#nightModeActive").on('click', function (event) {
    if (event.target.attributes.checked.value == 'true') {
      $(".OnNightModeActive").removeClass("hidden")
      $("#nightModeActiveInt").val(1)
    }else {
      $(".OnNightModeActive").addClass("hidden")
      $("#nightModeActiveInt").val(0)
    }
  })

  var beginHour = $('#nightModeBeginHourField').val()
  $('#nightModeBeginHour').val(beginHour)
  var beginMinute = $('#nightModeBeginMinuteField').val()
  $('#nightModeBeginMinute').val(beginMinute)
  var endHour = $('#nightModeEndHourField').val()
  $('#nightModeEndHour').val(endHour)
  var endMinute = $('#nightModeEndMinuteField').val()
  $('#nightModeEndMinute').val(endMinute)

  colorR = $('#colorNightR').val()
  colorG = $('#colorNightG').val()
  colorB = $('#colorNightB').val()
  colorStringNight = 'rgb(' + colorR + ',' + colorG + ',' + colorB + ')'
  var colorPickerNight = new iro.ColorPicker('#pickerNight', {
    width: 200,
    color: colorStringNight
  })
  var design = $('#designNightField').val()
  console.log(design)
  $('#designNight').val(design)
  $('#colorNightR').val(colorPickerNight.color.rgb.r)
  $('#colorNightG').val(colorPickerNight.color.rgb.g)
  $('#colorNightB').val(colorPickerNight.color.rgb.b)

  colorPickerNight.on('color:change', function (color) {
    console.log(color.rgb)
    $('#colorNightR').val(color.rgb.r)
    $('#colorNightG').val(color.rgb.g)
    $('#colorNightB').val(color.rgb.b)
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
