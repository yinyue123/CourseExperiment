<%
	Response.write "hello"
	set Conn=server.CreateObject("ADODB.CONNECTION")
	StrConn="driver={SQL Server};server=qds167524598.my3w.com;uid=qds167524598;pwd=yinyue123;database=qds167524598_db"
	Conn.open StrConn

%>