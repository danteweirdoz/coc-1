#coding:utf8
'''
#=============================================================================
#     FileName: init.py
#         Desc: 
#       Author: Hector
#        Email: myourys@gmail.com
#     HomePage: http://www.yiwuye.com
#      Version: 0.0.1
#   LastChange: 2013-11-01 00:09:42
#      History:
#=============================================================================
'''


from app.packet import TPack
from app.DbAccess import Connection

GPack = TPack(78,37,38,48,9,0)
Conn = Connection(host='localhost', database='coc', user='root', password='root')

from app.msgProcess import MsgProcess
MsgPro = MsgProcess()


from users import *



