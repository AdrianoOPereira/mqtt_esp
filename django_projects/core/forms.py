from django import forms
from core.models import Awb


class AwbForms(forms.ModelForm):
    awb = forms.CharField(label='awb', max_length=30, min_length=2),
    cpf = forms.EmailField(label='cpf', max_length=2),