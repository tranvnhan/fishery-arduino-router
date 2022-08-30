#!/bin/bash
exec 3>&1 4>&2
trap 'exec 2>&4 1>&3' 0 1 2 3
exec 1>>/home/pi/raspcam.log 2>&1

python3 /home/pi/Desktop/timelapse/timelapse.py
ret=$?
if [ $ret -ne 0 ]; then
     echo "PiCameraError - Nothing uploaded"
     #Handle failure
     exit 0;
fi

chown pi:pi /home/pi/Pictures/latest

sshpass -p **** sftp -oBatchMode=no -oStrictHostKeyChecking=no rp1@10.8.0.1:VGD_ISD/rp1 <<< $"put -r /home/pi/Pictures/latest"

rm -rf /home/pi/Pictures/latest

exit 0;
