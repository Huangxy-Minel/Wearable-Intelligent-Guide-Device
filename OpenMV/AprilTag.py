import sensor, image, time, math
def FindAprilTag():
    img = sensor.snapshot()
    tag_msg = []
    for tag in img.find_apriltags(): # defaults to TAG36H11 without "families".
        img.draw_rectangle(tag.rect(), color = (255, 0, 0))
        img.draw_cross(tag.cx(), tag.cy(), color = (0, 255, 0))
        degress = 180 * tag.rotation() / math.pi
        tag_msg.append([tag.id(),degress])
    return tag_msg

def FindAprilTagPosition():
    f_x = (2.8 / 3.984) * 160 # 默认值
    f_y = (2.8 / 2.952) * 120 # 默认值
    c_x = 160 * 0.5 # 默认值(image.w * 0.5)
    c_y = 120 * 0.5 # 默认值(image.h * 0.5)
    img = sensor.snapshot()
    position_msg = []#tagid，xyz距离，xyz旋转角度
    for tag in img.find_apriltags(fx=f_x, fy=f_y, cx=c_x, cy=c_y): # 默认为TAG36H11
        img.draw_rectangle(tag.rect(), color = (255, 0, 0))
        img.draw_cross(tag.cx(), tag.cy(), color = (0, 255, 0))
        position_msg.append([tag.id(),tag.x_translation(), tag.y_translation(), tag.z_translation(), \
        degrees(tag.x_rotation()), degrees(tag.y_rotation()), degrees(tag.z_rotation())])
    return position_msg
