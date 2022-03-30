#ifndef DEFINES_H
#define DEFINES_H

#include <cassert>

#define OD_Assert(...) assert(__VA_ARGS__)

#define OD_PRINT_INFO(...) printf(__VA_ARGS__)

#define OD_E        2.71828182845904523536   // e
#define OD_LOG2E    1.44269504088896340736   // log2(e)
#define OD_LOG10E   0.434294481903251827651  // log10(e)
#define OD_LN2      0.693147180559945309417  // ln(2)
#define OD_LN10     2.30258509299404568402   // ln(10)
#define OD_PI       3.14159265358979323846   // pi
#define OD_PI_2     1.57079632679489661923   // pi/2
#define OD_PI_4     0.785398163397448309616  // pi/4
#define OD_1_PI     0.318309886183790671538  // 1/pi
#define OD_2_PI     0.636619772367581343076  // 2/pi
#define OD_2_SQRTPI 1.12837916709551257390   // 2/sqrt(pi)
#define OD_SQRT2    1.41421356237309504880   // sqrt(2)
#define OD_SQRT1_2  0.707106781186547524401  // 1/sqrt(2)

#endif //DEFINES_H
