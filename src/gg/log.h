#if !defined(GG_LOG_H)
#define GG_LOG_H

void Log_Info(const char* info);
void Log_Warn(const char* warning);
void Log_Err(const char* error);
char* Log_TextFormat(const char* format, ...);

#endif  // GG_LOG_H
