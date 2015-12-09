/* future_prod.c */

#include <future.h>

#include <string.h>
#include <stdio.h>
/*------------------------------------------------------------------------
 * Future Producer
 *------------------------------------------------------------------------
 */
typedef unsigned int uint;

uint future_prod(future *fut) 
{
    
    int i, j;
    j = (int)fut;
    
    j = echo_client();
    future_set(fut, &j);
    return OK;
}

int echo_client()
{
	int	i;			/* index into buffer		*/
	int	retval;			/* return value			*/
	char	msg[] = "Message received.."; /* message to send	*/
	char	inbuf[1500];		/* buffer for incoming reply	*/
	int32	slot;			/* UDP slot to use		*/
	int32	msglen;			/* length of outgoing message	*/
	uint32	remoteip;		/* remote IP address to use	*/
	uint16	echoport= 8888;		/* port number for UDP echo	*/
	uint16	locport	= 52743;	/* local port to use		*/
	int32	retries	= 3;		/* number of retries		*/
	int32	delay	= 2000;		/* reception delay in ms	*/
	char args[] = "192.168.1.100";
	char prog[] = "udpreceiver";

	/* For argument '--help', emit help about the 'udptransfer' command	*/
	
	udp_init();

	/*if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Use: %s  REMOTEIP\n\n", args[0]);
		printf("Description:\n");
		printf("\tBounce a message off a remote UDP Linux machine\n");
		printf("Options:\n");
		printf("\tREMOTEIP:\tIP address in dotted decimal\n");
		printf("\t--help\t display this help and exit\n");
		return 0;
	}*/

	/* Check for valid IP address argument */

	/*if (nargs != 2) {
		fprintf(stderr, "%s: invalid argument(s)\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
				args[0]);
		return 1;
	}*/

	if (dot2ip(args, &remoteip) == SYSERR) {
		fprintf(stderr, "%s: invalid IP address argument\r\n",
			prog);
		return 1;
	}

	

	/* register local UDP port */

	slot = udp_register(remoteip, echoport, locport);
	if (slot == SYSERR) {
		fprintf(stderr, "%s: could not reserve UDP port %d\n",
				prog, locport);
		return 1;
	}

	/* Retry sending outgoing datagram and getting response */

	msglen = strnlen(msg, 1200);
	for (i=0; i<retries; i++) {
		retval = udp_send(slot, msg, msglen);
		if (retval == SYSERR) {
			fprintf(stderr, "%s: error sending UDP \n",
				prog);
			return 1;
		}

		retval = udp_recv(slot, inbuf, sizeof(inbuf), delay);
		if (retval == TIMEOUT) {
			fprintf(stderr, "%s: timeout...\n", prog);
			continue;
		} else if (retval == SYSERR) {
			fprintf(stderr, "%s: error from udp_recv \n",
				prog);
			udp_release(slot);
			return 1;
		}
		break;
	}

	udp_release(slot);
	if (retval == TIMEOUT) {
		fprintf(stderr, "%s: retry limit exceeded\n",
			prog);
		return 1;
	}

	/* Response received - check contents */

	if (retval != msglen) {
		fprintf(stderr, "%s: sent %d bytes and received %d\n",
			prog, msglen, retval);
		return 1;
	}
	for (i = 0; i < msglen; i++) {
		if (msg[i] != inbuf[i]) {
			fprintf(stderr, "%s: reply differs at byte %d\n",
				prog, i);
			return 1;
		}
	}
	int msgnum = (int)msg;
	kprintf("\n %s \n",inbuf);
	//printf("Message passed to Linux..\n");
	return msgnum;
}
