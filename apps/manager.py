#!/usr/bin/env python
#coding:utf-8

from flask_script import Manager
from tornado.httpserver import HTTPServer
from tornado.ioloop import IOLoop
from tornado.web import Application
from recognition.worker import route
from apps import app

manager = Manager(app)


@manager.command
def socketrun():
    apps = Application(route)
    server = HTTPServer(apps)
    server.bind(5000)
    try:
        server.start()
        IOLoop.current().start()
    except KeyboardInterrupt:
        IOLoop.current().stop()
        pass

