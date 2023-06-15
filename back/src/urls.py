"""
URL configuration for src project.

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/4.2/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.contrib import admin
from django.urls import path
from src.apps.core import views
urlpatterns = [
    path('admin/', admin.site.urls),
    path('publish/', views.publish_message, name='publish'),
    path('', views.index, name='home'),
    path('robot/<int:robot_id>/', views.robot, name='robot'),
    path('robot_create/', views.addrobot, name='addrobot'),
    path('robot_delete/<int:robot_id>/', views.deleterobot, name='deleterobot'),
    path('robot_bluetooth/<int:robot_id>/', views.change_type_robot, name='robot_bluetooth'),
    path('sensor_data/', views.sensor_data, name='sensor_data'),
    path('robot_data/', views.robot_data, name='robot_data')
]
