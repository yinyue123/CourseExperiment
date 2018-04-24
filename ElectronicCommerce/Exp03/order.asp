<!--#include file="conn.asp"-->
<html>
<head>
<title>殷悦的商城</title>
</head>
<body>
	<p align="right">
	用户:<%Response.write Session("username")%>
	<a href="login">登录</a>
	<a href="?submit=logout">注销</a>
	<a href="">管理</a>
	</p>
	<%
		If Session("username")="" Then
			Response.write "<script LANGUAGE='javascript'>alert('未登录，请先登录!')</script>"
		End If
		Dim submit
			submit=Trim(Request("submit"))
		If submit="logout" Then
			Session("username")=""
			Response.write "<script LANGUAGE='javascript'>alert('已经退出登录!');window.location.href='index.asp'</script>"
		End If
	%>
	<p align="center">
	<a href="index.asp">选择商品</a> ->
	<a href="shoppingCart.asp">购物车</a> ->
	<a href="order.asp">管理订单</a>
	</p>

	<p align="center"><font fane="隶书" size="5">管理订单</font></p>
	<hr>
<%
		If Trim(Request("del"))<>"" Then
			strSql="select ONum from orders where username='"&Session("username")&"' and cfm=1 and ONum='"&Request("del")&"'"
			Set rs=conn.execute(strSql)
			strSql="delete from orders where ONum='"&rs("ONum")&"'"
			'Response.write strSql
			Set rs2=conn.execute(strSql)
			strSql="delete from buy where ONum='"&rs("ONum")&"'"
			Set rs2=conn.execute(strSql)
			'Response.write strSql
			'Response.End
			rs.close
		End If
%>
	<p align="center">
	<%
		strSql="select ONum,OTime,consignee,location,zipcode,tel from orders where username='"&Session("username")&"' and cfm=1 order by ONum desc"
		Set rs=conn.execute(strSql)
		If rs.eof Or rs.bof Then
			Response.write "暂无提交的订单"
			Response.End
		Else
			Do While Not rs.eof
			
	%>
	</p>
		<p align="center">
		订单信息
		<table border="2" width="80%">
			<tr>
				<th width="40%">用户</th>
				<th width="60%"><%Response.write Session("username")%></th>
			</tr>
			<tr>
				<th width="40%">订单号</th>
				<th width="60%"><%=rs("ONum")%></th>
			</tr>
			<tr>
				<th width="40%">订单时间</th>
				<th width="60%"><%=rs("OTime")%></th>
			</tr>
				<th width="40%">总价</th>
				<th width="60%">
				<%
					strSql="select sum(BCount*GPrice) as total from buy left join goods on buy.BName=goods.GName where ONum='"&rs("ONum")&"'"
					Set rs2=conn.execute(strSql)
					Response.write rs2("total")
				%></th>
		</table>

		商品信息
		<table border="3" width="80%">
			<tr>
				<th width="40%">商品名</th>
				<th width="30%">数量</th>
				<th width="30%">价格</th>
			</tr>
<%
		strSql="select BName,BCount,GPrice from buy left join goods on buy.BName=goods.GName where ONum='"&rs("ONum")&"'"
		Set rs2=conn.execute(strSql)
		If Not rs2.eof And Not rs2.bof Then
			Do While Not rs2.eof
%>
		<tr>
			<th><%=rs2("BName")%></th>
			<th><%=rs2("BCount")%></th>
			<th><%=rs2("GPrice")%></th>
		</tr>
<%
			rs2.movenext
			Loop
		End If
%>
		</table>
		物流信息
		<table border="2" width="80%">
			<tr>
				<th width="40%">收货人</th>
				<th width="60%"><%=rs("consignee")%></th>
			</tr>
			<tr>
				<th width="40%">收货地址</th>
				<th width="60%"><%=rs("location")%></th>
			</tr>
			<tr>
				<th width="40%">邮编</th>
				<th width="60%"><%=rs("zipcode")%></th>
			</tr>
			<tr>
				<th width="40%">电话</th>
				<th width="60%"><%=rs("tel")%></th>
			</tr>
		</table>
		
		<br>
		<input type="button" onclick="javascript:top.location='?del=<%=rs("ONum")%>';" value="删除订单">
	<%
		rs.movenext
		rs2.close
	%>
		<hr>
	<%
	Loop
	End If
	rs.close
	%>
	</p>

</body>
</html>