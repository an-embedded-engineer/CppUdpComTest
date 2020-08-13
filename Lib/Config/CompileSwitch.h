#pragma once
#define COM_SOCKET          (0)
#define COM_WINSOCK         (1)
#define COM_UNKNONW         (-1)

#if defined(_MSC_VER)
#define COM_TYPE            COM_WINSOCK
#elif defined(__unix__) || defined(__linux__) || defined(__MACH__)
#define COM_TYPE            COM_SOCKET
#else
#define COM_TYPE            COM_UNKNONW
#endif
