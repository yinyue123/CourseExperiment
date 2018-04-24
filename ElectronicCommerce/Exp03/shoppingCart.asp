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

	<p align="center"><font fane="隶书" size="5">购物车</font></p>
	<hr>
<%
		If Trim(Request("submit"))<>"" Then
			strSql="select ONum from orders where username='"&Session("username")&"' and cfm=0"
			Set rs=conn.execute(strSql)
			strSql="update orders set consignee='"&Request("consignee")&"',location='"&Request("location")&"',zipcode='"&Request("zipcode")&"',tel='"&Request("tel")&"' where ONum='"&rs("ONum")&"'"
			Set rs2=conn.execute(strSql)
			strSql="select BName,GCount from buy left join goods on buy.BName=goods.GName where ONum='"&rs("ONum")&"'"
			Set rs3=conn.execute(strSql)
			If Not rs3.eof And Not rs3.bof Then
				Do While Not rs3.eof
					'Response.write Request(rs3("BName"))&rs3("GCount")
					If CInt(Request(rs3("BName")))>=CInt(rs3("GCount")) Then
						Response.write "<script LANGUAGE='javascript'>alert('库存不足，请重新选择货物数量');history.go(-1)</script>"
						Response.End
					End If
					rs3.movenext
				Loop
			End If
			rs3.close
			strSql="select BName from buy where ONum='"&rs("ONum")&"'"
			Set rs2=conn.execute(strSql)
			If Not rs2.eof And Not rs2.bof Then
				Do While Not rs2.eof
					strSql="update buy set BCount='"&Request(rs2("BName"))&"' where ONum='"&rs("ONum")&"' and BName='"&rs2("BName")&"'"
					conn.execute(strSql)
					strSql="update goods set GCount=GCount-'"&Request(rs2("BName"))&"' where  GName='"&rs2("BName")&"'"
					'Response.write strSql
					'Response.End
					conn.execute(strSql)
					rs2.movenext
				Loop
			End If
			strSql="update orders set cfm=1 where ONum='"&rs("ONum")&"'"
			Set rs2=conn.execute(strSql)
			rs.close
			Response.write "<script LANGUAGE='javascript'>alert('已生成订单!')</script>"
		End If
%>
	<form>
	<p align="center">
		订单信息
		<table border="2" width="80%">
			<tr>
				<th width="40%">用户</th>
				<th width="60%"><%Response.write Session("username")%></th>
			</tr>
			<tr>
				<th width="40%">订单号</th>
				<th width="60%">
				<%
					strSql="select ONum,OTime from orders where username='"&Session("username")&"' and cfm=0"
					Set rs=conn.execute(strSql)
					If rs.eof Or rs.bof Then
						Response.write "购物车为空,请先选择商品"
						Response.end
					Else
						Response.write rs("ONum")
					End If
				%>
				</th>
			</tr>
			<tr>
				<th width="40%">订单时间</th>
				<th width="60%"><%=rs("OTime")%></th>
			</tr>
		</table>
		商品信息
		<table border="3" width="80%">
			<tr>
				<th width="40%">商品名</th>
				<th width="30%">数量</th>
				<th width="30%">价格</th>
			</tr>
<%
		strSql="select BName,GPrice from buy left join goods on buy.BName=goods.GName where ONum='"&rs("ONum")&"'"
		Set rs2=conn.execute(strSql)
		If Not rs2.eof And Not rs2.bof Then
			Do While Not rs2.eof
%>
		<tr>
			<th><%=rs2("BName")%></th>
			<th><input type="text" name=<%=rs2("BName")%> size=10></input></th>
			<th><%=rs2("GPrice")%></th>
		</tr>
<%
			rs2.movenext
			Loop
		End If
		rs.close
		rs2.close
%>
		</table>
		物流信息
		<table border="2" width="80%">
			<tr>
				<th width="40%">收货人</th>
				<th width="60%"><input type="text" name="consignee" size=20></th>
			</tr>
			<tr>
				<th width="40%">收货地址</th>
				<th width="60%"><input type="text" name="location" size=20></th>
			</tr>
			<tr>
				<th width="40%">邮编</th>
				<th width="60%"><input type="text" name="zipcode" size=20></th>
			</tr>
			<tr>
				<th width="40%">电话</th>
				<th width="60%"><input type="text" name="tel" size=20></th>
			</tr>
		</table>
		
		<br>
		<input type="submit" value="生成订单" name="submit">
		<input type="button" onclick="javascript:top.location='order.asp';" value="查看清单">
	</p>
	</form>
	

</body>
</html>