import sensor, image, time, math, pyb
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
            print(dist)
            picture_msg.append(picture_num)
            res = max(pmax, num, dist/NUM_SUBJECTS_IMGS,s)
            pmax = res[0]
            num = res[1]
        if num > 0:
            return num
        else:
            return 0

    else:
        return 0

def save_rbg_img():
    RED_LED_PIN = 1
    BLUE_LED_PIN = 3
    sensor.reset()
    sensor.set_pixformat(sensor.GRAYSCALE)
    sensor.set_framesize(sensor.VGA)
    sensor.set_windowing((320, 240))
    sensor.skip_frames(10)
    sensor.skip_frames(time = 2000)
    num = 25 #设置被拍摄者序号，第一个人的图片保存到s1文件夹，第二个人的图片保存到s2文件夹，以此类推。每次更换拍摄者时，修改num值。

    n = 5 #设置每个人拍摄图片数量。

    #连续拍摄n张照片，每间隔3s拍摄一次。
    while(n):
        #红灯亮
        pyb.LED(RED_LED_PIN).on()
        sensor.skip_frames(time = 3000) # Give the user time to get ready.等待3s，准备一下表情。

        #红灯灭，蓝灯亮
        pyb.LED(RED_LED_PIN).off()
        pyb.LED(BLUE_LED_PIN).on()

        #保存截取到的图片到SD卡
        print(n)
        #img_rgb = sensor.snapshot()
        #img_rgb.save("picture/s%s/%s.bmp" % (num, n) ) # or "example.bmp" (or others)
        #img = img_rgb.to_grayscale()
        #img_rgb = None
        img = sensor.snapshot()
        kpst = img.find_keypoints(max_keypoints=150, threshold=10, normalized=True, scale_factor=1.35)
        img = None
        image.save_descriptor(kpst,"picture/s%s/k%s.orb" % (num, n) )
        kpst = None
        print('ok')
        n -= 1
        pyb.LED(BLUE_LED_PIN).off()
        print("Done! Reset the camera to see the saved image.")

save_rbg_img()
#num = match_keypoints()
