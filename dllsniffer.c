#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
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
    printf("\nSocket Created Successfully.Waiting for Packets...\n\n");
    
    while(1) 
    {
        addr_len = sizeof(struct sockaddr_ll);
        int data_size = recvfrom(sock_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&phy_addr, &addr_len);
        
        if(data_size < 0) 
        {
            printf("Error in Receiving Packet.\n");
            continue;
        }
        packet_num++;
        printf("Packet #%d:\n",packet_num);
        printf("Type: %s\n",get_packet_type(phy_addr.sll_pkttype));
        printf("Size: %d bytes\n",data_size);
        printf("Data:\n");
        print_packet_hex(buffer,data_size);
        printf("\n*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n\n");
    }
    close(sock_fd);
    return 0;
}