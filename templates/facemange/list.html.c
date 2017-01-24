{% extends 'layouts/base.html' %}
{% block content %}

<script type="text/javascript">
    function selectcheck(){
        var selected = new Array();
        var elements = $("[name=elements]");
        for(var i=0;i<elements.length;i++){
            if(elements[i].checked){
                selected.push(elements[i].getAttribute("dirname"));
            };
        };
        if(selected.length==0){
            alert("请先选择要移动的文件");
            return 0;
        }
        $("#moves").click();
        
    };
</script>

<script type="text/javascript">
$(function(){
    $("#removes").click(function(){
        var selected = new Array();
        var elements = $("[name=elements]");
        for(var i=0;i<elements.length;i++){
            if(elements[i].checked){
                selected.push(elements[i].getAttribute("dirname"));
            };
        };
        if(selected.length==0){
            alert("请先选择要删除的文件");
            return 0;
        };
        $("#dsource").val(selected.join("|"));
    });
    $("#stops").click(function(){
        $("#status").val("stop");
        document.forms.submit();
    });
    $("#starts").click(function(){
        $("#status").val("start");
        document.forms.submit();
    });
    $("#allbox").click(function(){
        var elements = $("input[name=elements]");
        for(var i=0;i<elements.length;i++){
            if(this.checked){
                elements[i].checked = true;
                }else{
                elements[i].checked =false;
                };
        };
    });
});
</script>

<div class="row">
    <div class="col-lg-2">
        <div class="form-horizontal">
            <div class="panel panel-default">
                <div class="panel-heading">人脸检测</div>
                <div class="panel-body">
                    <div class="row"><div class="col-lg-12">
                    <div class="row" style="text-align:center;">
                        <input type="button" class="btn btn-primary" value=" 检 测 " id="cap">
                	<input type="button" class="btn btn-default" value=" 停 止 " id="caps">
                    </div>
                        </div>
                    </div>
                </div>
            </div>
            <div class="panel panel-default">
                <div class="panel-heading">目录结构</div>
                <div class="panel-body">
                    <div class="row"><div class="col-lg-12">
                            <div id="treeview" class="treeview">
                                    {% for dir in tdirs %}
                                    <a href="{{ url_for('facemange.list') }}?dirname={{ dir.name }}" class="list-group-item"><i class="fa fa-folder fa-lg"></i>  {{ dir.name }}</a>
                                    {% endfor %}
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            <!--div class="panel panel-default">
                <div class="panel-heading">服务器状态</div>
                <div class="panel-body">
                    <div class="row"><div class="col-lg-12">
                        <form action="{{ url_for('facemange.server') }}" method="POST" name="forms">
                            <input name="_csrf_token" type="hidden" value="{{ csrf_token() }}">
                            <input type="hidden" value="{{ status }}"  id="status" name="status">
                        <ul class="list-group">
                            <li class="list-group-item" ><i class="fa fa-home fa-lg"></i>  {{ status }}</li>
                        </ul>
                    	<div class="row">
				<div class="col-lg-12" style="margin-top:12px;text-align:center;">
                        	<input type="button" value="停止" class="btn btn-default" id="stops">
                        	<input type="button" value="启动" class="btn btn-primary" id="starts">
                        	</div>
			</div>
                        </form>
                    </div>
                </div>
            	</div>
            </div-->
        </div>
    </div>
    <div class="col-lg-10">
<div class="row">
<form role="form" method="post" action="{{ url_for('facemange.delete') }}">
    <input name="_csrf_token" type="hidden" value="{{ csrf_token() }}">
    <input name="dir" type="hidden" value="{{ dir }}">
    <input name="dsource" type="hidden" value="" id = "dsource">
    <ul class="nav nav-tabs"><li><a href="#" data-toggle="modal" data-target="#foldername"><i class="fa fa-folder fa-2x"></i>   新建用户</a></li></ul>
    <div class="row">
	<div class="col-lg-6">
            <table class="table table-bordered">
            <tr><td colspan="3">
                <div class="row" style="width:60%">
                    <div class="col-md-6">
                        <div class="checkbox"><label><input type="checkbox" id="allbox">{% if dirname %}<a href="{{ url_for('facemange.list') }}?did={{ dir }}">返回上一级</a>|<a href="{{ url_for('facemange.list') }}"> 全部文件</a>  >  {{ dir }} {% else %}<a href="{{ url_for('facemange.list') }}"> 全部文件</a>{% endif %}</label></div>
                    </div>
                    <div class="col-md-3"><button  class="btn btn-default" id="removes"><i class="fa fa-remove fa-fw"></i>删除</button>
                    </div>
                    <div class="col-md-3"><input type="hidden" id="moves" data-toggle="modal" data-target="#moveto">
                        <a href="javascript:void(0);" type="button"  class="btn btn-default" onclick="selectcheck()"><i class="icon-signin icon-large"></i>修正为</a>
                    </div>
                </div>
            </td></tr>
            {% for line in dirs %}
            <tr><td><input type="checkbox" name="elements" dirname="{{ line.name }}" isdir=1><a href="{{ url_for('facemange.list') }}?dirname={{ line.name }}"><i class="fa fa-folder fa-lg"></i>  {{ line.name }}</a></td><td>{{ line.ctime }}</td></tr>
            {% endfor %}
            </table>
            <div id="files">
            {% for lines in files|slice(counter) %}
                <div class="row">
                {% for line in lines %}
                <div class="col-xs-1" style="text-align:center;" >
                                <div class="row"><a href="/face/{{ dirpath }}/{{ line.name }}"><img src="/face/{{ dirpath }}/{{ line.name }}" class="img-responsive img-thumbnail" alt="{{ line.name }}"></a></div>
                                <div class="row"><input type="checkbox" name="elements" dirname="{{ line.name }}" isdir=0></div></div>
                {% endfor %}
                </div>
            {% endfor %}
            </div>
	</div>
	<div class="col-lg-6">
                <div id="webcam">
                    <div class="row" style="text-align:left;">
                        <video id="video" width="800" height="600"  ></video>
                        <canvas id="overlay" width="800" height="600" style="display:none;"></canvas>
                        <canvas id="scale" width="1920" height="1080" style="display:none;"></canvas>
                        <canvas id="scales" width="320" height="240" style="display:none;"></canvas>
                    </div>
                </div>
	</div>
    </div>
</form>
</div>
    </div>
</div>

<script type="text/javascript">
$(function(){


navigator.getUserMedia = (navigator.getUserMedia ||  navigator.webkitGetUserMedia || navigator.mozGetUserMedia || navigator.msGetUserMedia);
if (navigator.getUserMedia) {
    var global_c = 0;
    var wz = 0;
    var wh = 0;
    var canvas = document.getElementById("overlay");
    var contexdt = canvas.getContext("2d");
    contexdt.font="40px Arial";
    contexdt.fillStyle="#FFFF00";
    contexdt.lineWidth = 3;
    var video = document.getElementById("video");
    var scale = document.getElementById("scale");
    var sctx = scale.getContext("2d");
    var scales = document.getElementById("scales");
    var sctxs = scales.getContext("2d");

    videoObj = { "video": true };
    errBack = function (error) {
        console.log("Video capture error: ", error.code);
	$("#webcam").html("摄像头检测失败，请检查相关驱动和设置");
    };
    navigator.getUserMedia(videoObj, function (stream) {
        video.src = window.URL.createObjectURL(stream) || stream;
        video.play();
    }, errBack);

    var ws = new WebSocket('wss://' + document.domain + ':' + location.port + '/socket/posshow');
    ws.onopen=function(){
        console.log("testing");
    };
    ws.onmessage = function(event){
        console.log("len is " + global_c);
        data = eval('('+event.data+')');
        contexdt.clearRect(0,0,canvas.width,canvas.height);
        for(var i=0;i<data.pos.length;i++){
            contexdt.strokeStyle = "#00CC00";
            contexdt.strokeRect(data.pos[i][0] * data.wz, data.pos[i][1] * data.wh, data.pos[i][2] * data.wz, data.pos[i][3] * data.wh);
            contexdt.fillText(data.pos[i][4],data.pos[i][0] * data.wz ,data.pos[i][1] * data.wh+data.pos[i][3] * data.wh);
            //contexdt.strokeRect(data.pos[i][0] , data.pos[i][1] , data.pos[i][2] , data.pos[i][3] );
            //contexdt.fillText(data.pos[i][4],data.pos[i][0]  ,data.pos[i][1] * wh+data.pos[i][3] );
        }
    };
    var draw = function(){
        //console.log("globalc is " + global_c)
        var sdata = "";
        try{
            if(global_c % 10 ==0){
	         sctx.drawImage(video,0,0, scale.width, scale.height);
                 sdata = scale.toDataURL("image/jpg");
    		 wz = canvas.width / scale.width;
    		 wh = canvas.height / scale.height;
	    }else{
                 sctxs.drawImage(video,0,0, scales.width, scales.height);
                 sdata = scales.toDataURL("image/jpg");
    		 wz = canvas.width / scales.width;
    		 wh = canvas.height / scales.height;
            }
        }catch(e){
            if (e.name == "NS_ERROR_NOT_AVAILABLE") {
                return 0;
            } else {
                throw e;
            }
        }
        //contexdt.drawImage(video,0,0, canvas.width, canvas.height);
        var slen = sdata.length;
        var j = 0;
        var i = 0;
        var k = parseInt(slen/390000) + 1;
	var fname= (new Date()).valueOf();
        var sth;
        var sender = function(){
            if(j>=k){
                return ;
            }
            var d=sdata.substr(i,390000);
            var datas = JSON.stringify({"image":d,"fname":fname,"ser":j,"len":k,"wz":wz,"wh":wh});
            if(video.src){
                ws.send(datas);
            }
            j++;
            i = i + 390000;
            setTimeout(sender,40);
        };
        global_c++;
        setTimeout(sender,10);
        //var datas = JSON.stringify({"type":"data","image":canvas.toDataURL("image/jpg")});
    };
    var sh;
    $("#cap").click(function(){
        canvas.style.display="block";
        canvas.style.top = "0px";
        canvas.style.zIndex = '100001';
        canvas.style.position = "absolute";
        sh = setInterval(draw,150);
        //draw();
    });

    $("#caps").click(function(){
        clearInterval(sh);
        canvas.style.display="none";
        video.style.display = "block";
    });

}else { 
    $("#webcam").html("浏览器不支持此项操作");
};

});
</script>
{%  include 'facemange/newfolder.html' %}
{%  include 'facemange/moveto.html' %}

{% endblock%}
