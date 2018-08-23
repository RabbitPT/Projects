# PROJECT MADE BY LEONARDO ESTEVES AND JOÃO RALHO FOR DOGNAEDIS
# "DEVICE SHREDDER"

import time
import Adafruit_GPIO.SPI as SPI
import Adafruit_SSD1306
import RPi.GPIO as GPIO
import pyudev
import os
import subprocess
import functools
import os.path

from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont

#variables==========================
verify = False
verify1 = False
verify2 = False
x = 0
case = 0
#Functions==========================
def handle_event(action, device):
    global verify
    global verify1
    if(action=='remove'):
        verify = True
    if(action=='change'):
        verify1 = True
        
#FUNCTION TO DETECT IF THE DEVICE IS INSERTED IN THE USB PORT
def device_plugged():
    global verify2
    p1 = subprocess.Popen(["lsblk"], stdout=subprocess.PIPE)
    p2 = subprocess.Popen(["grep", "sd"], stdin=p1.stdout, stdout=subprocess.PIPE)
    p1.stdout.close()
    p3 = subprocess.Popen(["grep", "disk"], stdin=p2.stdout, stdout=subprocess.PIPE)
    p2.stdout.close()
    p4 = subprocess.Popen(["cut", "-f", "1", "-d", "" ""], stdin=p3.stdout, stdout=subprocess.PIPE)
    p3.stdout.close()

    frase = p4.communicate()[0]
    word = 'sda'

    if word.encode() in frase:
    	verify2 = True
    else:
    	verify2 = False

# FUNCTION TO SHOW THE CASES: FAT32, NTFS AND EXT4 
def draw_case(texto, imagem):
    W, H = (128, 64)
    image = Image.new('1', (width, height))
    logo = Image.open(imagem)
    position = ((image.width - logo.width),(image.height - logo.height))
    image.paste(logo, position)
    draw = ImageDraw.Draw(image)
    w, h = draw.textsize(texto)
    draw.text(((W-w)/2, top+1),       texto,  font=font, fill=255)
    disp.image(image)
    disp.display()
    
#FUNCTION TO SHOW THE CASE WHERE YOU ONLY SHRED THE PEN
def draw_case4():
    W, H = (128, 64)
    text1 = "JUST THE SHRED"
    text2 = "OF THE DEVICE"
    image = Image.new('1', (width, height))
    draw = ImageDraw.Draw(image)
    w, h = draw.textsize(text1)
    w1, h1 = draw.textsize(text2)
    draw.text(((W-w)/2,top+20), text1, font=font, fill=255)
    draw.text(((W-w1)/2,top+34), text2, font=font, fill=255)
    disp.image(image)
    disp.display()

#FUNCTION TO SHOW THE DOGNAEDIS LOGO
def inicial(imagem):
    image = Image.new('1', (width, height))
    logo = Image.open(imagem)
    position = ((image.width - logo.width),(image.height - logo.height))
    image.paste(logo, position)
    disp.image(image)
    disp.display()

#FUNCTION TO SH0W THE DEVICE IS NOT INSERTED
def pen_remove():
    W, H = (128, 64)
    text1 = "PLEASE INSERT"
    text2 = "THE DEVICE!"
    image = Image.new('1', (width, height))
    draw = ImageDraw.Draw(image)
    w, h = draw.textsize(text1)
    w1, h1 = draw.textsize(text2)
    draw.text(((W-w)/2,top+20), text1, font=font, fill=255)
    draw.text(((W-w1)/2,top+34), text2, font=font, fill=255)
    disp.image(image)
    disp.display()
  
#FUNCTION TO DRAW THE PLEASE WAIT ANIMATION  
def wait(x2,y2,x3,y3):
    x0 = 7
    y0 = 16
    x1 = 119
    y1 = 33
    image = Image.new('1', (width, height))
    draw = ImageDraw.Draw(image)
    draw.rectangle([x0,y0,x1,y1], fill=0, outline=255)
    draw.rectangle([x2,y2,x3,y3], fill=255, outline=0)
    draw.text((x+26,top+48), "PLEASE WAIT", font=font, fill=255)
    disp.image(image)
    disp.display()

#FUNCTION TO SHOW THE PROCESS IS COMPLETE
def complete():
    W, H = (128, 64)
    text = "Complete!"
    image = Image.new('1', (width, height))
    draw = ImageDraw.Draw(image)
    w, h = draw.textsize(text)
    draw.text(((W-w)/2,top+20), text, font=font, fill=255)
    disp.image(image)
    disp.display()
#MAIN===============================

#DETECTING THE DEVICE SETTINGS
context = pyudev.Context()
monitor = pyudev.Monitor.from_netlink(context)
monitor.filter_by('block', device_type='disk')
observer = pyudev.MonitorObserver(monitor, handle_event)
observer.start()

#BUTTONS SETTINGS
GPIO.setmode(GPIO.BCM)
GPIO.setup(26, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(19, GPIO.IN, pull_up_down=GPIO.PUD_UP)




#--------DEFINITIONS OF THE LCD------------------

# RASPBERY PI PIN CONFIGURATION
RST = None     #ON THE PiOLED THIS PIN ISN'T USED
DC = 23
SPI_PORT = 0
SPI_DEVICE = 0

# 128x32 DISPLAY WITH HARDWARE I2C:
#disp = Adafruit_SSD1306.SSD1306_128_32(rst=RST)
# 128x64 DISPLAY WITH HARDWARE I2C:
disp = Adafruit_SSD1306.SSD1306_128_64(rst=RST)
# INITIALIZE LIBRARY
disp.begin()
disp.clear()
disp.display()

# CREATE BLANK IMAGE TO DRAW
# MAKE SURE TO CREATE IMAGE WITH MODE '1' FOR 1-BIT COLOR
width = disp.width
height = disp.height
image = Image.new('1', (width, height))

# GET DRAWING OBJECT
draw = ImageDraw.Draw(image)

# DRAW A BLACK FILLED RECTANGLE TO CLEAR IMAGE
draw.rectangle((0,0,width,height), outline=0, fill=0)

# DEFINE SOME CONSTANTS TO ALLOW EASY DRAWING
padding = -2
top = padding
bottom = height-padding

# LOAD DEFAULT FONT
#font = ImageFont.load_default()
# LOAD CUSTOM FONT
font = ImageFont.truetype("DejaVuSansMono.ttf",12)
#------------------------------------------------------------------------------
inicial('Dognaedis.ppm')

while True:
    # VERIFY IF THE DEVICE IS PLUGGED
    device_plugged()
    # IF IT'S NOT PLUGGED
    while verify2 == False:
        device_plugged()
        # IF IT WAS REMOVED OR IF WE START THE PROGRAM WITHOU THE DEVICE INSERTED
        if verify == False:
            inicial('Dognaedis.ppm')
        else:
            pen_remove()
            case = 0
        time.sleep(.1)
    # IF IT'S PLUGGED
    while verify2 == True:
        if verify == True and case == 0:
            inicial('Dognaedis.ppm')
            
        device_plugged()
        # CHECKING THE INPUT STATE
        input_state1 = GPIO.input(26) #SWITCH CASE BUTTON
        input_state2 = GPIO.input(19) #SELECT BUTTON
                  
        #CHANGE THE CASE IF WE PRESS THE BUTTON
        if input_state1 == False:
            if case == 4:
                case = 1
            else:
                case = case + 1
            if case == 1:
                draw_case('FAT32', 'fat32setting.ppm')
            elif case == 2:
                draw_case('NTFS', 'ntfssetting.ppm')
            elif case == 3:
                draw_case('EXT4', 'ext4setting.ppm')
            elif case == 4:
                draw_case4()
        time.sleep(.2)


        if case == 1:
            # IF WE PRESS THE SELECT BUTTON
            if input_state2 == False:
		# START BY SHREDDING THE DEVICE
                p = subprocess.Popen(["sudo", "shred", "-vf", "-n", "3", "/dev/sda"])
                n = 0
                ended = False
		# MAKE THE WAITING ANIMATION
                while not ended:
                    n = (n+1) % 20
                    xt = (100 - abs(100 - (n*10) )) + 13
                    x1 = xt - 4
                    x2 = xt + 5
                    wait(x1, 17, x2, 32)
                    time.sleep(.1)
                    if n==0:
                        if p.poll() is not None:
                            ended = True
                # CREATE THE PARTITION TABLE            
                p = subprocess.Popen(["sudo", "parted", "-s", "/dev/sda", "mktable", "msdos"])
                n = 0
                ended = False
		# MAKE THE WAITING ANIMATION
                while not ended:
                    n = (n+1) % 20
                    xt = (100 - abs(100 - (n*10) )) + 13
                    x1 = xt - 4
                    x2 = xt + 5
                    wait(x1, 17, x2, 32)
                    time.sleep(.1)
                    if n==0:
                        if p.poll() is not None:
                            ended = True
		# CREATE A PARTITION WITH FAT32 FORMAT
                p = subprocess.Popen(["sudo", "parted", "-s", "/dev/sda", "--", "mkpart", "primary", "fat32", "4MiB", "-1s"])
                n = 0
                ended = False
		# MAKE THE WAIT ANIMATION
                while not ended:
                    n = (n+1) % 20
                    xt = (100 - abs(100 - (n*10) )) + 13
                    x1 = xt - 4
                    x2 = xt + 5
                    wait(x1, 17, x2, 32)
                    time.sleep(.1)
                    if n==0:
                        if p.poll() is not None:
                            ended = True
		# FORMAT THE DEVICE IN FAT32
                p = subprocess.Popen(["sudo", "mkfs.vfat", "/dev/sda1"])
                n = 0
                ended = False
		# MAKE THE WAITING ANIMATION
                while not ended:
                    n = (n+1) % 20
                    xt = (100 - abs(100 - (n*10) )) + 13
                    x1 = xt - 4
                    x2 = xt + 5
                    wait(x1, 17, x2, 32)
                    time.sleep(.1)
                    if n==0:
                        if p.poll() is not None:
                            ended = True
		# THE PROCESS IS DONE SO SHOW THE USER IT'S COMPLETE
                complete()
                time.sleep(5)
		# AFTER WAITING 5 SECONDS GO BACK TO THE MENU
                draw_case('FAT32', 'fat32setting.ppm')
        if case == 2: 
	    # IF WE PRESS THE SELECT BUTTON
            if input_state2 == False:
		# START BY SHREDDING THE DEVICE
                p = subprocess.Popen(["sudo", "shred", "-vf", "-n", "3", "/dev/sda"])
                n = 0
                ended = False
		# MAKE THE WAITING ANIMATION
                while not ended:
                    n = (n+1) % 20
                    xt = (100 - abs(100 - (n*10) )) + 13
                    x1 = xt - 4
                    x2 = xt + 5
                    wait(x1, 17, x2, 32)
                    time.sleep(.1)
                    if n==0:
                        if p.poll() is not None:
                            ended = True
		# CREATE THE PARTITION TABLE
                p = subprocess.Popen(["sudo", "parted", "-s", "/dev/sda", "mktable", "msdos"])
                n = 0
                ended = False
		# MAKE THE WAITING ANIMATION
                while not ended:
                    n = (n+1) % 20
                    xt = (100 - abs(100 - (n*10) )) + 13
                    x1 = xt - 4
                    x2 = xt + 5
                    wait(x1, 17, x2, 32)
                    time.sleep(.1)
                    if n==0:
                        if p.poll() is not None:
                            ended = True
		# CREATE A PARTITION WITH NTFS FORMAT
                p = subprocess.Popen(["sudo", "parted", "-s", "/dev/sda", "--", "mkpart", "primary", "NTFS", "4MiB", "-1s"])
                n = 0
                ended = False
		# MAKE THE WAITING ANIMATION
                while not ended:
                    n = (n+1) % 20
                    xt = (100 - abs(100 - (n*10) )) + 13
                    x1 = xt - 4
                    x2 = xt + 5
                    wait(x1, 17, x2, 32)
                    time.sleep(.1)
                    if n==0:
                        if p.poll() is not None:
                            ended = True
		# FORMAT THE DEVICE IN NTFS
                p = subprocess.Popen(["sudo", "mkfs.ntfs", "-f", "/dev/sda1"])
                n = 0
                ended = False
		# MAKE THE WAITITNG ANIMATION
                while not ended:
                    n = (n+1) % 20
                    xt = (100 - abs(100 - (n*10) )) + 13
                    x1 = xt - 4
                    x2 = xt + 5
                    wait(x1, 17, x2, 32)
                    time.sleep(.1)
                    if n==0:
                        if p.poll() is not None:
                            ended = True
		# THE PROCESS IS DONE SO SHOW THE USER IT'S COMPLETE 
                complete()
                time.sleep(5)
		# AFTER WAITING 5 SECONDS GO BACK TO THE MENU
                draw_case('NTFS', 'ntfssetting.ppm')
        if case == 3:
	    # IF WE PRESS THE SELECT BUTTON
            if input_state2 == False:
		# START BY SHREDDING THE DEVICE
                p = subprocess.Popen(["sudo", "shred", "-vf", "-n", "3", "/dev/sda"])
                n = 0
                ended = False
		# MAKE THE WAITING ANIMATION
                while not ended:
                    n = (n+1) % 20
                    xt = (100 - abs(100 - (n*10) )) + 13
                    x1 = xt - 4
                    x2 = xt + 5
                    wait(x1, 17, x2, 32)
                    time.sleep(.1)
                    if n==0:
                        if p.poll() is not None:
                            ended = True
                # CREATE THE PARTITION TABLE     
                p = subprocess.Popen(["sudo", "parted", "-s", "/dev/sda", "mktable", "msdos"])
                n = 0
                ended = False
		# MAKE THE WAITING ANIMATION
                while not ended:
                    n = (n+1) % 20
                    xt = (100 - abs(100 - (n*10) )) + 13
                    x1 = xt - 4
                    x2 = xt + 5
                    wait(x1, 17, x2, 32)
                    time.sleep(.1)
                    if n==0:
                        if p.poll() is not None:
                            ended = True
		# CREATE A PARTITION WITH EXT4 FORMAT
                p = subprocess.Popen(["sudo", "parted", "-s", "/dev/sda", "--", "mkpart", "primary", "ext4", "4MiB", "-1s"])
                n = 0
                ended = False
		# MAKE THE WAITING ANIMATION
                while not ended:
                    n = (n+1) % 20
                    xt = (100 - abs(100 - (n*10) )) + 13
                    x1 = xt - 4
                    x2 = xt + 5
                    wait(x1, 17, x2, 32)
                    time.sleep(.1)
                    if n==0:
                        if p.poll() is not None:
                            ended = True
		# CREATE A PARTITION WITH EXT4 FORMAT
                p = subprocess.Popen(["sudo", "mkfs.ext4", "/dev/sda1"])
                n = 0
                ended = False
		# MAKE THE WAITING ANIMATION
                while not ended:
                    n = (n+1) % 20
                    xt = (100 - abs(100 - (n*10) )) + 13
                    x1 = xt - 4
                    x2 = xt + 5
                    wait(x1, 17, x2, 32)
                    time.sleep(.1)
                    if n==0:
                        if p.poll() is not None:
                            ended = True
		# WHEN THE DEVICE IS IN EXT4 THE USER HAS NO PERMITIONS TO WRITE SO WE GIVE ALL THE PERMITIONS
		# FIRST WE MOUNT THE DEVICE
                p = subprocess.Popen(["sudo", "mount", "/dev/sda1", "/media/pen"])
                n = 0
                ended = False
		# MAKE THE WAITING ANIMATION
                while not ended:
                    n = (n+1) % 20
                    xt = (100 - abs(100 - (n*10) )) + 13
                    x1 = xt - 4
                    x2 = xt + 5
                    wait(x1, 17, x2, 32)
                    time.sleep(.1)
                    if n==0:
                        if p.poll() is not None:
                            ended = True
		# THEN WE GIVE ALL THE PERMITIONS
                p = subprocess.Popen(["sudo", "chmod", "777", "/media/pen"])
                ended = False
		# MAKE THE WAITING ANIMATION
                while not ended:
                    n = (n+1) % 20
                    xt = (100 - abs(100 - (n*10) )) + 13
                    x1 = xt - 4
                    x2 = xt + 5
                    wait(x1, 17, x2, 32)
                    time.sleep(.1)
                    if n==0:
                        if p.poll() is not None:
                            ended = True
		# FINALLY WE UNMOUNT THE DEVICE
                p = subprocess.Popen(["sudo", "umount", "/dev/sda1"])
                ended = False
		# MAKE THE WAITING ANIMATION
                while not ended:
                    n = (n+1) % 20
                    xt = (100 - abs(100 - (n*10) )) + 13
                    x1 = xt - 4
                    x2 = xt + 5
                    wait(x1, 17, x2, 32)
                    time.sleep(.1)
                    if n==0:
                        if p.poll() is not None:
                            ended = True
		# THE PROCESS IS DONE SO SHOW THE USER IT'S COMPLETE 
                complete()
                time.sleep(5)
		# AFTER WAITING 5 SECONDS GO BACK TO THE MENU
                draw_case('EXT4', 'ext4setting.ppm')
        if case == 4:
            # IF WE PRESS THE SELECT BUTTON
            if input_state2 == False:
		# START BY SHREDDING THE DEVICE
                p = subprocess.Popen(["sudo", "shred", "-vf", "-n", "3", "/dev/sda"])
                n = 0
                ended = False
		# MAKE THE WAITING ANIMATION
                while not ended:
                    n = (n+1) % 20
                    xt = (100 - abs(100 - (n*10) )) + 13
                    x1 = xt - 4
                    x2 = xt + 5
                    wait(x1, 17, x2, 32)
                    time.sleep(.1)
                    if n==0:
                        if p.poll() is not None:
                            ended = True
                # CREATE THE PARTITION TABLE            
                p = subprocess.Popen(["sudo", "parted", "-s", "/dev/sda", "mktable", "msdos"])
                ended = False
		# MAKE THE WAITING ANIMATION
                while not ended:
                    n = (n+1) % 20
                    xt = (100 - abs(100 - (n*10) )) + 13
                    x1 = xt - 4
                    x2 = xt + 5
                    wait(x1, 17, x2, 32)
                    time.sleep(.1)
                    if n==0:
                        if p.poll() is not None:
                            ended = True
		# THE PROCESS IS DONE SO SHOW THE USER IT'S COMPLETE 
                complete()
                time.sleep(5)
		# AFTER WAITING 5 SECONDS GO BACK TO THE MENU
                draw_case4()
        time.sleep(.1)

