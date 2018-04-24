
--4.
select * from Student;

--5.
select sno,sname from Student where sclass=1;

--6. 
select 2017-sage Bir from Student where sname='Αυ³Ώ';

--7. 

select Student.sclass,Student.sno,sname,Student.ssex,sage,Sdept,grade,cname,spno,ccredit from Student left join SC on (Student.sno=SC.sno and Student.sclass=SC.sclass) left join Course on(SC.cno=Course.cno) where sname like 'Αυ%';

--8.
select sname,ssex,grade from Student left join SC on (Student.sclass=SC.sclass and Student.sno=SC.sno) where cno=1;

--9.
select cno,cname from Course where spno is null;

--10. 
select * from Student where sclass=2 and ssex='Ε®';

--11. 
select cno,cname from Course where ccredit<=3 and ccredit>=2;

--12.

select Student.sclass,Student.sno,sname,cname,grade from Student left join SC on (Student.sno=SC.sno and Student.sclass=SC.sclass) left join Course  on(SC.cno=Course.cno) where SC.cno=1 or SC.cno=2;

--13.
select 
Student.sclass,Student.sno,sname,
Student.ssex,Sdept,SC.cno,grade 
from Student left join SC 
on (Student.sno=SC.sno and Student.sclass=SC.sclass) 
left join Course on(SC.cno=Course.cno) 
where exists 
(select * from SC where cno in(
select cno from Course where spno=1));

--14. 
select sclass,sno from SC where cno=2 and 
grade=(select max(grade) from SC where cno=2);

--15.
select sclass,sno from SC where sclass=1 and cno=2 and grade=(select min(grade) from SC where sclass=1 and cno=2);  

--16.
select sclass,sno,grade from SC where cno=2 and grade!=(select min(grade) from SC where cno=2);