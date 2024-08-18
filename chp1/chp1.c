#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************************************************************
* From Chapter 1
* 
* Simple program that will list the different interfaces on the machine which is returned by the call to getifaddrs()
*
* Internals called
*   - int getifaddrs(struct ifaddrs **ifap) : creates a linked list of structures describing the network interfaces of the local 
*   system, and stores the address of the first item of the list in *ifap
*
******************************************************************************************************************************/

int main() {

    struct ifaddrs *interfaces;
    
    // call to getifaddrs returns -1 if failed
    if (getifaddrs(&interfaces) == -1) {
        printf("getifaddrs call failed\n");
        return -1;
    }


    struct ifaddrs *interface = interfaces;

    while(interface) {

        // check if the address of the current interface is null,
        // if so move to the next interface
        if (interface->ifa_addr == NULL) { 
            interface = interface->ifa_next;
            continue;
        }

        // sa_family subfield used to determine the format of the address structure.
        int family = interface->ifa_addr->sa_family;
        if (family == AF_INET || family == AF_INET6) {

            printf("%s\t", interface->ifa_name);
            printf("%s\t", family == AF_INET ? "IPv4" : "IPv6");

            char ap[100];
            const int family_size = family == AF_INET ?
                sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
            getnameinfo(interface->ifa_addr,
                    family_size, ap, sizeof(ap), 0, 0, NI_NUMERICHOST);
            printf("\t%s\n", ap);

        }

        //move to the next interface
        interface = interface->ifa_next;
    }


    freeifaddrs(interfaces);
    return 0;
}