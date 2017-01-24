#!/usr/bin/env python
#coding:utf-8
import os
from warnings import warn


BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

LOGIN_URL = 'https://login.ops.qihoo.net:4430/sec/login?ref=https://dpl06.skyeyes.lycc.qihoo.net:8000/login'
SROOT = "self"
DROOT = "/data/liubo/face/%s" % SROOT
CROOT = "capture"
CCROOT = "/data/liubo/face/%s" % CROOT

def load_config():   
    try:
        mode = os.environ.get('MODE')   
    except KeyError,e:      
        mode = "DEV"    
    try:        
        if mode == 'PRODUCTION':            
            from .production import ProductionConfig           
            return ProductionConfig     
        else:           
            from .development import DevelopmentConfig          
            return DevelopmentConfig    
    except ImportError,e:       
        from .default import Config     
        return Config

class CeleryConfig(object):

    CELERY_ENABLE_UTC = True
    CELERY_TIMEZONE = 'Asia/Shanghai'
    CELERY_RESULT_BACKEND = 'redis://localhost:6700/1'
    BROKER_TRANSPORT_OPTIONS = {'fanout_prefix': True,'fanout_patterns': True,'visibility_timeout': 43200}
    BROKER_URL = "redis://localhost:6700/1"

