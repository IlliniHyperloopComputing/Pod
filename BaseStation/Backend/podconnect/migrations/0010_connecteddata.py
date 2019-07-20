# Generated by Django 2.1.3 on 2019-07-20 17:56

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('podconnect', '0009_auto_20190718_0022'),
    ]

    operations = [
        migrations.CreateModel(
            name='ConnectedData',
            fields=[
                ('date_time', models.DateTimeField(auto_now=True, primary_key=True, serialize=False)),
                ('tcp_connected', models.IntegerField(default=0)),
                ('udp_connected', models.IntegerField(default=0)),
            ],
        ),
    ]