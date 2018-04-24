<!--#include file="conn.asp"-->
<%
	Dim Name,GPrice,GCount,submit
	Set conn=Server.createobject("adodb.connection")
	submit=Trim(Request("submit"))
	If submit="Add" Then
		GName=Trim(Request("GName"))
		GPrice=Trim(Request("GPrice"))
		GCount=Trim(Request("GCount"))
		Response.write GName&GPrice&GCount
		If GName <> "" and GPrice <> "" and GCount <> "" Then
			strSql="select * from goods where GName='"&GName&"'"
			Set rs=server.CreateObject("adodb.recordset")
			rs.open strSql,conn,1,1
			If rs.eof Or rs.bof Then
				strSql="insert Into goods(GName,GPrice,GCount) Value ('"&GName&"','"&GPrice&"','"&GCount&"')"
				Set rs=server.CreateObject("adodb.recordset")
				rs.open strSql,conn,1,1	
				Response.write "<script LANGUAGE='javascript'>alert('Add successfully!');history.go(-1);window.location.reload()</script>"
				Response.End
			Else
				Response.write "<script LANGUAGE='javascript'>alert('Goods name exists!');history.go(-1)</script>"
				Response.End		
			End If
			Response.End
		Else
			Response.write "<script LANGUAGE='javascript'>alert('Please type Goods name,Price and Count then try again');history.go(-1)</script>"
		End If
	ElseIf submit="Buy" Then
		GName=Trim(Request("GName"))
		strSql="update goods set GCount=GCount-1 where GName='"&GName&"'"
		Set rs=server.CreateObject("adodb.recordset")
		rs.open strSql,conn,1,1
		Response.write "<script LANGUAGE='javascript'>alert('Buy successfully!');history.go(-1);window.location.reload()</script>"
	ElseIf submit="Delete" Then
		GName=Trim(Request("GName"))
		strSql="delete from goods where GName='"&GName&"'"
		Set rs=server.CreateObject("adodb.recordset")
		rs.open strSql,conn,1,1	
		Response.write "<script LANGUAGE='javascript'>alert('Delete  successfully!');history.go(-1);window.location.reload()</script>"
	End If
%>