#pragma once

#include <stdio.h>
#include <math.h>

typedef struct Vector2 {
	float x;
	float y;
} Vector2;
       
void PrintVector2(Vector2* v, const char* name) {
    printf("%s\t: [ %f %f ]\n", name, v->x, v->y);
}

typedef struct Vector3 {
	float x;
	float y;
	float z;
} Vector3;
       
void PrintVector3(Vector3* v, const char* name) {
    printf("%s\t: [ %f %f %f ]\n", name, v->x, v->y, v->z);
}

typedef struct Vector4 {
	float x;
	float y;
	float z;
	float w;
} Vector4;
       
void PrintVector4(Vector4* v, const char* name) {
    printf("%s\t: [ %f %f %f %f ]\n", name, v->x, v->y, v->z, v->w);
}

Vector2 Vector2Add(Vector2 v1, Vector2 v2) {
    Vector2 res = { v1.x + v2.x, v1.y + v2.y };
    return res;
}

Vector3 Vector3Add(Vector3 v1, Vector3 v2) {
    Vector3 res = { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
    return res;
}

Vector4 Vector4Add(Vector4 v1, Vector4 v2) {
    Vector4 res = { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w };
    return res;
}

Vector2 Vector2Scale(Vector2 v, float value) {
    Vector2 res = { v.x * value, v.y * value };
    return res;
}

Vector3 Vector3Scale(Vector3 v, float value) {
    Vector3 res = { v.x * value, v.y * value, v.z * value };
    return res;
}

Vector4 Vector4Scale(Vector4 v, float value) {
    Vector4 res = { v.x * value, v.y * value, v.z * value, v.w * value };
    return res;
}

float Vector2Dot(Vector2 v1, Vector2 v2) {
    float res = { v1.x * v2.x + v1.y * v2.y };
    return res;
}

float Vector3Dot(Vector3 v1, Vector3 v2) {
    float res = { v1.x * v2.x + v1.y * v2.y + v1.z * v2.z };
    return res;
}

float Vector4Dot(Vector4 v1, Vector4 v2) {
    float res = { v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w };
    return res;
}

