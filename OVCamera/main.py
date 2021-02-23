# Main - By: LRMbbj - 周五 2月 19 2021

import sensor, image, time
from pyb import UART
from pyb import Pin

uart = UART(3,19200)
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.VGA)
roi = (170,190,100,100)
sensor.set_windowing(roi)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
sensor.set_brightness(2)
clock = time.clock()

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


while(True):
    while(p_en.value() == 1):
        clock.tick()
        #img = sensor.snapshot().lens_corr(1.8)
        img = sensor.snapshot()
        red_circles = []
        for c in img.find_circles(threshold = 3500, x_margin = 10, y_margin = 10, r_margin = 10,
                r_min = 2, r_max = 100, r_step = 2):
            area = (c.x()-c.r(), c.y()-c.r(), 2*c.r(), 2*c.r())
            #area为识别到的圆的区域，即圆的外接矩形框
            statistics = img.get_statistics(roi=area)#像素颜色统计
            #print(statistics)
            #(0,100,0,120,0,120)是红色的阈值，所以当区域内的众数（也就是最多的颜色），范围在这个阈值内，就说明是红色的圆。
            #l_mode()，a_mode()，b_mode()是L通道，A通道，B通道的众数。
            if 0<statistics.l_mode()<100 and 0<statistics.a_mode()<127 and 0<statistics.b_mode()<127:#if the circle is red
                img.draw_circle(c.x(), c.y(), c.r(), color = (0, 0, 255))#识别到的红色圆形用红色的圆框出来
                red_circles.append(c)
            else:
                img.draw_rectangle(area, color = (255, 255, 255))
                #将非红色的圆用白色的矩形框出来
        max_circle = None
        max_circle = find_maxc(red_circles)
        if max_circle!=None:
        uart.write( "C\r\n" )
            print("In Center")
        print("FPS %f" % clock.fps())
