#coding:utf8
'''
#=============================================================================
#     FileName: main.py
#         Desc: 
#       Author: Hector
#        Email: myourys@gmail.com
#     HomePage: http://www.yiwuye.com
#      Version: 0.0.1
#   LastChange: 2013-10-31 22:48:51
#      History:
                暂时弃用,改用server.py 多线程方式
#=============================================================================
'''

from SocketServer import TCPServer, ThreadingMixIn,StreamRequestHandler
from app.init import *

#create thread allow muti client
class server(TCPServer,ThreadingMixIn):
    pass


class dataHandle(StreamRequestHandler):
    def handle(self):
        cliend_ip = self.request.getpeername() #rewrite handle
        print "client_ip:",cliend_ip

        data = ''
        while True:
            tmp = self.request.recv(2048)
            #self.request.sendall(tmp)
            #self.wfile.write(tmp)
            if(tmp.__len__() == 0):
                break
            data += tmp
            rt = MsgPro.handle(data,self.request)
            if(rt == -1):
                return
            else:
                continue

if __name__ == "__main__":
    #print Conn.getValue('select id from tb_users where u_name =%s and u_pwd=%s','admin','admin')
    servers=server(('192.168.1.113',11008),dataHandle)
    servers.serve_forever()
