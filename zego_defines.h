#pragma once

#define ZEGOAVKIT_API __attribute__((visibility("default")))

extern ZEGOAVKIT_API void ZegoExternalLogWithNotice(const char* content);

inline void log_string_notice(const char* content) { ZegoExternalLogWithNotice(content);}
