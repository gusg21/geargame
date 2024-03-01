#if !defined(GG_UTILS_H)
#define GG_UTILS_H

#ifdef APPLE
#define sprintf_s(a, b, c, ...) snprintf(a, b, c, __VA_ARGS__)
#define vsprintf_s(a, b, c, d) vsnprintf(a, b, c, d)
#endif

#endif // GG_UTILS_H
