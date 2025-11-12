#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <errno.h> 

struct s_ip {
	char *ip;
	int cidr;
} t_ip;

char *ft_strsep(char **stringp, const char *delim) {
    char *s;
    char *token;
    
    if (stringp == NULL || *stringp == NULL) {
        return NULL;
    }

    s = *stringp;
    token = s;

    char *delimiter_found = strchr(s, *delim);

    while (*s != '\0' && strchr(delim, *s) == NULL) {
        s++;
    }

    if (*s != '\0') {
        *s = '\0';
        *stringp = s + 1; 
    } else {
        *stringp = NULL;
    }

    return token; 
}

struct s_ip *getIpPrefix(char *ip) {
	char *token, *string, *tofree;
	struct s_ip *i = malloc(sizeof(struct s_ip));

	
	
	tofree = string = strdup(ip);

	if ((token = ft_strsep(&string, "/")) != NULL) {
		i->ip = malloc(sizeof(char) * strlen(token) + 1);
		strcpy(i->ip, token);
	}
	if ((token = ft_strsep(&string, "/")) != NULL) {
		i->cidr = atoi(token);
		if (i->cidr <= 0 || i->cidr > 32) {
			perror("Illegal prefix value, must be 0 < cidr <= 32");
			free(i->ip);
			free(i);
			exit(10);
		}
	}

	free(tofree);
	return i;
}

unsigned int getMask(int cidr) {
	unsigned int mask = 0;
	int numberOfIterations = 32 - cidr;

	for (int i = 31; i >= numberOfIterations; i--) {
		mask = mask | 1 << i;
	}
	return mask;
}

unsigned int ipToDecimal(char *ip) {
	char *token, *string, *tofree;
	unsigned int i = 0;
	unsigned int counter = 3;


	tofree = string = strdup(ip);

	while ((token = ft_strsep(&string, ".")) != NULL) {
		i = i + (atoi(token) * (unsigned int)pow(256, counter));
		counter--;
	}
	
	free(tofree);

	return i;
}

void printIp(unsigned int ip) {
	unsigned int p = 0;
	unsigned int t = 7;

	for (int i = 24; i >= 0; i = i - 8) {
		if (i > 0)
			printf("%u.", (ip >> i) & 0xFF);
		else
			printf("%u", (ip >> i) & 0xFF);
	}
	printf("\n");
}

int main(int argc, char **argv) {
	unsigned int ip = 0;
	unsigned int mask = 0;
	unsigned int network_ip = 0;
	unsigned int broadcast = 0;
	double nbr_hosts = 0;
	struct s_ip *nip;

	if (argc != 2) {
		printf("You must provide 1 argument\nFor example: 192.168.1.1/20\n");
		return -1;
	}

	nip = getIpPrefix(argv[1]);


	ip = ipToDecimal(nip->ip);
	mask = getMask(nip->cidr);
	broadcast = ip | ~mask;

	network_ip = ip & mask;

	nbr_hosts = pow(2, 32 - nip->cidr);

	printf("Network Address (NetID) : ");
	printIp(network_ip);

	printf("Mask of network : ");
	printIp(mask);
	
	
	printf("Broadcast Address : ");
	printIp(broadcast);
	
	printf("Min Host : ");
	printIp(network_ip | 0b1);

	printf("Max Host : ");
 	printIp(broadcast & (UINT_MAX << 1));

	printf("Total Number of hosts : %.0f\n", nbr_hosts);

	printf("Number of usable hosts : %.0f\n", nbr_hosts - 2);
	
	free(nip->ip);
	free(nip);

	return 0;
}
