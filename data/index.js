var clockTime = 0;
function updateTime() {
    if(clockTime == 0){
        document.getElementById("systemTime").textContent = "loading..."
    }else{
        clockTime = clockTime+1000;
    const clock = new Date(clockTime);
    const now = new Date();
    document.getElementById("clockTime").textContent =
        clock.toLocaleTimeString("de-DE");
    document.getElementById("systemTime").textContent =
        now.toLocaleTimeString("de-DE");
    }
}
setInterval(updateTime, 1000);
updateTime();

function syncTime() {
    alert("Zeit wurde mit dem Endgerät synchronisiert.");
}

// Haupt-Helligkeit
const brightnessSlider = document.getElementById("brightness");
const brightnessValue = document.getElementById("brightnessValue");
// Nacht-Helligkeit
const nightModeToggle = document.getElementById("nightModeToggle");
const nightBrightnessSlider = document.getElementById("nightBrightness");
const nightBrightnessValue = document.getElementById("nightBrightnessValue");

var colorPicker = new iro.ColorPicker();
var nightColorPicker = new iro.ColorPicker();

$("document").ready(()=>{
    $.get("api/config",function(data, status){
        if(status=="success"){
            if(!(typeof x === 'object' && !Array.isArray(x) && x !== null)){
                data = JSON.parse(data);
                window.config = data;
            }
            console.log(data)
            clockTime = data.time*1000
            
            // iro.js Color Picker – Hauptfarbe
            colorString = 'rgb(' + data.baseColor.r + ',' + data.baseColor.g + ',' + data.baseColor.b + ')'
            colorPicker = new iro.ColorPicker("#colorPicker", {
                width: 250,
                color: colorString,
                layout: [
                    { component: iro.ui.Wheel },
                    { component: iro.ui.Slider, options: { sliderType: "value" } }
                ]
            });
            colorPicker.on("color:change", function(color) {
                console.log("Primärfarbe:", color.hexString);
                 // make edit actions available
                const block = document.getElementById("ColorEdit");
                block.classList.remove("opacity-50", "pointer-events-none");
            });

            // Mode
            var modes = []
            data.modes.forEach(element => {
                if(element == data.baseColor.mode){
                    modes.push("<option selected>"+element+"</option>");
                }else{
                    modes.push("<option>"+element+"</option>");
                }
            });
            document.getElementById("colorMode").innerHTML = modes;

            brightnessSlider.value = data.baseColor.brightness;
            brightnessValue.textContent = data.baseColor.brightness;

            ///////
            // Nightmode
            ///////
            nightModeToggle.checked = data.nightMode.enabled;
            const block = document.getElementById("nightSettings");
            if (data.nightMode.enabled) {
                block.classList.remove("opacity-50", "pointer-events-none");
            } else {
                block.classList.add("opacity-50", "pointer-events-none");
            }

            // Nachtmodus aktivieren/deaktivieren
            nightModeToggle.addEventListener("change", function() {
                const block = document.getElementById("nightSettings");
                if (this.checked) {
                    block.classList.remove("opacity-50", "pointer-events-none");
                } else {
                    block.classList.add("opacity-50", "pointer-events-none");
                }
                // make edit actions available
                const block2 = document.getElementById("NightModeEdit");
                block2.classList.remove("opacity-50", "pointer-events-none");
            });   

            document.getElementById("nightStart").value = data.nightMode.startH.toString().padStart(2, "0") + ":" + data.nightMode.startM.toString().padStart(2, "0");
            document.getElementById("nightEnd").value = data.nightMode.endH.toString().padStart(2, "0") + ":" + data.nightMode.endM.toString().padStart(2, "0");

            colorString = 'rgb(' + data.nightMode.baseColor.r + ',' + data.nightMode.baseColor.g + ',' + data.nightMode.baseColor.b + ')'
            nightColorPicker = new iro.ColorPicker("#nightColorPicker", {
                width: 250,
                color: colorString,
                layout: [
                    { component: iro.ui.Wheel },
                    { component: iro.ui.Slider, options: { sliderType: "value" } }
                ]
            });
            nightColorPicker.on("color:change", function(color) {
                console.log("Nachtfarbe:", color.hexString);
                 // make edit actions available
                const block = document.getElementById("NightModeEdit");
                block.classList.remove("opacity-50", "pointer-events-none");
            });

            // Mode
            var modes = []
            data.modes.forEach(element => {
                if(element == data.nightMode.baseColor.mode){
                    modes.push("<option selected>"+element+"</option>");
                }else{
                    modes.push("<option>"+element+"</option>");
                }
            });
            document.getElementById("nightMode").innerHTML = modes;

            nightBrightnessSlider.value =data.nightMode.baseColor.brightness;
            nightBrightnessValue.textContent = data.nightMode.baseColor.brightness;

            // WiFi
            var ssids = []
            data.wiFiSSIDs.forEach(element => {
                if(element == data.WiFi){
                    ssids.push("<option selected>"+element+"</option>");
                }else{
                    ssids.push("<option>"+element+"</option>");
                }
            });
            document.getElementById("wifiSSID").innerHTML = ssids;
            document.getElementById("wifiPassword").value = data.password;
        }
    });
})

// Passwort anzeigen/verbergen
document.getElementById("togglePassword").addEventListener("click", function () {
    const input = document.getElementById("wifiPassword");
    if (input.type === "password") {
        input.type = "text";
        this.innerHTML = "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"512\" height=\"512\" viewBox=\"0 0 512 512\" version=\"1.1\" style=\"height:2em;width:2em\"><path d=\"M 58.500 185.888 C 51.101 187.806, 44.922 192.866, 41.879 199.500 C 39.671 204.312, 39.398 214.624, 41.354 219.305 C 43.307 223.979, 51.507 233.699, 65.563 248 L 77.849 260.500 61.871 288 C 39.498 326.506, 39.647 326.198, 39.643 334 C 39.636 348.296, 49.394 357.928, 63.934 357.978 C 71.180 358.003, 76.948 355.762, 81.242 351.253 C 83.204 349.193, 106.123 310.634, 116.568 291.821 C 117.395 290.331, 118.399 290.657, 125.500 294.721 C 135.653 300.532, 156.049 309.820, 166.750 313.505 C 171.287 315.068, 175 316.768, 175 317.283 C 175 317.798, 172.750 330.990, 170 346.598 C 167.250 362.206, 165 377.246, 165 380.020 C 165 391.528, 173.456 401.269, 185.315 403.422 C 194.846 405.153, 206.805 399.230, 210.830 390.785 C 212.078 388.167, 214.842 375.307, 217.936 357.719 C 220.721 341.890, 223 328.615, 223 328.219 C 223 327.824, 237.763 327.500, 255.807 327.500 L 288.614 327.500 293.839 357.500 C 299.621 390.689, 300.693 393.737, 308.524 399.225 C 320.919 407.912, 338.249 403.251, 344.649 389.507 C 345.931 386.753, 346.985 382.598, 346.990 380.273 C 346.996 377.948, 344.750 363.336, 342 347.802 C 339.250 332.267, 337.007 318.870, 337.015 318.029 C 337.025 316.997, 340.842 315.081, 348.765 312.133 C 361.449 307.412, 376.363 300.508, 387.500 294.199 L 394.500 290.234 411 318.833 C 420.075 334.562, 428.607 348.748, 429.959 350.358 C 438.454 360.470, 457.731 360.224, 466.442 349.893 C 470.641 344.913, 472 340.871, 472 333.364 C 472 325.552, 472.114 325.786, 448.630 285.500 L 434.056 260.500 446.390 248 C 460.436 233.764, 468.679 224.012, 470.646 219.305 C 472.602 214.624, 472.329 204.312, 470.121 199.500 C 466.090 190.712, 458.011 185.653, 447.944 185.614 C 438.485 185.577, 433.729 188.672, 421.231 203 C 405.463 221.076, 386.637 236.979, 367 248.810 C 329.175 271.599, 289.578 281.167, 242.716 278.840 C 184.966 275.973, 131.481 249.388, 91.181 203.519 C 85.856 197.458, 80.825 191.927, 80 191.227 C 74.676 186.711, 64.793 184.256, 58.500 185.888\" stroke=\"none\" fill=\"currentColor\" fill-rule=\"evenodd\"/></svg>";
    } else {
        input.type = "password";
         this.innerHTML = "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"512\" height=\"512\" viewBox=\"0 0 512 512\" version=\"1.1\" style=\"height:2em;width:2em\"><path d=\"M 238.591 93.030 C 186.115 97.702, 131.458 123.041, 77.341 167.786 C 53.718 187.318, 12.779 230.242, 2.868 245.869 C -1.166 252.231, -1.166 259.769, 2.868 266.131 C 12.725 281.674, 53.907 324.856, 77.341 344.220 C 142.860 398.362, 208.594 423.829, 270.310 418.983 C 302.196 416.479, 330.381 408.316, 362.304 392.340 C 408.574 369.183, 456.413 329.985, 496.209 282.620 C 510.574 265.524, 513 261.444, 513 254.381 C 513 251.238, 512.712 248.955, 512.359 249.307 C 512.007 249.660, 510.491 248.013, 508.991 245.647 C 499.269 230.317, 457.920 187.001, 434.659 167.780 C 392.470 132.917, 349.167 109.391, 308.354 99.158 C 285.550 93.441, 259.589 91.160, 238.591 93.030 M 238 128.037 C 184.034 134.658, 128.988 165.043, 74.138 218.489 C 58.938 233.300, 40 254.031, 40 255.860 C 40 258.624, 66.536 286.918, 85 303.842 C 132.584 347.454, 181.063 374.057, 228.500 382.588 C 242.093 385.033, 269.761 385.073, 283 382.668 C 326.145 374.829, 367.511 353.950, 410.778 318.172 C 430.001 302.277, 472 259.610, 472 255.978 C 472 254.176, 451.376 231.589, 437.319 217.997 C 385.280 167.675, 334.519 138.464, 283.500 129.479 C 272.557 127.551, 248.240 126.781, 238 128.037 M 242.500 155.079 C 213.589 159.053, 187.491 175.424, 171.319 199.728 C 144.528 239.993, 149.883 293.676, 184.103 327.897 C 218.324 362.117, 272.007 367.472, 312.272 340.681 C 348.981 316.255, 365.829 270.736, 353.909 228.188 C 344.023 192.899, 313.112 163.834, 277.744 156.573 C 267.828 154.537, 251.481 153.844, 242.500 155.079 M 242.500 190.413 C 218.547 195.952, 200.210 212.438, 192.519 235.350 C 178.718 276.461, 208.760 320.242, 252.451 322.692 C 291.587 324.885, 324.885 291.587, 322.692 252.451 C 321.145 224.864, 302.926 201.355, 276.096 192.326 C 266.705 189.166, 251.610 188.307, 242.500 190.413 M 0.320 256 C 0.320 259.575, 0.502 261.038, 0.723 259.250 C 0.945 257.462, 0.945 254.537, 0.723 252.750 C 0.502 250.963, 0.320 252.425, 0.320 256\" stroke=\"none\" fill=\"currentColor\" fill-rule=\"evenodd\"/></svg>";
    }
});

brightnessSlider.addEventListener("input", () => {
    brightnessValue.textContent = brightnessSlider.value;
});

nightBrightnessSlider.addEventListener("input", () => {
    nightBrightnessValue.textContent = nightBrightnessSlider.value;
});

////////
// Color
////////

const colorMode = document.getElementById("colorMode");
colorMode.addEventListener("change", function() {
    // make edit actions available
    const block = document.getElementById("ColorEdit");
    block.classList.remove("opacity-50", "pointer-events-none");
});  

brightnessSlider.addEventListener("change", function() {
    // make edit actions available
    const block = document.getElementById("ColorEdit");
    block.classList.remove("opacity-50", "pointer-events-none");
}); 

const ColorReset = document.getElementById("ColorReset");
ColorReset.addEventListener("click", function() {
    // reset
    colorMode.value = config.baseColor.mode;
    brightnessSlider.value = config.baseColor.brightness;
    brightnessValue.textContent = config.baseColor.brightness;
    document.getElementById("colorPicker").innerHTML = "";
    colorString = 'rgb(' + config.baseColor.r + ',' + config.baseColor.g + ',' + config.baseColor.b + ')'
    colorPicker = new iro.ColorPicker("#colorPicker", {
        width: 250,
        color: colorString,
        layout: [
            { component: iro.ui.Wheel },
            { component: iro.ui.Slider, options: { sliderType: "value" } }
        ]
    });
    colorPicker.on("color:change", function(color) {
        console.log("Farbe:", color.hexString);
    });
    // make edit actions gray again
    const block = document.getElementById("ColorEdit");
    block.classList.add("opacity-50", "pointer-events-none");
});  

const ColorSave = document.getElementById("ColorSave");
ColorSave.addEventListener("click", function() {
    // send new data to clock

    // make edit actions gray again
    const block = document.getElementById("ColorEdit");
    block.classList.add("opacity-50", "pointer-events-none");
});    

////////
// NightColor
////////

nightModeToggle.addEventListener("change", function() {
    // make edit actions available
    const block = document.getElementById("NightModeEdit");
    block.classList.remove("opacity-50", "pointer-events-none");
});  

const nightStartTime = document.getElementById("nightStart");
nightStartTime.addEventListener("change", function() {
    // make edit actions available
    const block = document.getElementById("NightModeEdit");
    block.classList.remove("opacity-50", "pointer-events-none");
});

const nightEndTime = document.getElementById("nightEnd");
nightEndTime.addEventListener("change", function() {
    // make edit actions available
    const block = document.getElementById("NightModeEdit");
    block.classList.remove("opacity-50", "pointer-events-none");
});  

const nightColorMode = document.getElementById("nightMode");
nightColorMode.addEventListener("change", function() {
    // make edit actions available
    const block = document.getElementById("NightModeEdit");
    block.classList.remove("opacity-50", "pointer-events-none");
});  

nightBrightnessSlider.addEventListener("change", function() {
    // make edit actions available
    const block = document.getElementById("NightModeEdit");
    block.classList.remove("opacity-50", "pointer-events-none");
}); 

const NightModeReset = document.getElementById("NightModeReset");
NightModeReset.addEventListener("click", function() {
    // reset
    nightModeToggle.checked = config.nightMode.enabled;
    const block2 = document.getElementById("nightSettings");
    if (config.nightMode.enabled) {
        block2.classList.remove("opacity-50", "pointer-events-none");
    } else {
        block2.classList.add("opacity-50", "pointer-events-none");
    }
    nightMode.value = config.nightMode.baseColor.mode;
    nightBrightnessSlider.value = config.nightMode.baseColor.brightness;
    nightBrightnessValue.textContent = config.nightMode.baseColor.brightness;
    nightStartTime.value = config.nightMode.startH.toString().padStart(2, "0") + ":" + config.nightMode.startM.toString().padStart(2, "0");
    nightEndTime.value = config.nightMode.endH.toString().padStart(2, "0") + ":" + config.nightMode.endM.toString().padStart(2, "0");
    
    document.getElementById("nightColorPicker").innerHTML = "";
    colorString = 'rgb(' + config.nightMode.baseColor.r + ',' + config.nightMode.baseColor.g + ',' + config.nightMode.baseColor.b + ')'
    nightColorPicker = new iro.ColorPicker("#nightColorPicker", {
        width: 250,
        color: colorString,
        layout: [
            { component: iro.ui.Wheel },
            { component: iro.ui.Slider, options: { sliderType: "value" } }
        ]
    });
    nightColorPicker.on("color:change", function(color) {
        console.log("Farbe:", color.hexString);
         // make edit actions available
                const block = document.getElementById("NightModeEdit");
                block.classList.remove("opacity-50", "pointer-events-none");
    });
    // make edit actions gray again
    const block = document.getElementById("NightModeEdit");
    block.classList.add("opacity-50", "pointer-events-none");
});  

const NightModeSave = document.getElementById("NightModeSave");
NightModeSave.addEventListener("click", function() {
    // send new data to clock

    // make edit actions gray again
    const block = document.getElementById("NightModeEdit");
    block.classList.add("opacity-50", "pointer-events-none");
});    

////////
// WiFi
////////

const wiFiSSID = document.getElementById("wifiSSID");
wiFiSSID.addEventListener("change", function() {
    // make edit actions available
    const block = document.getElementById("WiFiEdit");
    block.classList.remove("opacity-50", "pointer-events-none");
});  

const wiFiPasswd = document.getElementById("wifiPassword");
wiFiPasswd.addEventListener("change", function() {
    // make edit actions available
    const block = document.getElementById("WiFiEdit");
    block.classList.remove("opacity-50", "pointer-events-none");
}); 

const wiFiReset = document.getElementById("WiFiReset");
wiFiReset.addEventListener("click", function() {
    // reset
    wiFiSSID.value = config.WiFi;
    wiFiPasswd.value = config.password;
    // make edit actions gray again
    const block = document.getElementById("WiFiEdit");
    block.classList.add("opacity-50", "pointer-events-none");
});  

const wiFiSave = document.getElementById("WiFiSave");
wiFiSave.addEventListener("click", function() {
    // send new data to clock

    // make edit actions gray again
    const block = document.getElementById("WiFiEdit");
    block.classList.add("opacity-50", "pointer-events-none");
});    