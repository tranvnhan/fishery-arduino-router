from picamera import PiCamera
from os import system
import datetime
from time import sleep
#import subprocess

tlminutes = 1 #set this to the number of minutes you wish to run your timelapse camera
secondsinterval = 30 #number of seconds delay between each photo taken
fps = 30 #frames per second timelapse video
numphotos = int((tlminutes*60)/secondsinterval) #number of photos to take
print("number of photos to take = ", numphotos)

dateraw= datetime.datetime.now()
datetimeformat = dateraw.strftime("%Y-%m-%d_%H:%M")
folderformat = dateraw.strftime("%Y-%m-%d-%H-%M")
print("RPi started taking photos for your timelapse at: " + datetimeformat)

camera = PiCamera()
camera.resolution = (1024, 768)

#system('rm /home/pi/Pictures/*.jpg') #delete all photos in the Pictures folder before timelapse start
system('mkdir -p /home/pi/Pictures/latest/{}'.format(folderformat))

for i in range(numphotos):
    camera.capture('/home/pi/Pictures/latest/{}/image{}.jpg'.format(folderformat, i))
    sleep(secondsinterval)
print("Done taking photos.")
print("Please standby as your timelapse video is created.")

# system('ffmpeg -r {} -f image2 -s 1024x768 -nostats -loglevel 0 -pattern_type glob -i "/home/pi/Pictures/*.jpg" -vcodec libx264 -crf 25  -pix_fmt yuv420p /home/pi/Videos/{}.mp4'.format(fps, datetimeformat))
#system('rm /home/pi/Pictures/*.jpg')
# print('Timelapse video is complete. Video saved as /home/pi/Videos/{}.mp4'.format(datetimeformat))
#print('Copy to the NAS')
#system('sudo cp -r /home/pi/Pictures/{} /mnt/VGD_ISD/rp1'.format(folderformat))
#print("~./shUploadToNAS.sh '{}'".format(folderformat))
#system("~/shUploadToNAS.sh '{}'".format(folderformat))
#bashcommand = "sh /home/pi/shUploadToNAS.sh '{}'".format(folderformat)
#process = subprocess.Popen(bashcommand.split(), stdout=subprocess.PIPE)
#output, error = process.communicate()
#subprocess.call(['bash', "/home/pi/shUploadToNAS.sh '{}'".format(folderformat)])

