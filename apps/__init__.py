
import os
from flask import Flask,redirect,session,request,abort
from conf import load_config,BASE_DIR
from recognition.views import recognition
from facemange.views import facemange
from public.views import login
import jinja2
from apps.utils import Navbar,some_random_string

app = Flask(__name__,static_folder='static')
app.config.from_object(load_config())
app.register_blueprint(login)
app.register_blueprint(recognition)
app.register_blueprint(facemange)
loader = jinja2.ChoiceLoader([app.jinja_loader,jinja2.FileSystemLoader([os.path.join(BASE_DIR,'templates'),]),])
app.jinja_loader = loader

@app.route("/")
def initlize():
    return redirect("/login")

@app.context_processor
def nav_process():
    return dict(nav=Navbar)


@app.before_request
def csrf_protect():
    if request.method == "POST":
        token = session.pop('_csrf_token', None)
        if not token or token != request.form.get('_csrf_token'):
            if token != request.json.get('_csrf_token'):
            	abort(403)

def generate_csrf_token():
    if '_csrf_token' not in session:
        session['_csrf_token'] = some_random_string()
    return session['_csrf_token']

app.jinja_env.globals['csrf_token'] = generate_csrf_token 

