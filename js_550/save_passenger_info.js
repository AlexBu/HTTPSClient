var tourFlag;
var buttonOk = "取   消 ";
var buttonNo = "确   定 ";

// 提交订单前确认
function submit_form_confirm(formId, tFlag) {
	if (!submit_form_check(formId)) {
		return false;
	}
	var randVal = $("#rand").val();
	$("#confirmPassenger")
	.ajaxSubmit({
				url : 'confirmPassengerAction.do?method=checkOrderInfo&rand='
						+ randVal,
				type : "POST",
				data : {tFlag:tFlag},
				dataType : "json",
				success : function(data) {
					if ('Y' != data.errMsg) {
						alert(data.errMsg);
						$("#rand").val("");
						$("#img_rrand_code").click();
						$(":button").attr("disabled", false);
						$(":button").removeClass("long_button_x");
						return;
					} else if('N' == data.checkHuimd){
//						alert("对不起，由于您取消次数过多，今日将不能继续受理您的订票请求！");
						alert(data.msg);
						$(":button").attr("disabled", false);
						$(":button").removeClass("long_button_x");
						return;
					}else if('N' == data.check608){
//						alert("本车为实名制列车，实行一日一车一证一票制！");
						alert(data.msg);
						$(":button").attr("disabled", false);
						$(":button").removeClass("long_button_x");
						return;
					}
					else{
						showOrderDialog(tFlag);
					}
				},
				error : function(XMLHttpRequest, textStatus, errorThrown) {
					if(errorThrown.concat('登陆')){
						 alert("您离开页面的时间过长，请重新登录系统。");
						
					}else{
						alert("服务器繁忙，请稍候再试！");	
					}
					
					$(":button").attr("disabled", false);
					$(":button").removeClass("long_button_x");
					return;
				}
			});

}

function showOrderDialog(tFlag) {
	// 加入排队等待人数过多时的提示文案
	var queue_note = "";
	if (isAsync == '1' && $("#left_ticket").val() != "") {
		$
				.ajax({
					url : 'confirmPassengerAction.do?method=getQueueCount',
					type : "GET",
					data : {
						train_date : $("#start_date").val(),
						train_no : $("#train_no").val(),
						station : $("#station_train_code").val(),
						seat : $("#passenger_1_seat").val(),
						from : $("#from_station_telecode").val(),
						to : $("#to_station_telecode").val(),
						ticket : $("#left_ticket").val()
					},
					dataType : "json",
					success : function(data) {
						//queue_note = "尊敬的旅客，"+((data.theWaitTotalCount == undefined || data.theWaitTotalCount == null || data.theWaitTotalCount == "null") ? "" : "系统当前待处理的订单总数<font color='red'>" + data.theWaitTotalCount + "</font>，" )+"本次列车您选择的席别尚有余票"
						queue_note = "尊敬的旅客，本次列车您选择的席别尚有余票"
								+ getTicketCountDesc(data.ticket,
										$("#passenger_1_seat").val()) + "，";

						if (data.op_2) {
							queue_note += "目前排队人数已经超过余票张数，请您选择其他席别或车次，特此提醒。";
							// queue_note = "今日已有<font color=\"red\">" +
							// data.count +
							// "</font>人先于您提交相同的购票需求，目前排队人数已超过剩余票数，请您选择其他席别或车次。";

						} else {
							$(parent.document.body)
							.contents()
							.find(":button")
							.each(
									function(a) {
										if ($(this).html() == buttonNo) {
											$(this).attr("disabled",
													false).removeClass(
													"long_button_x");
										}
									});
							if (data.countT > 0) {
								queue_note += "目前排队人数<font color=\"red\">"
										+ data.countT + "</font>人，";
							}
							queue_note += "特此提醒。";
						}
						queue_note += "<br/><br/><font color=\"red\">请确认订单信息是否正确，如正确请点击“确定”，系统将为您随机分配席位。</font>";
						/*
						 * else if(data.op_1){ queue_note +=
						 * "目前排队人数已经超过余票张数，特此提醒。"; //queue_note = "今日已有<font
						 * color=\"red\">" + data.count + "</font>人先于您提交相同的购票需求，到处理您的需求时可能已无票，建议你们根据当前余票确定是否排队。"; }
						 */
						var note_elem = parent.document
								.getElementById("queue_note");
						if (note_elem != null) {
							note_elem.innerHTML = queue_note;
						}
					},
					error : function(XMLHttpRequest, textStatus, errorThrown) {
						return false;
					}
				});
	}

	tourFlag = tFlag;
	cardflag = false;// 是否使用护照
	var ticket_info = getTicketInfo();
	var passenger_info = getPassengerInfo();
	// 弹出窗口提示信息
	// 方式1
	// var msgtext = "<br/><p><b>您所要提交的订单信息如下，请确定是否正确（<font
	// color=\"red\">点击“确定”提交订单，点击“取消”返回修改</font>）：</b></p>";
	// msgtext += "<br/><p><b>一、车次信息：</b></p>";
	// msgtext += "<ul><li>"+ticket_info+"</li></ul>";
	// msgtext += "<p><b>二、乘车人身份信息：</b></p>";
	// msgtext += "<ul><li>"+passenger_info+"</li></ul>";
	// msgtext += "<p><b>注：系统将根据售出情况随机为您申请席位，暂不支持自选席位。</b></p>";
	// 方式2
	var titile = "提交订单确认";
	var msgtext = "<div class=\"submit-tips\">";
	msgtext += "<div class=\"submit-cont\">";
	msgtext += "<br/><div class=\"title\">车次信息</div>";
	msgtext += ticket_info;

	msgtext += "<div class=\"title\">乘车人信息</div>";
	msgtext += passenger_info;
	// if((to_station_code_=="LSO" || to_station_code_=="DAO" ||
	// to_station_code_=="ADO" || to_station_code_=="NQO"
	// ||to_station_code_=="THO" ) && cardflag == true) {
	// msgtext += "<p
	// style=\"color:#3177BF;\">注：1.系统将根据售出情况随机为您申请席位，暂不支持自选席位。</p>";
	// msgtext += "<p
	// style=\"color:#3177BF;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2.根据现行规定，外国人在购买进西藏火车票时，须出示西藏自治区外事办公室或旅游局、商务厅的批准函（电），或者出示中国内地司局级接待单位出具的、已征得自治区上述部门同意的证明信函。台湾同胞赴藏从事旅游、商务活动，须事先向西藏自治区旅游局或商务厅提出申请，购买进藏火车票时须出示有关批准函。</p>";
	// } else {
	// msgtext += "<p
	// style=\"color:#3177BF;\">注：系统将根据售出情况随机为您申请席位，暂不支持自选席位。</p>";
	// }
	msgtext += "<div id=\"queue_note\" style=\"font-size:14px;padding:20px;\">"
			+ queue_note + "</div>";
	msgtext += "</div></div>";
	parent.pop_confirm(titile, msgtext, failfunc, successfunc, buttonNo,
			buttonOk, 692);
	//提交订单之前，设置弹出窗口的"确认"按钮为disable，提交之后再根据请求结果设置成false
	$(parent.document.body).contents().find(":button")	.each(function(a) {
				if ($(this).html() == buttonNo) {
					$(this).attr("disabled",
							true).addClass(
							"long_button_x");
				}
			});
	submitfunction.apply(this);
}

function getTicketCountDesc(mark, seat) {
	rt = "";
	seat_1 = -1;
	seat_2 = -1;
	i = 0;
	while (i < mark.length) {
		s = mark.substr(i, 10);
		c_seat = s.substr(0, 1);
		if (c_seat == seat) {
			count = s.substr(6, 4);
			while (count.length > 1 && count.substr(0, 1) == "0") {
				count = count.substr(1, count.length);
			}
			count = parseInt(count);
			if (count < 3000) {
				seat_1 = count;
			} else {
				seat_2 = (count - 3000);
			}
		}
		i = i + 10;
	}

	if (seat_1 > -1) {
		rt += "<font color='red'>" + seat_1 + "</font>张";
	}
	if (seat_2 > -1) {
		rt += ",无座<font color='red'>" + seat_2 + "</font>张";
	}
	return rt;
}

// 弹出确认订单提示信息附带动作
var submitfunction = function() {
	// $("#confirmPassenger").submit();
};

// 取消提交订单
var successfunc = function() {
	$(":button").attr("disabled", false);
	$(":button").removeClass("long_button_x");
};

// 确定提交订单
var failfunc = function() {
	// alert(isAsync);
	if (isAsync == '1') {
		$(":button").attr("disabled", false);
		$(":button").removeClass("long_button_x");

		queueOrder(tourFlag);
	} else {
		$(":button").attr("disabled", true);
		$(":button").addClass("long_button_x");
		submit_form("confirmPassenger");
	}
};

function popQueueInfo(message) {
	// 弹出窗口提示信息
	var msg = message;
	var titile = "交易提示";
	var msgtext = "<div class=\"submit-tips\" style=\"width:475px\">";
	msgtext += "<div class=\"submit-cont\" style=\"width:460px\">";
	// if(msg != 'Y'){
	// msgtext += "<p>入队列失败，请重新提交。</p>";
	// msgtext += "<p>原因："+message+"，请重新提交。</p>";
	// }else{
	msgtext += "<p id='orderResultInfo'></p>";
	msgtext += "<p>您也可点击<font color=\"red\"><a target='main' id=\"link\" href=\""
			+ ctx
			+ "/order/myOrderAction.do?method=queryMyOrderNotComplete&leftmenu=Y&fakeParent=true\" style='color:#2C72BA'>未完成订单</a></font>，查看订单处理情况。</p>";
	// }

	msgtext += "</div></div>";
	parent.pop_confirm_queue(titile, msgtext, successfunc, 505);
	submitfunction.apply(this);
}
function dispMessage(message) {
	var info = parent.document.getElementById("orderResultInfo");
	info.innerHTML = message;
}

// 异步下单
function queueOrder(tourFlag) {
	popQueueInfo();

	var url;
	if (tourFlag == 'dc') {
		// 异步下单-单程
		url = 'confirmPassengerAction.do?method=confirmSingleForQueue';
	} else if (tourFlag == 'wc') {
		// 异步下单-往程
		url = 'confirmPassengerAction.do?method=confirmPassengerInfoGoForQueue';
	} else if (tourFlag == 'fc') {
		// 异步下单-返程
		url = 'confirmPassengerAction.do?method=confirmPassengerInfoBackForQueue';
	} else if (tourFlag == 'gc') {
		// 异步下单-改签
		url = 'confirmPassengerResignAction.do?method=confirmPassengerInfoResignForQueue';
	} else {
		dispMessage("下单失败！旅程形式为" + tourFlag);

		return;
	}

	dispMessage("正在处理，请稍候。");
	$("#confirmPassenger")
			.ajaxSubmit(
					{
						url : url,
						type : "POST",
						dataType : "json",
						success : function(data) {
							if (data.errMsg != 'Y') {
								dispMessage("出票失败，"
										+ data.errMsg
										+ " <a style='color:#2C72BA' onclick='closePopWin()'>点击修改</a>");
								refreshImg();
								return false;
							} else {
								timer = new OrderQueueWaitTime(tourFlag,
										waitFunc, finishFun);
								timer.start();
							}
						},
						error : function(XMLHttpRequest, textStatus,
								errorThrown) {
							dispMessage("下单失败！网络忙，请稍后再试。");
							return false;
						}
					});
}

function waitFunc(tourFlag, return_time, show_time) {
	if (return_time <= 5) {
		dispMessage("您的订单已经提交，系统正在处理中，请稍等。");
	} else if (return_time > 30 * 60) {
		dispMessage("您的订单已经提交，预计等待时间超过30分钟，请耐心等待。");
	} else {
		dispMessage("您的订单已经提交，最新预估等待时间" + show_time + "，请耐心等待。");
	}

}

function procFail(flag, returnObj) {
	var renewURL = "<a id='link' target='main' style='color:#2C72BA' onclick='closePopWin()' href='"
			+ ctx + "/order/querySingleAction.do?method=init'>[重新购票]</a>";
	var my12306URL = "<a id='link' target='main' style='color:#2C72BA' onclick='closePopWin()' href='"
			+ ctx + "/loginAction.do?method=initForMy12306'>[我的12306]</a>";
	if (flag == -1) {
		return;
	} else if (flag == -2) {
		if (returnObj.errorcode == 0) {
			dispMessage("占座失败，原因:" + returnObj.msg + " 请点击" + my12306URL
					+ ",办理其他业务.");
		} else {
			dispMessage("占座失败，原因:" + returnObj.msg + " 请点击" + renewURL
					+ ",重新选择其它车次.");
		}
	} else if (flag == -3) {
		dispMessage("订单已撤销 " + " 请点击" + renewURL + ",重新选择其它车次.");
	} else {
		// 进入未完成订单页面
		parent.closePopWin();
		var form = document.getElementById("confirmPassenger");
		form.action = "myOrderAction.do?method=queryMyOrderNotComplete&leftmenu=Y&fakeParent=true";
		form.submit();
	}
}
// 跳转-单程
function finishFun(tourFlag, time, returnObj) {
	if (time == -1) {
		var action_url = "";
		if (tourFlag == 'dc') {
			// 异步下单-单程
			action_url = "confirmPassengerAction.do?method=payOrder&orderSequence_no="
					+ returnObj.orderId;
		} else if (tourFlag == 'wc') {
			// 异步下单-往程
			action_url = "confirmPassengerAction.do?method=wcConfirm&orderSequence_no="
					+ returnObj.orderId;
		} else if (tourFlag == 'fc') {
			// 异步下单-返程
			action_url = "confirmPassengerAction.do?method=backPay&orderSequence_no="
					+ returnObj.orderId;
		} else if (tourFlag == 'gc') {
			// 异步下单-改签
			action_url = "confirmPassengerResignAction.do?method=resignPay&orderSequence_no="
					+ returnObj.orderId;
		}

		var form = document.getElementById("confirmPassenger");
		form.action = action_url;
		form.submit();
		parent.closePopWin();
	} else {
		procFail(time, returnObj);
	}
}

// 获取车次信息
function getTicketInfo() {
	// 方式1
	// var ticket_info = "1、出发日期：" + train_date_str_ + "<br/>";
	// ticket_info += "2、车次信息：" + station_train_code_ + "次&nbsp;&nbsp;"
	// + from_station_name_ + "(" + start_time_str_ + "开)&ndash;"
	// + to_station_name_ + "(" + arrive_time_str_ + "到)&nbsp;&nbsp;历时：" +
	// lishi_
	// + "<br/>";
	// 方式2
	// var ticket_info = "1、出发日期：" + train_date_str_ + "<br/>";
	// ticket_info += "2、车次：" + station_train_code_ + "<br/>";
	// ticket_info += "3、出发地：" + from_station_name_ +
	// "("+start_time_str_+"开)<br/>";
	// ticket_info += "4、目的地：" + to_station_name_ +
	// "("+arrive_time_str_+"到)<br/>";
	// ticket_info += "5、历时：" + lishi_ + "<br/>";
	// 方式3
	// var ticket_info = "<table width=\"100%\" border=\"1\" cellspacing=\"0\"
	// cellpadding=\"0\" class=\"lc_time\">";
	// ticket_info += "<colgroup style=\"width: 20%\"></colgroup>";
	// ticket_info += "<colgroup style=\"width: 20%\"></colgroup>";
	// ticket_info += "<colgroup style=\"width: 25%\"></colgroup>";
	// ticket_info += "<colgroup style=\"width: 10%\"></colgroup>";
	// ticket_info += "<colgroup style=\"width: 25%\"></colgroup>";
	// // ticket_info += "<colgroup style=\"width: 35%\"></colgroup>";
	// ticket_info += " <tr>";
	// ticket_info += " <td><h4>"+getTrainDate(train_date_str_)+"</h4></td>";
	// ticket_info += " <td><h4>"+station_train_code_+"次</h4></td>";
	// ticket_info += "
	// <td><h4>"+from_station_name_+"("+start_time_str_+"开)</h4></td>";
	// ticket_info += " <td><hr></td>";
	// ticket_info += "
	// <td><h4>"+to_station_name_+"("+arrive_time_str_+"到)</h4></td>";
	// // ticket_info += " <td><h4>全程历时（"+getLishi(lishi_)+"）</h4></td>";
	// ticket_info += " </tr>";
	// ticket_info += "</table>";
	// 方式4
	// var ticket_info = "<table width=\"100%\" border=\"0\" cellspacing=\"1\"
	// cellpadding=\"0\" class=\"table_list\">";
	// ticket_info += "<colgroup style=\"width: 15%\"></colgroup>";
	// ticket_info += "<colgroup style=\"width: 13%\"></colgroup>";
	// ticket_info += "<colgroup style=\"width: 24%\"></colgroup>";
	// ticket_info += "<colgroup style=\"width: 24%\"></colgroup>";
	// ticket_info += "<colgroup style=\"width: 24%\"></colgroup>";
	// ticket_info += "<tr>";
	// ticket_info += " <th scope=\"col\">发车日期</th>";
	// ticket_info += " <th scope=\"col\">车次</th>";
	// ticket_info += " <th scope=\"col\">上车站</th>";
	// ticket_info += " <th scope=\"col\">下车站</th>";
	// ticket_info += " <th scope=\"col\">全程历时</th>";
	// ticket_info += " </tr>";
	// ticket_info += " <tr>";
	// ticket_info += " <td>"+getTrainDate(train_date_str_)+"</td>";
	// ticket_info += " <td>"+station_train_code_+"</td>";
	// ticket_info += " <td>"+from_station_name_+" （"+start_time_str_+"开）</td>";
	// ticket_info += " <td>"+to_station_name_+" （"+arrive_time_str_+"到）</td>";
	// ticket_info += " <td>"+getLishi(lishi_)+"</td>";
	// ticket_info += " </tr>";
	// ticket_info += "</table>";

	// 方式5
	var ticket_info = "<table width=\"100%\" cellpadding=\"0\" cellspacing=\"0\" class=\"table-info\">";
	ticket_info += "  <tr>";
	ticket_info += "    <td>" + getTrainDate(train_date_str_) + "</td>";
	ticket_info += "    <td>" + station_train_code_ + "次</td>";
	ticket_info += "    <td>" + from_station_name_ + "(" + start_time_str_
			+ "开)</td>";
	ticket_info += "    <td>————</td>";
	ticket_info += "    <td>" + to_station_name_ + "(" + arrive_time_str_
			+ "到)</td>";
	ticket_info += "  </tr>";
	ticket_info += "</table>";

	return ticket_info;
}

// 获取”发车日期“，格式由2011-07-28转成2011年07月28日
function getTrainDate(train_date) {
	var df1 = new DateFormat("yyyy-MM-dd");
	var dt1 = df1.parse(train_date);
	var df2 = new DateFormat("yyyy年MM月dd日");
	var train_date_format = df2.format(dt1);
	return train_date_format;
}

// 获取”历时“，格式由00:30转成00小时30分钟
function getLishi(lishi) {
	var df1 = new DateFormat("HH:mm");
	var dt1 = df1.parse(lishi);
	var df2 = new DateFormat("HH小时mm分钟");
	var lishi_format = df2.format(dt1);
	return lishi_format;
}

// 获取乘车人信息
function getPassengerInfo() {
	// 方式1
	// var passenger_info = "<table id=\"psgTable\" width=\"770\"
	// cellpadding=\"0\" cellspacing=\"1\" class=\"table_clistb\"
	// align=\"center\">";
	// passenger_info += "<tr class=\"table_12px\">";
	// passenger_info += "<th width=\"50\">序号</th>";
	// passenger_info += "<th width=\"90\">席别</th>";
	// passenger_info += "<th width=\"90\">票种</th>";
	// passenger_info += "<th width=\"150\">姓名</th>";
	// passenger_info += "<th width=\"100\">证件类型</th>";
	// passenger_info += "<th width=\"\">证件号码</th>";
	// passenger_info += "<th width=\"100\">手机号</th>";
	// 方式2
	var passenger_info = "<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" class=\"table_infolist\">";
	passenger_info += "<tr>";
	passenger_info += "<th width=\"50\">序号</th>";
	passenger_info += "<th width=\"90\">席别</th>";
	passenger_info += "<th width=\"90\">票种</th>";
	passenger_info += "<th width=\"150\">姓名</th>";
	passenger_info += "<th width=\"100\">证件类型</th>";
	passenger_info += "<th width=\"\">证件号码</th>";
	passenger_info += "<th width=\"100\">手机号</th>";
	var no_ = 0; // 序号
	// var flag = false;//
	$(".passenger_class")
			.each(
					function(index) {
						if (this.style.display == "none") {
							return false;
						}
						var id = $(this).attr("id");
						var _isSave = $("#" + id + "_isSave")[0].checked;
						var _type = $("#_type").val();
						// alert(_type);
						if (_type == null || _type == "" || _type == "null"
								|| _isSave
								&& (_type == "resign" || _type == "back")) {
							no_++;
							// 只有选择需要购买返程票或者需要改签的记录才在弹出窗口显示
							var value = "<tr><td>" + no_ + "</td>";
							value += "<td align=\"center\">"
									+ $("#" + id + "_seat").find(
											"option:selected").text() + "</td>";
							value += "<td>"
									+ $("#" + id + "_ticket").find(
											"option:selected").text() + "</td>";
							value += "<td>" + $("#" + id + "_name").val()
									+ "</td>";
							if ($("#" + id + "_cardtype").find(
									"option:selected").text() == "护照") {
								cardflag = true;
							}
							value += "<td>"
									+ $("#" + id + "_cardtype").find(
											"option:selected").text() + "</td>";
							value += "<td>" + $("#" + id + "_cardno").val()
									+ "</td>";
							value += "<td>" + $("#" + id + "_mobileno").val()
									+ "</td>";
							passenger_info += value;
						}
					});

	passenger_info += "</tr>";
	passenger_info += "</table>";
	return passenger_info;
}

// 提交订单验证
function submit_form_check(formId) {
	var checkCardType = [];
	var isSelectOne = false;
	var noSelects = [];
	function toPassengerStringSplit(passenger) {
		var pValue = passenger.passenger_name + ","
				+ passenger.passenger_id_type_code + ","
				+ passenger.passenger_id_no;
		return pValue;
	}
	;
	var _seat_codes_resign = []; // 要改签的乘客席别
	$(".passenger_class").each(function() {
		var id = $(this).attr("id");
		if (this.style.display == "none") {
			return false;
		}
		var value = $("#" + id + "_seat").val();
		// 添加铺别
		value += "," + 0;
		if ($("#" + id + "_isSave")[0].checked) {
			// 不改签的不用验证"改签时，必须选择相同席别"。即同一席别的车票才能一起改签。
			_seat_codes_resign.push($("#" + id + "_seat").val());
		}
		value += "," + $("#" + id + "_ticket").val();
		value += "," + $("#" + id + "_name").val();
		value += "," + $("#" + id + "_cardtype").val();
		value += "," + $("#" + id + "_cardno").val();
		value += "," + $("#" + id + "_mobileno").val();
		if ($("#" + id + "_isSave")[0].checked) {
			value += "," + $("#" + id + "_isSave").val();
			isSelectOne = true;
		} else {
			value += ",N";
			noSelects.push(id + "_p");
		}
		if ($("#" + id + "_ticket").val() !== ticket_type_code.child) {
			checkCardType.push({
				name : $("#" + id + "_name").val(),
				cardno : $("#" + id + "_cardno").val(),
				cardtype : $("#" + id + "_cardtype").val()
			});
		}
		$("#" + id + "_p").val(value);
		var passenger = $("#" + id).data("passenger");
		if (passenger) {
			$("#" + id + "_old").val(toPassengerStringSplit(passenger));
		}
	});
	if ($("#_type").val() == "resign") {
		if (!isSupportBatchResign(_seat_codes_resign)) {
			alert("卧铺不支持批量改签，请单张改签！");
			return false;
		}
		if ($.unique(_seat_codes_resign).length > 1) {
			alert("改签时，必须选择相同席别");
			return false;
		}
	}
	var isCardSame = false;
	var cardSameName = ""; // 证件号码重复的乘客姓名
	var cardSameCardNo = ""; // 证件号码重复的证件号码
	$
			.each(
					checkCardType,
					function(index, item) {
						if (item.cardno == "") {
							return false;
						}
						$
								.each(
										checkCardType,
										function(index1, item1) {
											if (item.cardtype == "1") {
												// 一代身份证15位需要转成18位二代身份证进行验证
												if (index1 != index
														&& (item1.cardtype == '1'
																&& item1.cardno == item.cardno || item1.cardtype == '2'
																&& item1.cardno == getCardNo18(item.cardno))) {
													isCardSame = true;
													cardSameName = item.name;
													cardSameCardNo = item.cardno;
													return false;
												}
											} else if (item.cardtype == "2") {
												if (index1 != index
														&& (item1.cardtype == '1'
																&& getCardNo18(item1).cardno == item.cardno || item1.cardtype == '2'
																&& item1.cardno == item.cardno)) {
													isCardSame = true;
													cardSameName = item.name;
													cardSameCardNo = item.cardno;
													return false;
												}
											} else {
												if (index1 != index
														&& (item1.cardtype == item.cardtype)
														&& (item1.cardno == item.cardno)) {
													isCardSame = true;
													cardSameName = item.name;
													cardSameCardNo = item.cardno;
													return false;
												}
											}
										});
					});
	var id_mode = $("#orderRequest_id_mode").val();
	// 车次
	var _station_train_code = $("#_station_train_code").val();
	// 发车日期
	var _train_date_str = $("#_train_date_str").val();
	var train_date_str_year = _train_date_str.substr(0, 4);
	var train_date_str_month = _train_date_str.substr(5, 2);
	var train_date_str_day = _train_date_str.substr(8, 2);
	// alert(isCardSame);
	// alert(id_mode);
	if (isCardSame && id_mode == "Y") {
		alert("互联网售票实行实名制：" + cardSameCardNo + "只能购买" + train_date_str_year
				+ "年" + train_date_str_month + "月" + train_date_str_day + "日的"
				+ _station_train_code + "次的一张车票！");
		return;
	}
	if ($("#_type").val() !== "") {
		if (!isSelectOne) {
			alert("至少选择一位乘客");
			return false;
		}
	}
	if ($("#" + formId).valid()) {
		$(":button").attr("disabled", true);
		$(":button").addClass("long_button_x");
	} else {
		return false;
	}
	return true;
}

// 判断是否支持批量改签：卧铺不支持批量改签
function isSupportBatchResign(_seat_codes_resign) {
	var seats_wp = "3456ACFGHL"; // 所有卧铺席别码
	var seat_num = 0;
	$.each(_seat_codes_resign, function(i, val) {
		if (seats_wp.indexOf(val) != -1) {
			seat_num++;
		}
		if (seat_num >= 2) {
			return false;
		}
	});
	if (seat_num >= 2) {
		return false;
	} else {
		return true;
	}
}

// 提交订单
function submit_form(formId) {
	var sub_form = $("#" + formId);
	sub_form.target = "_self"; // 若不加此句，firefox浏览器下点击”确定“提交订单时弹出窗口不消失
	sub_form.submit();
}

// 15位身份证件号码转为18位
function getCardNo18(carNo15) {
	var cardNo18;
	if (carNo15.length == 15) {
		cardNo18 = idCardUpdate(carNo15);
	} else {
		cardNo18 = carNo15;
	}
	return cardNo18;
}

// 确认取消订单
function confirmCancel(formId) {
	if (confirm("您确认取消订单吗?")) {
		$(":button").attr("disabled", true);
		$(":button").addClass("long_button_x");
		cancelToQuery();
	}
}

$(document).ready(function() {
	var rules = {};
	var messages = {};
	var errorLabelContainer = $("#error_info");
	
	$(".passenger_class").each(function() {
		var id = $(this).attr("id");
		var seatTypeId = id + "_seat";
		var ticketTypeId = id + "_ticket";
		var nameId = id + "_name";
		var cardNoId = id + "_cardno";
		var cardTypeId = id + "_cardtype";
		var mobileNoId = id + "_mobileno";
		var isSave = id + "_isSave"; // 是否选择"返程"或者"改签"
		var checkRealName = cardTypeId + "@" + cardNoId;
		rules[nameId] = {
			required : true,
			byteRangeLength : [ 3, 20 ],
			checkNameCharBlank : checkRealName
		};
		rules[seatTypeId] = {
			required : false,
			checkSeatType : "#" + isSave
		};
		rules[ticketTypeId] = {
			required : true
		};
		rules[cardNoId] = {
			required : true,
			checkIdValidStr : true,
			// isIDCard:"#cardType",
			isSecIDCard : "#" + cardTypeId,
			isFirIDCard : "#" + cardTypeId,
			checkHkongMacao : "#" + cardTypeId,
			checkTaiw : "#" + cardTypeId,
			checkPassport : "#" + cardTypeId
		};
		rules[mobileNoId] = {
			isMobile : true
		};
		messages[seatTypeId] = {
			required : "请选择席别！<br>",
			checkSeatType : "请选择席别！<br>"
		};
		messages[ticketTypeId] = {
			required : "请选择票种！<br>"
		};
		messages[nameId] = {
			required : "请输入您的姓名！<br>",
			byteRangeLength : "允许输入的字符串在3-20个字符之间！<br>",
			checkNameCharBlank : "姓名只能包含中文或者英文！<br>"
		};
		messages[cardNoId] = {
			required : "请输入证件号码！<br>",
			// isIDCard:"请正确输入15或者18位的身份证号！",
			isSecIDCard : "请正确输入18位的身份证号！<br>",
			isFirIDCard : "请正确输入15或者18位的身份证号！<br>",
			checkIdValidStr : "输入的证件编号中包含中文信息或特殊字符！<br>",
			checkHkongMacao : "请输入有效的港澳居民通行证号码！<br>",
			checkTaiw : "请输入有效的台湾居民通行证号码！<br>",
			checkPassport : "请输入有效的护照号码！<br>"
		};
		messages[mobileNoId] = {
			isMobile : "您输入的手机号码不是有效的格式！<br>"
		};
	});
	var validate = $("#confirmPassenger").validate({
		rules : rules,
		messages : messages,
		errorLabelContainer : errorLabelContainer,
		onfocusout : function(element, errorClass, validClass) {
			if (this.check(element) && this.messages) {
				var messageObject = this.messages[element.id];
				if (messageObject) {
					messageObject.count--;
					if (messageObject.count == 0) {
						this.errorsFor(messageObject.el).hide();
					}
				}
				$(element).removeClass("error");
			}
		},
		onkeyup : function() {
		},
		onclick : function() {
		}
	});
	var showLabel = validate.showLabel;
	var checkForm = validate.checkForm;
	validate.checkForm = function() {
		checkForm.apply(validate);
		if (this.messages) {
			this.messages = null;
		}
	};
	validate.showLabel = function(element, message) {
		if (!this.messages) {
			this.messages = {};
		}
		if (this.messages[message]) {
			this.messages[message].count = this.messages[message].count + 1;
			this.messages[element.id] = this.messages[message];
			return;
		} else {
			this.messages[message] = {
				el : element,
				count : 1
			};
			this.messages[element.id] = this.messages[message];
		}
		showLabel.call(validate, element, message);
	};
	// 初始化铺别
	for ( var i = 1; i < 6; i++) {
		if ($("#passenger_" + i + "_seat").attr("disabled") == false) {
			selectSeatDetail(i);
		}
	}
});

// 根据席别选择具体的铺别，即“上”，“中”，“下”铺，
// flag=1时，为添加和删除乘车人，checkValue应取席别下拉框第一个option的值
// flag=2时，为修改席别时填充铺别下拉框，checkValue应取席别下拉框被选中的option的值
function selectSeatDetail(index) {
	// 获取Select选择的Value
	var checkValue = $("#passenger_" + index + "_seat").val();
	if (checkValue == '3' || checkValue == '5' || checkValue == 'C') {
		var count = $("#passenger_" + index + "_seat_detail_select option:last")
				.attr("index") + 1;
		if (count == 3) {
			$("#passenger_" + index + "_seat_detail_select option[value='3']")
					.after("<option value='2'>中铺</option>");
		}
		$("#passenger_" + index + "_seat_detail_select").removeAttr("disabled");
	} else if (checkValue == '4' || checkValue == '6' || checkValue == 'H'
			|| checkValue == 'L' || checkValue == 'F' || checkValue == 'G'
			|| checkValue == 'A') {
		$("#passenger_" + index + "_seat_detail_select option[value='2']")
				.remove();
		$("#passenger_" + index + "_seat_detail_select").removeAttr("disabled");
	} else {
		$("#passenger_" + index + "_seat_detail_select").attr("disabled",
				"disabled");
		$("#passenger_" + index + "_seat_detail_select option[value='0']")
				.attr("selected", "selected");
		$("#passenger_" + index + "_seat_detail").val("0");
	}
}

// 设置铺别隐藏域的值
function setSeatDetail(index) {
	$("#passenger_" + index + "_seat_detail").val(
			$("#passenger_" + index + "_seat_detail_select").val());
}

// 在拷贝行时，将trId2铺别中的option覆盖trId2铺别中的option
function copySeatDetailOption(trId1, trId2) {
	var optionCount1 = $("#" + trId1 + "_seat_detail_select option:last").attr(
			"index") + 1;
	var optionCount2 = $("#" + trId2 + "_seat_detail_select option:last").attr(
			"index") + 1;
	if (optionCount1 > optionCount2) {
		$("#" + trId1 + "_seat_detail_select option[value='2']").remove();
	} else if (optionCount1 < optionCount2) {
		$("#" + trId1 + "_seat_detail_select option[value='3']").after(
				"<option value='2'>中铺</option>");
	} else {
		// 不做处理
	}
}

//鼠标悬停显示提示框-姓名填写说明
function onStopHover_name(obj) {
	onStopHover(obj, $("#tip_info_div_name"));
}

//鼠标悬停显示提示框-填写说证件号码填写说明
function onStopHover_cardtype(obj) {
	onStopHover(obj, $("#tip_info_div_cardtype"));
}

//鼠标悬停显示提示框-姓名、证件号码填写说明
function onStopHover_name_cardtype(obj) {
	onStopHover(obj, $("#tip_info_div_name_cardtype"));
}

//鼠标悬停显示提示框
function onStopHover(obj, obj_div) {
	var div_height = obj_div.height();
	var position_ = $(obj).position();
	var pos_top = position_.top;
	var pos_left = position_.left;
	var pos_top_new = pos_top - div_height;
	var pos_left_new = pos_left;
	obj_div.attr("style", "display:block;position:absolute;z-index: 120;top:"
			+ pos_top_new + "px;left:" + pos_left_new + "px;");
	obj_div.show();
}

//鼠标移开隐藏提示框
function onStopOut() {
	$("#tip_info_div_name").hide();
	$("#tip_info_div_cardtype").hide();
	$("#tip_info_div_name_cardtype").hide();
}