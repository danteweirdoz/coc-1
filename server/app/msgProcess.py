#coding:utf8
'''
#=============================================================================
#     FileName: msgProcess.py
#         Desc: 数据处理类
#       Author: Hector
#        Email: myourys@gmail.com
#     HomePage: http://www.yiwuye.com
#      Version: 0.0.1
#   LastChange: 2013-11-01 20:46:29
#      History:
#=============================================================================
'''

import json
from app.init import *

class MsgProcess:
    def __init__(self):
        self.dicProcess = {}

    def AddMsgPro(self,command,pro):
        self.dicProcess[command] = pro

    def handle(self,buff,wfile):
        '''消息接收处理
        '''
        headLength = GPack.getHeadLength()
        count = 0
        while buff.__len__() >= headLength:
            uhead = GPack.unPack(buff[:headLength])
            if not uhead.get('result'):
                print "解包出错"
                return -1,''
            command = uhead.get('command')
            dataLength = uhead.get('length')
            msg = buff[headLength:headLength+dataLength]
            if msg.__len__() < dataLength:
                print "数据部分丢失"
                return -1,''
            if(headLength+dataLength < buff.__len__()):
                buff = buff[headLength+dataLength]
            else:
                buff = ''

            try:
                msg = json.loads(msg)
            except Exception:
                print '无法解包的json格式:%s' % msg
                return -1,''
            pro = self.dicProcess[command]
            if(not pro):
                print '无法处理该请求:%d' % command
                return -1,''
            pro(msg,wfile)
            count += 1
        return count,buff



