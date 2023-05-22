from django import forms


class RobotForm(forms.Form):
    ipaddr = forms.CharField(
        label="IP-адрес устройства",
        max_length=16
    )
    token = forms.CharField(
        label="Кодовое наименование устройства",
        max_length=16,
    )
