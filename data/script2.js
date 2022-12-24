// On github at @SantoCyber

// Get current sensor readings when the page loads
window.addEventListener('load', getReadings);


var chartH = new Highcharts.Chart({
  chart:{
    renderTo:'chart-historical'
  },
  series: [
    {
      name: 'Temperatura',
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
    {
      name: 'Hora',
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
    name: 'Hora',
    type: 'line',
//    dateTimeLabelFormats: { second: '%H:%M:%S' }
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



//Plot temperature in the temperature chart
function plotTemperature(jsonValue) {

 var keys2 = Object.keys(jsonValue.sensores);
    for (var f = 0; f < keys2.length; f++){

console.log(keys2);
  console.log(keys2.length);


  var keys = Object.keys(jsonValue.sensores[f]);
console.log(keys);
  console.log(keys.length);


    for (var i = 0; i < keys.length; i++){
//    var x = (new Date()).getTime();
    const key = keys[i];

    var x = String(jsonValue.sensores[f][keys[4]]);
    console.log(x);
    var y = Number(jsonValue.sensores[f][key]);
    console.log(y);

    if(chartH.series[i].data.length > 40) {
      chartH.series[i].addPoint([x, y], true, true, true);
    } else {
      chartH.series[i].addPoint([x, y], true, false, true);
    }

  }
}
}



// Function to get current readings on the webpage when it loads for the first time
function getReadings(){
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var myobj2 = JSON.stringify(this.responseText);
     var myobj3 = JSON.parse(myobj2);
var myObj = JSON.parse(myobj3);
//var myObj = myobj4[0]['Temperatura'][1]['Umidade'][2]['Pressao'][3]['CO2'];


console.log(myObj);
      plotTemperature(myObj);

    }
  };
  xhr.open("GET", "/log", true);
  xhr.send();
}







