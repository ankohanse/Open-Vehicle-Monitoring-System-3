/**
 * Project:      Open Vehicle Monitor System
 * Module:       Mitsubishi iMiEV, Citroen C-Zero, Mitsubishi iOn Webserver
 *
 * (c) 2018 Tamás Kovács
 * (c) 2018	Nikolay Shishkov <nshishkov@yahoo.com>
 * (c) 2018	Geir Øyvind Væidalo <geir@validalo.net>
 * (c) 2017  Michael Balzer <dexter@dexters-web.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include <stdio.h>
#include <string>
#include "ovms_metrics.h"
#include "ovms_events.h"
#include "ovms_config.h"
#include "ovms_command.h"
#include "metrics_standard.h"
#include "ovms_notify.h"
#include "ovms_webserver.h"

#include "vehicle_mitsubishi.h"

using namespace std;

#define _attr(text) (c.encode_html(text).c_str())
#define _html(text) (c.encode_html(text).c_str())


/**
 * WebInit: register pages
 */
void OvmsVehicleMitsubishi::WebInit()
{
   //vehicle menu:
   //MyWebServer.RegisterPage("/xtc/features", "Features", WebCfgFeatures, PageMenu_Vehicle, PageAuth_Cookie);
  // MyWebServer.RegisterPage("/xtc/batterydiag", "Battery Diagnostics", WebCfgBattery, PageMenu_Vehicle, PageAuth_Cookie);
   MyWebServer.RegisterPage("/xmi/battmon", "Battery monitor", WebBattMon, PageMenu_Vehicle, PageAuth_Cookie);
}


/**
 * WebCfgFeatures: configure general parameters (URL /xmi/config)
 */
void OvmsVehicleMitsubishi::WebCfgFeatures(PageEntry_t& p, PageContext_t& c)
{/*
  std::string error;
  bool canwrite, remote_charge_port;

  if (c.method == "POST") {
    // process form submission:
    canwrite = (c.getvar("canwrite") == "yes");
    remote_charge_port  = (c.getvar("remote_charge_port") == "yes");

    if (error == "") {
      // store:
      MyConfig.SetParamValueBool("xtc", "canwrite", canwrite);
      MyConfig.SetParamValueBool("xtc", "remote_charge_port", remote_charge_port);

      c.head(200);
      c.alert("success", "<p class=\"lead\">Think City EV feature configuration saved.</p>");
      MyWebServer.OutputHome(p, c);
      c.done();
      return;
    }

    // output error, return to form:
    error = "<p class=\"lead\">Error!</p><ul class=\"errorlist\">" + error + "</ul>";
    c.head(400);
    c.alert("danger", error.c_str());
  }
  else {
    // read configuration:
    canwrite = MyConfig.GetParamValueBool("xtc", "canwrite", false);
    remote_charge_port = MyConfig.GetParamValueBool("xtc", "remote_charge_port", false);

    c.head(200);
  }

  // generate form:

  c.panel_start("primary", "Think City EV feature configuration");
  c.form_start(p.uri);

  c.fieldset_start("General");
  c.input_checkbox("Enable CAN writes", "canwrite", canwrite,
    "<p>Controls overall CAN write access, some functions depend on this.</p>");
  c.input_radio_start("Battery type", "batt_type");
  c.input_radio_option("batt_type", "Enerdel", "", true);
  c.input_radio_option("batt_type", "Zebra", "", false);
  c.input_radio_end("This would change ranges, and display formats to reflect type of battery in the car");
  c.fieldset_end();

  c.print("<hr>");
  c.input_button("default", "Save");
  c.form_end();
  c.panel_end();
  c.done();*/
}


/**
 * WebCfgBattery: configure battery parameters (URL /xmi/battery)
 */
void OvmsVehicleMitsubishi::WebCfgBattery(PageEntry_t& p, PageContext_t& c)
{
    /*
  std::string error;
  //	  cap_act_kwh			Battery capacity in wH (Default: 270000)
  //  suffsoc          	Sufficient SOC [%] (Default: 0=disabled)
  //  suffrange        	Sufficient range [km] (Default: 0=disabled)
  //  maxrange         	Maximum ideal range at 20 °C [km] (Default: 160)
  std::string cap_act_kwh, maxrange, suffrange, suffsoc;

  if (c.method == "POST") {
    // process form submission:
    cap_act_kwh = c.getvar("cap_act_kwh");
    maxrange = c.getvar("maxrange");
    suffrange = c.getvar("suffrange");
    suffsoc = c.getvar("suffsoc");

    // check:
    if (!cap_act_kwh.empty()) {
      float n = atof(cap_act_kwh.c_str());
      if (n < 1)
        error += "<li data-input=\"cap_act_kwh\">Battery Wh capacity must be &ge; 1</li>";
    }
    if (!maxrange.empty()) {
      float n = atof(maxrange.c_str());
      if (n < 1)
        error += "<li data-input=\"maxrange\">Maximum range invalid, must be &ge; 1</li>";
    }
    if (!suffrange.empty()) {
      float n = atof(suffrange.c_str());
      if (n < 0)
        error += "<li data-input=\"suffrange\">Sufficient range invalid, must be &ge; 0</li>";
    }
    if (!suffsoc.empty()) {
      float n = atof(suffsoc.c_str());
      if (n < 0)
        error += "<li data-input=\"suffsoc\">Sufficient SOC invalid, must be 0…100</li>";
    }

    if (error == "") {
      // store:
      MyConfig.SetParamValue("xtc", "cap_act_kwh", cap_act_kwh);
      MyConfig.SetParamValue("xtc", "maxrange", maxrange);
      MyConfig.SetParamValue("xtc", "suffrange", suffrange);
      MyConfig.SetParamValue("xtc", "suffsoc", suffsoc);

      c.head(200);
      c.alert("success", "<p class=\"lead\">Think City battery setup saved.</p>");
      MyWebServer.OutputHome(p, c);
      c.done();
      return;
    }

    // output error, return to form:
    error = "<p class=\"lead\">Error!</p><ul class=\"errorlist\">" + error + "</ul>";
    c.head(400);
    c.alert("danger", error.c_str());
  }
  else {
    // read configuration:
  		cap_act_kwh = MyConfig.GetParamValue("xtc", "cap_act_kwh", XSTR(CGF_DEFAULT_BATTERY_CAPACITY));
    maxrange = MyConfig.GetParamValue("xtc", "maxrange", XSTR(CFG_DEFAULT_MAXRANGE));
    suffrange = MyConfig.GetParamValue("xtc", "suffrange", "0");
    suffsoc = MyConfig.GetParamValue("xtc", "suffsoc", "0");

    c.head(200);
  }

  // generate form:

  c.panel_start("primary", "Think City EV battery setup");
  c.form_start(p.uri);

  c.fieldset_start("Battery properties");

  c.input("number", "Battery capacity", "cap_nom_ah", cap_act_kwh.c_str(), "Default: " XSTR(CGF_DEFAULT_BATTERY_CAPACITY),
    "<p>This is the usable battery capacity of your battery when new.</p>",
    "min=\"1\" step=\"0.1\"", "Wh");

  c.input("number", "Maximum drive range", "maxrange", maxrange.c_str(), "Default: " XSTR(CFG_DEFAULT_MAXRANGE),
    "<p>The range you normally get at 100% SOC and 20 °C.</p>",
    "min=\"1\" step=\"1\"", "km");

  c.fieldset_end();

  c.fieldset_start("Charge control");

  c.input_slider("Sufficient range", "suffrange", 3, "km",
    atof(suffrange.c_str()) > 0, atof(suffrange.c_str()), 0, 0, 300, 1,
    "<p>Default 0=off. Notify/stop charge when reaching this level.</p>");
  c.input_slider("Sufficient SOC", "suffsoc", 3, "%",
    atof(suffsoc.c_str()) > 0, atof(suffsoc.c_str()), 0, 0, 100, 1,
    "<p>Default 0=off. Notify/stop charge when reaching this level.</p>");

  c.fieldset_end();

  c.print("<hr>");
  c.input_button("default", "Save");
  c.form_end();
  c.panel_end();
  c.done();
  */
}

/**
 * WebBattMon: (/xmi/battmon)
 * TODO
 */
void OvmsVehicleMitsubishi::WebBattMon(PageEntry_t& p, PageContext_t& c)
{
  c.head(200);
  c.print(
    "<div class=\"panel panel-primary panel-single\">"
    "<div class=\"panel-heading\">Mitsubishi i-MiEV, Citroen C-Zero, Peugeot iOn Battery Monitor</div>"
    "<div class=\"panel-body\">"
      "<div class=\"receiver\" id=\"livestatus\">"
        "<div id=\"cellvolts\" style=\"width: 100%; max-width: 1850px; height: 45vh; min-height: 266px; margin: 0 auto\"></div>"
        "<div id=\"celltemps\" style=\"width: 100%; max-width: 1850px; height: 25vh; min-height: 145px; margin: 0 auto\"></div>"
      "</div>"
    "</div>"
    "<div class=\"panel-footer\">"
      "<button class=\"btn btn-default\" data-cmd=\"xmi battreset\" data-target=\"#output\">Reset min/max</button>"
      "<samp id=\"output\" class=\"samp-inline\"></samp>"
    "</div>"
    "</div>"
    ""
    "<style>"
    ""
    "#cellvolts .highcharts-boxplot-box {"
      "fill: #90e8a3;"
      "fill-opacity: 0.6;"
      "stroke: #90e8a3;"
      "stroke-opacity: 0.6;"
    "}"
    ".night #cellvolts .highcharts-boxplot-box {"
      "fill-opacity: 0.4;"
    "}"
    ""
    "#cellvolts .highcharts-boxplot-median {"
      "stroke: #193e07;"
      "stroke-width: 5px;"
      "stroke-linecap: round;"
    "}"
    ".night #cellvolts .highcharts-boxplot-median {"
      "stroke: #afff45;"
    "}"
    ""
    "#cellvolts .highcharts-plot-line {"
      "fill: none;"
      "stroke: #3625c3;"
      "stroke-width: 1px;"
      "stroke-dasharray: 5;"
    "}"
    ".night #cellvolts .highcharts-plot-line {"
      "stroke: #728def;"
    "}"
    ""
    "#celltemps .highcharts-boxplot-box {"
      "fill: #eabe85;"
      "fill-opacity: 0.6;"
      "stroke: #eabe85;"
      "stroke-opacity: 0.6;"
    "}"
    ".night #celltemps .highcharts-boxplot-box {"
      "fill-opacity: 0.4;"
    "}"
    ""
    "#celltemps .highcharts-boxplot-median {"
      "stroke: #7d1313;"
      "stroke-width: 5px;"
      "stroke-linecap: round;"
    "}"
    ".night #celltemps .highcharts-boxplot-median {"
      "stroke: #fdd02e;"
    "}"
    ""
    "#celltemps .highcharts-plot-line {"
      "fill: none;"
      "stroke: #3625c3;"
      "stroke-width: 1px;"
      "stroke-dasharray: 5;"
    "}"
    ".night #celltemps .highcharts-plot-line {"
      "stroke: #728def;"
    "}"
    ""
    "</style>"
    ""
    "<script type=\"text/javascript\">"
    ""
    "var cellvolts;"
    ""
    "function get_cell_data() {"
      "var data = { cells:[],  volts: [], vdevs: [], voltmean: 0  };"
      "var i, sum=0, act, min, max, base;"
      "for (i=0; i<88; i++) {"
        "base = \"xmi.b.cell.volts.\";"
        "act = metrics[base+\"act\"][i];"
        "min = metrics[base+\"min\"][i];"
        "max = metrics[base+\"max\"][i];"
        "sum += act;"
        "data.cells.push(i+1);"
        "data.volts.push([min,min,act,max,max]);"
        "data.vdevs.push(metrics[base+\"maxdev\"][i]);"
      "}"
      "data.voltmean = sum / 88;"
      "return data;"
    "}"
    ""
    "function update_cell_chart() {"
      "var data = get_cell_data();"
      "cellvolts.xAxis[0].setCategories(data.cells);"
      "cellvolts.yAxis[0].removePlotLine('plot-line-mean');"
      "cellvolts.yAxis[0].addPlotLine({ id: 'plot-line-mean', value: data.voltmean });"
      "cellvolts.series[0].setData(data.volts);"
    "}"
    ""
    "function init_cellvolts() {"
      "var data = get_cell_data();"
      "cellvolts = Highcharts.chart('cellvolts', {"
        "chart: {"
          "type: 'boxplot',"
          "events: {"
            "load: function () {"
              "$('#livestatus').on(\"msg:metrics\", function(e, update){"
                "if (/\\.b\\.cell.volts/.test(Object.keys(update).join()))"
                  "update_cell_chart();"
              "});"
            "}"
          "},"
          "zoomType: 'y',"
          "panning: true,"
          "panKey: 'ctrl',"
        "},"
        "title: { text: null },"
        "credits: { enabled: false },"
        "legend: { enabled: false },"
        "xAxis: { categories: data.cells },"
        "yAxis: {"
          "title: { text: null },"
          "labels: { format: \"{value:.2f}V\" },"
          "minTickInterval: 0.01,"
          "plotLines: [{ id: 'plot-line-mean', value: data.voltmean }],"
          "minorTickInterval: 'auto',"
        "},"
        "series: [{"
          "name: 'Voltage',"
          "data: data.volts,"
          "tooltip: {"
            "headerFormat: '<em>Cell #{point.key}</em><br/>',"
            "pointFormat: ''"
              "+ '● <b>Act: {point.median:.3f}V</b><br/>'"
              "+ '▲ Max: {point.high:.3f}V<br/>'"
              "+ '▼ Min: {point.low:.3f}V<br/>'"
          "},"
          "whiskerLength: 0,"
          "animation: {"
            "duration: 300,"
            "easing: 'easeOutExpo'"
          "},"
        "}]"
      "});"
    "}"
    ""
    ""
    "var celltemps;"
    ""
    "function get_cmod_data() {"
      "var data = { cmods: [], temps: [], tdevs: [], tempmean: 0 };"
      "var i, sum=0, act, min, max, base;"
      "for (i=0; i<66; i++) {"
        "base = \"xmi.b.cell.temps.\";"
        "act = metrics[base+\"act\"][i];"
        "min = metrics[base+\"min\"][i];"
        "max = metrics[base+\"max\"][i];"
        "sum += act;"
        "data.cmods.push(i+1);"
        "data.temps.push([min,min,act,max,max]);"
        "data.tdevs.push(metrics[base+\"maxdev\"][i]);"
      "}"
      "data.tempmean = sum / 66;"
      "return data;"
    "}"
    ""
    "function update_cmod_chart() {"
      "var data = get_cmod_data();"
      "celltemps.xAxis[0].setCategories(data.cmods);"
      "celltemps.yAxis[0].removePlotLine('plot-line-mean');"
      "celltemps.yAxis[0].addPlotLine({ id: 'plot-line-mean', value: data.tempmean });"
      "celltemps.series[0].setData(data.temps);"
    "}"
    ""
    "function init_celltemps() {"
      "var data = get_cmod_data();"
      "celltemps = Highcharts.chart('celltemps', {"
        "chart: {"
          "type: 'boxplot',"
          "events: {"
            "load: function () {"
              "$('#livestatus').on(\"msg:metrics\", function(e, update){"
                "if (/\\.b\\.cell.temps/.test(Object.keys(update).join()))"
                  "update_cmod_chart();"
              "});"
            "}"
          "},"
          "zoomType: 'y',"
          "panning: true,"
          "panKey: 'ctrl',"
        "},"
        "title: { text: null },"
        "credits: { enabled: false },"
        "legend: { enabled: false },"
        "xAxis: { categories: data.cmods },"
        "yAxis: {"
          "title: { text: null },"
          "labels: { format: \"{value:.0f} °C\" },"
          "minTickInterval: 1,"
          "plotLines: [{ id: 'plot-line-mean', value: data.tempmean }],"
          "minorTickInterval: 'auto',"
        "},"
        "series: [{"
          "name: 'Temperature',"
          "data: data.temps,"
          "tooltip: {"
            "headerFormat: '<em>Module #{point.key}</em><br/>',"
            "pointFormat: ''"
              "+ '● <b>Act: {point.median:.0f}°C</b><br/>'"
              "+ '▲ Max: {point.high:.0f}°C<br/>'"
              "+ '▼ Min: {point.low:.0f}°C<br/>'"
          "},"
          "whiskerLength: 0,"
          "animation: {"
            "duration: 300,"
            "easing: 'easeOutExpo'"
          "},"
        "}]"
      "});"
    "}"
    ""
    ""
    "function init_charts() {"
      "init_cellvolts();"
      "init_celltemps();"
    "}"
    ""
    "if (window.Highcharts) {"
      "init_charts();"
    "} else {"
      "$.ajax({"
        "url: \"/assets/charts.js\","
        "dataType: \"script\","
        "cache: true,"
        "success: function(){ init_charts(); }"
      "});"
    "}"
    ""
    "</script>");
  c.done();
}


/**
 * GetDashboardConfig: Mitsubishi i-MiEVm Citroen C-Zero, Peugeot iOn specific dashboard setup
 */
void OvmsVehicleMitsubishi::GetDashboardConfig(DashboardConfig& cfg)
{
  cfg.gaugeset1 =
    "yAxis: [{"
      // Speed:
      "min: 0, max: 135,"
      "plotBands: ["
        "{ from: 0, to: 75, className: 'green-band' },"
        "{ from: 75, to: 100, className: 'yellow-band' },"
        "{ from: 100, to: 135, className: 'red-band' }]"
    "},{"
      // Voltage:
      "min: 294, max: 362,"
      "plotBands: ["
        "{ from: 194, to: 317, className: 'red-band' },"
        "{ from: 317, to: 341, className: 'yellow-band' },"
        "{ from: 341, to: 362, className: 'green-band' }]"
    "},{"
      // SOC:
      "min: 0, max: 100,"
      "plotBands: ["
        "{ from: 0, to: 12.5, className: 'red-band' },"
        "{ from: 12.5, to: 25, className: 'yellow-band' },"
        "{ from: 25, to: 100, className: 'green-band' }]"
    "},{"
      // Efficiency:
      "min: 0, max: 300,"
      "plotBands: ["
        "{ from: 0, to: 120, className: 'green-band' },"
        "{ from: 120, to: 250, className: 'yellow-band' },"
        "{ from: 250, to: 300, className: 'red-band' }]"
    "},{"
      // Power:
      "min: -30, max: 75,"
      "plotBands: ["
        "{ from: -30, to: 0, className: 'violet-band' },"
        "{ from: 0, to: 15, className: 'green-band' },"
        "{ from: 15, to: 40, className: 'yellow-band' },"
        "{ from: 40, to: 75, className: 'red-band' }]"
    "},{"
      // Charger temperature:
      "min: 20, max: 55, tickInterval: 20,"
      "plotBands: ["
        "{ from: 20, to: 40, className: 'normal-band border' },"
        "{ from: 40, to: 55, className: 'red-band border' }]"
    "},{"
      // Battery temperature:
      "min: -15, max: 65, tickInterval: 25,"
      "plotBands: ["
        "{ from: -15, to: 0, className: 'red-band border' },"
        "{ from: 0, to: 40, className: 'normal-band border' },"
        "{ from: 40, to: 65, className: 'red-band border' }]"
    "},{"
      // Inverter temperature:
      "min: 20, max: 55, tickInterval: 20,"
      "plotBands: ["
        "{ from: 20, to: 40, className: 'normal-band border' },"
        "{ from: 40, to: 55, className: 'red-band border' }]"
    "},{"
      // Motor temperature:
      "min: 20, max: 100, tickInterval: 25,"
      "plotBands: ["
        "{ from: 20, to: 75, className: 'normal-band border' },"
        "{ from: 75, to: 100, className: 'red-band border' }]"
    "}]";
}
