extern String local_ip;
extern int cover_state, flat_panel_state;
extern float motor_angle, motor_temp;

const char main_page[] PROGMEM = R"""(
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8">
        <title>Main Page - Automatic Telescope Cover</title>
    </head>
    <body class="body" onload="process()">
        <div class="wrapper">
            <div class="header">
                <h1>Automatic Telescope Cover</h1>
            </div>
            <div class="content">
                <div class="system_info">
                    <h2>System Infomation</h2>
                    <div class="system_info_wifi">
                        <p class="system_info_wifi_p1">WiFi: <span id="wifi_ssid_text" style="font-weight: bold;">Unknown</span></p> 
                        <p class="system_info_wifi_p2">IP Address: <span id="ip_text" style="font-weight: bold;">Unknown</span></p>
                    </div>
                    <br>
                    <div class="system_info_states">
                        <p class="system_info_states_p1">Cover State: <span id="cover_state_text">Unknown</span></p>
                        <p class="system_info_states_p2">Flat Panel State: <span id="flat_panel_state_text">Unknown</span></p>
                    </div>
                    <br>
                    <div class="system_info_sensors">
                        <p class="system_info_sensors_p1">Motor Angle: <span id="motor_angle_text">Unknown</span>°</p>
                        <p class="system_info_sensors_p2">Motor Temperature: <span id="motor_temp_text">Unknown</span> °C</p>
                    </div>
                </div>
                <div class="controllers">
                    <h2>Controllers</h2>
                    <p style="font-weight: bold; font-size: 18px;">Cover</p>
                    <button class="controllers_buttons" onclick="open_cover()">Open</button>
                    <button class="controllers_buttons" onclick="close_cover()">Close</button> 
                    <p style="font-weight: bold; font-size: 18px;">Flat Panel</p>
                    <button class="controllers_buttons" onclick="open_flat_panel()">Open</button>
                    <button class="controllers_buttons" onclick="close_flat_panel()">Close</button> 
                </div>
            </div>
            <div class="footer">
                <h2>Contact</h2>
                <p>Website: <a href="http://nthuobs.phys.nthu.edu.tw/">NTHU Observatory</a></p>
                <p>Author: Jamie Chang (<a href="mailto:jamiechang917@gmail.com">jamiechang917@gmail.com</a>)</p>
                <p>Firmware: 2022/01/18</p>
            </div>
        </div>
    </body>
    <style>
        a{
            text-decoration: none;
            color: skyblue;
        }
        a:hover {
            text-decoration-line: underline;
            text-decoration-color: salmon;
        }
        .body {
            margin: 0;
            padding: 0;
            color: rgb(185, 185, 185);
            background-color:rgb(46, 46, 46);
            font-family: Arial, Helvetica, sans-serif;
        }
        .wrapper{
            text-align: center;
            height: 100%;
            width: 100%;
        }
        .header {
            margin: none;
            padding: 0.5em;
            background-color:rgb(112, 12, 12);
            text-align: left;
        }
        .system_info_wifi,
        .system_info_states,
        .system_info_sensors {
            display: inline-flex;
            flex-wrap: wrap;
            flex-direction: row;
            justify-content: space-around ;
            width: 50%;
        }
        .system_info_wifi_p1,
        .system_info_wifi_p2,
        .system_info_states_p1,
        .system_info_states_p2 {
            /* text-align: left; */
            width: 50%;
        }
        .controllers_buttons {
            width:160px;
            height:40px;
            font-size:20px;
            font-weight: normal;
            text-shadow: none;
            margin: 0.5em;
            text-align: center;
            cursor: pointer;
            transition: 0.8s;
        }
        .controllers_buttons:nth-of-type(odd) {
            color: black;
            background-color: green;
        }
        .controllers_buttons:nth-of-type(even) {
            color: black;
            background-color: red;
        }
        .controllers_buttons:hover {
            opacity: 0.7;
            color: white;
        }
    </style>
    <script type="text/javascript">
        // global variable for all functions
        var xmlHttp = createXmlHttpObject();

        function createXmlHttpObject() {
            if (window.XMLHttpRequest) { // check whether the user's browser is supported
                xmlHttp = new XMLHttpRequest();
            }
            else {
                xmlHttp = new ActiveXObject("Microsoft.XMLHTTP"); 
            }
            return xmlHttp;
        }

        function open_cover() {
            var xhttp = new XMLHttpRequest();
            xhttp.open("PUT", "open_cover", false);
            xhttp.send();
        }

        function close_cover() {
            var xhttp = new XMLHttpRequest();
            xhttp.open("PUT", "close_cover", false);
            xhttp.send();
        }

        function open_flat_panel() {
            var xhttp = new XMLHttpRequest();
            xhttp.open("PUT", "open_flat_panel", false);
            xhttp.send();
        }
        function close_flat_panel() {
            var xhttp = new XMLHttpRequest();
            xhttp.open("PUT", "close_flat_panel", false);
            xhttp.send();
        }

        function update_html() { // update html from xml stream
            var message;
            var xmlResponse;
            var xmlDoc;
            
            // get xml data
            xmlResponse = xmlHttp.responseXML;
            
            // wifi_ssid
            xmlDoc = xmlResponse.getElementsByTagName("wifi_ssid");
            message = xmlDoc[0].firstChild.nodeValue;
            document.getElementById("wifi_ssid_text").innerHTML = message;
            
            // local_ip
            xmlDoc = xmlResponse.getElementsByTagName("local_ip");
            message = xmlDoc[0].firstChild.nodeValue;
            document.getElementById("ip_text").innerHTML = message;

            // cover_state
            xmlDoc = xmlResponse.getElementsByTagName("cover_state");
            message = xmlDoc[0].firstChild.nodeValue;
            if (message == 0) {
                document.getElementById("cover_state_text").innerHTML = "Close";
                document.getElementById("cover_state_text").style.color = "red";
            }
            else if (message == 1) {
                document.getElementById("cover_state_text").innerHTML = "Open";
                document.getElementById("cover_state_text").style.color = "green";
            }

            // flat_panel_state
            xmlDoc = xmlResponse.getElementsByTagName("flat_panel_state");
            message = xmlDoc[0].firstChild.nodeValue;
            if (message == 0) {
                document.getElementById("flat_panel_state_text").innerHTML = "Close";
                document.getElementById("flat_panel_state_text").style.color = "red";
            }
            else if (message == 1) {
                document.getElementById("flat_panel_state_text").innerHTML = "Open";
                document.getElementById("flat_panel_state_text").style.color = "green";
            }

        }

        function process() { // ask for an xml stream
            if (xmlHttp.readyState == 0 || xmlHttp.readyState == 4) {
                xmlHttp.open("PUT", "xml", true);
                // xmlHttp.setRequestHeader("Content-type", "text/xml");
                // xmlHttp.overrideMimeType('text/xml'); // force to resolve as xml
                xmlHttp.onreadystatechange = update_html;
                xmlHttp.send(null);
            }
            setTimeout("process()", 100); // big pages need a longer timeout.
        }
    </script>
</html>
)""";