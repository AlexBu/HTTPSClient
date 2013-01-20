//左侧点击弹开

$(function(){
	
	$(".i-sl-con dd h3").click(function(){
	    var $ul = $(this).siblings("ul");
	    if($ul.is(":visible")){
//	      $(this).parent().removeClass();
//	      $ul.hide();
	    }else{
	      $(this).parent().children("h3").children("em").addClass("open");
	      $ul.show();
	      $(this).parent().siblings().children("h3").children("em").removeClass("open");
	      $(this).parent().siblings().children("ul").hide();
	    }
	  });
	
});
