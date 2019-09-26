#!/usr/bin/env python
#coding="utf-8"

import re,sys,os
import pymysql

#print (sys.path)


db = pymysql.connect("192.168.6.154","bioinfo", "123456","BioinfoTest",charset='utf8')		# 打开数据库连接

cursor = db.cursor()				# 使用cursor()方法获取操作游标 

##############增,删,改
cursor.execute("select * from lzw_test")
print ("{0}\n".format(cursor.fetchall()))

try:
#	cursor.execute("insert into lzw_test(name,heigth,weigth,interest) values('wxf',175,75,'lalala')")	# 使用execute方法执行SQL语句,虽然增加，但是不会更新到数据库
#	cursor.execute("delete from lzw_test where name='wxf'")	
	cursor.execute("update lzw_test set interest='reading' where name='lzw'")
	db.commit()												# 添加更新到数据库
except:
	db.rollback()												# 如果发生错误则回滚

cursor.execute("select * from lzw_test")
print ("{0}\n".format(cursor.fetchall()))

#############查
try:
	cursor.execute("select * from lzw_test")
	#data = cursor.fetchone()			# 使用 fetchone() 方法获取一条数据
	data = cursor.fetchall()                        # 使用 fetchone() 方法获取所有数据
	print ("{0}\n".format(data))
	print (type(data))
except:
	print ("Error: unable to fetch data")
##########创建数据库表
cursor.execute("create table xxx(key_1 varchar(255),key_2 varchar(255),value varchar(255))")
cursor.execute("drop table xxx")

db.close()					# 关闭数据库连接

