/*
 * 
 * example:
 * w1 = new OrderQueueWaitTime("dc",waitMethod,finishMethod);
 * w1.start();
 * 
 * function waitMethod(tourFlag,dispSecond,showTime){}
 * function finishMethod(tourFlag,returnTime,waitObj){}
 * 
 */
function OrderQueueWaitTime(tourFlag,waitMethod,finishMethod){
	this.tourFlag = tourFlag;
	this.waitMethod = waitMethod;
	this.finishMethod = finishMethod;
	
	this.dispTime = 1;
	this.nextRequestTime = 1;
	this.isFinished = false;
	this.waitObj;
}

OrderQueueWaitTime.prototype.start = function(){
	var t = this;
	t.timerJob();
	window.setInterval(function(){t.timerJob();}, 1000);
};

OrderQueueWaitTime.prototype.timerJob = function(){
	if(this.isFinished){
		return;
	}
	//alert(this.dispTime);
	if(this.dispTime <= 0){
		this.isFinished = true;
		//alert("ok");
		this.finishMethod(this.tourFlag,this.dispTime,this.waitObj);
		return;
	}
	
	if(this.dispTime == this.nextRequestTime){
		this.getWaitTime();
	}
	
	//格式化时间，把秒转换为时分秒
	var second = this.dispTime;
	var show_time = "";
//	var hour = parseInt(second / 3600);  //时
//	if(hour > 0){
//		show_time = hour + "小时";
//		second = second % 3600;
//	}
//	var minute = parseInt(second / 60);  //分
//	if(minute >= 1){
//		show_time = show_time + minute + "分";
//		second = second % 60;
//	} else if(hour >= 1 && second > 0){
//		show_time = show_time + "0分";
//	}
//	if(second > 0){
//		show_time = show_time + second + "秒";
//	}
	
	//时间大于等于1分钟的，按分钟倒计时，小于1分钟的按1分钟显示
	var minute = parseInt(second / 60);
	if (minute >= 1) {
		show_time = minute + "分";
		second = second % 60;
	} else {
//		show_time = second + "秒";
		show_time = "1分";
	}
	
	this.waitMethod(this.tourFlag, this.dispTime>1?--this.dispTime:1, show_time);
};

OrderQueueWaitTime.prototype.getWaitTime = function(){
	var t = this;
	$.ajax(
	  { url :'myOrderAction.do?method=queryOrderWaitTime',
		type :"GET",
		data:{tourFlag : t.tourFlag},
		dataType: "json", 
		success:function(data){
			if(data != null){
				t.waitObj = data;
				t.dispTime = data.waitTime;
				var flashWaitTime = parseInt(data.waitTime/1.5);
				flashWaitTime = flashWaitTime>60?60:flashWaitTime;
				var nextTime = data.waitTime - flashWaitTime;
				
				t.nextRequestTime = nextTime<=0?1:nextTime;
				//alert(t.dispTime);
				//alert(t.nextRequestTime);
			}
		 },
		error:function(XMLHttpRequest, textStatus, errorThrown) {
			 //alert("计算排队时间失败！");
			 return false;
		}
	});
};