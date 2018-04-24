<!--#include file="conn.asp"-->
<%
	Dim username,password
	Dim conn,rs
	username=Trim(Request("username"))
	password=Trim(Request("password"))
	confirm=Trim(Request("confirm"))
	If username <> "" and password <> "" Then
		If password <> confirm Then
			Response.write "<script LANGUAGE='javascript'>alert('Password or confirm must be the same!');history.go(-1)</script>"
		End if
		strSql="select * from account where username='"&username&"'"
		Set rs=server.CreateObject("adodb.recordset")
		rs.open strSql,conn,1,1
		If rs.eof Or rs.bof Then
			strSql="insert Into account(username,password) Values ('"&username&"','"&password&"')"
			'Set rs=server.CreateObject("adodb.recordset")
			'rs.open strSql,conn,1,1	
			Set rs=conn.execute(strSql)
			Response.write "<script LANGUAGE='javascript'>alert('Register success.Please login!');history.go(-2)</script>"
			Response.End
		Else
			Response.write "<script LANGUAGE='javascript'>alert('Username exist!');history.go(-1)</script>"
			Response.End		
		End If
		Response.End
		rs.close
	Else
		Response.write "<script LANGUAGE='javascript'>alert('Please type username and password then try again');history.go(-1)</script>"
	End If
%>