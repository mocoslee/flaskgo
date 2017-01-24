#coding:utf-8
import setuptools

setuptools.setup( 
    name = 'recognition',
    version = '0.0.1',
    description = 'recognition',
    author = 'lining',
    author_email = 'mocoslee@gmail.com',
    url = '',
    install_requires = ['Flask==0.10.1','uWSGI==2.0.11.1','requests','Flask-Script==0.3.1','tornado'],      
    packages = ['apps','conf','templates','public','facemange'],  
    scripts = ["manager.py"],    
    include_package_data = True,    
    classifiers=['Development Status :: 4 - Beta Development Status','Environment :: Console','Topic :: Utilities',    ],
)
