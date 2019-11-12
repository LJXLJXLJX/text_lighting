#!/user/bin/env python
# coding=utf-8
'''
@project : my_text_renderer_lighting
@author  : LJX
@time   : 2019-06-03 14:30:07
'''

import numpy as np


class Vec3:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z

    def length(self):
        return (self.x ** 2 + self.y ** 2 + self.z ** 2) ** 0.5

    def normalized(self):
        return Vec3(self.x / self.length(), self.y / self.length(), self.z / self.length())

    def __add__(self, other):
        if type(other) == Vec3:
            return Vec3(self.x + other.x, self.y + other.y, self.z + other.z)
        if type(other) in [int, float]:
            return Vec3(self.x + other, self.y + other, self.z + other)

    def __radd__(self, other):
        if type(other) == Vec3:
            return Vec3(self.x + other.x, self.y + other.y, self.z + other.z)
        if type(other) in [int, float]:
            return Vec3(self.x + other, self.y + other, self.z + other)

    def __sub__(self, other):
        if type(other) == Vec3:
            return Vec3(self.x - other.x, self.y - other.y, self.z - other.z)
        if type(other) in [int, float]:
            return Vec3(self.x - other, self.y - other, self.z - other)

    def __rsub__(self, other):
        if type(other) == Vec3:
            return Vec3(self.x - other.x, self.y - other.y, self.z - other.z)
        if type(other) in [int, float]:
            return Vec3(self.x - other, self.y - other, self.z - other)

    def __mul__(self, other):
        if type(other) == Vec3:
            return Vec3(self.x * other.x, self.y * other.y, self.z * other.z)
        if type(other) in [int, float]:
            return Vec3(self.x * other, self.y * other, self.z * other)

    def __rmul__(self, other):
        if type(other) == Vec3:
            return Vec3(self.x * other.x, self.y * other.y, self.z * other.z)
        if type(other) in [int, float]:
            return Vec3(self.x * other, self.y * other, self.z * other)


# 绕法线旋转180度后取反方向
def reflect(v, n):
    return (v - 2 * dot(v, n) * n).normalizeVec3()


def dot(v1, v2):
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z


class Light:
    def __init__(self):
        self.position = Vec3(0, 0, 0)
        self.strength = 0.5  # 光线强度 0~1


class PointLight(Light):
    def __init__(self, strength=0.8, fall_of_start=10, fall_of_end=1000):
        super().__init__()
        self.fall_of_start = fall_of_start
        self.fall_of_end = fall_of_end
        self.strength = strength

    def calcAttenuation(self, distance, fall_off_start, fall_off_end):
        value = (fall_off_end - distance) / (fall_off_end - fall_off_start)
        return np.clip(value, 0, 1)


class Material:
    def __init__(self, diffuse=0.2, fresnelR0=0.1, shininess=0.3):
        self.diffuse = diffuse
        self.fresnelR0 = fresnelR0
        self.shininess = shininess  # 越小越粗糙


def computePointLight(L, mat, pos, normal, view_pos):
    def schlickFresnel(R0, normal, light_vec):
        cos_incident_angle = np.clip(dot(normal.normalized(), light_vec.normalized()), 0.0, 1.0)
        f0 = 1.0 - cos_incident_angle
        reflect_percent = R0 + (1.0 - R0) * f0 ** 5
        return reflect_percent

    def blinnPhong(light_strength, light_vec, normal, to_eye, mat):
        m = mat.shininess * 256.0
        half_vec = (to_eye + light_vec).normalized()
        roughness_factor = (m + 8.0) * max(dot(half_vec, normal), 0.0) ** m / 8
        fresnel_factor = schlickFresnel(mat.fresnelR0, half_vec, light_vec)
        spec_albedo = fresnel_factor * roughness_factor
        # LDR
        spec_albedo = spec_albedo / (spec_albedo + 1.0)
        return (mat.diffuse + spec_albedo) * light_strength

    light_vec = (L.position - pos)
    d = light_vec.length()
    if d > L.fall_of_end:
        return 0.0
    light_vec = light_vec.normalized()
    # 光照的角度也会影响强度
    ndotl = max(dot(light_vec, normal), 0.0)
    light_strength = L.strength * ndotl
    # 衰减
    att = L.calcAttenuation((pos - view_pos).length(), L.fall_of_start, L.fall_of_end)
    light_strength *= att

    to_eye = (view_pos - pos).normalized()
    return blinnPhong(light_strength, light_vec, normal, to_eye, mat)
