from django.urls import path
from . import views
from .views import index, enviar_comando_arduino
from django.contrib.auth import views as auth_views

urlpatterns = [
    path("", index, name="index"),
    path('enviar_serial/<str:cmd>/', enviar_comando_arduino, name='enviar_serial'),
    path('login/', auth_views.LoginView.as_view(template_name='login.html'), name='login'),
]
