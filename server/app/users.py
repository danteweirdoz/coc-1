#coding:utf8
'''
#=============================================================================
#     FileName: users.py
#         Desc: 用户相关
#       Author: Hector
#        Email: myourys@gmail.com
#     HomePage: http://www.yiwuye.com
#      Version: 0.0.1
#   LastChange: 2013-11-01 21:44:32
#      History:
#=============================================================================
'''

from app.init import *
import json

def Pro1001(jsData,request):
    #wfile.write(rt)
    query = "select id from tb_users where u_name=%s and u_pwd = %s"
    uid = Conn.getValue(query,str(jsData['username']),str(jsData['password']))
    if not uid:
        uid = 0
    rtData = {'uid':uid}
    rt = GPack.pack(1001,json.dumps(rtData))
    request.sendall(rt)


MsgPro.AddMsgPro(1001,Pro1001)

