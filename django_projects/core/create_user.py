from django.db import migrations
from django.contrib.auth.models import User

# def create_initial_user(apps, schema_editor):
def criar_usuario_inicial(apps, schema_editor):    
    if not User.objects.filter(username='adriano').exists():
        user = User.objects.create_user( # para que a senha seja criptografada corretamente.
            username='adriano',
            email='adriano.oliveirapereira@gmail.com',
            password='visa18'
        )
        user.first_name = 'Vanessa'
        user.last_name = 'Pereira'
        user.save()

class Migration(migrations.Migration):

    dependencies = [
        ('core', '0001_initial'),  # ajuste conforme necessário
    ]

    operations = [
        migrations.RunPython(criar_usuario_inicial),
    ]

