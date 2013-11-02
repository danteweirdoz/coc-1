#coding:utf8
'''
#=============================================================================
#     FileName: server.py
#         Desc: 多线程版本
#       Author: Hector
#        Email: myourys@gmail.com
#     HomePage: http://www.yiwuye.com
#      Version: 0.0.1
#   LastChange: 2013-11-01 23:27:27
#      History:
#=============================================================================
'''

import socket
import threading
import SocketServer
from app.init import *

class ThreadedTCPRequestHandler(SocketServer.BaseRequestHandler):

    def handle(self):
        data = ''
        while True:
            tmp = self.request.recv(2048)
            #self.request.sendall(tmp)
            #self.wfile.write(tmp)
            if(tmp.__len__() == 0):
                break
            data += tmp
            rt ,data= MsgPro.handle(data,self.request)
            if(rt == -1):
                return
            else:
                continue

class ThreadedTCPServer(SocketServer.ThreadingMixIn, SocketServer.TCPServer):
    pass


if __name__ == "__main__":
    # Port 0 means to select an arbitrary unused port
    HOST, PORT = "192.168.1.113", 11008

    server = ThreadedTCPServer((HOST, PORT), ThreadedTCPRequestHandler)
    ip, port = server.server_address

    # Start a thread with the server -- that thread will then start one
    # more thread for each request
    server_thread = threading.Thread(target=server.serve_forever)
    # Exit the server thread when the main thread terminates
    #server_thread.daemon = True
    server_thread.start()
    print "Server loop running in thread"

    #server.shutdown()


