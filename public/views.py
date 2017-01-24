#!/usr/bin/env python

from flask.views import MethodView
from flask import Blueprint,redirect,request,session,make_response,render_template,url_for
import requests

from conf import LOGIN_URL


class Login(MethodView):

    template_name = "public/public.html"

    def get(self):

        sid = request.args.get("sid")
        content = requests.get("https://login.ops.qihoo.net:4430/sec/login?sid=%s" % sid)
        try:
            user_info = content.json()
        except Exception:
            user_info = {}

        if user_info.has_key('user'):
            session['username'] = user_info['user']
            session['display'] = user_info['display'] 
            return redirect(url_for("facemange.list"))
        else:
            return redirect(LOGIN_URL)

class Logout(MethodView):

    def get(self):

        try:
            session.pop("username")
        except Exception as err:
            pass
        
        return redirect(url_for("login.login"))
        


login = Blueprint('login',__name__,template_folder='templates',url_prefix="/")
login.add_url_rule('login',endpoint='login',view_func = Login.as_view('login'))
login.add_url_rule('logout',endpoint='logout',view_func = Logout.as_view('logout'))

