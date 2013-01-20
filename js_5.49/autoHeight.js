function doIframe() {
	o = document.getElementsByTagName('iframe');
	for ( var i = 0; i < o.length; i++) {
		if (/\bautoHeight\b/.test(o[i].className)) {
			setHeight(o[i]);
			addEvent(o[i], 'load', doIframe);
		}
	}
}

function setHeight(e) {
	// 目前测试的ie8、firefox3.6.3、google、搜狗浏览器都可以使用
	try {
		var height = e.contentWindow.document.body.scrollHeight;
		if (height < 300) {
			height = 300;
		}
		e.height = height + 30;

		var ictheight = height + 80;
		if(userAgent() != "ie9"){
			document.getElementById('ictiframe').src = "http://www.12306.cn/mormhweb/ggxxfw/wbyyzj/201105/t20110529_1905.jsp?height="
				+ ictheight;
		}
	} catch (ex) {
		e.height = 710;
		if(userAgent() != "ie9"){
			document.getElementById('ictiframe').src = "http://www.12306.cn/mormhweb/ggxxfw/wbyyzj/201105/t20110529_1905.jsp?height=" + 780;
		}
	}
}
function addEvent(obj, evType, fn) {

	if (obj.addEventListener) {
		obj.addEventListener(evType, fn, false);
		return true;
	} else if (obj.attachEvent) {

		obj.detachEvent("on" + evType, fn);

		var r = obj.attachEvent("on" + evType, fn);
		return r;
	} else {
		return false;
	}
}

function userAgent() {

	var ua = navigator.userAgent;

	ua = ua.toLowerCase();

	var match = /(webkit)[ \/]([\w.]+)/.exec(ua) ||

	/(opera)(?:.*version)?[ \/]([\w.]+)/.exec(ua) ||

	/(msie) ([\w.]+)/.exec(ua) ||

	!/compatible/.test(ua) && /(mozilla)(?:.*? rv:([\w.]+))?/.exec(ua) || [];
	// 如果需要获取浏览器版本号：match[2]
	if(match[1] == "msie"){
		if (parseInt(match[2]) === 7) // ie7
		{
			if(ua.indexOf("trident/5.0") != -1){// ie9  此处为当ie9处于兼容模式下时的情况
				return "ie9";
			}else{
				return match[1];
			}
		}else if (parseInt(match[2]) === 9) // ie9
		{
			return "ie9";
		}else{
			return match[1];
		}
	}else{
		return match[1];
	}
}

if (document.getElementById && document.createTextNode) {
	addEvent(window, 'load', doIframe);
}
