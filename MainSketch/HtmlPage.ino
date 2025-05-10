const char http_response_200_ok[] = R"=====(
HTTP/1.1 200 OK
Content-Type: text/html
Connnection: close
)=====";


// This string includes HTML response headers and begin of html document
const char html_doctype[] = R"=====(
<!DOCTYPE html>
<html>
)=====";



// This string includes HTML head element with css
const char html_head[] = R"=====(
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {
      background-color: rgb(161, 202, 248);
    }
    .container {
      background-color: rgb(240, 240, 240);
      max-width: 400px;
      padding: 5px;
      box-sizing: border-box;
    }

    h1 {
      margin: 0;
      padding-bottom: 10px;
      font-size: 200%;
    }

    .myText {
      font-size: 20px;
      margin: 2px;
    }

    .myButton {
      width: 160px;
      height: 55px;
      margin: 5px 10px;
      background-color: rgb(25, 24, 121); /* Green */
      /* border: none; */
      border: 1px solid rgb(0, 0, 0);
      color: white;
      padding: 5px 10px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      font-size: 20px;
      font-weight: bold;
      box-shadow: 0 3px 6px 0 rgba(0,0,0,0.2), 0 2px 7px 0 rgba(0,0,0,0.19);
      float: left;
    }
    .myButton:hover {
      background-color: rgb(174, 173, 233); /* Green */
      color: black;
      box-shadow: 0 3px 6px 0 rgba(0,0,0,0.24), 0 6px 15px 0 rgba(0,0,0,0.19);
    }

    .selectionBlock {
      font-size: 16px;
      margin: 10px 2px;
      float: left;
    }

    .selection {
      font-size: 14px;
    }

    .slidecontainer {
      width: 98%;
      margin: auto;
    }
  
    .slider {
      -webkit-appearance: none;
      width: 98%;
      margin: auto;
      height: 50px;
      background: rgb(180,180,180);
      outline: none;
      opacity: 0.7;
      -webkit-transition: .2s;
      transition: opacity .2s;
      border: 4px solid rgb(0, 0, 0);    }
    
    .slider::-webkit-slider-thumb {
      -webkit-appearance: none;
      appearance: none;
      width: 50px;
      height: 50px;
      background: #4CAF50;
      cursor: pointer;
    }
    
    .slider::-moz-range-thumb {
      width: 50px;
      height: 50px;
      background: #4CAF50;
      cursor: pointer;
    }

    input[type="color"] {
      -webkit-appearance: none;
      /* border: none; */
      width: 100px;
      height: 50px;
    }
    input[type="color"]::-webkit-color-swatch-wrapper {
      padding: 0;
    }
    input[type="color"]::-webkit-color-swatch {
      border: none;
    }

    .clearfix::after {
      content: "";
      clear: both;
      display: table;
    }
  </style>
</head>
)=====";



const char html_body_starting[] = R"=====(
<body onload="onLoadFunc()">
)=====";


const char html_script_start[] = R"=====(
  <script>
)=====";

const char html_script_end[] = R"=====(
</script>
)=====";

const char html_script_white[] = R"=====(
    var whiteIntensitySlider = document.getElementById("whiteIntensity");
    var whiteIntensityValue = document.getElementById("whiteIntensityValue");
    whiteIntensityValue.innerHTML = whiteIntensitySlider.value;
    whiteIntensitySlider.oninput = function() {
      whiteIntensityValue.innerHTML = this.value;
      whiteIntensitySlider.style.opacity = 0.3 + 0.7 * (this.value / 100);
    }
)=====";

const char html_script_rgb[] = R"=====(
    var rgbIntensitySlider = document.getElementById("rgbIntensity");
    var rgbIntensityValue = document.getElementById("rgbIntensityValue");
    rgbIntensityValue.innerHTML = rgbIntensitySlider.value;
    rgbIntensitySlider.oninput = function() {
      rgbIntensityValue.innerHTML = this.value;
      rgbIntensitySlider.style.opacity = 0.3 + 0.7 * (this.value / 100);
    }

    var rgbColorInput = document.getElementById("rgbColor");
    rgbColorInput.onchange = function() {
      rgbIntensitySlider.style.background = rgbColorInput.value;
    }

    var hiddenRgbModeIntensity = document.getElementById("hiddenRgbModeIntensityId");
    function onSetModeClick() {
      hiddenRgbModeIntensity.value = rgbIntensitySlider.value;
    }
)=====";

const char html_script_onLoadFunc[] = R"=====(
    function onLoadFunc() {
      if (rgbIntensitySlider) {
        rgbIntensitySlider.style.background = rgbColorInput.value;
        rgbIntensitySlider.style.opacity = 0.3 + 0.7 * (this.value / 100);
      }
      if (whiteIntensitySlider) {
        whiteIntensitySlider.style.background = rgb(255,255,255);
        whiteIntensitySlider.style.opacity = 0.3 + 0.7 * (this.value / 100);
      }
    }
)=====";


const char html_body_html_ending[] = R"=====(
</body>
</html>
)=====";



// This string includes HTML body element. This is the main content element
const char html_body_content_start[] = R"=====(
  <div class="container">
    <h1>WiFi LED controller</h1>
)=====";

const char html_body_content_separator[] = R"=====(
    <br>
    <br>
    <br>
)=====";

const char html_body_content_end[] = R"=====(
  </div>
)=====";


const char html_body_content_white[] = R"=====(
    <div class="clearfix">
      <form action="index.html">
        <div class="slidecontainer">
          <label for="whiteIntensity" class="myText">WHITE intensity:</label>
          <input type="range" min="0" max="100" value="%WhiteLedIntensityMarker%" class="slider" id="whiteIntensity" name="whiteIntensity">
          <p class="myText">WHITE intensity: <span id="whiteIntensityValue"></span></p>
        </div>
        <button class="myButton" type="submit">SET WHITE LEDS</button>
      </form>
      <form action="index.html">
        <button class="myButton" type="submit" name="whiteOff" value="1">WHITE LEDS OFF</button>
      </form>
    </div>
)=====";


const char html_body_content_rgb[] = R"=====(
    <div class="clearfix">
        <form action="index.html">
        <label for="rgbColor" class="myText">Select RGB color:</label>
        <input type="color" id="rgbColor" name="rgbColor" value="%RGBLedColorMarker%"><br><br>
        <div class="slidecontainer">
          <label for="rgbIntensity" class="myText">RGB intensity:</label>
          <input type="range" min="0" max="100" value="%RGBLedIntensityMarker%" class="slider" id="rgbIntensity" name="rgbIntensity">
          <p class="myText">RGB intensity: <span id="rgbIntensityValue"></span></p>
        </div>
        <button class="myButton" type="submit">SET RGB LEDS COLOR</button>
      </form>
      <form action="index.html">
        <button class="myButton" type="submit" name="rgbOff" value="1">RGB LEDS 
        OFF</button>
      </form>
    </div>

    <br>
    <div class="clearfix">
      <form action="index.html">
        <div class="selectionBlock">
          <label for="rgbMode">RGB mode:</label>
          <select class="selection" id="rgbMode" name="rgbMode">
            <option value="random_slow" %RGBLedMode1Marker%>Random SLOW</option>
            <option value="random_fast" %RGBLedMode2Marker%>Random FAST</option>
            <option value="mode_3" %RGBLedMode3Marker%>Mode 3</option>
            <option value="mode_4" %RGBLedMode4Marker%>Mode 4</option>
          </select>
        </div>
        <input type="hidden" id="hiddenRgbModeIntensityId" name="rgbIntensity" value="rgbIntensity" />
        <button onclick="onSetModeClick()" class="myButton" type="submit">SET RGB Mode</button>
      </form>
    </div>
)=====";


String IntTo2CharHexString(int value)
{
  if (value < 0) value = 0;
  if (value > 255) value = 255;
  String retValue = String(value, HEX);
  if (retValue.length() == 1)
  {
    retValue = "0" + retValue;
  }
  return retValue;
}


String whiteLedIntensityMarker = "%WhiteLedIntensityMarker%"; // value should be 0 to 100 in that format
String rgbLedIntensityMarker = "%RGBLedIntensityMarker%";     // value should be 0 to 100 in that format
String rgbLedColorMarker = "%RGBLedColorMarker%";             // value should be like #AFAFFF
String rgbLedMode1Marker = "%RGBLedMode1Marker%";               // 
String rgbLedMode2Marker = "%RGBLedMode2Marker%";               // 
String rgbLedMode3Marker = "%RGBLedMode3Marker%";               // 
String rgbLedMode4Marker = "%RGBLedMode4Marker%";               // 
String rgbModeSelectedValue = R"=====(selected="selected")=====";

String create_html_bodyContent(bool white, bool rgb, int whiteLedIntensity, int rgbLedIntensity, int red, int green, int blue, String CurrentRGBMode)
{
  String htmlBodyContent = String(html_body_content_start);
  if (white && rgb)
  {
    htmlBodyContent += String(html_body_content_white);
    htmlBodyContent += String(html_body_content_separator);
    htmlBodyContent += String(html_body_content_rgb);
  }
  else if (white)
  {
    htmlBodyContent += String(html_body_content_white);
  }
  else if (rgb)
  {
    htmlBodyContent += String(html_body_content_rgb);
  }
  htmlBodyContent += String(html_body_content_end);

  if (white)
  {
    if (whiteLedIntensity < 0) whiteLedIntensity = 0;
    if (whiteLedIntensity > 100) whiteLedIntensity = 100;
    String whiteLedIntensityValue = String(whiteLedIntensity);
    htmlBodyContent.replace(whiteLedIntensityMarker, whiteLedIntensityValue);
  }

  if (rgb)
  {
    if (rgbLedIntensity < 0) rgbLedIntensity = 0;
    if (rgbLedIntensity > 100) rgbLedIntensity = 100;
    String rgbLedIntensityValue = String(rgbLedIntensity);
    htmlBodyContent.replace(rgbLedIntensityMarker, rgbLedIntensityValue);
  
    String rgbColorValue = "#" + IntTo2CharHexString(red) + IntTo2CharHexString(green) + IntTo2CharHexString(blue);
    htmlBodyContent.replace(rgbLedColorMarker, rgbColorValue);

    if (CurrentRGBMode == RGB_MODE_RANDOM_SLOW)
    {
      htmlBodyContent.replace(rgbLedMode1Marker, rgbModeSelectedValue);
      htmlBodyContent.replace(rgbLedMode2Marker, "");
      htmlBodyContent.replace(rgbLedMode3Marker, "");
      htmlBodyContent.replace(rgbLedMode4Marker, "");
    }
    else if (CurrentRGBMode == RGB_MODE_RANDOM_FAST)
    {
      htmlBodyContent.replace(rgbLedMode1Marker, "");
      htmlBodyContent.replace(rgbLedMode2Marker, rgbModeSelectedValue);
      htmlBodyContent.replace(rgbLedMode3Marker, "");
      htmlBodyContent.replace(rgbLedMode4Marker, "");
    }
    else if (CurrentRGBMode == RGB_MODE_MODE3)
    {
      htmlBodyContent.replace(rgbLedMode1Marker, "");
      htmlBodyContent.replace(rgbLedMode2Marker, "");
      htmlBodyContent.replace(rgbLedMode3Marker, rgbModeSelectedValue);
      htmlBodyContent.replace(rgbLedMode4Marker, "");
    }
    else if (CurrentRGBMode == RGB_MODE_MODE4)
    {
      htmlBodyContent.replace(rgbLedMode1Marker, "");
      htmlBodyContent.replace(rgbLedMode2Marker, "");
      htmlBodyContent.replace(rgbLedMode3Marker, "");
      htmlBodyContent.replace(rgbLedMode4Marker, rgbModeSelectedValue);
    }
  }
  
  return htmlBodyContent; 
}

String generate_html_page(bool white, bool rgb, int whiteLedIntensity, int rgbLedIntensity, int red, int green, int blue, String CurrentRGBMode)
{
  String html_script = html_script_start;
  if (white)
  {
    html_script += html_script_white;
  }
  if (rgb)
  {
    html_script += html_script_rgb;
  }
  html_script += html_script_onLoadFunc;
  html_script += html_script_end;
  
  String page = String(html_doctype) + String(html_head) + String(html_body_starting);

  page += create_html_bodyContent(white, rgb, whiteLedIntensity, rgbLedIntensity, red, green, blue, CurrentRGBMode);
 
  page += String(html_script) + String(html_body_html_ending);
  return page;
}
