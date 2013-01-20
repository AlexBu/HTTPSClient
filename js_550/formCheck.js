
//姓名中文或者英文
jQuery.validator.addMethod("checkNameChar", function(value, element) {
	return this.optional(element) || /^[a-zA-Z\u4E00-\u9FA5]+$/.test(value);
}, "wrong name.");
//姓名中文或者英文,英文可包含空格。
jQuery.validator.addMethod("checkNameBlank", function(value, element) {
	return this.optional(element) || /^[a-z A-Z\u4E00-\u9FA5]+$/.test(trim(value));
}, "wrong name.");
// 姓名中文或者英文，除一代和二代身份证外可以有空格
jQuery.validator.addMethod("checkNameCharBlank", function(value, element, param) {
	var temp = param.split("@");
	if ($("#"+temp[1]).val() == '') {
		return true;
	} else {
		if ($("#"+temp[0]).val() == '1' || $("#"+temp[0]).val() == '2') {
			return this.optional(element) || /^[a-zA-Z·.．\u4E00-\u9FA5]+$/.test(value);
		} else {
			return this.optional(element) || /^[a-z A-Z·.．\u4E00-\u9FA5]+$/.test(value);
		}
	}
}, "wrong name.");
//用户名只能包括英文字母、数字
jQuery.validator.addMethod("validateUserName", function(value, element) {
	//return this.optional(element) || /^[a-zA-Z0-9\u4E00-\u9FA5]+$/.test(value);
	return this.optional(element) || /^[a-zA-Z][a-zA-Z0-9]+$/.test(value);
}, "wrong username.");
// 验证码只能包括英文字母、数字
jQuery.validator.addMethod("validateRandCode", function(value, element) {
	return this.optional(element) || /^[a-zA-Z0-9]+$/.test(value);
}, "wrong randCode.");
//只能包括中文、英文、数字
jQuery.validator.addMethod("validateName", function(value, element) {
	return this.optional(element) || /^[a-zA-Z\u4E00-\u9FA50-9\_]+$/.test(value);
}, "wrong username.");
//只能包括中文、英文、数字、"_"
jQuery.validator.addMethod("validateUsersName", function(value, element) {
	//return this.optional(element) || /^[a-zA-Z\u4E00-\u9FA50-9\_]+$/.test(value);
	return this.optional(element) || /^[A-Za-z]{1}([A-Za-z0-9]|[_]){0,29}$/.test(value);
}, "用户名只能由字母、数字或_组成");

// 手机号码验证
jQuery.validator.addMethod("isMobile", function(value, element) {
	var length = value.length;
	return this.optional(element)
			|| (length == 11&&/^[0-9]+$/.test(value));
}, "wrong mobile phone ");

//出生日期校验
jQuery.validator.addMethod("validatreBirthday", function(value, element) {
	return this.optional(element)
			|| /[1-2][0,8,9][0-9][0-9]-[0,1][0-9]-[0,1,2,3][0-9]+$/.test(value);
}, "wrong birthday ");

// 电话号码验证    
jQuery.validator.addMethod("isPhone", function(value, element) {
	var tel = /^(\d{3,4}-?)?\d{7,9}$/g;
	return this.optional(element) || (tel.test(value));
}, "wrong telephone");

//密码验证，不包含'<>    
jQuery.validator.addMethod("checkPassward",
		function(value, element, param) {
			var validate=true;
			for ( var i = 0; i < value.length; i++) {
				if (value.charCodeAt(i) ==39 || value.charCodeAt(i) ==60 || value.charCodeAt(i) ==62) {
					validate=false;
				}
				if(!validate){
					break;
				}
			}
			return this.optional(element)
					|| validate;
		}, "Passward wrong");

// 验证，不包含'"<>?    39 34 60 62 63
jQuery.validator.addMethod("illegalChar",
		function(value, element, param) {
			var validate=true;
			for ( var i = 0; i < value.length; i++) {
				if (value.charCodeAt(i) ==39 || value.charCodeAt(i) ==60 || value.charCodeAt(i) ==62 || value.charCodeAt(i) ==34 || value.charCodeAt(i) ==63) {
					validate=false;
				}
				if(!validate){
					break;
				}
			}
			return this.optional(element)
					|| validate;
		}, "Illegal char wrong");

// 多个值的异步校验,param为object类型
jQuery.validator.addMethod("checkAnsy", function(value, element,param) {
	var _url = param[0];
	var ajaxdata={};
	jQuery.each(param,function(index,data){
		if(index>0){
		    ajaxdata[data] = $('#'+data).val();
		}
	});
	$.ajax(
			{//QueryETicketAction
				url :_url,
				type :'POST',
				dataType :'text',
				data:ajaxdata,
				success:function(data,textStatus){
				if(data=="false")
				   return false;
				} ,
				error:function(XMLHttpRequest, textStatus, errorThrown){
				   return true;
				}
				}
			);
	return true;
}, "wrong cardNo");

//自定义方法，中文两个字节
jQuery.validator.addMethod("byteRangeLength",
		function(value, element, param) {
			var length = value.length;
			for ( var i = 0; i < value.length; i++) {
				if (value.charCodeAt(i) > 127) {
					length++;
				}
			}
			return this.optional(element)
					|| (length >= param[0] && length <= param[1]);
		}, "length wrong");

// 邮政编码验证    
jQuery.validator.addMethod("isZipCode", function(value, element) {
	var tel = /^[0-9]{6}$/;
	return this.optional(element) || (tel.test(value));
}, "wrong zipcode");
//校验密码提示问题
jQuery.validator.addMethod("isQuestionNull", function(value, element,param) {
	if(jQuery.trim(value)!=''){
		if(jQuery.trim($(param[0]).val())=='customQuestion'&&jQuery.trim($(param[1]).val())==''||jQuery.trim($(param[0]).val())==''){
			return false;
		}
	}
	return true;
}, "you should input the question");

//校验密码提示答案
jQuery.validator.addMethod("isAnswerNull", function(value, element,param) {
		if((jQuery.trim($(param[0]).val())=='customQuestion'&&jQuery.trim($(param[1]).val())!='')||(jQuery.trim($(param[0]).val())!='')){
			if(jQuery.trim(value)=='')
			return false;
	}
	return true;
}, "you should input the answer");


function checkIfIdCard(cardtype){
		if (cardtype== "2"||cardtype== "1") {
			return true;
		}
		return false;
}

function checkIfSecIdCard(cardtype){
	if (cardtype== "1") {
		return true;
	}
	return false;
}

function checkIfFirIdCard(cardtype){
	if (cardtype== "2") {
		return true;
	}
	return false;
}

function checkCardForHKorTW(cardtype){
	if (cardtype == "C" || cardtype == "G") {
		return true;
	}
	return false;
}

jQuery.validator.addMethod("checkHkongMacao", function(value, element,param) {
	if($(param).val()=="C"){ //11位，第1位字母H、M，后面全是数字
		var tel =  /^[HMhm]{1}[0-9]{10}$/;
		return this.optional(element) || (tel.test(value));
	} else {
		return true;
	}
}, "wrong format.");

jQuery.validator.addMethod("checkTaiw", function(value, element,param) {
	if($(param).val()=="G"){//台湾8或者10位数字
		var tel1 =  /^[0-9]{8}$/;
		var tel2 =  /^[0-9]{10}$/;
		return this.optional(element) || (tel1.test(value)) || (tel2.test(value));
	} else {
		return true;
	}
}, "wrong format.");

jQuery.validator.addMethod("checkPassport", function(value, element,param) {
	if($(param).val()=="B"){//护照规则，5-17位，由数字和字母组成，不能全是字母
		var tel2= /^[a-zA-Z]*$/;
		if(tel2.test(value)){
			return false;
		}
		var tel =  /^[a-zA-Z0-9]{5,17}$/;
		return this.optional(element) || (tel.test(value));
	} else {
		return true;
	}
}, "wrong format.");

// 席别验证
jQuery.validator.addMethod("checkSeatType", function(value, element,param) {
	var isCheck = $(param).attr("checked");
	if(isCheck || isCheck=="true"){//选择“改签”或者“返程”时，需要验证席别不能为空
		if(value == "" || value=="null") {
			return false;
		}
		return true;
	} else {
		// 不选择的话不验证
		return true;
	}
}, "wrong format.");

function validateIdCard(value)
{var iSum = 0;
var info = "";
var sId;
var aCity = {
	11 : "北京",
	12 : "天津",
	13 : "河北",
	14 : "山西",
	15 : "内蒙",
	21 : "辽宁",
	22 : "吉林",
	23 : "黑龙",
	31 : "上海",
	32 : "江苏",
	33 : "浙江",
	34 : "安徽",
	35 : "福建",
	36 : "江西",
	37 : "山东",
	41 : "河南",
	42 : "湖北",
	43 : "湖南",
	44 : "广东",
	45 : "广西",
	46 : "海南",
	50 : "重庆",
	51 : "四川",
	52 : "贵州",
	53 : "云南",
	54 : "西藏",
	61 : "陕西",
	62 : "甘肃",
	63 : "青海",
	64 : "宁夏",
	65 : "新疆",
	71 : "台湾",
	81 : "香港",
	82 : "澳门",
	91 : "国外"
};
//如果输入的为15位数字,则先转换为18位身份证号
	if (value.length == 15)
		sId = idCardUpdate(value);
	else
		sId = value;

	if (!/^\d{17}(\d|x)$/i.test(sId)) {
		return false;
	}
	sId = sId.replace(/x$/i, "a");
	//非法地区
	if (aCity[parseInt(sId.substr(0, 2))] == null) {
		return false;
	}
	var sBirthday = sId.substr(6, 4) + "-" + Number(sId.substr(10, 2))
			+ "-" + Number(sId.substr(12, 2));
	var d = new Date(sBirthday.replace(/-/g, "/"));
	//非法生日
	if (sBirthday != (d.getFullYear() + "-" + (d.getMonth() + 1) + "-" + d
			.getDate())) {
		return false;
	}
	for ( var i = 17; i >= 0; i--) {
		iSum += (Math.pow(2, i) % 11) * parseInt(sId.charAt(17 - i), 11);
	}
	if (iSum % 11 != 1) {
		return false;
	}
	return true;
}

function validateSecIdCard(value)
{var iSum = 0;
var info = "";
var sId= value;
var aCity = {
	11 : "北京",
	12 : "天津",
	13 : "河北",
	14 : "山西",
	15 : "内蒙",
	21 : "辽宁",
	22 : "吉林",
	23 : "黑龙",
	31 : "上海",
	32 : "江苏",
	33 : "浙江",
	34 : "安徽",
	35 : "福建",
	36 : "江西",
	37 : "山东",
	41 : "河南",
	42 : "湖北",
	43 : "湖南",
	44 : "广东",
	45 : "广西",
	46 : "海南",
	50 : "重庆",
	51 : "四川",
	52 : "贵州",
	53 : "云南",
	54 : "西藏",
	61 : "陕西",
	62 : "甘肃",
	63 : "青海",
	64 : "宁夏",
	65 : "新疆",
	71 : "台湾",
	81 : "香港",
	82 : "澳门",
	91 : "国外"
};
	if (!/^\d{17}(\d|x)$/i.test(sId)) {
		return false;
	}
	sId = sId.replace(/x$/i, "a");
	//非法地区
	if (aCity[parseInt(sId.substr(0, 2))] == null) {
		return false;
	}
	var sBirthday = sId.substr(6, 4) + "-" + Number(sId.substr(10, 2))
			+ "-" + Number(sId.substr(12, 2));
	var d = new Date(sBirthday.replace(/-/g, "/"));
	//非法生日
	if (sBirthday != (d.getFullYear() + "-" + (d.getMonth() + 1) + "-" + d
			.getDate())) {
		return false;
	}
	for ( var i = 17; i >= 0; i--) {
		iSum += (Math.pow(2, i) % 11) * parseInt(sId.charAt(17 - i), 11);
	}
	if (iSum % 11 != 1) {
		return false;
	}
	return true;
}

function validateFirIdCard(value)
{var iSum = 0;
var info = "";
var sId;
var aCity = {
	11 : "北京",
	12 : "天津",
	13 : "河北",
	14 : "山西",
	15 : "内蒙",
	21 : "辽宁",
	22 : "吉林",
	23 : "黑龙",
	31 : "上海",
	32 : "江苏",
	33 : "浙江",
	34 : "安徽",
	35 : "福建",
	36 : "江西",
	37 : "山东",
	41 : "河南",
	42 : "湖北",
	43 : "湖南",
	44 : "广东",
	45 : "广西",
	46 : "海南",
	50 : "重庆",
	51 : "四川",
	52 : "贵州",
	53 : "云南",
	54 : "西藏",
	61 : "陕西",
	62 : "甘肃",
	63 : "青海",
	64 : "宁夏",
	65 : "新疆",
	71 : "台湾",
	81 : "香港",
	82 : "澳门",
	91 : "国外"
};
	//如果输入的为15位数字,则先转换为18位身份证号
	if (value.length == 15)
		sId = idCardUpdate(value);
	else
		sId = value;
	if (!/^\d{17}(\d|x)$/i.test(sId)) {
		return false;
	}
	sId = sId.replace(/x$/i, "a");
	//非法地区
	if (aCity[parseInt(sId.substr(0, 2))] == null) {
		return false;
	}
	var sBirthday = sId.substr(6, 4) + "-" + Number(sId.substr(10, 2))
			+ "-" + Number(sId.substr(12, 2));
	var d = new Date(sBirthday.replace(/-/g, "/"));
	//非法生日
	if (sBirthday != (d.getFullYear() + "-" + (d.getMonth() + 1) + "-" + d
			.getDate())) {
		return false;
	}
	for ( var i = 17; i >= 0; i--) {
		iSum += (Math.pow(2, i) % 11) * parseInt(sId.charAt(17 - i), 11);
	}
	if (iSum % 11 != 1) {
		return false;
	}
	return true;
}

//jQuery.validator.addMethod("isIDCard", function(value, element, param) {
//	if(!checkIfIdCard($(param).val())){
//		return true;
//	}
//	return validateIdCard(value);
//	}, "wrong");
jQuery.validator.addMethod("isSecIDCard", function(value, element, param) {
	if(!checkIfSecIdCard($(param).val())){
		return true;
	}
	return validateSecIdCard(value);
	}, "wrong");
jQuery.validator.addMethod("isFirIDCard", function(value, element, param) {
	if(!checkIfFirIdCard($(param).val())){
		return true;
	}
	return validateFirIdCard(value);
	}, "wrong");
function idCardUpdate(_str) {
	var idCard18;
	var regIDCard15 = /^(\d){15}$/;
	if (regIDCard15.test(_str)) {
		var nTemp = 0;
		var ArrInt = new Array(7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8,
				4, 2);
		var ArrCh = new Array('1', '0', 'X', '9', '8', '7', '6', '5', '4', '3',
				'2');
		_str = _str.substr(0, 6) + '1' + '9' + _str.substr(6, _str.length - 6);
		for ( var i = 0; i < _str.length; i++) {
			nTemp += parseInt(_str.substr(i, 1)) * ArrInt[i];
		}
		_str += ArrCh[nTemp % 11];
		idCard18 = _str;
	} else {
		idCard18 = "#";
	}
	return idCard18;
}

jQuery.validator.addMethod("checkIdValidStr", function(value, element) {
	var tel =  /^[a-zA-Z0-9\_\-\(\)]+$/;
	return this.optional(element) || (tel.test(value));
}, "wrong id");

// 订单查询中，验证车次。
jQuery.validator.addMethod("checkTrainCode", function(value, element) {
	var tel =  /^[a-zA-Z0-9\_\-\(\)]+$/;
	return this.optional(element) || (tel.test(trim(value)));
}, "wrong train_code");

// 订单查询中，验证车次的长度，最大长度为8个字符。
jQuery.validator.addMethod("checkTrainCodeLen", function(value, element) {
	if (trim(value).length <= 8) {
		return true;
	} else {
		return false;
	}
}, "wrong train_code length");

//订单查询中，验证订单号，仅允许输入字母和数字。
jQuery.validator.addMethod("checkSequenceNo", function(value, element) {
	var tel =  /^[a-zA-Z0-9\_\-\(\)]+$/;
	return this.optional(element) || (tel.test(trim(value)));
}, "wrong sequence_no");

// 订单查询中，验证订单号的长度，长度为10个字符。
jQuery.validator.addMethod("checkSequenceNoLen", function(value, element) {
	if(trim(value) == "") {
		return true;
	}
	if (trim(value).length == 10) {
		return true;
	} else {
		return false;
	}
}, "wrong sequence_no length");

// 订单查询中，验证证件号码长度不能多于18位
jQuery.validator.addMethod("checkIdNoLen", function(value, element) {
	if (trim(value).length <= 18) {
		return true;
	} else {
		return false;
	}
}, "wrong id_no length");
//订单查询中，验证证件号码只允许输入字母和数字
jQuery.validator.addMethod("checkIdValid", function(value, element) {
	var tel =  /^[a-zA-Z0-9\_\-\(\)]+$/;
	return this.optional(element) || (tel.test(trim(value)));
}, "wrong id");

jQuery.validator.addMethod("checkWriteSpace", function(value, element) {
	for(var i =0;i<value.length;i++){
		if(value.charCodeAt(i)== 32){
			return false;
		}
	}
	return true;
}, "contain writespace");


function checkSexByCardId(value,chardTypeId,cardNoId){
	function _checkSexByCardId(value,cardId){
		
		var sexMark = null;
		if(cardId.length == 15){
			sexMark = cardId.substring(14,15);
		}else if(cardId.length == 18){
			sexMark = cardId.substring(16,17);
		}else{
			return true;
		}
		if(sexMark == "x"||sexMark == "X"){
			sexMark = "10";
		}
		var sexMarkInt = parseInt(sexMark);
		var sexMarkOddEven = sexMarkInt%2;
		if(sexMarkOddEven ===0 && value==='F'){
			return true;
		}else if(sexMarkOddEven ===1 && value==='M'){
			return true;
		}else{
			return false;
		}
	}
	var cardId = $(cardNoId).val();
	if(checkIfSecIdCard($(chardTypeId).val())&&validateSecIdCard(cardId)){
		if(cardId !== ""){
			return _checkSexByCardId(value,cardId);
		}else{
			return true;
		}
	}else if(checkIfFirIdCard($(chardTypeId).val())&&validateFirIdCard(cardId)){
		if(cardId !== ""){
			return _checkSexByCardId(value,cardId);
		}else{
			return true;
		}
	}else{
		return true;
	}
}

function checkBirdDateByCardId(value,chardTypeId,cardNoId){
	var cardBirddate = null;
	var cardId = $(cardNoId).val();
	
	if(checkIfSecIdCard($(chardTypeId).val())&&cardId!==""&&validateSecIdCard(cardId)){
		cardBirddate =cardId.substring(6,14);
	}else if(checkIfFirIdCard($(chardTypeId).val())&&cardId!==""&&validateFirIdCard(cardId)){
		if(cardId.length == 15){
			cardBirddate ="19"+cardId.substring(6,12);
		}else if(cardId.length == 18){
			cardBirddate =cardId.substring(6,14);
		}
	}else{
		return true;
	}
	
	if(value !== ""){
		value = value.replace(/-/g, "");
		if(value != cardBirddate){
			return false;
		}else{
			return true;
		}
	}else{
		return true;
	}
}


function checkSex(sexValue,cardTypeId,cardNoId){
	if(!checkSexByCardId(sexValue,cardTypeId,cardNoId)){
		if(!confirm("性别与身份证中的性别不符，是否继续?")){
			return false;
		}else{
			return true;
		}
	}else{
		return true;
	}
}

function checkBirdate(birdDateValue,cardTypeId,cardNoId){
	if(!checkBirdDateByCardId(birdDateValue,cardTypeId,cardNoId)){
		if(!confirm("出生日期与身份证中的出生日期不符，是否继续?")){
			return false;
		}else{
			return true;
		}
	}else{
		return true;
	}
}

//替换特殊字符
function replaceChar(str) {
	var v = str.value.replace(/['"<> ?]/g,"");
	str.value= v;
}

//验证只能包含中文、英文、数字
function checkNameChar1(str) {
	return /^[a-zA-Z0-9\u4E00-\u9FA5]+$/.test(str);
}
//删除左右两端的空格
function trim(str) {
	return str.replace(/(^\s*)|(\s*$)/g, "");
}
//删除左边的空格
function ltrim(str) {  
	return str.replace(/(^\s*)/g, "");
}
//删除右边的空格
function rtrim(str) {
	return str.replace(/(\s*$)/g, "");
}