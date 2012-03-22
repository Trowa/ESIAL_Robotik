#include "EthernetNetIf.h"
#include "UDPSocket.h"
#include "ConsignController.h"

void udp_data_init(ConsignController* c , Odometrie* o);
void udp_data_add(char* name, double value);
void udp_data_send();
void udp_data_destroy();
void udp_data_load();
void udp_data_save();
