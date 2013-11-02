#coding:utf8
'''
#=============================================================================
#     FileName: packet.py
#         Desc: 数据包类
#       Author: Hector
#        Email: myourys@gmail.com
#     HomePage: http://www.yiwuye.com
#      Version: 0.0.1
#   LastChange: 2013-10-31 22:52:47
#      History:
#=============================================================================
'''

import struct

class TPackError(Exception):
    """A TPackError occurred"""
    def __str__(self):
        s = self.__doc__
        if self.args:
            s = "%s: %s" % (s,' '.join(self.args))
        s = "%s." % s
        return s

class TPack:
    def __init__(self,HEAD0 = 0,HEAD1=0,HEAD2=0,HEAD3=0,protoVersion = 0,serverVersion=0):
        """ 初始化数据

        四位文件头"""
        self.HEAD0 = HEAD0
        self.HEAD1 = HEAD1
        self.HEAD2 = HEAD2
        self.HEAD3 = HEAD3

        self.protoVersion = protoVersion
        self.serverVersion = serverVersion

    def getHeadLength(self):
        return 17

    def unPack(self,pack):
        '''解包

        返回解包结果'''
        try:
            udoc = struct.unpack('!sssss3I',pack)
        except TPackError,de:
            return {'result':False,'command':0,'length':0}

        HEAD0 = ord(udoc[0])
        HEAD1 = ord(udoc[1])
        HEAD2 = ord(udoc[2])
        HEAD3 = ord(udoc[3])

        protoVersion = ord(udoc[4])
        serverVersion = udoc[5];
        length = udoc[6] 
        command = udoc[7]

        if HEAD0 <> self.HEAD0 or HEAD1 <> self.HEAD1 or \
                HEAD2<>self.HEAD2 or HEAD3 <> self.HEAD3 or \
                protoVersion <> self.protoVersion or serverVersion<>self.serverVersion:
            return {'result':False,'command':0,'length':length}

        return {'result':True,'command':command,'length':length}

    def pack(self,commandId,data):
        '''打包数据
        @param data: 数据类容
        @param commandId: 功能号'''

        HEAD0 = chr(self.HEAD0)
        HEAD1 = chr(self.HEAD1)
        HEAD2 = chr(self.HEAD2)
        HEAD3 = chr(self.HEAD3)

        protoVersion = chr(self.protoVersion)
        serverVersion = self.serverVersion
        length = data.__len__()
        doc = struct.pack('!sssss3I',HEAD0,HEAD1,HEAD2,HEAD3,\
                           protoVersion,serverVersion,length,commandId)
        doc = doc + data
        return doc

