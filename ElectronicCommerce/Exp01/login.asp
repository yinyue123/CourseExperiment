<!--#include file="conn.asp"-->
<%
	Dim username,password
	Dim conn,rs
	username=Trim(Request("username"))
	password=Trim(Request("password"))
	If username <> "" and password <>"" Then
		
		strSql="select * from account where username='"&username&"' and password='"&password&"'"
		Set rs=server.CreateObject("adodb.recordset")
		rs.open strSql,conn,1,1
		If rs.eof Or rs.bof Then
			Response.write "<script LANGUAGE='javascript'>alert('Username or password wrong!');history.go(-1)</script>"
			Response.end
		End If
		rs.close
		Session("username")=username
		Response.Redirect("goods.asp")
		Response.End
	Else
		Response.write "<script LANGUAGE='javascript'>alert('Please type username and password then try again');history.go(-1)</script>"
	End If
%>