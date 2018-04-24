# -*- coding: utf-8 -*
import os
import socket
import hashlib
import tkinter.messagebox
from tkinter import *
from urllib import request,parse,error

class Login(Frame):
	url='http://115.159.155.95/skey.php'
	N=0;
	def __init__(self, master):
		# abc
		frame=Frame(master)
		frame.pack()
		self.lab1=Label(frame,text = "用户:")
		self.lab1.grid(row=0,column=0,sticky=W)
		self.lab2=Label(frame,text="密码:")
		self.lab2.grid(row=1,column=0,sticky=W)
		self.lab2=Label(frame,text="验证码:")
		self.lab2.grid(row=2,column=0,sticky=W)
		self.ent1=Entry(frame)
		self.ent1.grid(row=0,column=1,sticky=W)
		self.ent2=Entry(frame,show="*")
		self.ent2.grid(row=1,column=1,sticky=W)
		self.ent3=Entry(frame)
		self.ent3.grid(row=2,column=1,sticky=W)
		self.button1=Button(frame,text="登录",command=self.Log)
		self.button1.grid(row=3,column=0,sticky=W)
		self.button2=Button(frame,text="注册",command=self.Reg)
		self.button2.grid(row=3,column=1,sticky=W)
		self.button3=Button(frame,text="退出",command=frame.quit)
		self.button3.grid(row=3,column=1,sticky=E)
	def md5n(self,pas,n):
		while n>0:
			n=n-1
			hash=hashlib.md5()
			hash.update(pas.encode("utf8"))
			pas=hash.hexdigest()
			print(pas)
		return pas

	def Log(self):
		s1=self.ent1.get()
		s2=self.ent2.get()
		s3=self.ent3.get()
		info=[]
		info.append(('action','login'))
		info.append(('user',s1))
		tkinter.messagebox.showinfo('提示', "消息:接受种子成功")
		seed='seedseed'
		with open(s1 + '.txt', 'r') as f:
			N=eval(f.read())
			f.close()
			md5npas = self.md5n(s2, N - 1)
			self.ent3.delete(0, len(s3))
			self.ent3.insert(0, md5npas)
			info.append(('pass', md5npas))
			print(info)
			data = bytes(parse.urlencode(info), encoding='utf8')
			print(data)
		with request.urlopen(self.url,data=data) as rsp:
			msg = rsp.read().decode('utf8')
			if msg.find('成功')!=-1:
				with open(s1 + '.txt', 'w') as f:
					f.write(str(N-1))
					f.close()
			tkinter.messagebox.showinfo('提示', "消息:\n%s" % msg)

	def Reg(self):
		s1 = self.ent1.get()
		s2 = self.ent2.get()
		s3 = self.ent3.get()
		N=100
		info = []
		info.append(('action', 'register'))
		info.append(('user', s1))
		md5npas = self.md5n(s2, N)
		self.ent3.delete(0, len(s3))
		self.ent3.insert(0, md5npas)
		info.append(('pass', md5npas))
		info.append(('count',N))
		print(info)
		data = bytes(parse.urlencode(info), encoding='utf8')
		print(data)
		with request.urlopen(self.url, data=data) as rsp:
			msg = rsp.read().decode('utf8')
			print(rsp)
			print(msg)
			if msg.find('成功')!=-1:
				with open(s1 + '.txt', 'w') as f:
					f.write(str(N))
					f.close()

			tkinter.messagebox.showinfo('提示', "消息:\n%s" % msg)
		s1=self.ent1.get()
		s2=self.ent2.get()

root=Tk()
root.title("登录")
app=Login(root)
root.mainloop()
