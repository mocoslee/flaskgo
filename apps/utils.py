#!/usr/bin/env python

import random
from hashlib import sha1
from flask import redirect
from public.views import session,request

def some_random_string():

    num =  random.random() * 10000
    return sha1(str(num)).hexdigest()

def login_require(func):

    def wrapped(cls):
        if not session.get('username',None):
            return redirect("/login")
        return func(cls)
    return wrapped



class SidebarItem(object):
    def __init__(self, link, title, child=[]):
        self.link = link
        if title:
            self.title = title
        else:
            self.title = link
        self.child = child


class Navbar(object):

    sidebar_items = []
    @classmethod
    def add_sidebar_item(self, link, title=None, child=[]):
        self.sidebar_items.append(SidebarItem(title, link, child=child))

    @property
    def user(self):
        return session.get('display',None)

