var timerStartTime = null;
var timer = null;
var websocket = null;
var serverBrowserTimeDelta = 0;
// 判断浏览器是否支持 WebSocket
if ('WebSocket' in window) {
	websocket = new WebSocket("ws://" + document.domain + ":9976/");
	websocket.onopen = function() {

	}
	websocket.onerror = function() {

	}
	websocket.onclose = function() {

	}
	websocket.onmessage=function(msg){
		var j = JSON.parse(msg.data);
		if (j.hasOwnProperty('headshotCount')) {
			document.getElementById("headshotCount").innerHTML = j.headshotCount;
		}
		if (j.hasOwnProperty('timerState')) {
			if (j.timerState == 0x03) { // 归零然后开始
				if (j.hasOwnProperty('serverTimestamp')) {
					// 服务器与浏览器的时差
					serverBrowserTimeDelta = j.serverTimestamp - new Date();
				}
				timerStartTime = new Date(j.startTimestamp);
				if (timer != null) {
					clearInterval(timer);
				}
				timer = setInterval(function() { timerFunc(timerStartTime) }, 50);
			} else if (j.timerState == 0x02) {  // 继续计时
				if (j.hasOwnProperty('startTimestamp')) {
					if (j.hasOwnProperty('serverTimestamp')) {
						// 服务器与浏览器的时差
						serverBrowserTimeDelta = j.serverTimestamp - new Date();
					}
					timerStartTime = new Date(j.startTimestamp);        // 与服务端矫正时间
				}
				if (timer != null) {
					clearInterval(timer);
				}
				timer = setInterval(function() { timerFunc(timerStartTime) }, 50);
			} else if (j.timerState == 0x05) {  // 暂停计时
				if (timer != null) {
					clearInterval(timer);
					timer = null;
				}
			} else if (j.timerState == 0x04) {  // 停止计时
				if (timer != null) {
					clearInterval(timer);
					timer = null;
				}
				timerStartTime = null;
			} else if (j.timerState == 0x01) {  // 归零
				if (timer != null) {
					clearInterval(timer);
					timer = null;
				}
				timerStartTime = null;
				document.getElementById("timerMinute").innerHTML = "00";
				document.getElementById("timerSecond").innerHTML = "00";
				document.getElementById("timerMs").innerHTML = "00";
			}
		}
	}
	// 监听窗口关闭事件，当窗口关闭时，主动去关闭websocket连接，防止连接还没断开就关闭窗口，server端会抛异常。
	window.onbeforeunload = function () {
		if (timer != null) {
			clearInterval(timer);
			timer = null;
			timerStartTime = null;
		}
		websocket.close();
	}
}
else {
	alert('The browser not support websocket')
}

function timerFunc(timerStartTime) {
	var now = new Date();
	var deltaTime = now - timerStartTime + serverBrowserTimeDelta;
	var m = parseInt(deltaTime / 1000 / 60);
	var s = parseInt(deltaTime / 1000) % 60;
	var ms = parseInt((deltaTime % 1000) / 10);
	document.getElementById("timerMinute").innerHTML = m.toString().padStart(2, '0');
	document.getElementById("timerSecond").innerHTML = s.toString().padStart(2, '0');
	document.getElementById("timerMs").innerHTML = ms.toString().padStart(2, '0');
}
