#!/usr/bin/env python
import sys,os,re
import mysql.connector

db=mysql.connector.connect(host="192.168.6.154",user="bioinfo",passwd="123456")

cursor=db.cursor()

cursor.execute("show databases;")

print ("{0}\n".format(cursor.fetchall()))
#sys.exit(0)


cursor.execute("CREATE DATABASE xxxx")

sys.exit(0)

#cursor.execute("use xxxx;")

#cursor.execute("create table yyyy(primary_key int auto_increment primary key, key1 varchar(255), key2 varchar(255))")

#cursor.execute("insert into yyyy(key1,key2) values('3','7')")

#cursor.ececute("select * from yyyy")

#data=cursor.fetchall()

#print("{0}\n".format(data))


db.commit()


