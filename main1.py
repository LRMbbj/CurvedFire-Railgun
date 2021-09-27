# Main - By: LRMbbj - 周五 2月 19 2021

import sensor, image, time
from pyb import UART
from pyb import Pin
from pyb import math
size = (100,200)#横坐标和纵坐标

uart = UART(3,19200)
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.VGA)
roi = (170,190,100,100）#（截取识别面积）
sensor.set_windowing(roi)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
sensor.set_brightness(2)
clock = time.clock()
PI = 3.1406380562059946

p_en = Pin('P7', Pin.IN, Pin.PULL_UP)#设置p_in为输入引脚，并开启上拉电阻


def find_maxc(cs):
    if len(cs)==0:
        return None
    max = cs[0]
    if len(cs)==1:
        return max
    for c in cs:
        if c.r()>max.r():
            max = c
    return max

def get_fHFOV(h = 0.617 846 555 21 ):
    th = math.tan(h)
    f = 36/th
    return f

def HFOV_new(f, cx):
    x = cx-90
    angle = 2*atan2(x/f)
    return angle

while(True):
    while(p_en.value() == 1):
        clock.tick()
        #img = sensor.snapshot().lens_corr(1.8)
        img = sensor.snapshot()#截图保存图像
        red_circles = []#create a circles
        for c in img.find_circles(threshold = 3500, x_margin = 10, y_margin = 10, r_margin = 10,
                r_min = 2, r_max = 100, r_step = 2):
            area = (c.x()-c.r(), c.y()-c.r(), 2*c.r(), 2*c.r())
            #area为识别到的圆的区域，即圆的外接矩形框
            statistics = img.get_statistics(roi=area)#像素颜色统计
            #print(statistics)
            #(13, 48, 19, 63, 9, 66)red_threahold better
            #(0,100,0,120,0,120)是红色的阈值，所以当区域内的众数（也就是最多的颜色），范围在这个阈值内，就说明是红色的圆。
            #l_mode()，a_mode()，b_mode()是L通道，A通道，B通道的众数。

            if 0<statistics.l_mode()<100 and 0<statistics.a_mode()<127 and 0<statistics.b_mode()<127:#if the circle is red
                img.draw_circle(c.x(), c.y(), c.r(), color = (0, 0, 255))#识别到的红色圆形用蓝色的圆框出来
                red_circles.append(c)
            else:
                img.draw_rectangle(area, color = (255, 255, 255))
                #将非红色的圆用白色的矩形框出来
        img.draw_cross(90,60,size=10)
        max_circle = None
        max_circle = find_maxc(red_circles)
        if max_circle!=None:
            f = get_fHFOV()
            angle = HFOV_new(f, c.x())
            uart.write("angle\r\n")
            print('angle=',angle)
            uart.write( "C\r\n" ) 1000 0100 = -4 0x15 0x14 \r \n
            print("In Center")

        print("FPS %f" % clock.fps())
