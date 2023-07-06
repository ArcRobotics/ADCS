#ifndef ADCS_Server_H_
#define ADCS_Server_H_

  #include <WiFi.h>
  #include <WebServer.h>
  
  // Replace with your network credentials
  const char* ssid = "WEFC1F48";
  const char* password = ",-8g9Al*!;Km=S-p&j";
  
  WebServer server(80);
  
  void handleRoot();
  void handleRefresh();
  void handlePause();
  void run_client();
  void Server_init();
  String getHTMLPage();
  String getRandomNumber();
  
  unsigned long lastRefreshTime = 0;
  String refreshedData;
  bool isPaused = false;
  const int MAX_DATA_POINTS = 50; // Maximum number of data points to store
//=================================================================  
  void Server_init() {
  
    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
    }
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  
    // Initialize built-in LED pin as an output
    pinMode(LED_BUILTIN, OUTPUT);
  
    // Route for root / web page
    server.on("/", handleRoot);
    server.on("/refresh", handleRefresh);
    server.on("/pause", handlePause);
  
    // Start server
    server.begin();
  
    // Turn on the built-in LED to indicate server startup
    digitalWrite(LED_BUILTIN, HIGH);
  }
//=================================================================  
  void run_client() {
    server.handleClient();
  
    if (!isPaused) {
      // Refresh section every 500ms
      if (millis() - lastRefreshTime >= 500) {
        refreshedData = getRandomNumber();
        lastRefreshTime = millis();
      }
    }
  }
//=================================================================  
  void handleRoot() {
    String htmlPage = getHTMLPage();
    server.send(200, "text/html", htmlPage);
  
    // Print input data
    if (server.hasArg("textbox1") && server.hasArg("textbox2") && server.hasArg("textbox3")) {
      String textbox1Value = server.arg("textbox1");
      String textbox2Value = server.arg("textbox2");
      String textbox3Value = server.arg("textbox3");
      Serial.print("Roll (Textbox 1): ");
      Serial.println(textbox1Value);
      Serial.print("Pitch (Textbox 2): ");
      Serial.println(textbox2Value);
      Serial.print("Yaw (Textbox 3): ");
      Serial.println(textbox3Value);
    }
  }
//=================================================================  
  void handleRefresh() {
    server.send(200, "text/plain", refreshedData);
  }
//=================================================================  
  void handlePause() {
    isPaused = !isPaused;
    server.send(200, "text/plain", isPaused ? "Paused" : "Resumed");
  }
//=================================================================  
  String getHTMLPage() {
    String htmlPage = "<html><head>";
    htmlPage += "<style>body { background-color: #FAFAFA; }</style>";
    htmlPage += "<script src='https://cdn.jsdelivr.net/npm/chart.js'></script>";
    htmlPage += "</head><body>";
    htmlPage += "<h1>ADCS Control Web Server</h1>";
    htmlPage += "<form action='/'>";
    htmlPage += "<label for='textbox1'>Roll:</label>";
    htmlPage += "<input type='text' id='textbox1' name='textbox1'>";
    htmlPage += "<label for='textbox2'>Pitch:</label>";
    htmlPage += "<input type='text' id='textbox2' name='textbox2'>";
    htmlPage += "<label for='textbox3'>Yaw:</label>";
    htmlPage += "<input type='text' id='textbox3' name='textbox3'>";
    htmlPage += "<input type='submit' value='Submit'>";
    htmlPage += "</form>";
    htmlPage += "<div>";
    htmlPage += "<p><strong>Actual ROLL:</strong></p>";
    htmlPage += "<p id='refreshedData'>Data will be updated here.</p>";
    htmlPage += "<div style='width: 800px;'>"; // Set the width of the chart container
    htmlPage += "<div style='height: 400px;'>"; // Set the width of the chart container
    htmlPage += "<canvas id='chart' width='400' height='200'></canvas>";
    htmlPage += "<button id='pauseButton' onclick='togglePause()'>Pause</button>";
    htmlPage += "</div>";
    htmlPage += "<script>";
    htmlPage += "var chartData = {";
    htmlPage += "  labels: [],";
    htmlPage += "  datasets: [{";
    htmlPage += "    label: 'Signal',";
    htmlPage += "    data: [],";
    htmlPage += "    borderColor: 'white',";
    htmlPage += "    backgroundColor: 'transparent'";
    htmlPage += "  }]";
    htmlPage += "};";
    htmlPage += "var isPaused = false;";
    htmlPage += "var chartUpdateInterval;";
    htmlPage += "function fetchData() {";
    htmlPage += "  if (!isPaused) {";
    htmlPage += "    fetch('/refresh')";
    htmlPage += "      .then(response => response.text())";
    htmlPage += "      .then(data => {";
    htmlPage += "        document.getElementById('refreshedData').innerText = data;";
    htmlPage += "        updateChart(data);";
    htmlPage += "      });";
    htmlPage += "  }";
    htmlPage += "}";
    htmlPage += "function togglePause() {";
    htmlPage += "  fetch('/pause')";
    htmlPage += "    .then(response => response.text())";
    htmlPage += "    .then(status => {";
    htmlPage += "      isPaused = !isPaused;";
    htmlPage += "      document.getElementById('pauseButton').innerText = isPaused ? 'Resume' : 'Pause';";
    htmlPage += "      document.getElementById('refreshedData').innerText = status;";
    htmlPage += "      if (isPaused) {";
    htmlPage += "        clearInterval(chartUpdateInterval);";
    htmlPage += "      } else {";
    htmlPage += "        chartUpdateInterval = setInterval(fetchData, 500);";
    htmlPage += "      }";
    htmlPage += "    });";
    htmlPage += "}";
    htmlPage += "function createChart() {";
    htmlPage += "  var ctx = document.getElementById('chart').getContext('2d');";
    htmlPage += "  var chart = new Chart(ctx, {";
    htmlPage += "    type: 'line',";
    htmlPage += "    data: chartData,";
    htmlPage += "    options: {";
    htmlPage += "      scales: {";
    htmlPage += "        x: {";
    htmlPage += "          display: true,";
    htmlPage += "          scaleLabel: {";
    htmlPage += "            display: true,";
    htmlPage += "            labelString: 'Time'";
    htmlPage += "          }";
    htmlPage += "        },";
    htmlPage += "        y: {";
    htmlPage += "          display: true,";
    htmlPage += "          scaleLabel: {";
    htmlPage += "            display: true,";
    htmlPage += "            labelString: 'Signal'";
    htmlPage += "          },";
    htmlPage += "          beginAtZero: true";
    htmlPage += "        }";
    htmlPage += "      }";
    htmlPage += "    }";
    htmlPage += "  });";
    htmlPage += "  chartData.datasets[0].borderColor = 'black';";
    htmlPage += "}";
    htmlPage += "function updateChart(data) {";
    htmlPage += "  var chart = Chart.getChart('chart');";
    htmlPage += "  if (chart.data.labels.length >= " + String(MAX_DATA_POINTS) + ") {";
    htmlPage += "    chart.data.labels.shift();";
    htmlPage += "    chart.data.datasets[0].data.shift();";
    htmlPage += "  }";
    htmlPage += "  chart.data.labels.push('');";
    htmlPage += "  chart.data.datasets[0].data.push(data);";
    htmlPage += "  chart.update();";
    htmlPage += "}";
    htmlPage += "createChart();";
    htmlPage += "chartUpdateInterval = setInterval(fetchData, 500);";
    htmlPage += "</script>";
    htmlPage += "</body></html>";
  
    return htmlPage;
  }  
//=================================================================  
  String getRandomNumber() {
    // Generate a random number
    int randomNumber = random(100);
  
    // Convert the random number to a string
    String randomNumberString = String(randomNumber);
  
    return randomNumberString;
  }
#endif
