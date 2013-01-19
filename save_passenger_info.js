var tourFlag;
var buttonOk = "ȡ   �� ";
var buttonNo = "ȷ   �� ";

// �ύ����ǰȷ��
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
//						alert("�Բ���������ȡ���������࣬���ս����ܼ����������Ķ�Ʊ����");
						alert(data.msg);
						$(":button").attr("disabled", false);
						$(":button").removeClass("long_button_x");
						return;
					}else if('N' == data.check608){
//						alert("����Ϊʵ�����г���ʵ��һ��һ��һ֤һƱ�ƣ�");
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
					if(errorThrown.concat('��½')){
						 alert("���뿪ҳ���ʱ������������µ�¼ϵͳ��");
						
					}else{
						alert("��������æ�����Ժ����ԣ�");	
					}
					
					$(":button").attr("disabled", false);
					$(":button").removeClass("long_button_x");
					return;
				}
			});

}

function showOrderDialog(tFlag) {
	// �����Ŷӵȴ���������ʱ����ʾ�İ�
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
						//queue_note = "�𾴵��ÿͣ�"+((data.theWaitTotalCount == undefined || data.theWaitTotalCount == null || data.theWaitTotalCount == "null") ? "" : "ϵͳ��ǰ������Ķ�������<font color='red'>" + data.theWaitTotalCount + "</font>��" )+"�����г���ѡ���ϯ��������Ʊ"
						queue_note = "�𾴵��ÿͣ������г���ѡ���ϯ��������Ʊ"
								+ getTicketCountDesc(data.ticket,
										$("#passenger_1_seat").val()) + "��";

						if (data.op_2) {
							queue_note += "Ŀǰ�Ŷ������Ѿ�������Ʊ����������ѡ������ϯ��򳵴Σ��ش����ѡ�";
							// queue_note = "��������<font color=\"red\">" +
							// data.count +
							// "</font>���������ύ��ͬ�Ĺ�Ʊ����Ŀǰ�Ŷ������ѳ���ʣ��Ʊ��������ѡ������ϯ��򳵴Ρ�";

							$(parent.document.body)
									.contents()
									.find(":button")
									.each(
											function(a) {
												if ($(this).html() == buttonNo) {
													$(this).attr("disabled",
															true).addClass(
															"long_button_x");
												}
											});
						} else {
							if (data.countT > 0) {
								queue_note += "Ŀǰ�Ŷ�����<font color=\"red\">"
										+ data.countT + "</font>�ˣ�";
							}
							queue_note += "�ش����ѡ�";
						}
						queue_note += "<br/><br/><font color=\"red\">��ȷ�϶�����Ϣ�Ƿ���ȷ������ȷ������ȷ������ϵͳ��Ϊ���������ϯλ��</font>";
						/*
						 * else if(data.op_1){ queue_note +=
						 * "Ŀǰ�Ŷ������Ѿ�������Ʊ�������ش����ѡ�"; //queue_note = "��������<font
						 * color=\"red\">" + data.count + "</font>���������ύ��ͬ�Ĺ�Ʊ���󣬵�������������ʱ��������Ʊ���������Ǹ��ݵ�ǰ��Ʊȷ���Ƿ��Ŷӡ�"; }
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
	cardflag = false;// �Ƿ�ʹ�û���
	var ticket_info = getTicketInfo();
	var passenger_info = getPassengerInfo();
	// ����������ʾ��Ϣ
	// ��ʽ1
	// var msgtext = "<br/><p><b>����Ҫ�ύ�Ķ�����Ϣ���£���ȷ���Ƿ���ȷ��<font
	// color=\"red\">�����ȷ�����ύ�����������ȡ���������޸�</font>����</b></p>";
	// msgtext += "<br/><p><b>һ��������Ϣ��</b></p>";
	// msgtext += "<ul><li>"+ticket_info+"</li></ul>";
	// msgtext += "<p><b>�����˳��������Ϣ��</b></p>";
	// msgtext += "<ul><li>"+passenger_info+"</li></ul>";
	// msgtext += "<p><b>ע��ϵͳ�������۳�������Ϊ������ϯλ���ݲ�֧����ѡϯλ��</b></p>";
	// ��ʽ2
	var titile = "�ύ����ȷ��";
	var msgtext = "<div class=\"submit-tips\">";
	msgtext += "<div class=\"submit-cont\">";
	msgtext += "<br/><div class=\"title\">������Ϣ</div>";
	msgtext += ticket_info;

	msgtext += "<div class=\"title\">�˳�����Ϣ</div>";
	msgtext += passenger_info;
	// if((to_station_code_=="LSO" || to_station_code_=="DAO" ||
	// to_station_code_=="ADO" || to_station_code_=="NQO"
	// ||to_station_code_=="THO" ) && cardflag == true) {
	// msgtext += "<p
	// style=\"color:#3177BF;\">ע��1.ϵͳ�������۳�������Ϊ������ϯλ���ݲ�֧����ѡϯλ��</p>";
	// msgtext += "<p
	// style=\"color:#3177BF;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2.�������й涨��������ڹ�������ػ�Ʊʱ�����ʾ�������������°칫�һ����ξ֡�����������׼�����磩�����߳�ʾ�й��ڵ�˾�ּ��Ӵ���λ���ߵġ���������������������ͬ���֤���ź���̨��ͬ�����ش������Ρ����������������������������ξֻ�������������룬������ػ�Ʊʱ���ʾ�й���׼����</p>";
	// } else {
	// msgtext += "<p
	// style=\"color:#3177BF;\">ע��ϵͳ�������۳�������Ϊ������ϯλ���ݲ�֧����ѡϯλ��</p>";
	// }
	msgtext += "<div id=\"queue_note\" style=\"font-size:14px;padding:20px;\">"
			+ queue_note + "</div>";
	msgtext += "</div></div>";

	parent.pop_confirm(titile, msgtext, failfunc, successfunc, buttonNo,
			buttonOk, 692);
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
		rt += "<font color='red'>" + seat_1 + "</font>��";
	}
	if (seat_2 > -1) {
		rt += ",����<font color='red'>" + seat_2 + "</font>��";
	}
	return rt;
}

// ����ȷ�϶�����ʾ��Ϣ��������
var submitfunction = function() {
	// $("#confirmPassenger").submit();
};

// ȡ���ύ����
var successfunc = function() {
	$(":button").attr("disabled", false);
	$(":button").removeClass("long_button_x");
};

// ȷ���ύ����
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
	// ����������ʾ��Ϣ
	var msg = message;
	var titile = "������ʾ";
	var msgtext = "<div class=\"submit-tips\" style=\"width:475px\">";
	msgtext += "<div class=\"submit-cont\" style=\"width:460px\">";
	// if(msg != 'Y'){
	// msgtext += "<p>�����ʧ�ܣ��������ύ��</p>";
	// msgtext += "<p>ԭ��"+message+"���������ύ��</p>";
	// }else{
	msgtext += "<p id='orderResultInfo'></p>";
	msgtext += "<p>��Ҳ�ɵ��<font color=\"red\"><a target='main' id=\"link\" href=\""
			+ ctx
			+ "/order/myOrderAction.do?method=queryMyOrderNotComplete&leftmenu=Y&fakeParent=true\" style='color:#2C72BA'>δ��ɶ���</a></font>���鿴�������������</p>";
	// }

	msgtext += "</div></div>";
	parent.pop_confirm_queue(titile, msgtext, successfunc, 505);
	submitfunction.apply(this);
}
function dispMessage(message) {
	var info = parent.document.getElementById("orderResultInfo");
	info.innerHTML = message;
}

// �첽�µ�
function queueOrder(tourFlag) {
	popQueueInfo();

	var url;
	if (tourFlag == 'dc') {
		// �첽�µ�-����
		url = 'confirmPassengerAction.do?method=confirmSingleForQueueOrder';
	} else if (tourFlag == 'wc') {
		// �첽�µ�-����
		url = 'confirmPassengerAction.do?method=confirmPassengerInfoGoForQueue';
	} else if (tourFlag == 'fc') {
		// �첽�µ�-����
		url = 'confirmPassengerAction.do?method=confirmPassengerInfoBackForQueue';
	} else if (tourFlag == 'gc') {
		// �첽�µ�-��ǩ
		url = 'confirmPassengerResignAction.do?method=confirmPassengerInfoResignForQueue';
	} else {
		dispMessage("�µ�ʧ�ܣ��ó���ʽΪ" + tourFlag);

		return;
	}

	dispMessage("���ڴ������Ժ�");
	$("#confirmPassenger")
			.ajaxSubmit(
					{
						url : url,
						type : "POST",
						dataType : "json",
						success : function(data) {
							if (data.errMsg != 'Y') {
								dispMessage("��Ʊʧ�ܣ�"
										+ data.errMsg
										+ " <a style='color:#2C72BA' onclick='closePopWin()'>����޸�</a>");
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
							dispMessage("�µ�ʧ�ܣ�����æ�����Ժ����ԡ�");
							return false;
						}
					});
}

function waitFunc(tourFlag, return_time, show_time) {
	if (return_time <= 5) {
		dispMessage("���Ķ����Ѿ��ύ��ϵͳ���ڴ����У����Եȡ�");
	} else if (return_time > 30 * 60) {
		dispMessage("���Ķ����Ѿ��ύ��Ԥ�Ƶȴ�ʱ�䳬��30���ӣ������ĵȴ���");
	} else {
		dispMessage("���Ķ����Ѿ��ύ������Ԥ���ȴ�ʱ��" + show_time + "�������ĵȴ���");
	}

}

function procFail(flag, returnObj) {
	var renewURL = "<a id='link' target='main' style='color:#2C72BA' onclick='closePopWin()' href='"
			+ ctx + "/order/querySingleAction.do?method=init'>[���¹�Ʊ]</a>";
	var my12306URL = "<a id='link' target='main' style='color:#2C72BA' onclick='closePopWin()' href='"
			+ ctx + "/loginAction.do?method=initForMy12306'>[�ҵ�12306]</a>";
	if (flag == -1) {
		return;
	} else if (flag == -2) {
		if (returnObj.errorcode == 0) {
			dispMessage("ռ��ʧ�ܣ�ԭ��:" + returnObj.msg + " ����" + my12306URL
					+ ",��������ҵ��.");
		} else {
			dispMessage("ռ��ʧ�ܣ�ԭ��:" + returnObj.msg + " ����" + renewURL
					+ ",����ѡ����������.");
		}
	} else if (flag == -3) {
		dispMessage("�����ѳ��� " + " ����" + renewURL + ",����ѡ����������.");
	} else {
		// ����δ��ɶ���ҳ��
		parent.closePopWin();
		var form = document.getElementById("confirmPassenger");
		form.action = "myOrderAction.do?method=queryMyOrderNotComplete&leftmenu=Y&fakeParent=true";
		form.submit();
	}
}
// ��ת-����
function finishFun(tourFlag, time, returnObj) {
	if (time == -1) {
		var action_url = "";
		if (tourFlag == 'dc') {
			// �첽�µ�-����
			action_url = "confirmPassengerAction.do?method=payOrder&orderSequence_no="
					+ returnObj.orderId;
		} else if (tourFlag == 'wc') {
			// �첽�µ�-����
			action_url = "confirmPassengerAction.do?method=wcConfirm&orderSequence_no="
					+ returnObj.orderId;
		} else if (tourFlag == 'fc') {
			// �첽�µ�-����
			action_url = "confirmPassengerAction.do?method=backPay&orderSequence_no="
					+ returnObj.orderId;
		} else if (tourFlag == 'gc') {
			// �첽�µ�-��ǩ
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

// ��ȡ������Ϣ
function getTicketInfo() {
	// ��ʽ1
	// var ticket_info = "1���������ڣ�" + train_date_str_ + "<br/>";
	// ticket_info += "2��������Ϣ��" + station_train_code_ + "��&nbsp;&nbsp;"
	// + from_station_name_ + "(" + start_time_str_ + "��)&ndash;"
	// + to_station_name_ + "(" + arrive_time_str_ + "��)&nbsp;&nbsp;��ʱ��" +
	// lishi_
	// + "<br/>";
	// ��ʽ2
	// var ticket_info = "1���������ڣ�" + train_date_str_ + "<br/>";
	// ticket_info += "2�����Σ�" + station_train_code_ + "<br/>";
	// ticket_info += "3�������أ�" + from_station_name_ +
	// "("+start_time_str_+"��)<br/>";
	// ticket_info += "4��Ŀ�ĵأ�" + to_station_name_ +
	// "("+arrive_time_str_+"��)<br/>";
	// ticket_info += "5����ʱ��" + lishi_ + "<br/>";
	// ��ʽ3
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
	// ticket_info += " <td><h4>"+station_train_code_+"��</h4></td>";
	// ticket_info += "
	// <td><h4>"+from_station_name_+"("+start_time_str_+"��)</h4></td>";
	// ticket_info += " <td><hr></td>";
	// ticket_info += "
	// <td><h4>"+to_station_name_+"("+arrive_time_str_+"��)</h4></td>";
	// // ticket_info += " <td><h4>ȫ����ʱ��"+getLishi(lishi_)+"��</h4></td>";
	// ticket_info += " </tr>";
	// ticket_info += "</table>";
	// ��ʽ4
	// var ticket_info = "<table width=\"100%\" border=\"0\" cellspacing=\"1\"
	// cellpadding=\"0\" class=\"table_list\">";
	// ticket_info += "<colgroup style=\"width: 15%\"></colgroup>";
	// ticket_info += "<colgroup style=\"width: 13%\"></colgroup>";
	// ticket_info += "<colgroup style=\"width: 24%\"></colgroup>";
	// ticket_info += "<colgroup style=\"width: 24%\"></colgroup>";
	// ticket_info += "<colgroup style=\"width: 24%\"></colgroup>";
	// ticket_info += "<tr>";
	// ticket_info += " <th scope=\"col\">��������</th>";
	// ticket_info += " <th scope=\"col\">����</th>";
	// ticket_info += " <th scope=\"col\">�ϳ�վ</th>";
	// ticket_info += " <th scope=\"col\">�³�վ</th>";
	// ticket_info += " <th scope=\"col\">ȫ����ʱ</th>";
	// ticket_info += " </tr>";
	// ticket_info += " <tr>";
	// ticket_info += " <td>"+getTrainDate(train_date_str_)+"</td>";
	// ticket_info += " <td>"+station_train_code_+"</td>";
	// ticket_info += " <td>"+from_station_name_+" ��"+start_time_str_+"����</td>";
	// ticket_info += " <td>"+to_station_name_+" ��"+arrive_time_str_+"����</td>";
	// ticket_info += " <td>"+getLishi(lishi_)+"</td>";
	// ticket_info += " </tr>";
	// ticket_info += "</table>";

	// ��ʽ5
	var ticket_info = "<table width=\"100%\" cellpadding=\"0\" cellspacing=\"0\" class=\"table-info\">";
	ticket_info += "  <tr>";
	ticket_info += "    <td>" + getTrainDate(train_date_str_) + "</td>";
	ticket_info += "    <td>" + station_train_code_ + "��</td>";
	ticket_info += "    <td>" + from_station_name_ + "(" + start_time_str_
			+ "��)</td>";
	ticket_info += "    <td>��������</td>";
	ticket_info += "    <td>" + to_station_name_ + "(" + arrive_time_str_
			+ "��)</td>";
	ticket_info += "  </tr>";
	ticket_info += "</table>";

	return ticket_info;
}

// ��ȡ���������ڡ�����ʽ��2011-07-28ת��2011��07��28��
function getTrainDate(train_date) {
	var df1 = new DateFormat("yyyy-MM-dd");
	var dt1 = df1.parse(train_date);
	var df2 = new DateFormat("yyyy��MM��dd��");
	var train_date_format = df2.format(dt1);
	return train_date_format;
}

// ��ȡ����ʱ������ʽ��00:30ת��00Сʱ30����
function getLishi(lishi) {
	var df1 = new DateFormat("HH:mm");
	var dt1 = df1.parse(lishi);
	var df2 = new DateFormat("HHСʱmm����");
	var lishi_format = df2.format(dt1);
	return lishi_format;
}

// ��ȡ�˳�����Ϣ
function getPassengerInfo() {
	// ��ʽ1
	// var passenger_info = "<table id=\"psgTable\" width=\"770\"
	// cellpadding=\"0\" cellspacing=\"1\" class=\"table_clistb\"
	// align=\"center\">";
	// passenger_info += "<tr class=\"table_12px\">";
	// passenger_info += "<th width=\"50\">���</th>";
	// passenger_info += "<th width=\"90\">ϯ��</th>";
	// passenger_info += "<th width=\"90\">Ʊ��</th>";
	// passenger_info += "<th width=\"150\">����</th>";
	// passenger_info += "<th width=\"100\">֤������</th>";
	// passenger_info += "<th width=\"\">֤������</th>";
	// passenger_info += "<th width=\"100\">�ֻ���</th>";
	// ��ʽ2
	var passenger_info = "<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" class=\"table_infolist\">";
	passenger_info += "<tr>";
	passenger_info += "<th width=\"50\">���</th>";
	passenger_info += "<th width=\"90\">ϯ��</th>";
	passenger_info += "<th width=\"90\">Ʊ��</th>";
	passenger_info += "<th width=\"150\">����</th>";
	passenger_info += "<th width=\"100\">֤������</th>";
	passenger_info += "<th width=\"\">֤������</th>";
	passenger_info += "<th width=\"100\">�ֻ���</th>";
	var no_ = 0; // ���
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
							// ֻ��ѡ����Ҫ���򷵳�Ʊ������Ҫ��ǩ�ļ�¼���ڵ���������ʾ
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
									"option:selected").text() == "����") {
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

// �ύ������֤
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
	var _seat_codes_resign = []; // Ҫ��ǩ�ĳ˿�ϯ��
	$(".passenger_class").each(function() {
		var id = $(this).attr("id");
		if (this.style.display == "none") {
			return false;
		}
		var value = $("#" + id + "_seat").val();
		// ����̱�
		value += "," + $("#" + id + "_seat_detail").val();
		if ($("#" + id + "_isSave")[0].checked) {
			// ����ǩ�Ĳ�����֤"��ǩʱ������ѡ����ͬϯ��"����ͬһϯ��ĳ�Ʊ����һ���ǩ��
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
			alert("���̲�֧��������ǩ���뵥�Ÿ�ǩ��");
			return false;
		}
		if ($.unique(_seat_codes_resign).length > 1) {
			alert("��ǩʱ������ѡ����ͬϯ��");
			return false;
		}
	}
	var isCardSame = false;
	var cardSameName = ""; // ֤�������ظ��ĳ˿�����
	var cardSameCardNo = ""; // ֤�������ظ���֤������
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
												// һ�����֤15λ��Ҫת��18λ�������֤������֤
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
	// ����
	var _station_train_code = $("#_station_train_code").val();
	// ��������
	var _train_date_str = $("#_train_date_str").val();
	var train_date_str_year = _train_date_str.substr(0, 4);
	var train_date_str_month = _train_date_str.substr(5, 2);
	var train_date_str_day = _train_date_str.substr(8, 2);
	// alert(isCardSame);
	// alert(id_mode);
	if (isCardSame && id_mode == "Y") {
		alert("��������Ʊʵ��ʵ���ƣ�" + cardSameCardNo + "ֻ�ܹ���" + train_date_str_year
				+ "��" + train_date_str_month + "��" + train_date_str_day + "�յ�"
				+ _station_train_code + "�ε�һ�ų�Ʊ��");
		return;
	}
	if ($("#_type").val() !== "") {
		if (!isSelectOne) {
			alert("����ѡ��һλ�˿�");
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

// �ж��Ƿ�֧��������ǩ�����̲�֧��������ǩ
function isSupportBatchResign(_seat_codes_resign) {
	var seats_wp = "3456ACFGHL"; // ��������ϯ����
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

// �ύ����
function submit_form(formId) {
	var sub_form = $("#" + formId);
	sub_form.target = "_self"; // �����Ӵ˾䣬firefox������µ����ȷ�����ύ����ʱ�������ڲ���ʧ
	sub_form.submit();
}

// 15λ���֤������תΪ18λ
function getCardNo18(carNo15) {
	var cardNo18;
	if (carNo15.length == 15) {
		cardNo18 = idCardUpdate(carNo15);
	} else {
		cardNo18 = carNo15;
	}
	return cardNo18;
}

// ȷ��ȡ������
function confirmCancel(formId) {
	if (confirm("��ȷ��ȡ��������?")) {
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
		var isSave = id + "_isSave"; // �Ƿ�ѡ��"����"����"��ǩ"
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
			required : "��ѡ��ϯ��<br>",
			checkSeatType : "��ѡ��ϯ��<br>"
		};
		messages[ticketTypeId] = {
			required : "��ѡ��Ʊ�֣�<br>"
		};
		messages[nameId] = {
			required : "����������������<br>",
			byteRangeLength : "����������ַ�����3-20���ַ�֮�䣡<br>",
			checkNameCharBlank : "����ֻ�ܰ������Ļ���Ӣ�ģ�<br>"
		};
		messages[cardNoId] = {
			required : "������֤�����룡<br>",
			// isIDCard:"����ȷ����15����18λ�����֤�ţ�",
			isSecIDCard : "����ȷ����18λ�����֤�ţ�<br>",
			isFirIDCard : "����ȷ����15����18λ�����֤�ţ�<br>",
			checkIdValidStr : "�����֤������а���������Ϣ�������ַ���<br>",
			checkHkongMacao : "��������Ч�ĸ۰ľ���ͨ��֤���룡<br>",
			checkTaiw : "��������Ч��̨�����ͨ��֤���룡<br>",
			checkPassport : "��������Ч�Ļ��պ��룡<br>"
		};
		messages[mobileNoId] = {
			isMobile : "��������ֻ����벻����Ч�ĸ�ʽ��<br>"
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
	// ��ʼ���̱�
	for ( var i = 1; i < 6; i++) {
		if ($("#passenger_" + i + "_seat").attr("disabled") == false) {
			selectSeatDetail(i);
		}
	}
});

// ����ϯ��ѡ�������̱𣬼����ϡ������С������¡��̣�
// flag=1ʱ��Ϊ��Ӻ�ɾ���˳��ˣ�checkValueӦȡϯ���������һ��option��ֵ
// flag=2ʱ��Ϊ�޸�ϯ��ʱ����̱�������checkValueӦȡϯ��������ѡ�е�option��ֵ
function selectSeatDetail(index) {
	// ��ȡSelectѡ���Value
	var checkValue = $("#passenger_" + index + "_seat").val();
	if (checkValue == '3' || checkValue == '5' || checkValue == 'C') {
		var count = $("#passenger_" + index + "_seat_detail_select option:last")
				.attr("index") + 1;
		if (count == 3) {
			$("#passenger_" + index + "_seat_detail_select option[value='3']")
					.after("<option value='2'>����</option>");
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

// �����̱��������ֵ
function setSeatDetail(index) {
	$("#passenger_" + index + "_seat_detail").val(
			$("#passenger_" + index + "_seat_detail_select").val());
}

// �ڿ�����ʱ����trId2�̱��е�option����trId2�̱��е�option
function copySeatDetailOption(trId1, trId2) {
	var optionCount1 = $("#" + trId1 + "_seat_detail_select option:last").attr(
			"index") + 1;
	var optionCount2 = $("#" + trId2 + "_seat_detail_select option:last").attr(
			"index") + 1;
	if (optionCount1 > optionCount2) {
		$("#" + trId1 + "_seat_detail_select option[value='2']").remove();
	} else if (optionCount1 < optionCount2) {
		$("#" + trId1 + "_seat_detail_select option[value='3']").after(
				"<option value='2'>����</option>");
	} else {
		// ��������
	}
}

//�����ͣ��ʾ��ʾ��-������д˵��
function onStopHover_name(obj) {
	onStopHover(obj, $("#tip_info_div_name"));
}

//�����ͣ��ʾ��ʾ��-��д˵֤��������д˵��
function onStopHover_cardtype(obj) {
	onStopHover(obj, $("#tip_info_div_cardtype"));
}

//�����ͣ��ʾ��ʾ��-������֤��������д˵��
function onStopHover_name_cardtype(obj) {
	onStopHover(obj, $("#tip_info_div_name_cardtype"));
}

//�����ͣ��ʾ��ʾ��
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

//����ƿ�������ʾ��
function onStopOut() {
	$("#tip_info_div_name").hide();
	$("#tip_info_div_cardtype").hide();
	$("#tip_info_div_name_cardtype").hide();
}a

