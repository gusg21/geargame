#if !defined(GG_UTILS_H)
#define GG_UTILS_H

#ifdef APPLE
#define sprintf_s(a, b, c, d) sprintf(a, c, d)
#define vsprintf_s(a, b, c, d) vsprintf(a, c, d)
#endif

#endif // GG_UTILS_H
