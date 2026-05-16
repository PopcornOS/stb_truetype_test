#include "pop_trig.h"

#define TAU 6.2831853f
#define PI  3.14159265

// Return the largest integer <= x
int _floor(float x) {
    int i = (int)x;          // truncate toward zero
    if (x < 0.0f && x != (float)i) {
        return i - 1;        // adjust for negatives
    }
    return i;
}

// Return the smallest integer >= x
int _ceil(float x) {
    int i = (int)x;          // truncate toward zero
    if (x > 0.0f && x != (float)i) {
        return i + 1;        // adjust for positives
    }
    return i;
}

// Power function
double _pow(double base, double exp) {
    if (exp == 0.0) return 1.0;
    int iexp = (int) exp;
    double result = 1.0;
    for (int i = 0; i < iexp; i++) {
        result *= base;
    }
    return result;
}

// Simple square root using Newton-Raphson
double _sqrt(double x) {
    if (x <= 0.0) return 0.0;

    double guess = x;
    for (int i = 0; i < 20; i++) {
        guess = 0.5 * (guess + x / guess);
    }
    return guess;
}

// Simple floating-point modulus
double _fmod(double x, double y) {
    if (y == 0.0) return 0.0; // crude guard against divide-by-zero
    int quotient = (int)(x / y); // truncates toward zero
    return x - (double)quotient * y;
}

float _cos(float x) {
    while (x < 0) x += TAU;
    while (x >= TAU) x -= TAU;

    int idx = (int)(x * (TABLE_SIZE / TAU));
    return (float)cos_table[idx] / AMPLITUDE;
}

float _asin(float y) {
    if (y > 1.0f) y = 1.0f;
    if (y < -1.0f) y = -1.0f;

    // Binary search in sin_table
    int lo = 0, hi = TABLE_SIZE - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        float val = (float)sin_table[mid] / AMPLITUDE;
        if (val < y) lo = mid + 1;
        else hi = mid - 1;
    }
    // Convert index back to angle
    return (float)lo * (TAU / TABLE_SIZE);
}

float _acos(float y) {
    return (PI / 2.0f) - _asin(y);
}

void* _memcpy(void* dst, const void *src, size_t n) {
    // convert to byte arrays
    unsigned char* cdst = (unsigned char*)dst;
    unsigned char* csrc = (unsigned char*)src;
    
    // copy
    for (size_t i = 0; i < n; i++) {
        cdst[i] = csrc[i];
    }
    
    return dst;
}

size_t _strlen(char* s) {
    size_t i = 0;
    while (s[i] != 0) { i++; }
    return i;
}

void* _memset(void* dst, int val, size_t n) {
    // convert to bytes
    unsigned char* cdst = (unsigned char*)dst;
    unsigned char cval = (unsigned char)val;
    
    // set
    for (size_t i = 0; i < n; i++) {
        cdst[i] = cval;
    }
    
    return dst;
}

void _assert(BOOL x) {
    if (!(x)) {
        svc->println(svc, L"assertion failed");
        *((volatile int*)0) = 0; // crash the system
    }
}

#define STBTT_ifloor(x)    _floor(x)
#define STBTT_iceil(x)     _ceil(x)
#define STBTT_sqrt(x)      _sqrt(x)
#define STBTT_pow(x, y)    _pow(x, y)
#define STBTT_fmod(x, y)   _fmod(x, y)
#define STBTT_cos(x)       _cos(x)
#define STBTT_acos(x)      _acos(x)
#define STBTT_fabs(x)      x < 0.0 ? -x : x
#define STBTT_malloc(x,u)  ((void)(u),svc->memalloc(svc, x))
#define STBTT_free(x,u)    ((void)(u),svc->memfree(svc, x))
#define STBTT_assert(x)    _assert(x)
#define STBTT_strlen(x)    _strlen(x)
#define STBTT_memcpy       _memcpy
#define STBTT_memset       _memset

#include "stb_truetype.h"