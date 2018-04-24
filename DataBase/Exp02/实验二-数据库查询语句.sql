--第一大题

--1.
select A.sclass,A.sno from SC A inner join SC B on (A.sclass=B.sclass and A.sno=B.sno) where A.cno=1 and B.cno=2;

--2. 
select sclass,sno from SC A where cno=1 and not exists  (select * from SC where A.sclass=sclass and A.sno=sno and cno=2);

--3.
select SC.sclass,SC.sno,sname,ssex,sdept,cno,grade from SC right join (select Student.sclass,Student.sno,sname,ssex,sdept from Student right join (select sclass,sno from SC group by sclass,sno having avg(grade)>85) as G on (Student.sclass=G.sclass and Student.sno=G.sno)) as E on (SC.sclass=E.sclass and SC.sno=E.sno);

--4. 
--use hello;
select sclass,sno,sname from student where not exists(
	select * from sc sc1 where sc1.sclass='1' and sc1.sno='2' and not exists(
		select * from sc sc2 where sc2.sno=student.sno and sc1.cno=sc2.cno
))

--5. 
select sname,grade from SC left join Student on 
(SC.sclass=Student.sclass and SC.sno=Student.sno)  where cno=2 order by grade desc;

--6. 
select sclass,sno,sum(grade) sum from SC group by sclass,sno;

--7. 
select sclass,avg(grade) arg from SC where sclass=1 and cno=1 group by sclass;

--8. 
insert into
Student(sclass,sno,sname,ssex,sage,Sdept)
values(1,4,'老王','男',30,'SI');
insert into 
Course(cno,cname,spno,ccredit)
values(8,'无机化学','2',4);
insert into 
SC(sclass,sno,cno,grade)
values(1,4,8,100);

--9. 
delete from SC where cno=3;
select * from SC;

--10. 
update Student 
set sage=sage+1 
where ssex='男' and exists(
select * from SC where Student.sclass=SC.sclass and 
Student.sno=SC.sno and cno=1);
select * from Student;

--第二大题

--1
--create view  V_ISStudent (sclass,sno,sname,ssex,sage,Sdept) as 
select sclass,sno,sname,ssex,sage,Sdept from Student where Sdept='IS'; 

create view S_C_GRADE(sno,sclass,sname,cname,grade)
as 
select Student.sno,Student.sclass,Student.sname,cname,grade 
from Student 
left join SC on Student.sno=SC.sno and Student.sclass=SC.sclass
left join Course on SC.cno=Course.cno; 
 
--2
SELECT     dbo.Student.sclass, dbo.Student.sno, dbo.Student.sname, dbo.Student.ssex, dbo.Student.sage, dbo.Student.Sdept, dbo.Course.cname, dbo.Course.ccredit, dbo.SC.grade
FROM         dbo.Student 
LEFT OUTER JOIN dbo.SC ON dbo.Student.sclass = dbo.SC.sclass AND dbo.Student.sno = dbo.SC.sno 
LEFT OUTER JOIN dbo.Course ON dbo.SC.cno = dbo.Course.cno
WHERE     (dbo.Student.Sdept = 'IS') AND (dbo.SC.grade > 90)

--3
create view V_NUM_AVG(Sdept,sum_num,avr_age)
as 
select Sdept,count(sno),avg(sage) 
from Student 
group by Sdept;

--4
select Student.sclass,Student.sno,sname,grade from SC 
left join Student on SC.sno=Student.sno and SC.sclass=Student.sclass
left join Course on SC.cno=Course.cno 
where SC.cno=1 and Sdept='IS';

--5
select * from V_ISStudent where sage<20;

--6
update V_ISStudent set sname='刘辰' where sclass=1 and sno=2;

