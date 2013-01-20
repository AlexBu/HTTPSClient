/**
 * 
 * 
 */
var seat_type_size;// 票种，学生=1，正常=4
var default_seat_code;// 初始默认显示的席别

(function($) {
	Array.prototype.remove = function(b) {
		var a = this.indexOf(b);
		if (a >= 0) {
			this.splice(a, 1);
			return true;
		}
		return false;
	};

	var ticketTypeReserveFlagMap = null;
	var checkbox_passenger = null;
	var _disabled_all = null;
	function toPassengerString(passenger) {
		var pValue = passenger.passenger_name
				+ passenger.passenger_id_type_code + passenger.passenger_id_no;
		return pValue;
	}

	/**
	 * disabled某行
	 */
	function disabledAll(trId) {
		$("#" + trId + "_name").val("");
		$("#" + trId + "_cardtype").val("");
		$("#" + trId + "_cardno").val("");
		$("#" + trId + "_mobileno").val("");
		$("#" + trId + "_ticket").val("");
		$("#" + trId + "_seat option[value='" + default_seat_code + "']").attr(
				"selected", "selected");
		$("#" + trId + "_seat_detail").val("0");
		$("#" + trId + "_old").val("");
		$("#" + trId + "_name").attr("disabled", "disabled");
		$("#" + trId + "_cardtype").attr("disabled", "disabled");
		$("#" + trId + "_cardno").attr("disabled", "disabled");
		$("#" + trId + "_mobileno").attr("disabled", "disabled");
		$("#" + trId + "_p").attr("disabled", "disabled");
		$("#" + trId + "_ticket").attr("disabled", "disabled");
		$("#" + trId + "_seat").attr("disabled", "disabled");
		$("#" + trId + "_seat_detail_select").attr("disabled", "disabled");
		$("#" + trId + "_seat_detail").attr("disabled", "disabled");
		$("#" + trId + "_cardtype").attr("disabled", "disabled");
		$("#" + trId + "_isSave").attr("disabled", "disabled");
		$("#" + trId + "_old").attr("disabled", "disabled");
	}
	/**
	 * not disabled 某行
	 */
	function visibledAll(trId) {
		$("#" + trId + "_name").removeAttr("disabled");
		$("#" + trId + "_cardtype").removeAttr("disabled");
		$("#" + trId + "_cardno").removeAttr("disabled");
		$("#" + trId + "_mobileno").removeAttr("disabled");
		$("#" + trId + "_p").removeAttr("disabled");
		$("#" + trId + "_ticket").removeAttr("disabled");
		$("#" + trId + "_seat").removeAttr("disabled");
		$("#" + trId + "_seat_detail_select").removeAttr("disabled");
		$("#" + trId + "_seat_detail").removeAttr("disabled");
		$("#" + trId + "_cardtype").removeAttr("disabled");
		$("#" + trId + "_isSave").removeAttr("disabled");
		$("#" + trId + "_old").removeAttr("disabled");
	}
	/**
	 * 初始化票种、席别关系及学生票的限制规则
	 */
	function _passanger_seat_ticket_init(limitBuySeatTicketDTO,
			studentDisableCheckbox) {
		var seat_type_codes = limitBuySeatTicketDTO.seat_type_codes;
		var ticket_type_codes = limitBuySeatTicketDTO.ticket_type_codes;
		var tickettype_seattype = limitBuySeatTicketDTO.ticket_seat_codeMap;
		function addOptionTo(select, selectOptions, defaultValue) {
			var $select = select;
			var html = "";
			for ( var tni = 0; tni < selectOptions.length; tni++) {
				var selectOption = selectOptions[tni];
				var selected = defaultValue == selectOption.id ? "selected"
						: "";
				html += "<option value='" + selectOption.id + "'" + selected
						+ ">" + selectOption.value + "</option>";
			}
			$select.html(html);
			return $select;
		}
		;
		// 获取初始显示的席别
		default_seat_code = $("#passenger_1_seat").val();
		$(".passenger_class")
				.each(
						function() {
							var id = $(this).attr("id");
							// 初始化席别
							var _seat_select_default_value = $(
									"#" + id + "_seat").val();
							var _seat_select = addOptionTo(
									$("#" + id + "_seat"), seat_type_codes,
									_seat_select_default_value);
							_seat_select.attr("style", "width:74px");// 控制席别下拉框的宽度2011-06-19
							// 响应席别选框点击动作(席别为空提示)
							_seat_select
									.click(function() {

										var _ticket_select_current_value = $(
												"#" + id + "_ticket").val();
										var _seat_select_filter = tickettype_seattype[_ticket_select_current_value];
										var has_no_seat_msg = "";
										var ticket_name = "";
										// 若票种无可购席别，则提示用户
										if (_seat_select_filter == "") {
											if (_ticket_select_current_value == "1") {
												ticket_name = "成人票";
											} else if (_ticket_select_current_value == "2") {
												ticket_name = "儿童票";
											} else if (_ticket_select_current_value == "3") {
												ticket_name = "学生票";
											} else if (_ticket_select_current_value == "4") {
												ticket_name = "残军票";
											} else {
												ticket_name = "";
											}
											has_no_seat_msg = ticket_name
													+ "可购席别票额不足！";
											if (has_no_seat_msg != "") {
												alert(has_no_seat_msg);
											}
										}
									});
							// 初始化票种
							var _ticket_select_default_value = $(
									"#" + id + "_ticket").val();
							var _ticket_select = addOptionTo($("#" + id
									+ "_ticket"), ticket_type_codes,
									_ticket_select_default_value);
							// 响应票种列选框改变动作
							_ticket_select
									.change(function() {
										if (ticket_type_code.student == $(this)
												.val()) {

											// $("#" + id +
											// "_name").attr("disabled","disabled");
											// //disabled情况下jQuery
											// validate验证框架不生效，改成readonly
											// $("#" + id +
											// "_name").attr("readonly","true");
											// // readonly对"证件类型"下拉框不生效
											$("#" + id + "_name").attr(
													"disabled", "disabled");
											$("#" + id + "_cardtype").attr(
													"disabled", "disabled");
											$("#" + id + "_cardno").attr(
													"disabled", "disabled");
											$("#" + id + "_mobileno").attr(
													"disabled", "disabled");
											if (studentDisableCheckbox) {
												$("#" + id + "_isSave")[0].checked = true;
												$("#" + id + "_isSave")
														.removeAttr("disabled");
											}

											else {
												$("#" + id + "_isSave")[0].checked = false;
												$("#" + id + "_isSave").attr(
														"disabled", "disabled");
											}
											var passenger = $("#" + id).data(
													"passenger");
											// 判断当前时间是否在学生票的可买时间内
											if (!isStudentTicketDateValid()) {
												var alertMessage = "学生票的乘车时间为每年的暑假6月1日至9月30日、寒假12月1日至3月31日，目前不办理学生票业务。";
												alert(alertMessage);
												$("#" + id + "_ticket")
														.val("1");
												return;
											}

											// 当前行有乘客信息(通过常用联系人选择上)
											if (passenger != null) {
												var pValue = toPassengerString(passenger);
												// 如果乘客类型不是学生，则清空姓名、证件类型、证件号码、手机号。保存到常用联系人置灰
												if (passenger.passenger_type != "3") {
													$("#" + id + "_name").val(
															"");
													$("#" + id + "_cardtype")
															.val("");
													$("#" + id + "_cardno")
															.val("");
													$("#" + id + "_mobileno")
															.val("");
													$("#" + id).data(
															"passenger", null);
													$("#" + pValue + " input")
															.attr("checked",
																	false);
													// eg:选择学生后，原来的passenger没用了，做处理。
													checkbox_passenger = null;
													$(
															"#"
																	+ pValue
																	+ "[class='_checkbox_class']")
															.attr("checked",
																	false);
													var alertMessage = "请从常用联系人中选择学生旅客";
													alert(alertMessage);
												}
											} else {
												$("#" + id + "_name").val("");
												$("#" + id + "_cardtype").val(
														"");
												$("#" + id + "_cardno").val("");
												$("#" + id + "_mobileno").val(
														"");
												$("#" + id).data("passenger",
														null);
												$("#" + pValue + " input")
														.attr("checked", false);
												var alertMessage = "请从常用联系人中选择学生旅客";
												if (checkbox_passenger) {
													if (checkbox_passenger.passenger_type != 3) {// 处理选择checkbox是学生时不提示用户
														alert(alertMessage);
													}
													checkbox_passenger = null;
												}

											}
										} else if ($("#" + id)[0].style.display != "none"
												&& !_disabled_all) {// 避免处理被隐藏的行
											$("#" + id + "_name").removeAttr(
													"disabled");
											$("#" + id + "_cardtype")
													.removeAttr("disabled");
											$("#" + id + "_cardno").removeAttr(
													"disabled");
											$("#" + id + "_mobileno")
													.removeAttr("disabled");
											$("#" + id + "_isSave")[0].checked = true;
											$("#" + id + "_isSave").removeAttr(
													"disabled");
										}
										var _seat_select_filter = tickettype_seattype[$(
												this).val()];
										var _seat_select_default_val = $(
												"#" + id + "_seat").val();
										addOptionTo($("#" + id + "_seat"),
												_seat_select_filter,
												_seat_select_default_val);
										do_reserve_flag();
									});
							_ticket_select.change();
						});
	}

	// 如果是学生票，判断乘车时间是否在学生票的可买时间内
	function isStudentTicketDateValid() {
		var sTicketStartDate = studentTicketStartDate.split("&");
		var sTicketEndDate = studentTicketEndDate.split("&");

		var curDate = new Date();
		var curYear = curDate.getFullYear();

		var compDateString = train_date_str_ + " 00:00:00";

		/*
		 * IE9下不支持trim var
		 * summenrHolidayStart=curYear+"-"+sTicketStartDate[0].trim(); var
		 * summenrHolidayEnd=curYear+"-"+sTicketEndDate[0].trim(); var
		 * winterHolidayStart=curYear+"-"+sTicketStartDate[1].trim(); var
		 * winterHolidayEnd=(parseInt(curYear,
		 * 10)+1)+"-"+sTicketEndDate[1].trim();
		 */
		var summenrHolidayStart = curYear + "-" + sTicketStartDate[0];
		var summenrHolidayEnd = curYear + "-" + sTicketEndDate[0];
		var winterHolidayStart = curYear + "-" + sTicketStartDate[1];
		var winterHolidayEnd = curYear + "-" + sTicketEndDate[1];

		var winterHolidayStart1 = curYear + "-" + sTicketStartDate[2];
		var winterHolidayEnd1 = curYear + "-" + sTicketEndDate[2];
		var winterHolidayStart2 = (parseInt(curYear, 10) + 1) + "-"
				+ sTicketStartDate[2];
		var winterHolidayEnd2 = (parseInt(curYear, 10) + 1) + "-"
				+ sTicketEndDate[2];
		if ((compDateString >= summenrHolidayStart && compDateString <= summenrHolidayEnd)
				|| (compDateString >= winterHolidayStart && compDateString <= winterHolidayEnd)
				|| (compDateString >= winterHolidayStart1 && compDateString <= winterHolidayEnd1)
				|| (compDateString >= winterHolidayStart2 && compDateString <= winterHolidayEnd2)) {
			return true;
		} else {
			return false;
		}
	}

	/**
	 * 学生票、残军票不可网上支付，当选择学生票或残军票时，自动将支付方式更改为网上预订，不可以选网上支付。
	 */
	function do_reserve_flag() {
		var epayFlag = true;
		$(".passenger_class")
				.each(
						function() {
							var id = $(this).attr("id");
							if (!ticketTypeReserveFlagMap[$(
									"#" + id + "_ticket").val()]
									&& this.style.display != "none") {
								epayFlag = false;
							}
						});
		if (epayFlag) {
			$("#reserve_flag_A").removeAttr("disabled");
			$("#reserve_flag_A").attr("checked", true);
		} else {
			// 网上支付：学生票、残军票暂时也需要支付
			$("#reserve_flag_A").removeAttr("disabled");
			$("#reserve_flag_A").attr("checked", true);
			// 网上预订
			// $("#reserve_flag_A").attr("disabled","disabled");
			// $("#reserve_flag_A").attr("checked",false);
			// $("#reserve_flag_B").attr("checked",true);
		}
	}

	/**
	 * 初始化行tr，以便显示用户信息与查找列表对应起来
	 */
	function init(passengerJson) {
		$(".passenger_class").each(function(index, _tr) {
			var trId = _tr.id;
			var value = "";
			value += $("#" + trId + "_name").val();
			value += $("#" + trId + "_cardtype").val();
			value += $("#" + trId + "_cardno").val();
			for ( var tni = 0; tni < passengerJson.length; tni++) {
				var passenger = passengerJson[tni];
				var pValue = toPassengerString(passenger);
				$("#" + trId + " a:first").unbind("click");
				$("#" + trId + " a:first").bind("click", function() {
					delete_passenger.call(this, trId);
				});
				if (pValue === value) {
					passenger["trId"] = trId;
					$("#" + trId).data("passenger", passenger);
					$("#" + trId + "_mobileno").val(passenger.mobile_no);// 进入改签预订的页面，页面信息显示不全，手机号为空
					$("#" + pValue + " input").attr("checked", true);
					break;
				} else if (_disabled_all) {
					passenger["disabled"] = true;
				}
			}
		});
	}
	/**
	 * 获取当前票数输入行数目
	 */
	function get_show_tr_number() {
		var count = 0;
		$(".passenger_class").each(function(index, _tr) {
			var trId = _tr.id;
			if (_tr.style.display != "none") {
				count++;
			}
		});
		return count;
	}
	/**
	 * 获取第一个为空的可输入票行(包括那些隐藏的)
	 */
	function get_first_empty_tr(isHide) {
		var isFisrst = true;
		var firstTr = null;
		// 查看被隐藏的第一列
		if (!isHide) {
			$(".passenger_class").each(function(index, tr) {
				if (!isFisrst) {
					return;
				}
				if (tr.style.display !== "none") {
					var trId = tr.id;
					var value = "";
					value += $("#" + trId + "_name").val();
					value += $("#" + trId + "_cardno").val();
					value += $("#" + trId + "_mobileno").val();
					if (value === "" && isFisrst) {
						firstTr = trId;
						isFisrst = false;
					}
				}
			});
		} else {
			$(".passenger_class").each(function(index, tr) {
				if (!isFisrst) {
					return;
				}
				if (tr.style.display == "none") {
					var trId = tr.id;
					var value = "";
					value += $("#" + trId + "_name").val();
					value += $("#" + trId + "_cardno").val();
					value += $("#" + trId + "_mobileno").val();
					if (value === "" && isFisrst) {
						firstTr = trId;
						isFisrst = false;
					}
				}
			});
		}
		return firstTr;
	}
	/**
	 * 删除某行
	 */
	function delete_passenger(trId) {
		var passenger = $("#" + trId).data("passenger");
		if (passenger) {
			passenger.trId = null;
			var pValue = toPassengerString(passenger);
			var input = $("#" + pValue + " ._checkbox_class")[0];
			if (input) {
				input.checked = false;
			}
		}

		$("#" + trId).data("passenger", null);
		$("#" + trId + "_name").val("");
		$("#" + trId + "_cardtype").val("");
		$("#" + trId + "_cardno").val("");
		$("#" + trId + "_mobileno").val("");
		// $("#" + trId + "_ticket").val("");
		// 默认成人
		if (seat_type_size == 1) {
			$("#" + trId + "_ticket").val("3");
			// $("#" + trId + "_seat
			// option[value='"+default_seat_code+"']").attr("selected",
			// "selected");
			$("#" + trId + "_seat_detail").val("0");
		} else {
			$("#" + trId + "_ticket").val("1");
			// $("#" + trId + "_seat
			// option[value='"+default_seat_code+"']").attr("selected",
			// "selected");
			$("#" + trId + "_ticket").removeAttr("disabled");
			$("#" + trId + "_seat_detail").val("0");
			$("#" + trId + "_name").removeAttr("disabled");// 学生情况
			$("#" + trId + "_cardtype").removeAttr("disabled");
			$("#" + trId + "_cardno").removeAttr("disabled");
			$("#" + trId + "_mobileno").removeAttr("disabled");
			$("#" + trId + "_isSave")[0].checked = true;
			$("#" + trId + "_isSave").removeAttr("disabled");
		}
		var needMove = false;
		var cusorTr = trId;
		removeValidateMessage(trId);
		$(".passenger_class").each(function(index, tr) {
			var _trId = tr.id;
			// 删除所有的输入框error class
			// removeValidateMessage(_trId);
			if (trId == _trId) {
				needMove = true;
				// 但删除是第一行时，初始化铺别信息。
				selectSeatDetail(trId.substring(trId.length - 1, trId.length));
				return;
			}
			if (tr.style.display !== "none") {
				if (needMove) {
					copyTRToTR(cusorTr, _trId);
					cusorTr = _trId;
				}
			}
		});
		do_reserve_flag();
		$("#error_info").html("");
		if (get_show_tr_number() <= 1) {
			return;
		}
		disabledAll(cusorTr);
		$("#" + cusorTr).hide();
		// 若有“为空的可输入票行(包括那些隐藏的)”，则显示“添加1位乘车人”链接
		var firstEmptytrId1 = get_first_empty_tr(true);
		if (firstEmptytrId1) {
			$(".add_ticket_passenger").show();
		}
	}
	function removeValidateMessage(trId) {
		$("#" + trId + "_seat").removeClass("error");
		$("#" + trId + "_seat_detail_select").removeClass("error");
		$("#" + trId + "_seat_detail").removeClass("error");
		$("#" + trId + "_ticket").removeClass("error");
		$("#" + trId + "_name").removeClass("error");
		$("#" + trId + "_cardtype").removeClass("error");
		$("#" + trId + "_cardno").removeClass("error");
		$("#" + trId + "_mobileno").removeClass("error");
	}
	/**
	 * 处理显示第几位
	 */
	function appendEmptyTr() {
		var firstEmptytrId = get_first_empty_tr(true);
		if (!firstEmptytrId) {
			alert("不能继续添加，已经是最大的可购票数");
			return;
		}
		$("#" + firstEmptytrId + "_name").val("");
		$("#" + firstEmptytrId + "_cardtype").val("");
		$("#" + firstEmptytrId + "_cardno").val("");
		$("#" + firstEmptytrId + "_mobileno").val("");
		$("#" + firstEmptytrId + "_ticket").val("");
		$(
				"#" + firstEmptytrId + "_seat option[value='"
						+ default_seat_code + "']")
				.attr("selected", "selected");
		$("#" + firstEmptytrId + "_seat_detail_select").val("0");
		$("#" + firstEmptytrId + "_seat_detail").val("0");
		$("#" + firstEmptytrId).data("passenger", null);
		$("#" + firstEmptytrId + "_isSave")[0].checked = true;
		$("#" + firstEmptytrId + "_isSave").removeAttr("disabled");
		$("#" + firstEmptytrId).show();
		visibledAll(firstEmptytrId);
		// 在添加乘车人时，初始化铺别。
		selectSeatDetail(firstEmptytrId.substring(firstEmptytrId.length - 1,
				firstEmptytrId.length));
		var needMove = false;
		var cusorTr = firstEmptytrId;
		$(".passenger_class").each(function(index, tr) {
			var _trId = tr.id;
			if (firstEmptytrId == _trId) {
				needMove = true;
				return;
			}
			if (needMove && tr.style.display !== "none") {
				copyTRToTR(cusorTr, _trId);
				cusorTr = _trId;
			}
		});
		parent.doIframe();
		// 若无“为空的可输入票行(包括那些隐藏的)”，则隐藏显示“添加1位乘车人”链接
		var firstEmptytrId1 = get_first_empty_tr(true);
		if (!firstEmptytrId1) {
			$(".add_ticket_passenger").hide();
		}
		return cusorTr;
	}

	/**
	 * 设置元素是否Disabled
	 */
	function setDisabled(trId1, trId2, attrName) {
		if (($("#" + trId2 + attrName).attr("disabled"))) {
			$("#" + trId1 + attrName).attr("disabled", "disabled");
		} else {
			$("#" + trId1 + attrName).removeAttr("disabled");
		}
	}

	/**
	 * 把trId2的拷贝到trId1
	 */
	function copyTRToTR(trId1, trId2) {
		// $("#" + trId1 + "_seat").addClass($("#" + trId2 +
		// "_seat").attr("class")); //拷贝错误验证样式
		// Copy value
		$("#" + trId1 + "_name").val($("#" + trId2 + "_name").val());
		$("#" + trId1 + "_cardtype").val($("#" + trId2 + "_cardtype").val());
		$("#" + trId1 + "_cardno").val($("#" + trId2 + "_cardno").val());
		$("#" + trId1 + "_mobileno").val($("#" + trId2 + "_mobileno").val());
		$("#" + trId1 + "_ticket").val($("#" + trId2 + "_ticket").val());
		$("#" + trId1 + "_seat").val($("#" + trId2 + "_seat").val());
		copySeatDetailOption(trId1, trId2);
		// copy disabled and checked
		setDisabled(trId1, trId2, "_ticket");
		setDisabled(trId1, trId2, "_name");
		setDisabled(trId1, trId2, "_cardtype");
		setDisabled(trId1, trId2, "_cardno");
		setDisabled(trId1, trId2, "_mobileno");
		setDisabled(trId1, trId2, "_isSave");
		// if(($("#" + trId2 + "_ticket").attr("disabled"))) {
		// $("#" + trId1 + "_ticket").attr("disabled","disabled");
		// }
		// if(($("#" + trId2 + "_name").attr("disabled"))) {
		// $("#" + trId1 + "_name").attr("disabled","disabled");
		// }
		// if(($("#" + trId2 + "_cardtype").attr("disabled"))) {
		// $("#" + trId1 + "_cardtype").attr("disabled","disabled");
		// }
		// if(($("#" + trId2 + "_cardno").attr("disabled"))) {
		// $("#" + trId1 + "_cardno").attr("disabled","disabled");
		// }
		// if(($("#" + trId2 + "_mobileno").attr("disabled"))) {
		// $("#" + trId1 + "_mobileno").attr("disabled","disabled");
		// }
		// if(($("#" + trId2 + "_isSave").attr("disabled"))) {
		// $("#" + trId1 + "_isSave").attr("disabled","disabled");
		// }
		if (($("#" + trId2 + "_isSave").attr("checked"))) {
			$("#" + trId1 + "_isSave").attr("checked", "checked");
		} else {
			$("#" + trId1 + "_isSave").removeAttr("checked");
		}
		// 铺别
		$("#" + trId1 + "_seat_detail_select").attr("disabled",
				$("#" + trId2 + "_seat_detail_select").attr("disabled"));
		$("#" + trId1 + "_seat_detail_select").val(
				$("#" + trId2 + "_seat_detail_select").val());
		$("#" + trId1 + "_seat_detail").val(
				$("#" + trId2 + "_seat_detail").val());

		var trId1Passenger = $("#" + trId1).data("passenger");
		var trId2Passenger = $("#" + trId2).data("passenger");
		$("#" + trId1).data("passenger", trId2Passenger);
		if (trId2Passenger) {
			trId2Passenger.trId = trId1;
		}
		$("#" + trId1 + " a:first").unbind("click");
		$("#" + trId1 + " a:first").bind("click", function() {
			delete_passenger.call(this, trId1);
		});
		$("#" + trId2 + " a:first").unbind("click");
		$("#" + trId2 + " a:first").bind("click", function() {
			delete_passenger.call(this, trId2);
		});
		$("#" + trId2).data("passenger", trId1Passenger);
		if (trId1Passenger) {
			trId1Passenger.trId = trId2;
		}
		// TODO 动态调用setDisabled()方法设置，故可删除change事件
		// $("#" + trId1 + "_ticket").change();
	}

	/**
	 * 判断某行的值是否为空
	 */
	function checkTrIsEmpty(trId) {
		var value = $("#" + trId + "_name").val()
				+ $("#" + trId + "_cardno").val()
				+ $("#" + trId + "_mobileno").val();
		return value == "";
	}

	/**
	 * 响应常用联系人的checkbox点击事件
	 */
	function click_passenger_ticket(passenger, checked, input) {
		checkbox_passenger = passenger;
		var child = ticket_type_code.child;
		var student = ticket_type_code.student;
		var disability = ticket_type_code.disability;
		var adult = ticket_type_code.adult;
		if (checked) {
			var trId = get_first_empty_tr(false);
			;
			if (!trId) {
				trId = appendEmptyTr();
				if (!trId) {
					input.checked = false;
					return;
				}
			} else {

				if ($("#" + trId + "_ticket").val() == student
						&& passenger.passenger_type != student) {
					alert("该乘客不是学生，不能添加");
					input.checked = false;

					if (seat_type_size == 1) {
						$("#" + trId + "_name").attr("disabled", "disabled");
						$("#" + trId + "_cardtype")
								.attr("disabled", "disabled");
						$("#" + trId + "_cardno").attr("disabled", "disabled");
						$("#" + trId + "_mobileno")
								.attr("disabled", "disabled");

						$("#" + trId + "_isSave")[0].checked = false;
						$("#" + trId + "_isSave").attr("disabled", "disabled");
					}
					return;
				}

			}
			if (passenger.passenger_type == student) {

				// 假如是学生，不让修改常联系人
				$("#" + trId + "_name").attr("disabled", "disabled");
				$("#" + trId + "_cardtype").attr("disabled", "disabled");
				$("#" + trId + "_cardno").attr("disabled", "disabled");
				$("#" + trId + "_mobileno").attr("disabled", "disabled");
				$("#" + trId + "_ticket").val(student);
				// $("#" + trId + "_ticket").change();
			} else if (passenger.passenger_type == child) {

				$("#" + trId + "_ticket").val(child);

			} else if (passenger.passenger_type == disability) {

				$("#" + trId + "_ticket").val(disability);

			} else {
				$("#" + trId + "_ticket option:first").attr("selected",
						"selected");
				if ($("#" + trId + "_ticket").val() == student
						&& passenger.passenger_type != student) {
					alert("该乘客不是学生，不能添加");
					input.checked = false;
					if (seat_type_size == 1) {
						$("#" + trId + "_name").attr("disabled", "disabled");
						$("#" + trId + "_cardtype")
								.attr("disabled", "disabled");
						$("#" + trId + "_cardno").attr("disabled", "disabled");
						$("#" + trId + "_mobileno")
								.attr("disabled", "disabled");

						$("#" + trId + "_isSave")[0].checked = false;
						$("#" + trId + "_isSave").attr("disabled", "disabled");
					}
					return;
				}
			}

			// 响应票种改变事件-回显席别等
			$("#" + trId + "_ticket").change();
			$("#" + trId + "_name").val(passenger.passenger_name);
			$("#" + trId + "_cardtype").val(passenger.passenger_id_type_code);
			$("#" + trId + "_cardno").val(passenger.passenger_id_no);
			$("#" + trId + "_mobileno").val(passenger.mobile_no);
			$("#" + trId).data("passenger", passenger);
			$("#" + trId + "_seat").attr("style", "width:74px");// 控制席别下拉框的宽度2011-06-19
			$("#" + trId).show();
			// 受让人不让编辑
//			if (passenger.passenger_flag == ticket_type_code.passenger_flag) {
//				$("#" + trId + "_ticket").attr("disabled", "disabled");
//				$("#" + trId + "_name").attr("disabled", "disabled");
//				$("#" + trId + "_cardtype").attr("disabled", "disabled");
//				$("#" + trId + "_cardno").attr("disabled", "disabled");
//				$("#" + trId + "_isSave")[0].checked = false;
//				$("#" + trId + "_isSave").attr("disabled", "disabled");
//				$("#" + trId + "_ticket").val(adult);
//			}
			passenger["trId"] = trId;
			$("#" + trId + " a:first").unbind("click");
			$("#" + trId + " a:first").bind("click", function() {
				delete_passenger.call(this, trId);
			});
		} else {
			var trId = passenger.trId;
			delete_passenger(trId);
		}

	}
	;
	/**
	 * 响应添加按钮
	 */
	function add_passenger_ticket() {
		var trId = appendEmptyTr();
		$("#" + trId + " a:first").unbind("click");
		$("#" + trId + " a:first").bind("click", function() {
			delete_passenger.call(this, trId);
		});
		if (seat_type_size == 1) {
			$("#" + trId + "_name").attr("disabled", "disabled");
			$("#" + trId + "_cardtype").attr("disabled", "disabled");
			$("#" + trId + "_cardno").attr("disabled", "disabled");
			$("#" + trId + "_mobileno").attr("disabled", "disabled");

			$("#" + trId + "_isSave")[0].checked = false;
			$("#" + trId + "_isSave").attr("disabled", "disabled");

			// 判断当前时间是否在学生票的可买时间内
			if (!isStudentTicketDateValid()) {
				var alertMessage = "学生票的乘车时间为每年的暑假6月1日至9月30日、寒假12月1日至3月31日，目前不办理学生票业务。";
				alert(alertMessage);
				$("#" + trId + "_ticket").val("1");
				return;
			}
		}

	}
	;

	/**
	 * 全部checkbox按钮
	 */
	function init_select_all_Checkbox() {
		$("#checkbox_select_all").click(function() {
			if (this.checked) {
				$(".passenger_class").each(function() {
					var id = this.id;
					if (this.style.display !== "none") {
						$("#" + id + "_isSave").attr("checked", true);
					}
				});
			} else {
				$(".passenger_class").each(function() {
					var id = this.id;
					if (this.style.display !== "none") {
						$("#" + id + "_isSave").attr("checked", false);
					}
				});
			}
		});
	}
	/**
	 * 每行的checkbox的动作
	 */
	function init_isSave_checkbox() {
		$(".passenger_class").each(function() {
			var id = this.id;
			$("#" + id + "_isSave").click(function() {
				var checkedCount = 0;
				var showTr = 0;
				$(".passenger_class").each(function() {
					var id = this.id;
					if (this.style.display !== "none") {
						showTr++;
						if ($("#" + id + "_isSave").attr("checked")) {
							checkedCount++;
						}
					}
				});
				if (showTr == checkedCount) {
					$("#checkbox_select_all").attr("checked", true);
				} else {
					$("#checkbox_select_all").attr("checked", false);
				}
			});
		});
	}

	/**
	 * 初始化常用联系人
	 */
	function query_passenger_add(_thisInput, passengerJson,
			showPassengerFilter, selectPassengerFn) {
		var filterPassengers = [];
		var $showPassengerFilter = $("#" + showPassengerFilter).html("");
		var html = "";
		var indexFilter = 0;
		$
				.each(
						passengerJson,
						function(index, passenger) {
							filterPassengers.push(passenger);
							var student_flag = passenger.passenger_type == "3" ? "(学生)"
									: "";
							var passenger_flag_style = passenger.passenger_flag == ticket_type_code.passenger_flag ? "color:red;"
									: "";
							// alert(passenger_flag_style+"---"+passenger.passenger_name);
							var pValue = toPassengerString(passenger);
							if (passenger.disabled) {
								html += "<div style='float:left;width:110px;' id='"
										+ pValue
										+ "'><span class=\"disable\"><input class='_checkbox_class' type='checkbox' name='checkbox"
										+ indexFilter
										+ "'  value='"
										+ indexFilter

										+ "' disabled='disabled'/></span><span class=\"disable\" style=\'"
										+ passenger_flag_style
										+ "\'>"

										+ passenger.passenger_name
										+ student_flag + "</span></div>";
							} else {
								html += "<div style='float:left;width:110px;' id='"
										+ pValue
										+ "'><span><input class='_checkbox_class' type='checkbox' name='checkbox"
										+ indexFilter
										+ "' id='"
										+ pValue
										+ "' value='"
										+ indexFilter
										+ "'  /></span><span style=\'"
										+ passenger_flag_style
										+ "\'>"
										+ passenger.passenger_name
										+ student_flag + "</span></div>";
							}
							if (index > 0 && index % 8 == 7) {
								html += "<div style='clear: both;'/>";
							}
							indexFilter++;
						});

		$showPassengerFilter.html(html);
		$("#" + showPassengerFilter + " ._checkbox_class").click(
				function(e) {
					var input = e.target;
					var index = $(input).val();
					var filterPassenger = filterPassengers[index];
					selectPassengerFn.apply(filterPassenger, [ filterPassenger,
							input.checked, input ]);
				});
	}
	;
	var initText = "中文或拼音首字母";

	/**
	 * 对外的接口
	 */
	$.fn.query_passenger = function(passengerJson, showPassengerFilter,
			_disalbed, limitBuySeatTicketDTO, _ticketTypeReserveFlagMap,
			studentDisableCheckbox) {
		seat_type_size = limitBuySeatTicketDTO.ticket_type_codes.length;
		var selectPassengerFn = click_passenger_ticket;
		ticketTypeReserveFlagMap = _ticketTypeReserveFlagMap;
		_disabled_all = _disalbed;

		var _thisInput = this;
		$(this).val(initText);
		query_passenger_add(_thisInput, passengerJson, showPassengerFilter,
				selectPassengerFn);
		init(passengerJson);
		_passanger_seat_ticket_init(limitBuySeatTicketDTO,
				studentDisableCheckbox);
		init_isSave_checkbox();
		init_select_all_Checkbox();
		this.click(function() {
			if ($(this).val() === initText) {
				$(this).val("");
			}
		});
		$(".add_ticket_passenger").click(add_passenger_ticket);
		// 查询乘客信息
		this.keyup(function() {
			var val = $.trim($(_thisInput).val());
			if (val === initText) {
				val = "";
			}
			var reg = new RegExp('^' + val + '.*$', 'im');
			var $showPassengerFilter = $("#" + showPassengerFilter);
			$.each(passengerJson, function(index, passenger) {
				var pValue = toPassengerString(passenger);
			    if (reg.test(passenger.passenger_name)
						|| reg.test(passenger.first_letter) || val == "") {
					$(document.getElementById(pValue)).show();
				} else {
					$(document.getElementById(pValue)).hide();  //$("#"+pValue).show()或$("#"+pValue).hide()若pValue带有空格则无法定位元素，空格在jquery里特殊含义
				}
			});
		});
	};
})(jQuery);