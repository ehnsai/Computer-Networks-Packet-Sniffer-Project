#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>

void print_packet_hex(unsigned char *buffer, int len) 
{
    for(int i = 0; i < len; i++) 
    {
        printf("%02x", buffer[i]);
        if((i + 1) % 16 == 0)
        {
            printf("\n");
        }
        else
        {
            printf(" ");
        }
    }
    printf("\n");
}

char* get_packet_type(int pkt_type) 
{
    switch(pkt_type) 
    {
        case PACKET_OUTGOING:
            return "Outgoing";
        case PACKET_HOST:
            return "Incoming";
        case PACKET_BROADCAST:
            return "Broadcast";
        case PACKET_MULTICAST:
            return "Multicast";
        default:
            return "unknown";
    }
}

char* get_protocol_name(unsigned short proto) 
{
    switch(ntohs(proto)) 
    {
        case ETH_P_IP:
            return "IP";
        case ETH_P_ARP:
            return "ARP";
        case ETH_P_IPV6:
            return "IPv6";
        default:
            return "Other";
    }
}

int main() 
{
    int sock_fd;
    unsigned char buffer[2048];
    struct sockaddr_ll phy_addr;
    int addr_len;
    int packet_num = 0;
    
    sock_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if(sock_fd < 0)
     {
        printf("Error in Creating Socket.\n");
        return 1;
    }
    printf("\nSocket Created.Waiting for Packets...\n\n");
    
    while(1) 
    {
        addr_len = sizeof(struct sockaddr_ll);
        int data_size = recvfrom(sock_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&phy_addr, &addr_len);
        
        if(data_size < 0) 
        {
            printf("Error in Receiving Packet.\n");
            continue;
        }
        struct ethhdr *eth_header = (struct ethhdr *)buffer;
        packet_num++;
        printf("Packet #%d:\n",packet_num);
        printf("Type: %s\n",get_packet_type(phy_addr.sll_pkttype));
        printf("Upper Protocol: %s\n",get_protocol_name(eth_header->h_proto));
        printf("Size: %d bytes\n",data_size);
        printf("Data:\n");
        print_packet_hex(buffer,data_size);
        printf("\n*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n\n");
    }
    close(sock_fd);
    return 0;
}
