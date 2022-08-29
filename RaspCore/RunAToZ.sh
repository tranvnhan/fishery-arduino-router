python3 /home/pi/Desktop/timelapse/timelapse.py
sshpass -p $PASSWORD sftp -oBatchMode=no -oStrictHostKeyChecking=no rp1@10.8.0.1:VGD_ISD/rp1 <<< $"put -r /home/pi/Pictures/latest"
rm -rf /home/pi/Pictures/latest
