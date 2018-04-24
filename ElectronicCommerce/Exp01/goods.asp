<!--#include file="conn.asp"-->
<html>
<head>
	<title>Goods information</title>
</head>

<body>
	<p>Username:<%Response.write Session("username")%>
	<input type="button" onclick="javascript:history.go(-1);" value="Logout"></p>
	<h1 align="center">Goods information</h1>
	<hr>
	<h3>Add goods</h3>
	<form method="GET" action="opr.asp">
		<table border="2" width="80%">
			<tr>
				<th>Goods name:</th>
				<th><input type="text" name="GName" size=20></th>
			</tr>
			<tr>
				<th>Price:</th>
				<th><input type="text" name="GPrice" size=20></th>
			</tr>
			<tr>
				<th>Count:</th>
				<th><input type="text" name="GCount" size=20></th>
			</tr>
			<tr>
				<th>&nbsp;</th>
				<th>
					<input type="submit" value="Add" name="submit">
					<input type="reset" value="Reset" name="reset">
				</th>
			<tr>
		</table>
	</form>
	<h3>Goods list</h3>
	<table border="4" width="80%">
		<tr>
			<th>Name</th>
			<th>Price</th>
			<th>Count</th>
			<th>Operate</th>
		</tr>
<%
		strSql="select GName,GPrice,GCount from goods"
		Set rs=server.CreateObject("adodb.recordset")
		rs.open strSql,conn,1,1
		If Not rs.eof And Not rs.bof Then
			Do While Not rs.eof
%>
		<tr>
			<th><%=rs("GName")%></th>
			<th><%=rs("GPrice")%></th>
			<th><%=rs("GCount")%></th>
			<th>
				<a href="opr.asp?submit=Buy&GName=<%=rs("GName")%>">Buy</a>
				<a href="opr.asp?submit=Delete&GName=<%=rs("GName")%>">Delete</a>
			</th>
		</tr>
<%
			rs.movenext
			Loop
		End If
		rs.close
%>
	</table>
</body>
</html>