#pragma once
#define COM_SOCKET          (0)
#define COM_WINSOCK         (1)
#define COM_MACSOCK         (2)
#define COM_UNKNONW         (-1)

#if defined(_MSC_VER)
#define COM_TYPE            COM_WINSOCK         /* Visual Studio : WinSock */
#elif defined(__MACH__)
#define COM_TYPE            COM_MACSOCK         /* Mac : Mac Socket */
#elif defined(__linux__)
#define COM_TYPE            COM_SOCKET          /* Linux : Socket */
#else
#define COM_TYPE            COM_UNKNONW         /* Other : Not Support */
#endif

