#include "queue.h"
#include "skel.h"

/*
struct ether_header {
	u_char ether_dhost[6];
	u_char ether_shost[6];
	u_short ether_type;
}*/

// Static ARP Table
struct arp_entry *arp_table;
int arp_len;

// Routing table
struct route_table_entry *rtable;
int rtable_len;

//Next hop entry
struct route_table_entry next_hop;
uint32_t next_hop_ip;
int next_hop_interface;

// Auxiliary function for Longest Prefix Match, it returns the length of the mask in order to find the best match
int get_mask_type (uint32_t mask) {

	if(mask == 0x00000000)
		return 0;

	if(mask == 0xFF000000)
		return 1;

	if(mask == 0xFFFFF000)
		return 2;

	if(mask == 0xFFFFFF00)
		return 3;

	if(mask == 0xFFFFFFFF)
		return 4;

	return -1;
}

// Checks is a mac address is the broadcast address
int check_if_broadcast(uint8_t* mac) {
	int d = 1;
	for(int i = 0; i < 6 && d; i++) {
		if(*(mac + sizeof(uint8_t) * i) != 255)
			d = 0;
	}

	return d;
}

// Compares 2 mac addresses, returns 1 if they're the same
int compare_macs(uint8_t *mac1, uint8_t *mac2) {

	int d = 1;
	for (int i = 0; i < 6 && d; i++) {
		if(*(mac1 + sizeof(uint8_t) * i) != *(mac2 + sizeof(uint8_t) * i))
			d = 0;
	}

	return d;
}

// The compare function for the quicksort of the rtable needed for binary search
int comp_rtable(const void* a, const void* b) {
	struct route_table_entry* r1 = (struct route_table_entry*) a;
	struct route_table_entry* r2 = (struct route_table_entry*) b;

	if(ntohl(r1->prefix) == ntohl(r2->prefix))
		return (ntohl(r1->mask) - ntohl(r2->mask));
	
	return (ntohl(r1->prefix) - ntohl(r2->prefix));
}

// Longest Prefix Match implemented with binary search
void LPM (uint32_t dest_ip, int l, int r, uint32_t* k) {
	
	if (r > l) {
		int mid = l + (r - l) / 2;

		// Get the ip and masktype from mid
		uint32_t mid_ip = ntohl(rtable[mid].prefix);
		int mid_mask = ntohl(rtable[mid].mask);

		//printf("%u, %u, %u\n", mid_ip, dest_ip, mid_mask);
		//printf("%d, %d, %d", l, r, mid);
		
		
		// If the ip in the mid matches the one we're looking for (destinatination ip)
		if (mid_ip == (dest_ip & mid_mask)) {
			//printf("aici\n");
			
			// If its the first match we found or the found is better (more precise), we change the rtable_found variable
			if(*k == 0) {
				//printf("hey1\n");
				//next_hop = rtable[mid];
				//memcpy(&next_hop, &rtable[mid], sizeof(struct route_table_entry));
				next_hop_ip = ntohl(rtable[mid].next_hop);
				next_hop_interface = ntohl(rtable[mid].interface);
				*k = ntohl(rtable[mid].mask);
			} else {
				if(*k < mid_mask) {
					//printf("hey\n");
					next_hop_ip = ntohl(rtable[mid].next_hop);
					next_hop_interface = rtable[mid].interface;
					*k = ntohl(rtable[mid].mask);
				}
			}

			// If the match we found is actually the most precise, finish the search
			if(mid_mask == 4294967295) {
				return;
			} else {
			//Continue searching for a better match otherwise
				LPM(dest_ip, mid + 1, r, k);
			}
		}
		
		
		// Search is the first half
		if(mid_ip > (dest_ip & ntohl(rtable[mid].mask)))
			LPM(dest_ip, l, mid, k);

		
		// Search in the second half
		if(mid_ip < (dest_ip & ntohl(rtable[mid].mask)))
			LPM(dest_ip, mid + 1, r, k);	
	}
}

// Gets the mac address for the next hop using the static ARP table
void get_mac_for_next_hop (uint32_t next_hop_ip, uint8_t* mac) {

	for (int i = 0; i < arp_len; i++) {
		if (ntohl(arp_table[i].ip) == next_hop_ip) {
			memcpy(mac, arp_table[i].mac, 6 * sizeof(uint8_t));
		}
	}

}

int main(int argc, char *argv[])
{
	packet m;
	int rc;

	// Do not modify this line
	init(argc - 2, argv + 2);

	//Reading and initialising static ARP table because it seems to hard to implement the protocol :c
	arp_table = malloc(sizeof(struct arp_entry) * 6);
	DIE(arp_table == NULL, "memory");
	arp_len = parse_arp_table("arp_table.txt", arp_table);

	//Reading and initialising static Routing table
	rtable = malloc(sizeof(struct route_table_entry) * 100000);
	DIE(rtable == NULL, "memory");
	rtable_len = read_rtable(argv[1], rtable);

	///FILE* teste = fopen("teste_output.txt", "w");

	
	while (1) {
		rc = get_packet(&m);
		DIE(rc < 0, "get_packet");

		// Extract ethernet header
		if(m.len < sizeof(struct ether_header))
			continue;
		
		struct ether_header *eth_hdr = (struct ether_header *) m.payload;

		//Check MAC Destination
		uint8_t *packet_mac_dst = malloc(sizeof(uint8_t) * 6);
		get_interface_mac(m.interface, packet_mac_dst);
		
		/*
		for(int i = 0; i < 6; i++)
			printf("%u ", packet_mac_dst[i]);
		printf("\n");
		
		for(int i = 0; i < 6; i++)
			printf("%u ", eth_hdr->ether_dhost[i]);
		printf("\n");
		*/

		if(!compare_macs(eth_hdr->ether_dhost, packet_mac_dst) && !check_if_broadcast(eth_hdr->ether_dhost))
			continue;
			
		// The packet is neither IPv4 nor ARP
		if(ntohs(eth_hdr->ether_type) != 0x0800 && ntohs(eth_hdr->ether_type) != 0x806)
			continue;

		// IPv4 packet + forward starts here
		if(ntohs(eth_hdr->ether_type) == 0x0800) {

			
			
			// Extract IPv4 header
			if(m.len < sizeof(struct iphdr) + sizeof(struct ether_header))
				continue;

			struct iphdr *ip_header = (struct iphdr *) (m.payload + sizeof(struct ether_header));
			
			// Check if ICMP message
			
			if (ip_header->protocol == 1) {
				// TO DO ICMP
			} else {
				// Check the IPv4 checksum
				uint16_t header_checksum = ip_checksum((uint8_t *)ip_header, sizeof(struct iphdr));
				if (header_checksum != 0)
					continue;
				
				// Check if TTL is 1 or zero, if not just decrease it
				if(ip_header->ttl <= 1)
					continue;
				
				ip_header->ttl--;
				
				//Longest Prefix Match
				uint32_t k = 0;
				//printf("%u, %u\n\n", ntohl(ip_header->saddr), ntohl(ip_header->daddr));
				qsort(rtable, rtable_len, sizeof(struct route_table_entry), comp_rtable);
				LPM(ntohl(ip_header->daddr), 0, rtable_len - 1, &k);
				//printf("%u, %u, %d\n", ntohl(ip_header->daddr), ntohl(found_rtable->prefix), ntohl(found_rtable->mask));	

				
				if(k == 0) {
					//TO DO destination unreachable
				} else {
					
					// Prepare packet for forward
					
					// Change the source address to this router's address
					memcpy(eth_hdr->ether_shost, eth_hdr->ether_dhost, 6 * sizeof(uint8_t));
					
					/*
					for(int i = 0; i < 6; i++)
						printf("%u ", packet_mac_dst[i]);
					printf("\n");
					
					for(int i = 0; i < 6; i++)
						printf("%u ", eth_hdr->ether_shost[i]);
					printf("\n");
					*/

					
					// Change the dest address to next hop router's address
					uint8_t *next_hop_mac = malloc(sizeof(uint8_t));
					get_mac_for_next_hop(next_hop_ip, next_hop_mac);
					memcpy(eth_hdr->ether_dhost, next_hop_mac, 6 * sizeof(uint8_t));
					printf("%u, %u, %d", next_hop_ip, k, next_hop_interface);
					printf("\n%u\n", ntohl(ip_header->daddr));


					for(int i = 0; i < 6; i++)
						printf("%u ", next_hop_mac[i]);
					printf("\n");

					for(int i = 0; i < 6; i++)
						printf("%u ", eth_hdr->ether_dhost[i]);
					printf("\n");
					
					// Change IPv4 checksum
					ip_header->check = 0;
					header_checksum = ip_checksum((uint8_t*)ip_header, sizeof(struct iphdr));
					ip_header->check = header_checksum;

					// Change packet interface??
					m.interface = next_hop_interface;

					// Forward packet
					send_packet(&m);
				}
				
			}
			
		}

		if(ntohs(eth_hdr->ether_type) == 0x806) {
			//ARP
		}
	}
	
}
