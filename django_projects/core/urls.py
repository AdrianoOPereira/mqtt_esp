##from django_projects.mqtt.urls import path

from django.urls import path, include 
from . import views
from .views import index, enviar_comando_arduino

urlpatterns = [
    path("", index, name="index.html"),
    path('enviar_serial/<str:cmd>/', enviar_comando_arduino, name='enviar_serial'),
]
