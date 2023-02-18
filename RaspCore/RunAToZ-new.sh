#!/bin/bash
exec 3>&1 4>&2
trap 'exec 2>&4 1>&3' 0 1 2 3
exec 1>>/home/pi/raspcam.log 2>&1

foldername=$(date +%Y-%m-%d-%H-%M)
mkdir -p "/home/pi/Pictures/latest/$foldername"

# raspistill
raspistill -o "/home/pi/Pictures/latest/$foldername/i%04d.jpg" -q 15 -rot 180 -w 1920 -h 1080 -n  -t 3000 -tl 1000 -v

chown pi:pi /home/pi/Pictures/latest

node send_msg_telegram.js  "/home/pi/Pictures/latest/$foldername/i0003.jpg"

sshpass -p *** sftp -oBatchMode=no -oStrictHostKeyChecking=no rp1@hkust-vgd.nas.ust.hk:VGD_ISD/rp1 <<< $"put -r /home/pi/Pictures/latest" && rm -rf "/home/pi/Pictures/latest/$foldername"

exit 0;
