import sensor, image, time, math, pyb, gc
from pyb import UART
def find_AprilTag():
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QQVGA) # we run out of memory if the resolution is much bigger...
    sensor.skip_frames(30)
    sensor.set_auto_gain(False)  # must turn this off to prevent image washout...
    sensor.set_auto_whitebal(False)  # must turn this off to prevent image washout...
    clock = time.clock()

    clock.tick()
    img = sensor.snapshot()
    tag_msg = []
    for tag in img.find_apriltags(): # defaults to TAG36H11 without "families".
        img.draw_rectangle(tag.rect(), color = (255, 0, 0))
        img.draw_cross(tag.cx(), tag.cy(), color = (0, 255, 0))
        degress = 180 * tag.rotation() / math.pi
        tag_msg.append([tag.id(),degress])
    img = None
    return tag_msg

def degrees(radians):
    return (180 * radians) / math.pi

def find_AprilTag_position():
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QQVGA) # we run out of memory if the resolution is much bigger...
    sensor.skip_frames(30)
    sensor.set_auto_gain(False)  # must turn this off to prevent image washout...
    sensor.set_auto_whitebal(False)  # must turn this off to prevent image washout...
    clock = time.clock()


    f_x = (2.8 / 3.984) * 160 # 默认值
    f_y = (2.8 / 2.952) * 120 # 默认值
    c_x = 160 * 0.5 # 默认值(image.w * 0.5)
    c_y = 120 * 0.5 # 默认值(image.h * 0.5)
    clock.tick()
    img = sensor.snapshot()
    position_msg = []#tagid，xyz距离，xyz旋转角度
    for tag in img.find_apriltags(fx=f_x, fy=f_y, cx=c_x, cy=c_y): # 默认为TAG36H11
        img.draw_rectangle(tag.rect(), color = (255, 0, 0))
        img.draw_cross(tag.cx(), tag.cy(), color = (0, 255, 0))
        position_msg.append([tag.id(),tag.x_translation(), tag.y_translation(), tag.z_translation(), \
        degrees(tag.x_rotation()), degrees(tag.y_rotation()), degrees(tag.z_rotation())])
    img = None
    if position_msg == []:
        position_msg = [0]
    return position_msg

def min(pmin, num, a, s):
    if a<pmin:
        pmin=a
        num=s
    return [pmin,num]

def max(pmax, num, a, s):
    if a>pmax:
        pmax=a
        num=s
    return [pmax,num]

def match_keypoints():#image.get_similarity(image);
    sensor.reset()
    sensor.set_contrast(3)
    sensor.set_gainceiling(16)
    sensor.set_framesize(sensor.VGA)
    sensor.set_windowing((320, 240))
    sensor.set_pixformat(sensor.GRAYSCALE)

    sensor.skip_frames(10)
    sensor.set_auto_gain(False, value=100)
    clock = time.clock()
    NUM_SUBJECTS = 2 #识别物体数量
    NUM_SUBJECTS_IMGS = 5 #物体特征图像数量

    clock.tick()
    #img_rgb = sensor.snapshot() #RGB图像
    #img = img_rgb.to_grayscale() #灰度图像
    img = sensor.snapshot()
    kpts2 = img.find_keypoints(max_keypoints=150, threshold=10, normalized=True)
    img = None
    num = 0 #记录最相似的图片编号
    pmax = 0 #记录最大特征点数
    picture_msg = []#记录每张图片匹配到最大特征点数的子图片
    #position = [] #记录特征点位置
    #temp_correct = 0
    if(kpts2):
        for s in range(1, NUM_SUBJECTS+1):
            dist = 0 #记录每个图像的特征点数之和
            max_point = 0
            picture_num = 0

            for i in range(1, NUM_SUBJECTS_IMGS+1):
                kpts1 = image.load_descriptor("picture/s%d/k%d.orb"%(s, i))
                match = image.match_descriptor(kpts1, kpts2, threshold=85)
                kpts1 = None
                point_num = match.count()
                if (point_num>5):
                    dist += point_num
                    res = max(max_point, picture_num, point_num, i)
                    max_point = res[0]
                    picture_num = res[1]
            picture_msg.append(picture_num)
            res = max(pmax, num, dist/NUM_SUBJECTS_IMGS,s)
            pmax = res[0]
            num = res[1]
        if num > 0:
            return [num]
        else:
            return [0]

    else:
        return [0]

def match_faces():
    sensor.reset() # Initialize the camera sensor.
    sensor.set_pixformat(sensor.GRAYSCALE) # or sensor.GRAYSCALE
    sensor.set_framesize(sensor.B128X128) # or sensor.QQVGA (or others)
    sensor.set_windowing((92,112))
    sensor.skip_frames(10) # Let new settings take affect.

    NUM_SUBJECTS = 3 #图像库中不同人数，一共6人
    NUM_SUBJECTS_IMGS = 5 #每人有20张样本图片

    img = sensor.snapshot()
    d0 = img.find_lbp((0, 0, img.width(), img.height()))

    img = None
    pmin = 999999
    num=0

    for s in range(1, NUM_SUBJECTS+1):
        dist = 0
        for i in range(1, NUM_SUBJECTS_IMGS+1):
            img = image.Image("singtown/s%d/%d.pgm"%(s, i))
            d1 = img.find_lbp((0, 0, img.width(), img.height()))
            #d1为第s文件夹中的第i张图片的lbp特征
            dist += image.match_descriptor(d0, d1)#计算d0 d1即样本图像与被检测人脸的特征差异度。
            img = None
        res = min(pmin, num, dist/NUM_SUBJECTS_IMGS, s)#特征差异度越小，被检测人脸与此样本更相似更匹配。
        pmin = res[0]
        num = res[1]
    return [num]


if __name__ == '__main__':
    pin = pyb.Pin("P3", pyb.Pin.IN, pull=pyb.Pin.PULL_UP)
    image_msg = []
    while(1):
        AprilTag_msg = 0
        picture_msg = 0
        face_msg = 0
        image_msg.clear()
        while(1):
            if(pin.value() == 0):
                uart = UART(3, 115200, timeout_char=100)
                while(1):
                    buf = uart.read(6)
                    if buf is not None:
                        if buf[0] == 49 and buf[1] == 49 and buf[2] == 49:
                            break
                    if(pin.value() == 1):
                        buf = None
                        uart.deinit()
                        break
                break
            time.sleep(1000)
        print(buf)
        if buf == None:
            flag = 52
        else:
            flag = int(buf[3])
        flag -= 48
        image_msg = []#AprilTagID，AprilTagxyz距离，xyz旋转角度，匹配到的人脸编号，匹配到的图像编号
        if flag == 0:
            AprilTag_msg = find_AprilTag_position()
            image_msg.append(AprilTag_msg)
            gc.mem_free()
        if flag == 1:
            picture_msg = match_keypoints()
            image_msg.append(picture_msg)
            gc.mem_free()
        if flag == 2:
            face_msg = match_faces()
            image_msg.append(face_msg)
            gc.mem_free()
        if flag == 3:
            image_msg.append([1])
            gc.mem_free()
        if image_msg != []:
            data = str(image_msg[0])
            data = data+'\r\n'
            print(data)
            uart.write(data)
            time.sleep(1000)
            uart.deinit()
