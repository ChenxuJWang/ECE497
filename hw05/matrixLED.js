    var socket;
    var firstconnect = true,
        i2cNum  = "0x70",
	disp = [];
	var disp2 = [];
    
// Create a matrix of LEDs inside the <table> tags.
var matrixData1;
for(var j=7; j>=0; j--) {
	matrixData1 += '<tr>';
	for(var i=0; i<8; i++) {
	    matrixData1 += '<td><div class="LED1" id="id1'+i+'_'+j+
		'" onclick="LEDclick1('+i+','+j+')">'+
		i+','+j+'</div></td>';
	    }
	matrixData1 += '</tr>';
}

var matrixData2;
for(var j=7; j>=0; j--) {
	matrixData2 += '<tr>';
	for(var i=0; i<8; i++) {
	    matrixData2 += '<td><div class="LED2" id="id2'+i+'_'+j+
		'" onclick="LEDclick2('+i+','+j+')">'+
		i+','+j+'</div></td>';
	    }
	matrixData2 += '</tr>';
}

$('#matrixLED1').append(matrixData1);
$('#matrixLED2').append(matrixData2);

// The slider controls the overall brightness
$("#slider1").slider({min:0, max:15, slide: function(event, ui) {
	socket.emit("i2cset",  {i2cNum: i2cNum, i: ui.value+0xe0, disp: 1});
    }});

// Send one column when LED is clicked.
function LEDclick1(i, j) {  
//	alert(i+","+j+" clicked");
    disp[i] ^= 0x1<<j;
    socket.emit('i2cset', {i2cNum: i2cNum, i: 2*i, 
			     disp: '0x'+disp[i].toString(16)});
//	socket.emit('i2c', i2cNum);
    // Toggle bit on display
    if(disp[i]>>j&0x1 === 1) {
        $('#id1'+i+'_'+j).addClass('on');
    } else {
        $('#id1'+i+'_'+j).removeClass('on');
    }
}

// Send one column when LED is clicked.
function LEDclick2(i, j) {  
    disp2[i] ^= 0x1<<j;
    socket.emit('i2cset', {i2cNum: i2cNum, i: 2*i + 1, 
			     disp: '0x'+disp2[i].toString(16)});
//	socket.emit('i2c', i2cNum);
    // Toggle bit on display
    if(disp2[i]>>j&0x1 === 1) {
        $('#id2'+i+'_'+j).addClass('on2');
    } else {
        $('#id2'+i+'_'+j).removeClass('on2');
    }
}

    function connect() {
      if(firstconnect) {
        socket = io.connect(null);

        // See https://github.com/LearnBoost/socket.io/wiki/Exposed-events
        // for Exposed events
        socket.on('message', function(data)
            { status_update("Received: message " + data);});
        socket.on('connect', function()
            { status_update("Connected to Server"); });
        socket.on('disconnect', function()
            { status_update("Disconnected from Server"); });
        socket.on('reconnect', function()
            { status_update("Reconnected to Server"); });
        socket.on('reconnecting', function( nextRetry )
            { status_update("Reconnecting in " + nextRetry/1000 + " s"); });
        socket.on('reconnect_failed', function()
            { message("Reconnect Failed"); });

        socket.on('matrix',  matrix);

    socket.emit('i2cset', {i2cNum: i2cNum, i: 0x21, disp: 1}); // Start oscillator (p10)
    socket.emit('i2cset', {i2cNum: i2cNum, i: 0x81, disp: 1}); // Disp on, blink off (p11)
    socket.emit('i2cset', {i2cNum: i2cNum, i: 0xe7, disp: 1}); // Full brightness (page 15)
    /*
	i2c_smbus_write_byte(file, 0x21); 
	i2c_smbus_write_byte(file, 0x81);
	i2c_smbus_write_byte(file, 0xe7);
    */
        // Read display for initial image.  Store in disp[]
        socket.emit("matrix", i2cNum);

        firstconnect = false;
      }
      else {
        socket.socket.reconnect();
      }
    }

    function disconnect() {
      socket.disconnect();
    }

    // When new data arrives, convert it and display it.
    // data is a string of 16 values, each a pair of hex digits.
    function matrix(data) {
        var i, j;
        disp = [];
        disp2 = [];
        //        status_update("i2c: " + data);
        // Make data an array, each entry is a pair of digits
        data = data.split(" ");
        //        status_update("data: " + data);
        // Every other pair of digits are Green. The others are red.
        // Ignore the red.
        // Convert from hex.
        for (i = 0; i < data.length; i += 2) {
            disp[i/2] = parseInt(data[i], 16);
            disp2[i/2] = parseInt(data[i+1], 16);
        }
        //        status_update("disp: " + disp);
        // i cycles through each column
        for (i = 0; i < disp.length; i++) {
            // j cycles through each bit
            for (j = 0; j < 8; j++) {
                if (((disp[i] >> j) & 0x1) === 1) {
                    $('#id1' + i + '_' + j).addClass('on');
                } else {
                    $('#id1' + i + '_' + j).removeClass('on');
                }
                // RED
                if (((disp2[i] >> j) & 0x1) === 1) {
                    $('#id2' + i + '_' + j).addClass('on2');
                } else {
                    $('#id2' + i + '_' + j).removeClass('on2');
                }
            }
        }
    }

    function status_update(txt){
	$('#status').html(txt);
    }

    function updateFromLED(){
      socket.emit("matrix", i2cNum);    
    }

connect();

$(function () {
    // setup control widget
    $("#i2cNum").val(i2cNum).change(function () {
        i2cNum = $(this).val();
    });
});
