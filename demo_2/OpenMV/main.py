import pyb, sensor, image, time
enable_lens_corr = True # turn on for straighter lines...
sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE) # grayscale is faster
sensor.set_framesize(sensor.QQVGA)
sensor.set_vflip(True)
sensor.set_hmirror(True)
sensor.skip_frames(time = 2000)
clock = time.clock()

uart = pyb.UART(3,9600,timeout_char=1000)
uart.init(9600,bits=8,parity = None, stop=1, timeout_char=1000)

# All lines also have `x1()`, `y1()`, `x2()`, and `y2()` methods to get their end-points
# and a `line()` method to get all the above as one 4 value tuple for `draw_line()`.

while(True):
    clock.tick()
    img = sensor.snapshot().histeq()
    if enable_lens_corr: img.lens_corr(1.8) # for 2.8mm lens...
    
    # `merge_distance` controls the merging of nearby lines. At 0 (the default), no
    # merging is done. At 1, any line 1 pixel away from another is merged... and so
    # on as you increase this value. You may wish to merge lines as line segment
    # detection produces a lot of line segment results.
    
    # `max_theta_diff` controls the maximum amount of rotation difference between
    # any two lines about to be merged. The default setting allows for 15 degrees.

    for l in img.find_line_segments(merge_distance = 20, max_theta_diff = 5):
        rho = l.rho()
        theta = l.theta()
        x = l.x2() - 80
        if rho < 0:
            theta  += 180
        if theta >= 315:
            theta -= 360;
        if (-45 <= theta <= 45) and l.y2() > 80:
            img.draw_line(l.line(), color = (255, 0, 0), thickness=5)
            print(theta, x)
            uart.write(("%d %d\r\n" % (theta, x)).encode())

    #print("FPS %f" % clock.fps())