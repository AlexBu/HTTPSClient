var mygrid = null;
$(function() {
	
	mygrid = new dhtmlXGridObject('gridbox');
	mygrid.setImagePath(dhtmlx_img_path);
	//mygrid.setHeader("序号,车次,查询区间,#cspan,区间运行时刻,#cspan,#cspan,余票信息,#cspan,#cspan,#cspan,#cspan,#cspan,#cspan,#cspan,#cspan,列车信息");
	//mygrid.attachHeader("#rspan,#rspan,发站,到站,发时,到时,历时,硬座,软座,硬卧,软卧,特等座,一等座,二等座,高级软卧,无座,等级");
	//去掉观光座、一等包座前
//	mygrid.setHeader("<div style='text-align:center;'>车次&nbsp;&nbsp;</div>,<div style='text-align:center;'>查询区间</div>,#cspan,#cspan,<div style='text-align:center;'>余票信息</div>,#cspan,#cspan,#cspan,#cspan,#cspan,#cspan,#cspan,#cspan,#cspan,#cspan,#cspan,#cspan,<div style='text-align:center;'>购票&nbsp;&nbsp;</div>");
//	mygrid.attachHeader("#rspan,发站,到站,历时,商务座,观光座,一等包座,特等座,一等座,二等座,高级软卧,软卧,硬卧,软座,硬座,无座,其他,#rspan");
//	mygrid.setInitWidths("56,90,90,44,44,44,58,44,44,44,58,43,43,43,43,43,43,50");
//	mygrid.setColAlign("center,center,center,center,center,center,center,center,center,center,center,center,center,center,center,center,center,center,center");
//	mygrid.setColTypes("ro,ro,ro,ro,ro,ro,ro,ro,ro,ro,ro,ro,ro,ro,ro,ro,ro,ro");
	// 去掉观光座、一等包座
	mygrid.setHeader("<div style='text-align:center;'>车次&nbsp;&nbsp;</div>,<div style='text-align:center;'>查询区间</div>,#cspan,#cspan,<div style='text-align:center;'>余票信息</div>,#cspan,#cspan,#cspan,#cspan,#cspan,#cspan,#cspan,#cspan,#cspan,#cspan,<div style='text-align:center;'>购票&nbsp;&nbsp;</div>");
	mygrid.attachHeader("#rspan,发站,到站,历时,商务座,特等座,一等座,二等座,高级软卧,软卧,硬卧,软座,硬座,无座,其他,#rspan");
	//显示预售期列宽
	mygrid.setInitWidths("62,96,96,50,45,45,45,45,59,39,39,39,39,39,38,140");
	//不显示预售期列宽
//	mygrid.setInitWidths("62,96,96,50,50,50,50,50,64,49,49,49,49,49,49,62");
	mygrid.setColAlign("center,center,center,center,center,center,center,center,center,center,center,center,center,center,center,center");
	mygrid.setColTypes("ro,ro,ro,ro,ro,ro,ro,ro,ro,ro,ro,ro,ro,ro,ro,ro");
	mygrid.enableTooltips("false");
	//mygrid.setColSorting("str,str,str,str,str,str,str,str,str,str,str,str,str,str,str,str,str,str");
	mygrid.init();
	mygrid.enableAlterCss("even","uneven");
	mygrid.setSkin("light");
	//mygrid.splitAt(2);
	//mygrid.startFastOperations();
	//mygrid.addRow("1","2","3","4","5","22","22","22","22","22","22","22","22","22","22","22","22","22");
   
	mygrid.parse(new Array(),"jsarray");
	dhtmlxError.catchError("parse",function(a,b,data){
		renameButton('research_u');
		$("#submitQuery").click(sendQueryFunc);
		if(canBuyStudentTicket=='Y'){
			stu_renameButton('research_u');
			$("#stu_submitQuery").attr("disabled",false);
		}
    });
	
	//绑定查询按钮单击事件开始
	$("#submitQuery").click(sendQueryFunc);
	$("#stu_submitQuery").click(sendQueryFunc);
	//绑定查询按钮单击事件开始
	if(isStudentTicketDateValid()){
		canBuyStudentTicket='Y';
		$("#stu_submitQuery").attr("disabled",false);
	}else{
		canBuyStudentTicket='N';
		stu_invalidQueryButton();
	}
	 
	 //隐藏和显示高级查询条件开始
//	 slidediv('#click1', '#show1');
	 //隐藏和显示高级查询条件结束
	 
	 $("#seatType").change(function() {
		 $("#seatNum").val("0");
		});

//	 var tip =  '<span>简码为车站简拼前两位或三位，简拼超过三位使用前两位加最后一位，例如上海虹桥SHQ。</span>';
	 var tip =  '';
	 
	 
	 if(typeof(resignOldNum) == "undefined"){ 
	 }else{
//		 tip+='<br><span>3、改签车票时，不能变更出发地、目的地、票种和身份信息。</span>';
		 tip+='<span>改签车票时，不能变更出发地、目的地、票种和身份信息。</span>';
	 }
	
	 $("#tip").append(tip);	 
	 
	 $("#seatNum").change(function() {
		    var currentSelected = $(this).find("option:selected");
		    if(currentSelected.val()!=0){
			addSeatTypeAndNum("#seatType", "#seatNum", "#seatAddResult");
		    }
		});
	 
	 //车次改变清空预订席别数量
	 $("#trainCodeText").change(function(event){
		 $("#seatAddResult").empty();
	 });
	 
	 
	//列车等级改变清空预订席别数量
	 if($(":checkbox[name='trainClassArr']").length>0){
	 $(":checkbox[name='trainClassArr']").each(function(index,dom){
			 $(dom).click(function(){
				 $("#seatAddResult").empty();
				 $("#seatNum").val("0"); 
			 });
		 });
      };
	
    
});

String.prototype.replaceAll  = function(s1,s2){   
    return this.replace(new RegExp(s1,"gm"),s2);
};

