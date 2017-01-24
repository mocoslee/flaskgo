#!/usr/bin/env python
#coding:utf-8
import sys
reload(sys)
sys.setdefaultencoding('utf-8')
import os
from re import search as _S,I as _I
from shutil import move,rmtree
from flask import Blueprint,render_template,request,session,url_for,redirect
from flask.views import MethodView
from time import localtime,strftime
from apps.utils import login_require,Navbar,SidebarItem
from conf import DROOT,SROOT,BASE_DIR
import subprocess

class Delete(MethodView):

    def post(self):
        
        source = request.form.get("dsource")
        dir = request.form.get("dir")
        sdir = os.path.join(DROOT,dir)
        for f in source.split("|"):
            if os.path.isdir(os.path.join(sdir,f)):
                rmtree(os.path.join(sdir,f))
            else:
                os.remove(os.path.join(sdir,f))

        for f in os.listdir(DROOT):
            if os.path.isdir(os.path.join(DROOT,f)):
                if not os.listdir(os.path.join(DROOT,f)):
                    rmtree(os.path.join(DROOT,f))
        return redirect("%s?dirname=%s" % (url_for('facemange.list'),dir))


class MoveTo(MethodView):

    def post(self):
        
        target = request.form.get("target")
        source = request.form.get("source")
        dir = request.form.get("dirz")

        sdir = os.path.join(DROOT,dir)
        tdir = os.path.join(DROOT,target)

        for f in source.split("|"):
            if os.path.isfile(os.path.join(sdir,f)):
                if os.path.isfile(os.path.join(tdir,f)):
                    os.remove(os.path.join(tdir,f))
                move(os.path.join(sdir,f),tdir)

        for f in os.listdir(DROOT):
            if os.path.isdir(os.path.join(DROOT,f)):
                if not os.listdir(os.path.join(DROOT,f)):
                    rmtree(os.path.join(DROOT,f))

        return redirect("%s?dirname=%s" % (url_for('facemange.list'),dir))



class CServer(MethodView):


    def post(self):
        
        status = request.form.get("status","")
        fd = open("%s/status" % BASE_DIR,"w")
        fd.write(status)
        fd.close()
        cat = subprocess.Popen(["supervisorctl",status,"recognition_server"], stdout=subprocess.PIPE)
        print cat.stdout.readlines()

        return redirect(url_for('facemange.list'))


class Dirs(MethodView):

    template_name = "facemange/list.html"

    @login_require
    def get(self):

        dirs = []
        files = []
        tdirs = []
        dirname = request.args.get("dirname","")
        for f in os.listdir(DROOT):
            if os.path.isdir(os.path.join(DROOT,f)) and (not _S(r'_',f)):
                tdirs.append(dict(name=f))

        if not os.path.isdir(os.path.join(DROOT,dirname)):
            dirname = ""

        dir = os.path.join(DROOT,dirname)
        dirpath = os.path.join(SROOT,dirname)
        for f in os.listdir(dir):
            if _S(r'same',f,_I):
                continue
            info = os.stat(os.path.join(dir,f))
            ctime = strftime("%Y-%m-%d %H:%M:%S",localtime(info.st_ctime))
            if os.path.isdir(os.path.join(dir,f)):
                dirs.append(dict(name=f,ctime=ctime))
            else:
                files.append(dict(name=f,ctime=ctime))

        dirs = sorted(dirs,key=lambda x:x["ctime"],reverse=True)
        files = sorted(files,key=lambda x:x["ctime"],reverse=True)

        counter = len(files)/12+1

        fd = open("%s/status" % BASE_DIR,"r")
        status = fd.readline()
        fd.close()
        


        return render_template(self.template_name,files=files,dirs=dirs,dirname=dirname,dirpath=dirpath,counter=counter,tdirs=tdirs,dir=dirname,status=status)

    def post(self):
        
        dirname = request.form.get("dirname","")
        odir = request.form.get("dir","")
        dirs = os.path.join(DROOT,dirname)
        if not os.path.isdir(dirs):
            os.makedirs(dirs,mode=0755)
        return redirect("%s?dirname=%s" % (url_for("facemange.list"),odir))




facemange = Blueprint('facemange',__name__,template_folder='templates',url_prefix='/facemange')
facemange.add_url_rule(rule='/list',endpoint='list',view_func=Dirs.as_view('list'))
facemange.add_url_rule(rule='/move',endpoint='move',view_func=MoveTo.as_view('move'))
facemange.add_url_rule(rule='/delete',endpoint='delete',view_func=Delete.as_view('delete'))
facemange.add_url_rule(rule='/server',endpoint='server',view_func=CServer.as_view('server'))
Navbar.add_sidebar_item(u'人脸识别','/',child=[SidebarItem(u'/facemange/list',u'人脸识别')])

