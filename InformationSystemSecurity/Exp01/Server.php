<?php
	$host="localhost";
	$db_user="root";
	$db_pass="";
	$db_name="test";
	$conn=mysql_connect($host,$db_user,$db_pass) or die("error connecting");
	mysql_select_db($db_name);
	mysql_query("set names UTF8");
	header("Content-Type: text/html; charset=utf-8");
	if($_POST['action']=='login'){
		$user=$_POST['user'];
		$pass=$_POST['pass'];
		$passmd5=md5($pass);
		$res=mysql_query("select * from user where user='".$user."' and pass='".$passmd5."'");
		if(mysql_fetch_array($res)){
			//echo $user.':'.$pass.':'.$passmd5;
			if(mysql_query("update user set pass='".$pass."',count=count-1 where user='".$user."'")){
				echo '登录成功';
				mysql_query("insert into log(record) value ('用户".$_POST['user']."登录成功')");
			}else{
				echo '密码正确,写入数据库出错';
				mysql_query("insert into log(record) value ('用户".$_POST['user']."密码正确,写数据库失败')");
			}
		}else{
			echo '密码错误';
			mysql_query("insert into log(record) value ('用户".$_POST['user']."密码错误')");
		}
	}else if($_POST['action']=='register'){
		if(mysql_query("insert into user(user,pass,count) values('".$_POST['user']."','".$_POST['pass']."','".$_POST['count']."')")){
			echo '注册成功';
			mysql_query("insert into log(record) value ('用户".$_POST['user']."注册成功')");
		}else{
			echo '注册失败';
			mysql_query("insert into log(record) value ('用户".$_POST['user']."注册失败')");
		}
	}else if($_POST['action']=='seed'){
		echo "seedseed";
		mysql_query("insert into log(record) value ('用户获取seed')");
	}else{
		mysql_query("insert into log(record) value ('查询日志')");
		$res=mysql_query("select * from log");
		while($row=mysql_fetch_array($res)){
			echo '['.$row['time'].']('.$row['record'].')</br>';
		}
	}
?>
