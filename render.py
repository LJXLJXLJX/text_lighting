#!/user/bin/env python
# coding=utf-8
'''
@project : my_text_renderer_lighting
@author  : LJX
@time   : 2019-06-03 14:44:30
'''

import cv2 as cv
import numpy as np
from PIL import Image, ImageDraw, ImageFont
from copy import deepcopy
from time import time
from lighting import *
import sys

epsilon = sys.float_info.epsilon


def cv2ImgAddText(img, text, pos, font_file_path, textColor=255, textSize=20):
    img = Image.fromarray(img)
    draw = ImageDraw.Draw(img)
    fontText = ImageFont.truetype(font_file_path, textSize, encoding="utf-8")
    draw.text(pos, text, textColor, font=fontText)
    return np.asarray(img)


# 使用光照方程渲染字符像素区域，背景必须为255 且不被渲染
def render(img,
           point_light,
           mat,
           is_text=True,
           view_hight=30.0,
           ambient_light=0.5):
    if img.dtype == np.uint8:
        img = img.astype(np.float32)
        img /= 255
    img.setflags(write=1)
    norm = Vec3(0, 0, 1)
    view_pos = Vec3(img.shape[0] // 2, img.shape[1] // 2, view_hight)
    for i in range(img.shape[0]):
        for j in range(img.shape[1]):
            # 首先忽略白色背景
            if is_text and abs(img[i][j] - 1.0) < epsilon:
                continue
            pix_pos = Vec3(i, j, 0)
            diffuse = mat.diffuse * img[i][j]
            ambient = ambient_light * diffuse
            tmp_mat = Material(diffuse, mat.fresnelR0, mat.shininess)
            light = computePointLight(point_light, tmp_mat, pix_pos, norm,
                                      view_pos)
            img[i][j] = ambient + light
    return img


def merge_text_and_bg(text_img, bg_img):
    bg_img = cv.resize(bg_img, (text_img.shape[1], text_img.shape[0]))
    ret = deepcopy(bg_img)
    for i in range(text_img.shape[0]):
        for j in range(text_img.shape[1]):
            if abs(text_img[i][j] - 1.0) < epsilon:
                continue
            ret[i][j] = text_img[i][j]
    return ret


if __name__ == '__main__':
    text = '喜欢唱跳rap篮球'
    white_bg = np.zeros((32, 280), dtype=np.uint8)
    white_bg += 255
    gray_bg = white_bg + 250
    # bg = cv.imread('./bg/bg.jpg', 0)
    # bg = cv.resize(bg, (280, 32))
    img = cv2ImgAddText(gray_bg, text, (10, 5),
                        'fonts/SourceHanSans-Normal.ttf', 50, 20)

    img = img.astype(np.float32)
    img /= 255
    cv.imshow('origin', img)
    cv.imwrite('ctrl.jpg', img * 255)

    pl = PointLight(0.5, 10, 500)
    pl.position = Vec3(40, 0, 100.0)
    mat_text = Material(0.8, 0.8, 0.3)
    mat_bg = Material(0.5, 0.5, 0.1)

    # img_text = render(img, pl, mat_text, view_hight=30.0, ambient_light=0.5)
    # img_bg = render(gray_bg, pl, mat_bg, False, 30, 0.7)
    #
    # img=merge_text_and_bg(img_text,img_bg)

    start = time()
    img = render(img, pl, mat_bg, False, 30, 0.5)
    print(time() - start)

    cv.imshow('render', img)
    cv.waitKey()
    cv.destroyAllWindows()
