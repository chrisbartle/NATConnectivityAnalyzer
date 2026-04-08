#ifndef STUNOS_H
#define STUNOS_H

#ifdef _WIN32
#include <windows.h>
#include <winsock2.h>
//#include <psapi.h>
#else
#include <time.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
//memcopy_s isn't available for Linux, create a wrapper for memcpy
inline int memcpy_s(void* dest, size_t destsz, const void* src, size_t count) {
    if (!dest || !src) return EINVAL;
    if (count > destsz) return ERANGE;
    memcpy(dest, src, count);
    return 0;
}
//Ditto with strncpy_s
inline int strncpy_s(char* dest, size_t destsz,
                     const char* src, size_t count)
{
    if (!dest || !src) return EINVAL;
    if (destsz == 0) return ERANGE;

    size_t toCopy = (count < destsz - 1) ? count : destsz - 1;

    std::memcpy(dest, src, toCopy);
    dest[toCopy] = '\0';

    if (count >= destsz) {
        return ERANGE; // truncation occurred
    }

    return 0;
}
//In Windows SOCKET is a special type, for Linux it's just an int
typedef int SOCKET;
//Mapping Windows terms back to Linux
typedef ushort USHORT;
#define _strdup strdup
#define SOCKET_ERROR -1
#define SOCKADDR sockaddr

//SYSTEMTIME not available in Linux
struct SYSTEMTIME {
    unsigned short wYear;
    unsigned short wMonth;
    unsigned short wDayOfWeek;
    unsigned short wDay;
    unsigned short wHour;
    unsigned short wMinute;
    unsigned short wSecond;
    unsigned short wMilliseconds;
};

//Nothing to return for this diagnostic function outside of Windows
inline int WSAGetLastError() {return 0;}
#endif

//Time function that will work in both Linux and Windows
inline void StunGetSystemTime(SYSTEMTIME* st) {
#ifdef _WIN32
    // Use the native Windows API
    GetSystemTime(st);
#else
    // Use POSIX for Linux/Unix
    struct timeval tv;
    struct tm tm_info;

    gettimeofday(&tv, NULL);
    gmtime_r(&tv.tv_sec, &tm_info);

    st->wYear         = tm_info.tm_year + 1900;
    st->wMonth        = tm_info.tm_mon + 1; // tm_mon is 0-11
    st->wDayOfWeek    = tm_info.tm_wday;
    st->wDay          = tm_info.tm_mday;
    st->wHour         = tm_info.tm_hour;
    st->wMinute       = tm_info.tm_min;
    st->wSecond       = tm_info.tm_sec;
    st->wMilliseconds = static_cast<unsigned short>(tv.tv_usec / 1000);
#endif
}

#endif // STUNOS_H
