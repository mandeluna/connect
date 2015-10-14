/*  getarp.c -- This simple program uses an IOCTL socket call to read an entry  */
/*  from the ARP cache of the local machine.  Original source unknown.  */
/*  Usage:  getarp <ip-number>   */
/*  Example:  getarp  148.85.2.1 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/*  arp_flags and at_flags field values */
#define	ATF_INUSE	0x01	/* entry in use */
#define ATF_COM		0x02	/* completed entry (enaddr valid) */
#define	ATF_PERM	0x04	/* permanent entry */
#define	ATF_PUBL	0x08	/* publish entry (respond for other host) */
#define	ATF_USETRAILERS	0x10	/* has requested trailers */
#define	ATF_PROXY	0x20	/* Do PROXY arp */

main (int argc, char *argv[]) {

    int fd;

    union
    {
	struct sockaddr_in *psa;
	struct sockaddr    *pga;
    } ua;

    unsigned char *eap;
    struct ifreq ifreq;

    if (argc < 2) exit(0);

    memset(&ifreq, 0, sizeof(struct ifreq));

    ua.pga = &(ifreq.ifr_addr);
    ua.psa->sin_family = AF_INET;
    ua.psa->sin_addr.s_addr = inet_addr(argv[1]);
    // printf("Ethernet address (before): %s\n", inet_ntoa(ua.psa->sin_addr));

    fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (fd < 0) {
    	perror("socket");
    	exit(0);
    }

    strcpy(ifreq.ifr_name, "eth0");

    if (ioctl(fd, SIOCGIFHWADDR, &ifreq) < 0) {
    	perror("ioctl");
    	exit(0);
    }
    printf("IP address:       %s\n", inet_ntoa(ua.psa->sin_addr));

    eap = (unsigned char *) &(ifreq.ifr_hwaddr.sa_data[0]);
    printf("Ethernet address: %02X:%02X:%02X:%02X:%02X:%02X",
       	eap[0], eap[1], eap[2], eap[3], eap[4], eap[5]);
    printf("\n");

    close(fd);
    exit(0);
}
