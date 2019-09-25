
var ws;//var ws = new WebSocket("ws://" + location.hostname + "/ws");

var temperatura = document.getElementById("temperatura");
var lumenes = document.getElementById("lumenes");

function connect() {
    ws = new WebSocket("ws://" + location.hostname + "/ws");
    ws.onopen = function () {
        // subscribe to some channels
        console.log("WebSocket Open");
    };

    ws.onmessage = function (evt) {
        var json = JSON.parse(evt.data);
        if(typeof json.lumenes !== "undefined"){
            let rgb = (255/4095)*json.lumenes;
            document.body.style.backgroundColor = "rgba("+rgb+","+rgb+","+rgb+", 0.75)";
            lumenes.innerText = json.lumenes+"lm";
        }
        if(typeof json.temperatura !== "undefined"){
            temperatura.innerText = json.temperatura+"°C";
        }
        console.log(json);
    };

    ws.onclose = function (evt) {
        console.log('Socket is closed. Reconnect will be attempted in 1 second.', evt.reason);
        setTimeout(function () {
            connect();
        }, 1000);
    };

    ws.onerror = function (err) {
        console.error('Socket encountered error: ', err.message, 'Closing socket');
        ws.close();
    };
}

connect();