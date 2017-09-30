var index = 0;
var runAfterInstalled=1;

var setup = {
    init: function () {
        this.initPage();//初始化安装界面
        this.initInstallPath();//初始化安装路径
        this.bindEvent();//绑定事件
    },
    initPage: function () {
        external.SetSize(520, 420);
    	external.SetTitleRect(0,0,480,50);
    	var width = (screen.width-520)/2;
    	var height = (screen.height-420)/2

    	setTimeout(function(){

			var bRet = external.IsOSVersionGreaterXPSP3();
			if(bRet == false)
			{
				handleDialog('cannotInstall', '您当前系统版本太低，不能安装，请先升级系统后再安装！');
			}

			//检测当前安装状态：0 -- 未安装任何蜜蜂； 1-- 已安装蜜蜂2.0； 2---已经安装当前版本; 3--已经安装同一个包; 4 -- 已安装更新版本
    		var ret = external.CheckInstalled();
    		if(ret == 1 )
    		{
				//覆盖老版本
                handleDialog('uninstallold', '您需要先卸载之前的版本，然后重新运行安装包安装。 是否立即卸载旧版本？');
    		}
    		else if(ret == 2)
    		{
				//覆盖新版本
				//之前已经弹过系统窗了，在此不再弹出覆盖安装提示
				RecoverInstall();
    		}
			else if(ret == 3)
			{
				 handleDialog('cover', '您已安装该产品，是否覆盖安装？');
			}
			else if(ret == 4)
			{
				handleDialog('cannotInstall', '您已安装更高版本，不能覆盖安装，请先卸载当前版本后再安装！');
			}
			else
			{
				external.SetSize(520, 420);
    			change_page();
			}

        },500);
        external.Show(520, 420);
    },
    initInstallPath: function () {
        //选择安装路径
		var input_information = external.GetInstallDir();//默认路径信息
        //external.MainWnd.setSpaceCallBackHandler("GetSpace");
        $('#installPath').val(input_information);
    },
    bindEvent: function () {
        //关闭
        $('i.close').bind('click', function () {
			if($(this).hasClass('disabled'))
			{
				return;
			}
            external.Close()
        });

        //点击自定义选项
        $('#btnShowOptions span').toggle(function () {
            external.SetSize(520,565);
            $('.section').removeClass('close').addClass('open');
            $('.option-wrap').slideDown(50);
        }, function () {
            $('.section').addClass('close');
            $('.option-wrap').slideUp(50);
            external.SetSize(520,420);
        });

        //点击复选框
        $('.checkbox[id!="btnAgreeService"]').toggle(function () {
            $(this).removeClass('checked');
            $(this).children('input').val('0');
        }, function () {
            $(this).addClass('checked');
            $(this).children('input').val('1');
        });

        //点击同意服务条款
        $('#btnAgreeService').bind('click', function () {
            if ($(this).hasClass('checked')) {
                $(this).removeClass('checked');
                $('#btnStartSetup').addClass('disabled');
            } else {
                $(this).addClass('checked');
                $('#btnStartSetup').removeClass('disabled');
            }
        });

        //点击开始安装
        $('#btnStartSetup').unbind().bind('click', function () {
            if ($(this).hasClass('disabled')) {
                return;
            }

            install($(this));//开始安装
        });

        //取消安装
        $('.btn-setup-cancel').bind('click', function () {
            external.Close();
        });

        //无法安装时点击完成
        $('.btn-setup-ok').bind('click', function () {
            $('.dialogMain').hide();
            $('.dialog').hide();
        });

		//无法安装时点击完成
        $('.btn-setup-ok-close').bind('click', function () {
            external.Close();
        });

		//确认卸载老版本
        $('.btn-setup-ok-unistallold').bind('click', function () {
            external.RecoverInstall();
        });

        //覆盖直接安装
        $('.btn-setup-cover').bind('click', function () {
            if (external.CheckAppRunning())
            {
               handleDialog('force', '应用程序仍在运行，请确认是否强制关闭并覆盖安装？');
            } else {
                installForce();
            }
        });

        //强制覆盖
        $('.btn-setup-force').bind('click', function () {
            installForce();
        });

        //继续安装
        $('.btn-setup-resetup').bind('click', function () {
            $('.step2').hide();
            $('.success').show();
        });

        //完成安装
        $('.btn-setup-success').bind('click', function () {
            external.Close(1);//1：拉起程序  0、空  关闭程序
        });

        //安装失败
        $('.btn-setup-error').bind('click', function () {
           external.Close();
        });

        //点击选择安装路径
        $('#btnChoosePath').unbind().bind('click', function () {
            if ($(this).hasClass('disabled')) {
                return;
            }
			var rel = external.GetInstallDir();
			$('#installPath').val(rel);
            var rel=external.SelectFolderDialog("请选择安装路径");
            if (rel=='') {
               handleDialog('noRight', '你没有权限把“蜜蜂”安装到该路径');
               return false;
            } else {
                $('#installPath').val(rel);
                //external.MainWnd.SetSpaceCallBackHandler("GetSpace");
            }
        });

        $("body").keydown(function(e) {
            if (e.ctrlKey == 1) {
                if (e.keyCode==65)
                {
                    return false;
                }
            }
        })
    }
};

//安装空间判断，暂留
function GetSpace(free, need) {
    //$('.needspace').text("所需空间：" + need + "M");
    if(free > 1000) {
        var value = free/1000;
        value = value.toFixed(1);
        //$('.remainspace').text("可用空间：" + value + "G");
    } else {
        //$('.remainspace').text("可用空间：" + value + "M");
    }

    if (free - need < 100) {
        handleDialog('nospace', '您的磁盘空间不足，请清理磁盘或者重新选择安装路径。');
    }
}

//安装状态检查//bValue, nValue
function statusBar(bValue, nValue) {
    //$('.bar').css('backgroundImage', 'linear-gradient(to right, #FFCC00 0%, #FFCC00 '+ nValue +', #ccc '+ nValue +', #ccc 100%)');
    $('.num').css('width', nValue + '%');
    $('.des>.right').text(nValue + '%');
    //$(".load_welcome").text(""+nValue+"%").css("margin-left", ""+nValue-2+"%");
        //蜜蜂已经装完，开始装云会议。
	if(bValue == 0){
		//安装蜜蜂失败了
		//external.MainWnd.setSize(520,420);
		$(".step2").hide();
		$('.error1').show();
		$('.dialogMain').hide();
		$('.close').removeClass('disabled');
	}

    if (nValue == 100) {
        if ( bValue ) {//安装完成
            $('.step2').hide();
            $('.success').show();
        } else {
            $('.step2').hide();
            $('.error2').show();
        }

        clearTimeout(load_picture);
        $('.close').removeClass('disabled');
        $('.dialogMain').hide();
    }
}

//开始安装
function install (t) {
    var installUrl = $.trim($('#installPath').val());
    var _bDeskLink = $('input[name="createShortcut"]').val();//生成桌面快捷方式
    var _bLauncher = $('input[name="inLauncher"]').val();//添加到快捷启动栏
    var _bAutoRun = $('input[name="autoRun"]').val();//开机自动启动

    var installStatus = external.Install(installUrl, _bDeskLink, _bLauncher, _bAutoRun);
    if (!installStatus) {
        handleDialog('urlError', '抱歉，您不能安装在该路径，请重新选择安装路径！');
        return;
    }
    $('.section').removeClass('open');
    $('.step1').hide();
    external.SetSize(520, 420);
    $('i.close').addClass('disabled');
    $('.step2').show();
    $('.dialogMain').css({'z-index': 3, 'width': '460px'}).show();
    scrollBanner();
	external.SetCallBackHandler("statusBar");
}


//直接跳到安装进度页面
function RecoverInstall() {

    $('.section').removeClass('open');
	$('.step0').hide();
    $('.step1').hide();
    external.SetSize(520, 420);
    $('i.close').addClass('disabled');
    $('.step2').show();
    $('.dialogMain').css({'z-index': 3, 'width': '460px'}).show();
    scrollBanner();
	external.SetCallBackHandler("statusBar");
	external.RecoverInstall();
}

//强制安装
function installForce() {
    external.SetSize(1, 1);
    $('.dialogMain').css({'z-index': 3, 'width': '460px'}).show();
    $('.dialog').hide();
    $('.step0').hide();
    $('i.close').addClass('disabled');
    $('.step1').hide();
    $('.section').fadeIn(300, function () {
        $('.step2').show();
        var width = (screen.width-520)/2;
        var height = (screen.height-420)/2
        external.Show(520, 420);
        external.SetTitleRect(0,0,480,50);
    });

	scrollBanner();
    external.SetCallBackHandler("statusBar");
	external.RecoverInstall();

}

//安装过程
var index = 1, load_picture = '', isFirst = true;
function scrollBanner() {
    index++;
    if (index > 5) {
        index = 1;
    }

    if (isFirst) {
        isFirst = false;
    } else {
        $('#productImg').fadeOut(function () {
            $(this).attr('src', index + '.png');
            $(this).fadeIn();
        });
    }

	load_picture = setTimeout(function(){scrollBanner()}, 5000);
}

//点击提示框做对应操作
function handleDialog(type, msg) {
    var _content = $('.dialog').find('.content');
    var _options = $('.dialog').find('.btn-option');
    _content.html(msg);
    switch (type) {
		case 'uninstallold':
            _options.find('a').hide();
            _options.find('a:eq(5), a:eq(0)').show();
            break;
        case 'cover':
            _options.find('a').hide();
            _options.find('a:eq(5), a:eq(3)').show();
            break;
        case 'cannotInstall':
            _options.find('a').hide();
            _options.find('a:eq(1)').show();
            break;
        case 'force':
            _options.find('a').hide();
            _options.find('a:eq(5), a:eq(4)').show();
            break;
        case 'nospace':
            _options.find('a').hide();
            _options.find('a:eq(2)').show();
            break;
        case 'noRight':
            _options.find('a').hide();
            _options.find('a:eq(2)').show();
            break;
        case 'urlError':
            _options.find('a').hide();
            _options.find('a:eq(2)').show();
            break;
        default:
            break;

    }

    $('.dialogMain').show();
    $('.dialog').show();
    return;

    $('.section').fadeOut(200, function () {
        external.Show(360,170);
        var width = (screen.width-360)/2;
        var height = (screen.height-170)/2
    	external.SetTitleRect(0,0,360,30);
        $('.dialog').show();
    });
}

function change_page()
{
   $('.step0').hide();
   $('.step1').show();
}

//page init
$(document).ready(function () {
    setup.init();//启动页面
});
