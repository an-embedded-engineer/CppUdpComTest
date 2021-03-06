﻿#pragma once
#define COM_SOCKET                      (0)
#define COM_WINSOCK                     (1)
#define COM_MACSOCK                     (2)
#define COM_UNKNONW                     (-1)

#if defined(_MSC_VER)
#define COM_TYPE_BASE                   COM_WINSOCK         /* Visual Studio : WinSock */
#elif defined(__MACH__)
#define COM_TYPE_BASE                   COM_MACSOCK         /* Mac : Mac Socket */
#elif defined(__linux__)
#define COM_TYPE_BASE                   COM_SOCKET          /* Linux : Socket */
#else
#define COM_TYPE_BASE                   COM_UNKNONW         /* Other : Not Support */
#endif

#define COM_TYPE_OVERRIDE_ENABLED       (0)
#define COM_TYPE_OVERRIDE               (COM_MACSOCK)

#if COM_TYPE_OVERRIDE_ENABLED == 1
#define COM_TYPE                        (COM_TYPE_OVERRIDE)
#else
#define COM_TYPE                        (COM_TYPE_BASE)
#endif

#if defined(_MSC_VER) && COM_TYPE != COM_WINSOCK
#define SOCKET_INTERFACE_STUB_ENABLED   (1)
#else
#define SOCKET_INTERFACE_STUB_ENABLED   (0)
#endif



#define TARGET_MSVC                     (0)
#define TARGET_MAC                      (1)
#define TARGET_LINUX                    (2)
#define TARGET_UNKNOWN                  (-1)

#if defined(_MSC_VER)
#define TARGET_TYPE                     TARGET_MSVC         /* Visual Studio */
#elif defined(__MACH__)
#define TARGET_TYPE                     TARGET_MAC          /* Mac */
#elif defined(__linux__)
#define TARGET_TYPE                     TARGET_LINUX        /* Linux */
#else
#define TARGET_TYPE                     TARGET_UNKNOWN      /* Other */
#endif


#define TEST_COM_MODE_UNICAST           (0)
#define TEST_COM_MODE_MULTICAST         (1)
#define TEST_COM_MODE_BROADCAST         (2)

#define TEST_COM_MODE                   TEST_COM_MODE_UNICAST


#define TEST_MODE_SCOKET_ADAPTER        (0)
#define TEST_MODE_UDP_SINGLE            (1)
#define TEST_MODE_UDP_MULTI             (2)

#define TEST_MODE                       TEST_MODE_UDP_MULTI

#define LOCAL_IP                        "127.0.0.1"
#define SENDER_IP                       "192.168.1.10"
#define RECEIVER_IP                     "192.168.1.11"

#define UNICAST_IP                      LOCAL_IP
#define MULTICAST_IP                    "224.0.0.32"
#define BROADCAST_IP                    "255.255.255.255"

#define UDP_PORT                        50000

