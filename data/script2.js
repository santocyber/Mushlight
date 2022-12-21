// On github at @SantoCyber

// Get current sensor readings when the page loads
window.addEventListener('load', getReadings2);


var chartH = new Highcharts.Chart({
  chart:{
    renderTo:'chart-historical'
  },
  series: [
    {
      name: 'Temperature',
      type: 'line',
      color: '#101D42',
      marker: {
        symbol: 'circle',
        radius: 13,
        fillColor: '#101D42',
      }
    },
    {
      name: 'Umidade',
      type: 'line',
      color: '#00A6A6',
      marker: {
        symbol: 'square',
        radius: 13,
        fillColor: '#00A6A6',
      }
    },
    {
      name: 'Pressao',
      type: 'line',
      color: '#8B2635',
      marker: {
        symbol: 'triangle',
        radius: 13,
        fillColor: '#8B2635',
      }
    },
    {
      name: 'CO2',
      type: 'line',
      color: '#71B48D',
      marker: {
        symbol: 'triangle-down',
        radius: 13,
        fillColor: '#71B48D',
      }
    },
  ],
  title: {
    text: undefined
  },
  xAxis: {
    type: 'datetime',
    dateTimeLabelFormats: { second: '%H:%M:%S' }
  },
  yAxis: {
    title: {
      text: 'TEMPERATURA UMIDADE PRESSAO CO2'
    }
  },
  credits: {
    enabled: false
  }
});



function getReadings2(){
var xmlhttp = new XMLHttpRequest();
xmlhttp.onreadystatechange = function() {
 if (this.readyState == 4 && this.status == 200) {


     var myObj = JSON.parse(this.responseText);

//     var myobj2 = JSON.stringify(this.responseText);
  //   let result = myobj2.replace(/^\s+|\s+$/gm,'');
    // let result2 = result.trim();

//     var myobj3 = JSON.parse(result2);
  //   var myobj4 = myobj3.replaceAll(' ','');
    // var myObj = JSON.parse(myobj4);
;
    // const myObj2 = myObj3.replaceAll('null', '');

//     var myObj2 = JSON.stringify(this.responseText, null, ' ');
//     var myObj2 = JSON.stringify(this.responseText, null, ' ');
//         myObj2 = myObj2.replaceAll('\\', '');
//var ctx = myObj3.getContext("Temperatura");

//    var myObj2 = JSON.parse(myObj);
//  var myObj2 = JSON.parse(myObj);
//  var myObj3 = JSON.parse(myObj2);
//JSON.parse(JSON.stringify(myObject)))
//    var myObj4 = myObj3["Temperatura"]["Umidade"]["Pressao"]["CO2"];


//    plotHistorical(myObj);



  var keys = Object.keys(myObj);
  console.log(keys);
  console.log(keys.length);

  for (var i = 0; i < keys.length; i++){
    var x = (new Date()).getTime();
    console.log('XXXX' + x);
    const key = keys[i];
    var y = Number(myObj[key]);
    console.log('YYYYY' + y);

    chartH.series[i].addPoint([x, y], true, false, true);

    console.log(myObj[key]);

}
    console.log('<pre>' + myObj + '</pre>');



    }
  };
  xmlhttp.open("GET", "/log", true);
  xmlhttp.send();
}
















if (!!window.EventSource) {
  var source = new EventSource('/events');

  source.addEventListener('open', function(e) {
    console.log("Events Connected");
  }, false);

  source.addEventListener('error', function(e) {
    if (e.target.readyState != EventSource.OPEN) {
      console.log("Events Disconnected");
    }
  }, false);

  source.addEventListener('message', function(e) {
    console.log("message", e.data);
  }, false);

}
