<%
	Set conn=Server.createobject("adodb.connection")
	'conn.Open "test"
	StrConn="driver={SQL Server};server=qds167524598.my3w.com;uid=qds167524598;pwd=yinyue123;database=qds167524598_db"
	Conn.open StrConn


	If conn.State <> 1 Then
		Response.write "Faild to connect Sql!"
		Response.End
	End If
%>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />