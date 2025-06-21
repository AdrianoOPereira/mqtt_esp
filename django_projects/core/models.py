from django.db import models

class Awb(models.Model):
    awb = models.CharField(max_length= 16)
    
    
    def __str__(self):
        return self.name
    
