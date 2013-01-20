/**
 * 弹出消息窗口
 * 
 * @param title
 *            标题
 * @param msgtext
 *            消息内容
 * @param fncancel
 *            按钮调用方法（取消）
 * @param fnok
 *            按钮调用方法（确定）
 * @param cancel
 *            按钮名称（取消）
 * @param ok
 *            按钮名称（确定）
 * @param width
 *            窗口宽度，不传值使用默认值width_default定义
 */
function pop_confirm(title, msgtext, fncancel, fnok, cancel, ok, width) {
	setIframMainHeight();
	var win = $(
			"<div id=\"yc\" style='text-align:left' title='" + title
					+ "'></div>").appendTo(document.body);
	var win_width;
	var width_default = "400";
	if (width == null || width == "" || width == undefined) {
		win_width = Number(width_default);
	} else {
		try {
			win_width = Number(width);
		} catch (e) {
			win_width = Number(width_default);
		}
	}
	win.html(msgtext);
	var buttons = {};
	buttons[ok] = function() {
		fnok.apply(win, [ win ]);
		win.dialog('close');
		win.dialog('destroy');
		win.remove();
	};
	buttons[cancel] = function() {
		fncancel.apply(win, [ win ]);
		win.dialog('close');
		win.dialog('destroy');
		win.remove();
	};
	win.dialog({
		height : "auto",
		width : win_width,
		modal : true,
		buttons : buttons,
		closeOnEscape : false,
		draggable : false,
		resizable : false
	});
	// $(".ui-widget-overlay").css({height:screen.height});
}
function setIframMainHeight() {
	var iframe = document.getElementById("main");
	if(iframe.height<screen.height) {
		iframe.height = screen.height;
	}
}
/**
 * 用于交易排队的弹出层
 * 
 * @param title
 * @param msgtext
 * @param fnok
 * @param cancer
 * @param width
 * @return
 */
var popWin;
function pop_confirm_queue(title, msgtext, fnok, width) {
	setIframMainHeight();
	win_pop = $(
			"<div  id='test_dilog_id' style='text-align:left' title='" + title
					+ "'></div>").appendTo(document.body);
	var win_width;
	var width_default = "400";
	if (width == null || width == "" || width == undefined) {
		win_width = Number(width_default);
	} else {
		try {
			win_width = Number(width);
		} catch (e) {
			win_width = Number(width_default);
		}
	}
	win_pop.html(msgtext);
	$("#link").click(function() {
		fnok.apply(win_pop, [ win_pop ]);
		win_pop.dialog('close');
		win_pop.dialog('destroy');
		win_pop.remove();
	});
	popWin = win_pop.dialog({
		height : "auto",
		width : win_width,
		modal : true,
		closeOnEscape : false,
		draggable : false,
		resizable : false
	});
}

function closePopWin() {
	popWin.dialog('close');
	popWin.dialog('destroy');
	popWin.remove();
}

/**
 * 弹出对话筐窗口
 * 
 * @param title
 *            标题
 * @param msgtext
 *            消息内容
 * @param fncancel
 *            按钮调用方法（取消）
 * @param fnok
 *            按钮调用方法（确定）
 * @param cancel
 *            按钮名称（取消）
 * @param ok
 *            按钮名称（确定）
 * @param width
 *            窗口宽度，不传值使用默认值width_default定义
 */
var integration_win_dialog;
function integrationpay_pop_confirm(title, msgtext, fncancel, fnok, cancel, ok,
		width) {
	setIframMainHeight();
	var win = $(
			"<div id=\"yc\" style='text-align:left' title='" + title
					+ "'></div>").appendTo(document.body);
	var win_width;
	var width_default = "400";
	if (width == null || width == "" || width == undefined) {
		win_width = Number(width_default);
	} else {
		try {
			win_width = Number(width);
		} catch (e) {
			win_width = Number(width_default);
		}
	}
	win.html(msgtext);
	var buttons = {};

	buttons[cancel] = function() {
		fncancel.apply(win, [ win ]);
		win.dialog('close');
		win.dialog('destroy');
		win.remove();
	};
	buttons[ok] = function() {
		var flage = fnok.apply(win, [ win ]);
		if (!flage == 'false') {
			win.dialog('close');
			win.dialog('destroy');
			win.remove();
		}
	};
	integration_win_dialog = win.dialog({
		height : "auto",
		width : win_width,
		modal : true,
		buttons : buttons,
		closeOnEscape : false,
		draggable : false,
		resizable : false
	});
}