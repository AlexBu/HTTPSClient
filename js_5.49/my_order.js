//对应my_order.jsp、my_order_not_complete.jsp

function batchResign(flag, sequenceNo){
	var formObj = document.getElementById("form_"+flag+"_"+sequenceNo);
	var key;
	var $checked = $("input:checked",formObj);
    
	for(var i=0;i<$checked.length;i++){
	     if(key){
	    	 key=key+$($checked[i]).val()+":";
	    	} else {
	    		key=$($checked[i]).val()+":";
		    }
	     }
	 if(!key){
			alert("请选择要改签的车票！");
			return;
 	}

	var $fromchecked1 = $("input:checked[resignflag!=Y]",formObj);
    var messageStr1 = "您选择的车票";
    if($fromchecked1.length>0){
           	 jQuery.each($fromchecked1,function(index,data){
           		 messageStr1= messageStr1+"["+$(data).attr("alertinfo")+"]";
               	 });
           	 messageStr1 = messageStr1 +"不能改签，请不要勾选。";
           	 alert(messageStr1);
           	 return ;
    }
    var $fromchecked = $("input:checked[resignflag!=Y]",formObj);
    var messageStr = "您选择的车票";
       
     var cannotresign = false;
	 var $willResign = $("input:checked[resignflag=Y]",formObj);
	 if($willResign.length>0){
    	 var standkey = $($willResign.get()).attr("resignkey");
       	 jQuery.each($willResign,function(index,data){
           	 if(standkey != $(data).attr("resignkey")){
       		    messageStr= messageStr+"["+$(data).attr("alertinfo")+"]";
       		    cannotresign = true;
           	 }
          });
       	 messageStr = messageStr +"不能和其他票同时批量改签，请不要勾选。请选择同一订单、同一日期、同一车次、相同批次、相同发到站、相同席别的车票进行批量改签。";
         if(cannotresign){
		         alert(messageStr);
		         //alert("您申请改签的车票不符合相同日期、车次、席别和发到站的批量改签条件！");
	           	 return;
         }
      }
   $.ajax(
	  {
		url :'queryResignAction.do?method=checkApplyResign',
		type :"POST",
		dataType :"text",
		data:{
			ticketkey:$($checked[0]).val()
			},
		success:function(data,textStatus){
			 if(data!='Y'){
				 alert(data);
			 } else {
				 
				 /*
	    	     var form = document.getElementById("myOrderForm");
	    	 	 form.action='myOrderAction.do?method=resign';
	    	 	 $('#from_order_date').val($('#queryOrderDTOfrom_order_date').val());
	    	     $('#to_order_date').val($('#queryOrderDTOto_order_date').val());
	    	     $('#ticket_key').val(key);
	    	     form.submit();
				 */
	    	    var form=document.getElementById("myOrderForm");
	    	 	form.action="myOrderAction.do?method=resign&from_order_date="+$('#queryOrderDTOfrom_order_date').val()+"&to_order_date="+$('#queryOrderDTOto_order_date').val()+"&ticket_key="+key;
	    	 	form.target="_self";
	    	 	form.submit();
			 }
		 },
		error:function(XMLHttpRequest, textStatus, errorThrown) {
					 alert("网络忙，请稍后重试");
			 }
		});
}

$(document).ready(function(){
	//设置默认的属性
	$("#queryForm").validate({
		    rules: {
			'queryOrderDTO.sequence_no': {
				checkSequenceNoLen: true,
				checkSequenceNo: true
			},
			'queryOrderDTO.train_code': {
				checkTrainCodeLen: true,
				checkTrainCode: true
			},
		    'queryOrderDTO.name':{
                byteRangeLength: [3,20], 
                checkNameBlank: true
	        },
			'queryOrderDTO.id_no':{
	        	checkIdNoLen: true,
	        	checkIdValid: true
			}
		},
		messages: {
			'queryOrderDTO.sequence_no': {
				checkSequenceNoLen: "订单号的长度为10个字符！<br/>",
				checkSequenceNo: "订单号只允许输入字母和数字！<br/>"
			},
			'queryOrderDTO.train_code': {
				checkTrainCodeLen: "车次不允许多于8个字符！<br/>",
				checkTrainCode: "车次只允许输入字母和数字！<br/>"
			},
			'queryOrderDTO.name':{
				byteRangeLength: "乘客姓名允许输入的字符串在3-20个字符之间！<br/>",
				checkNameBlank: "乘客姓名只能包含中文或者英文！<br/>"
			},
			'queryOrderDTO.id_no': {
				checkIdNoLen: "证件号码不允许多于18个字符！<br/>",
				checkIdValid: "证件号码只允许输入字母和数字！<br/>"
			}
		},
		errorLabelContainer: $("#error_info"),
		errorPlacement: function(error, element) {
			if ( element.is(":radio") )
				error.appendTo( element.parent().next() );
			else if ( element.is(":checkbox") )
				error.appendTo ( element.next() );
			else
				error.appendTo( element.parent().next() );
		}
	});

	// 分页相关页面元素的显示及隐藏
	try {
		if(recordCount != undefined && recordCount != null && recordCount != "" && recordCount != "null") {
			if(recordCount > 0) {
				ele_show_and_hide_has_record();
			} else {
				ele_show_and_hide_has_not_record();
			}
		}
	} catch (e) {
		// 未完成订单页面my_order_not_complete.jsp未定义recordCount，会报错"recordCount is not defined"
	}
	
	var myDate=new Date();
	myDate.setDate(myDate.getDate()-179);
	var min_date=myDate.getFullYear()+"-"+(myDate.getMonth()+1)+"-"+myDate.getDate();
	
   // 订单日期控件初始化
   $("#queryOrderDTOfrom_order_date").focus(function(){WdatePicker({minDate:min_date,isShowClear:false,readOnly:true,qsEnabled:false,onpicked:function(){
		$('#submitId').focus();
	}});});
   $("#queryOrderDTOto_order_date").focus(function(){WdatePicker({minDate:min_date,isShowClear:false,readOnly:true,qsEnabled:false,onpicked:function(){
		$('#submitId').focus();
	}});});
   
   // 乘车日期控件初始化
   $("#queryOrderDTOfrom_train_date").focus(function(){WdatePicker({maxDate:max_date,minDate:min_date,isShowClear:true,readOnly:true,qsEnabled:false,onpicked:function(){
		$('#submitId').focus();
	}});});
   $("#queryOrderDTOto_train_date").focus(function(){WdatePicker({maxDate:max_date,minDate:min_date,isShowClear:true,readOnly:true,qsEnabled:false,onpicked:function(){
		$('#submitId').focus();
	}});});

	//为 <div id = "toggler">添加toggler事件
   
   $.each($('.jdan_title[dpart=all]'),function(index,data){
	   var $data = $(data);
	   $(this).click(function (){
		   	$('#detail_'+$data.attr("id")).toggle();
		   	var imgobj = $('#img_'+$data.attr("id"));
		   	if(imgobj.attr("class")=="img_zhedie"){
		   		imgobj.removeClass();
		   		imgobj.addClass("img_dakai");
		   	}else{
		   		imgobj.removeClass();
		        imgobj.addClass("img_zhedie");
		   	}
		   	parent.doIframe() ;
		   });
	   
	});
   
   
   $.each($('.jdan_title[dpart=pay]'),function(index,data){
	   var $data = $(data);
	   $(this).click(function (){
		   	$('#detail_'+$data.attr("id")).toggle();
		   	var imgobj = $('#img_'+$data.attr("id"));
		   	if(imgobj.attr("class")=="img_zhedie"){
		   		imgobj.removeClass();
		   		imgobj.addClass("img_dakai");
		   	}else{
		   		imgobj.removeClass();
		        imgobj.addClass("img_zhedie");
		   	}
		   	parent.doIframe() ;
		   });
	   
	});
   
   
   
   $.each($('.jdan_title[dpart=book]'),function(index,data){
	   var $data = $(data);
	   $(this).click(function (){
		   	$('#detail_'+$data.attr("id")).toggle();
		   	var imgobj = $('#img_'+$data.attr("id"));
		   	if(imgobj.attr("class")=="img_zhedie"){
		   		imgobj.removeClass();
		   		imgobj.addClass("img_dakai");
		   	}else{
		   		imgobj.removeClass();
		        imgobj.addClass("img_zhedie");
		   	}
		   	parent.doIframe() ;
		   });
	   
	});
   

   //显示当前tab页
   var tab_id = $("#queryOrderDTOlocation_code").val();
   if(tab_id!=""&&tab_id!=undefined){
	   $("#nav_list li").attr("class","");
	   $('#nav_cont h1').attr('class','cont_tag');
	  
	   $('#'+tab_id).attr("class","slelcted");
	   $('#tag'+tab_id).attr("class","cont_tag_show");
	    
	   var $divtat = $('#tag'+tab_id+' div[class="jdan_title"]');
	   if($divtat.length>0){
		 //  $($divtat.get(0)).click();
		   if(selectedOrder){
			   var selected = null;
			   if(tab_id=='_1'){
				   selected = $('#all_'+selectedOrder);
			   }
			   if(tab_id=='_2'){
				   selected = $('#pay_'+selectedOrder);
			   }
			   if(tab_id=='_3'){
				   selected = $('#book_'+selectedOrder);
			   }
			  selected.click();
		   }else{
			   $($divtat.get(0)).click();
		   }
	   }
   }else{
	   $($('.jdan_title')[0]).click();//默认展开第一个
   }
   
    // 因为数据的复制需要几秒时间，因而延迟3秒，尽量避免查不到订单的情况
    
    if(disableBtn != undefined && disableBtn != null && disableBtn != "" && disableBtn != "null") {
	    if (disableBtn == 'true') {
		    disablesubmit();
			window.setTimeout('ablesubmit()', 3000);
		}
    }
});

//提交订单查询
function compareDate() {
	disablesubmit();
	 if(!checkSeqNoValid()){
		 alert("订单号只能由数字和字母组成，并且长度不能大于10个字符");
		 ablesubmit();
		 return false;
	 }
	/* compareDateUtil($("#queryOrderDTOto_order_date").val(),$("#queryOrderDTOfrom_order_date").val(),
			 $("#queryOrderDTOto_train_date").val(),$("#queryOrderDTOfrom_train_date").val());*/
	if(!compareDateUtil2($("#queryOrderDTOto_order_date").val(),$("#queryOrderDTOfrom_order_date").val())){
		ablesubmit();
	}
}
function disablesubmit(){
	$("#submitId").removeClass("search_u");
	$("#submitId").addClass("search_x");
	$("#submitId").attr("disabled",true); 	
}
function ablesubmit(){
	$("#submitId").removeClass("search_x");
	$("#submitId").addClass("search_u");
	$("#submitId").attr("disabled",false); 	
}
function checkSeqNoValid(){
	var seqno = $("#queryOrderDTOsequence_no").val();
	if(seqno!=""){
		if(!(/^[a-zA-Z0-9]+$/.test(seqno))){
			return false;
		}
		if(seqno.length>10){
			return false;
		}
	}
	return true;
}

function compareDateUtil(DateOne, DateTwo, DateThree, DateFour) {

	var li_id = $('#nav_list li[class=slelcted]').attr("id");
	
	$('#queryOrderDTOlocation_code').val(li_id);//选取当前 tab页的id.
	
	var OneMonth = DateOne.substring(5,DateOne.lastIndexOf ("-"));
	var OneDay = DateOne.substring(DateOne.length,DateOne.lastIndexOf ("-")+1);
	var OneYear = DateOne.substring(0,DateOne.indexOf ("-"));
	
	var TwoMonth = DateTwo.substring(5,DateTwo.lastIndexOf ("-"));
	var TwoDay = DateTwo.substring(DateTwo.length,DateTwo.lastIndexOf ("-")+1);
	var TwoYear = DateTwo.substring(0,DateTwo.indexOf ("-"));
	
	var ThreeMonth = DateThree.substring(5,DateThree.lastIndexOf ("-"));
	var ThreeDay = DateThree.substring(DateThree.length,DateThree.lastIndexOf ("-")+1);
	var ThreeYear = DateThree.substring(0,DateThree.indexOf ("-"));
	
	var FourMonth = DateFour.substring(5,DateFour.lastIndexOf ("-"));
	var FourDay = DateFour.substring(DateFour.length,DateFour.lastIndexOf ("-")+1);
	var FourYear = DateFour.substring(0,DateFour.indexOf ("-"));
	
	if (Date.parse(OneMonth+"/"+OneDay+"/"+OneYear) <
	Date.parse(TwoMonth+"/"+TwoDay+"/"+TwoYear))
	{
		alert("订单的开始日期不能大于结束日期，请重新选择。");
		return false;   
	} else if (Date.parse(ThreeMonth+"/"+ThreeDay+"/"+ThreeYear) <
	Date.parse(FourMonth+"/"+FourDay+"/"+FourYear))
	{
		alert("乘车始日不能大于乘车止日，请重新选择。");
		return false;   
	} else {
		//var form_ = $("#queryForm");
		if($("#queryForm").valid()) { 
			ele_show_and_hide_submit();
		} else {
			return false;
		}
		
		var form_ = document.getElementById("queryForm");
		form_.action="myOrderAction.do?method=queryMyOrder";
		form_.target="_self";
		form_.submit(); 
		
		return true;
	}
}

function compareDateUtil2(DateOne, DateTwo) {

	var li_id = $('#nav_list li[class=slelcted]').attr("id");
	
	$('#queryOrderDTOlocation_code').val(li_id);//选取当前 tab页的id.
	
	var OneMonth = DateOne.substring(5,DateOne.lastIndexOf ("-"));
	var OneDay = DateOne.substring(DateOne.length,DateOne.lastIndexOf ("-")+1);
	var OneYear = DateOne.substring(0,DateOne.indexOf ("-"));
	
	var TwoMonth = DateTwo.substring(5,DateTwo.lastIndexOf ("-"));
	var TwoDay = DateTwo.substring(DateTwo.length,DateTwo.lastIndexOf ("-")+1);
	var TwoYear = DateTwo.substring(0,DateTwo.indexOf ("-"));
	
	
	
	if (Date.parse(OneMonth+"/"+OneDay+"/"+OneYear) <
	Date.parse(TwoMonth+"/"+TwoDay+"/"+TwoYear))
	{
		alert("订单的开始日期不能大于结束日期，请重新选择。");
		return false;   
	} else {
		$("#submitId").removeClass("search_u");
		$("#submitId").addClass("search_x");
		$("#submitId").attr("disabled",false); 		
		//var form_ = $("#queryForm");
		if($("#queryForm").valid()) { 
			ele_show_and_hide_submit();
		} else {
			return false;
		}
		
		var form_ = document.getElementById("queryForm");
		form_.action="myOrderAction.do?method=queryMyOrder";
		form_.target="_self";
		form_.submit(); 
		
		return true;
	}
}

//订单详细信息 展示
function tableToggle(obj){
   var detail = 'detail'+obj.attr('id');
   $('#'+detail).toggle();
}

//改签
function resignTicket(key){
	var form=document.getElementById("myOrderForm");
	form.action='myOrderAction.do?method=resign';
	//alert("改签");
	$('#from_order_date').val($('#queryOrderDTOfrom_order_date').val());
    $('#to_order_date').val($('#queryOrderDTOto_order_date').val());
    $('#ticket_key').val(key);
    //alert($('#ticket_key').val());
    //form.target="_self";
    //alert(form.action);
    form.submit();
    //return true;
}

// 退票
function refundTicket(obj,ticket_key){
	// 置按钮无效
	$(obj).attr("disabled",true);
	$(obj).addClass("long_button_x");
    $('#from_order_date').val($('#queryOrderDTOfrom_order_date').val());
    $('#to_order_date').val($('#queryOrderDTOto_order_date').val());
    $('#ticket_key').val(ticket_key);
    var form = document.getElementById("myOrderForm");
//  form.action='ticketAction.do?method=initRefundTicket&ticket_key='+ticket_key;
    if($("#refundLogin").val()=='Y'){
    	form.action='order/ticketAction.do?method=initRefundTicket&ticket_key='+ticket_key;
	}else{
		form.action='ticketAction.do?method=initRefundTicket&ticket_key='+ticket_key;
	}
    form.target="_self";
    form.submit();
}

//取消订单 
function  cancelOrder(sequenceNo){
	if(confirm("您确认取消该订单吗？\r\n一天内3次申请车票成功后取消订单，当日将不能在网站购票！")) {
		if($("#transferForm").valid()){
			$(":button").attr("disabled",true);
			$(":button").addClass("long_button_x");
		}
		var form=document.getElementById("transferForm");
		$("#sequence_no").val(sequenceNo);
		form.target="_self";
	    form.submit();
	}
}

//取消排队订单 
function  cancelWaitOrder(tourFlag){
	if(confirm("您确认取消该订单吗？")) {
		if($("#transferForm").valid()){
			$(":button").attr("disabled",true);
			$(":button").addClass("long_button_x");
		}
		var form=document.getElementById("transferForm");
		$("#tour_flag").val(tourFlag);
		$("#sequence_no").val("waiting_order");
		form.target="_self";
	    form.submit();
	}
}
//取消改签
function  cancelResignOrder(sequenceNo){
	if(confirm("您确认取消改签吗？\r\n一天内3次申请车票成功后取消订单，当日将不能在网站购票！")) {
		var form=document.getElementById("transferForm");
		form.action='orderAction.do?method=cancelMyOrderNotCompleteResign';
		$("#sequence_no").val(sequenceNo);
		form.target="_self";
		if($("#transferForm").valid()){
			$(":button").attr("disabled",true);
			$(":button").addClass("long_button_x");
		}
		form.submit();
	}
}

//继续支付
function  epayOrder(orderSequence_no,con_pay_type){
	// 这段读取key的代码是为了应对 继续改签支付
	var key;
	var $checked = $("#checkbox_pay");
	for(var i=0;i<$checked.length;i++){
	    if(key){
	    	 key=key+$($checked[i]).val()+":";
    	} else {
    		key=$($checked[i]).val()+":";
	    }
	}
	
	
	
	var form=document.getElementById("myOrderForm");
	$('#ticket_key').val(key);
	form.action='myOrderAction.do?method=laterEpay&orderSequence_no='+orderSequence_no + '&con_pay_type=' + con_pay_type;
    form.submit();
}

//打印订单
function  printTickets(orderSequence_no){
	
	var form=document.getElementById("myOrderForm");
	form.target="_blank";
	form.action='myOrderAction.do?method=printTickets&orderSequence_no='+orderSequence_no;
	form.submit();
}


//继续确认
function  commitOrder(orderSequence_no){
	
	var form=document.getElementById("myOrderForm");
	form.action='myOrderAction.do?method=laterConfirmWindow&orderSequence_no='+orderSequence_no;
    form.submit();
}

// 点击全部
function checkAll(flag, sequenceNo){
	if ($("#link_" + flag + "_" + sequenceNo).attr("checked") == 'off') {
		$(":checkbox[name='checkbox_" + flag + "_" + sequenceNo + "']").attr("checked", true);
		var $link = $("[name='link_" + flag + "_" + sequenceNo + "']");
		$link.each(function(i, obj) {
			$(obj).html("取消");
		});
		$("#link_" + flag + "_" + sequenceNo).attr("checked", "on");
	} else if ($("#link_" + flag + "_" + sequenceNo).attr("checked") == 'on') {
		$(":checkbox[name='checkbox_" + flag + "_" + sequenceNo + "']").attr("checked", false);
		var $link = $("[name='link_" + flag + "_" + sequenceNo + "']");
		$link.each(function(i, obj) {
			$(obj).html("全部");
		});
		$("#link_" + flag + "_" + sequenceNo).attr("checked", "off");
	}
}