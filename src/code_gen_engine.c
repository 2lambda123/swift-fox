/** \file
    Swift Fox Compiler
    Copyright (C) 2009-2012 Marcin Szczodrak

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "code_gen.h"
#include "utils.h"

void generateFennecEngineC() {

	char *full_path = get_sfc_path("", "FennecEngineC.nc");
	FILE *fp = fopen(full_path, "w");

	struct modtab *mp;

	if (fp == NULL) {
		fprintf(stderr, "You do not have a permission to write into file: %s\n", full_path);
		exit(1);
	}


	fprintf(fp, "\n#include <Fennec.h>\n\n");
	fprintf(fp, "configuration FennecEngineC {\n");
	fprintf(fp, "provides interface ModuleCtrl;\n");
	fprintf(fp, "}\n\n");
	fprintf(fp, "implementation {\n\n");
	fprintf(fp, "components FennecEngineP;\n");
	fprintf(fp, "ModuleCtrl = FennecEngineP.ModuleCtrl;\n");

	fprintf(fp, "components LedsC;\n");
  	fprintf(fp, "FennecEngineP.Leds -> LedsC;\n\n");

	fprintf(fp, "components CachesC;\n");
  	fprintf(fp, "FennecEngineP.Fennec -> CachesC;\n\n");

  	fprintf(fp, "/* Defined and linked applications */\n\n");

  	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
    		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && ((mp->lib->type == TYPE_APPLICATION) || (mp->lib->type == TYPE_EVENT))) {
      			fprintf(fp, "components %sC as %s;\n",
						mp->lib->full_name,
						mp->lib->full_name);
      			fprintf(fp, "FennecEngineP.%sControl -> %s;\n\n",
						mp->lib->full_name,
						mp->lib->full_name);
      			fprintf(fp, "FennecEngineP.%sParams <- %s.%sParams;\n\n", 
						mp->lib->full_name, 
						mp->lib->full_name, 
						mp->lib->full_name);
     			fprintf(fp, "FennecEngineP.%sNetworkAMSend <- %s.NetworkAMSend;\n",
						mp->lib->full_name,
						mp->lib->full_name);
      			fprintf(fp, "FennecEngineP.%sNetworkReceive <- %s.NetworkReceive;\n",
						mp->lib->full_name,
						mp->lib->full_name);
      			fprintf(fp, "FennecEngineP.%sNetworkSnoop <- %s.NetworkSnoop;\n",
						mp->lib->full_name,
						mp->lib->full_name);
      			fprintf(fp, "FennecEngineP.%sNetworkPacket <- %s.NetworkPacket;\n",
						mp->lib->full_name,
						mp->lib->full_name);
      			fprintf(fp, "FennecEngineP.%sNetworkAMPacket <- %s.NetworkAMPacket;\n",
						mp->lib->full_name,
						mp->lib->full_name);
      			fprintf(fp, "FennecEngineP.%sNetworkPacketAcknowledgements <- %s.NetworkPacketAcknowledgements;\n",
						mp->lib->full_name, 
						mp->lib->full_name);
			if (mp->lib->type == TYPE_EVENT) {
				fprintf(fp, "FennecEngineP.%sEvent -> %s.Event;\n",
							mp->lib->full_name,
							mp->lib->full_name);
			}
			fprintf(fp, "\n");
    		}
  	}

  	fprintf(fp, "/* Defined and linked network modules */\n\n");

  	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
    		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_NETWORK) {
      			fprintf(fp, "components %sC as %s;\n",
						mp->lib->full_name,
						mp->lib->full_name);
      			fprintf(fp, "FennecEngineP.%sControl -> %s;\n\n",
						mp->lib->full_name,
						mp->lib->full_name); 
      			fprintf(fp, "FennecEngineP.%sParams <- %s.%sParams;\n\n", 
						mp->lib->full_name, 
						mp->lib->full_name, 
						mp->lib->full_name);
      			fprintf(fp, "FennecEngineP.%sNetworkAMSend -> %s.NetworkAMSend;\n",
						mp->lib->full_name, 
						mp->lib->full_name);
      			fprintf(fp, "FennecEngineP.%sNetworkReceive -> %s.NetworkReceive;\n",
						mp->lib->full_name,
						mp->lib->full_name);
      			fprintf(fp, "FennecEngineP.%sNetworkSnoop -> %s.NetworkSnoop;\n",
						mp->lib->full_name,
						mp->lib->full_name);
      			fprintf(fp, "FennecEngineP.%sNetworkAMPacket -> %s.NetworkAMPacket;\n",
						mp->lib->full_name,
						mp->lib->full_name);
      			fprintf(fp, "FennecEngineP.%sNetworkPacket -> %s.NetworkPacket;\n",
						mp->lib->full_name,
						mp->lib->full_name);
      			fprintf(fp, "FennecEngineP.%sNetworkPacketAcknowledgements -> %s.NetworkPacketAcknowledgements;\n",
						mp->lib->full_name,
						mp->lib->full_name);
      			fprintf(fp, "FennecEngineP.%sMacAMSend <- %s.MacAMSend;\n",
						mp->lib->full_name,
						mp->lib->full_name);
      			fprintf(fp, "FennecEngineP.%sMacReceive <- %s.MacReceive;\n",
						mp->lib->full_name, 
						mp->lib->full_name);
      			fprintf(fp, "FennecEngineP.%sMacSnoop <- %s.MacSnoop;\n",
						mp->lib->full_name,
						mp->lib->full_name);
      			fprintf(fp, "FennecEngineP.%sMacAMPacket <- %s.MacAMPacket;\n",
						mp->lib->full_name,
						mp->lib->full_name);
      			fprintf(fp, "FennecEngineP.%sMacPacket <- %s.MacPacket;\n",
						mp->lib->full_name,
						mp->lib->full_name);
      			fprintf(fp, "FennecEngineP.%sMacPacketAcknowledgements <- %s.MacPacketAcknowledgements;\n",
						mp->lib->full_name,
						mp->lib->full_name);
			fprintf(fp, "\n");
    		}
  	}

	fprintf(fp, "/* Defined and linked mac */\n\n");

	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp, "components %sC as %s;\n",
						mp->lib->full_name,
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sControl -> %s;\n",
						mp->lib->full_name,
						mp->lib->full_name);
      			fprintf(fp, "FennecEngineP.%sParams <- %s.%sParams;\n\n", 
						mp->lib->full_name, 
						mp->lib->full_name, 
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sMacAMSend -> %s.MacAMSend;\n",
						mp->lib->full_name,
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sMacReceive -> %s.MacReceive;\n",
						mp->lib->full_name,
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sMacSnoop -> %s.MacSnoop;\n",
						mp->lib->full_name,
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sMacPacket -> %s.MacPacket;\n",
						mp->lib->full_name,
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sMacAMPacket -> %s.MacAMPacket;\n",
						mp->lib->full_name,
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sMacPacketAcknowledgements -> %s.MacPacketAcknowledgements;\n",
						mp->lib->full_name,
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sRadioReceive <- %s.RadioReceive;\n",
						mp->lib->full_name,
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sRadioResource <- %s.RadioResource;\n", 
						mp->lib->full_name, 
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sRadioConfig <- %s.RadioConfig;\n", 
						mp->lib->full_name, 
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sRadioPower <- %s.RadioPower;\n", 
						mp->lib->full_name, 
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sReadRssi <- %s.ReadRssi;\n", 
						mp->lib->full_name, 
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sRadioBuffer <- %s.RadioBuffer;\n", 
						mp->lib->full_name, 
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sRadioSend <- %s.RadioSend;\n", 
						mp->lib->full_name, 
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sRadioPacket <- %s.RadioPacket;\n", 
						mp->lib->full_name, 
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sPacketIndicator <- %s.PacketIndicator;\n", 
						mp->lib->full_name, 
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sEnergyIndicator <- %s.EnergyIndicator;\n", 
						mp->lib->full_name, 
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sByteIndicator <- %s.ByteIndicator;\n", 
						mp->lib->full_name, 
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sRadioControl <- %s.RadioControl;\n", 
						mp->lib->full_name, 
						mp->lib->full_name);
			fprintf(fp, "\n");
		}
	}

	fprintf(fp, "/* Defined and linked radios */\n\n");

	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp, "components %sC as %s;\n", 
						mp->lib->full_name, 
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sControl -> %s;\n", 
						mp->lib->full_name, 
						mp->lib->full_name);
      			fprintf(fp, "FennecEngineP.%sParams <- %s.%sParams;\n\n", 
						mp->lib->full_name, 
						mp->lib->full_name, 
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sRadioReceive -> %s.RadioReceive;\n", 
						mp->lib->full_name, 
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sRadioResource -> %s.RadioResource;\n", 
						mp->lib->full_name, 
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sRadioConfig -> %s.RadioConfig;\n", 
						mp->lib->full_name, 
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sRadioPower -> %s.RadioPower;\n", 
						mp->lib->full_name, 
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sReadRssi -> %s.ReadRssi;\n", 
						mp->lib->full_name, 
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sRadioBuffer -> %s.RadioBuffer;\n", 
						mp->lib->full_name, 
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sRadioSend -> %s.RadioSend;\n", 
						mp->lib->full_name, 
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sRadioPacket -> %s.RadioPacket;\n", 
						mp->lib->full_name, 
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sPacketIndicator -> %s.PacketIndicator;\n", 
						mp->lib->full_name, 
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sEnergyIndicator -> %s.EnergyIndicator;\n", 
						mp->lib->full_name, 
						mp->lib->full_name);
			fprintf(fp, "FennecEngineP.%sByteIndicator -> %s.ByteIndicator;\n", 
						mp->lib->full_name, 
						mp->lib->full_name);
			fprintf(fp, "\n");
		}
	}

	fprintf(fp, "\n}\n");
	fclose(fp);
	free(full_path);
}

void generateFennecEngineP() {

        char *full_path = get_sfc_path("", "FennecEngineP.nc");
        FILE *fp = fopen(full_path, "w");

        struct modtab *mp;

        if (fp == NULL) {
                fprintf(stderr, "You do not have a permission to write into file: %s\n", full_path);
                exit(1);
        }

	fprintf(fp, "/* Swift Fox generated code for Fennec Fox Application module */\n");
  	fprintf(fp, "#include <Fennec.h>\n");
  	fprintf(fp, "module FennecEngineP {\n\n");
  	fprintf(fp, "provides interface ModuleCtrl;\n");

  	fprintf(fp, "uses interface Leds;\n");
  	fprintf(fp, "uses interface Fennec;\n");

  	fprintf(fp, "\n\t/* Application Modules */\n\n");

  	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
    		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && ((mp->lib->type == TYPE_APPLICATION) || (mp->lib->type == TYPE_EVENT))) {
      			fprintf(fp, "/* Application  Module: %s */\n",
						mp->lib->full_name);
      			fprintf(fp, "uses interface Mgmt as %sControl;\n", 
						mp->lib->full_name);
      			fprintf(fp, "provides interface %sParams;\n", 
						mp->lib->full_name);
      			fprintf(fp, "provides interface AMSend as %sNetworkAMSend;\n", 
						mp->lib->full_name);
      			fprintf(fp, "provides interface Receive as %sNetworkReceive;\n", 
						mp->lib->full_name);
      			fprintf(fp, "provides interface Receive as %sNetworkSnoop;\n", 
						mp->lib->full_name);
      			fprintf(fp, "provides interface Packet as %sNetworkPacket;\n", 
						mp->lib->full_name);
      			fprintf(fp, "provides interface AMPacket as %sNetworkAMPacket;\n", 
						mp->lib->full_name);
      			fprintf(fp, "provides interface PacketAcknowledgements as %sNetworkPacketAcknowledgements;\n", 
						mp->lib->full_name);
			if (mp->lib->type == TYPE_EVENT) {
				fprintf(fp, "uses interface Event as %sEvent;\n",
						mp->lib->full_name);
			}
			fprintf(fp, "\n");
    		}
  	}

  	fprintf(fp, "\n\t/* Network Modules */\n\n");

	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_NETWORK) {
			fprintf(fp, "/* Network Module: %s */\n", mp->lib->full_name);
			fprintf(fp, "uses interface Mgmt as %sControl;\n", mp->lib->full_name);
      			fprintf(fp, "provides interface %sParams;\n", 
						mp->lib->full_name);


			fprintf(fp, "uses interface AMSend as %sNetworkAMSend;\n", mp->lib->full_name);
			fprintf(fp, "uses interface Receive as %sNetworkReceive;\n", mp->lib->full_name);
			fprintf(fp, "uses interface Receive as %sNetworkSnoop;\n", mp->lib->full_name);
			fprintf(fp, "uses interface AMPacket as %sNetworkAMPacket;\n", mp->lib->full_name);
			fprintf(fp, "uses interface Packet as %sNetworkPacket;\n", mp->lib->full_name);
			fprintf(fp, "uses interface PacketAcknowledgements as %sNetworkPacketAcknowledgements;\n", mp->lib->full_name);
			fprintf(fp, "provides interface AMSend as %sMacAMSend;\n", mp->lib->full_name);
			fprintf(fp, "provides interface Receive as %sMacReceive;\n", mp->lib->full_name);
			fprintf(fp, "provides interface Receive as %sMacSnoop;\n", mp->lib->full_name);
			fprintf(fp, "provides interface Packet as %sMacPacket;\n", mp->lib->full_name);
			fprintf(fp, "provides interface AMPacket as %sMacAMPacket;\n", mp->lib->full_name);
			fprintf(fp, "provides interface PacketAcknowledgements as %sMacPacketAcknowledgements;\n", mp->lib->full_name);
			fprintf(fp, "\n");
		}
	}

	fprintf(fp, "\n\t/* MAC Modules */\n\n");

	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp, "/* MAC Module: %s */\n", mp->lib->full_name);
			fprintf(fp, "uses interface Mgmt as %sControl;\n", mp->lib->full_name);

      			fprintf(fp, "provides interface %sParams;\n", 
						mp->lib->full_name);


			fprintf(fp, "uses interface AMSend as %sMacAMSend;\n", mp->lib->full_name);
			fprintf(fp, "uses interface Receive as %sMacReceive;\n", mp->lib->full_name);
			fprintf(fp, "uses interface Receive as %sMacSnoop;\n", mp->lib->full_name);
			fprintf(fp, "uses interface Packet as %sMacPacket;\n", mp->lib->full_name);
			fprintf(fp, "uses interface AMPacket as %sMacAMPacket;\n", mp->lib->full_name);
			fprintf(fp, "uses interface PacketAcknowledgements as %sMacPacketAcknowledgements;\n", mp->lib->full_name);
			fprintf(fp, "provides interface Receive as %sRadioReceive;\n", mp->lib->full_name);
			fprintf(fp, "provides interface Resource as %sRadioResource;\n", mp->lib->full_name);
			fprintf(fp, "provides interface RadioConfig as %sRadioConfig;\n", mp->lib->full_name);
			fprintf(fp, "provides interface RadioPower as %sRadioPower;\n", mp->lib->full_name);
			fprintf(fp, "provides interface Read<uint16_t> as %sReadRssi;\n", mp->lib->full_name);
			fprintf(fp, "provides interface RadioBuffer as %sRadioBuffer;\n", mp->lib->full_name);
			fprintf(fp, "provides interface RadioSend as %sRadioSend;\n", mp->lib->full_name);
			fprintf(fp, "provides interface RadioPacket as %sRadioPacket;\n", mp->lib->full_name);
			fprintf(fp, "provides interface ReceiveIndicator as %sPacketIndicator;\n", mp->lib->full_name);
			fprintf(fp, "provides interface ReceiveIndicator as %sEnergyIndicator;\n", mp->lib->full_name);
			fprintf(fp, "provides interface ReceiveIndicator as %sByteIndicator;\n", mp->lib->full_name);
			fprintf(fp, "provides interface SplitControl as %sRadioControl;\n", mp->lib->full_name);
			fprintf(fp, "\n");
		}
	}

	fprintf(fp, "\n\t/* Radio Modules */\n\n");

	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp, "/* Radio Module: %s */\n", mp->lib->full_name);
			fprintf(fp, "uses interface Mgmt as %sControl;\n", mp->lib->full_name);

      			fprintf(fp, "provides interface %sParams;\n", 
						mp->lib->full_name);


			fprintf(fp, "uses interface Receive as %sRadioReceive;\n", mp->lib->full_name);
			fprintf(fp, "uses interface Resource as %sRadioResource;\n", mp->lib->full_name);
			fprintf(fp, "uses interface RadioConfig as %sRadioConfig;\n", mp->lib->full_name);
			fprintf(fp, "uses interface RadioPower as %sRadioPower;\n", mp->lib->full_name);
			fprintf(fp, "uses interface Read<uint16_t> as %sReadRssi;\n", mp->lib->full_name);
			fprintf(fp, "uses interface RadioBuffer as %sRadioBuffer;\n", mp->lib->full_name);
			fprintf(fp, "uses interface RadioSend as %sRadioSend;\n", mp->lib->full_name);
			fprintf(fp, "uses interface RadioPacket as %sRadioPacket;\n", mp->lib->full_name);
			fprintf(fp, "uses interface ReceiveIndicator as %sPacketIndicator;\n", mp->lib->full_name);
			fprintf(fp, "uses interface ReceiveIndicator as %sEnergyIndicator;\n", mp->lib->full_name);
			fprintf(fp, "uses interface ReceiveIndicator as %sByteIndicator;\n", mp->lib->full_name);
			fprintf(fp, "\n");
		}
	}

	fprintf(fp,"}\n\n");
	fprintf(fp,"implementation {\n\n");

	fprintf(fp,"uint8_t layer_request = 0;\n\n");
	fprintf(fp,"void radioControlStartDone(uint16_t module_id, uint8_t to_layer, error_t error);\n");
	fprintf(fp,"void radioControlStopDone(uint16_t module_id, uint8_t to_layer, error_t error);\n\n");

	fprintf(fp,"void module_startDone(uint8_t module_id, error_t error) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP module_startDone(%%d, %%d)\", module_id, error);\n");
	fprintf(fp,"\tif (layer_request) {\n");
	fprintf(fp,"\t\tradioControlStartDone(module_id, layer_request, error);\n");
	fprintf(fp,"\t\tlayer_request = 0;\n");
	fprintf(fp,"\t} else {\n");
	fprintf(fp,"\t\tsignal ModuleCtrl.startDone(module_id, error);\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");

	fprintf(fp,"void module_stopDone(uint8_t module_id, error_t error) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP module_stopDone(%%d, %%d)\", module_id, error);\n");
	fprintf(fp,"\tif (layer_request) {\n");
	fprintf(fp,"\t\tradioControlStopDone(module_id, layer_request, error);\n");
	fprintf(fp,"\t\tlayer_request = 0;\n");
	fprintf(fp,"\t} else {\n");
	fprintf(fp,"\t\tsignal ModuleCtrl.stopDone(module_id, error);\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");

	fprintf(fp,"command error_t ModuleCtrl.start(uint8_t module_id) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP ModuleCtrl.start(%%d)\", module_id);\n");
	fprintf(fp,"\tswitch(module_id) {\n\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
        	if (mp->lib != NULL && mp->lib->path && mp->id > 0) {
                	fprintf(fp, "\tcase %d:\n", mp->id);
			fprintf(fp, "\t\tdbg(\"FennecEngine\", \"FennecEngineP call %sControl.start()\");\n", mp->lib->full_name);
	                fprintf(fp, "\t\treturn call %sControl.start();\n", mp->lib->full_name);
        	}
	}
	fprintf(fp,"\t}\n");
	fprintf(fp,"\treturn FAIL;\n");
	fprintf(fp,"}\n\n");

	fprintf(fp,"command error_t ModuleCtrl.stop(uint8_t module_id) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP ModuleCtrl.stop(%%d)\", module_id);\n");

	fprintf(fp,"\tswitch(module_id) {\n\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
	        if (mp->lib != NULL && mp->lib->path && mp->id > 0) {
        	        fprintf(fp, "\tcase %d:\n", mp->id);
			fprintf(fp, "\t\tdbg(\"FennecEngine\", \"FennecEngineP call %sControl.stop()\");\n", mp->lib->full_name);
                	fprintf(fp, "\t\treturn call %sControl.stop();\n", mp->lib->full_name);
	        }
	}
	fprintf(fp,"\t}\n");
	fprintf(fp,"\treturn FAIL;\n");
	fprintf(fp,"}\n\n");

	fprintf(fp,"error_t AMSend_send(uint16_t module_id, uint8_t to_layer, am_addr_t addr, message_t* msg, uint8_t len) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP AMSend_send(%%d, %%d, %%d, 0x%%1x, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, addr, msg, len);\n");
	fprintf(fp,"\tmsg->conf = call Fennec.getConfId(module_id);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_NETWORK) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sNetworkAMSend.send(addr, msg, len);\n\n", mp->lib->full_name);
		}
	}
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sMacAMSend.send(addr, msg, len);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn FAIL;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"error_t AMSend_cancel(uint16_t module_id, uint8_t to_layer, message_t *msg) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP AMSend_cancel(%%d, %%d, 0x%%1x)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, msg);\n");
	fprintf(fp,"\tmsg->conf = call Fennec.getConfId(module_id);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_NETWORK) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sNetworkAMSend.cancel(msg);\n\n", mp->lib->full_name);
		}
	}
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sMacAMSend.cancel(msg);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn FAIL;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"void* AMSend_getPayload(uint16_t module_id, uint8_t to_layer, message_t *msg, uint8_t len) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP AMSend_getPayload(%%d, %%d, 0x%%1x, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, msg, len);\n");
	fprintf(fp,"\tmsg->conf = call Fennec.getConfId(module_id);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_NETWORK) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sNetworkAMSend.getPayload(msg, len);\n\n", mp->lib->full_name);
		}
	}
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sMacAMSend.getPayload(msg, len);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn NULL;\n");
	fprintf(fp,"}\n");
	fprintf(fp,"  }\n\n");


	fprintf(fp,"uint8_t AMSend_maxPayloadLength(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP AMSend_maxPayloadLength(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_NETWORK) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sNetworkAMSend.maxPayloadLength();\n\n", mp->lib->full_name);
		}
	}
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sMacAMSend.maxPayloadLength();\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"am_addr_t AMPacket_address(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP AMPacket_address(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_NETWORK) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sNetworkAMPacket.address();\n\n", mp->lib->full_name);
		}
	}
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sMacAMPacket.address();\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"am_addr_t AMPacket_destination(uint16_t module_id, uint8_t to_layer, message_t *msg) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP AMPacket_destination(%%d, %%d, 0x%%1x)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, msg);\n");
	fprintf(fp,"\tmsg->conf = call Fennec.getConfId(module_id);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_NETWORK) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sNetworkAMPacket.destination(msg);\n", mp->lib->full_name);
		}
	}
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sMacAMPacket.destination(msg);\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"am_addr_t AMPacket_source(uint16_t module_id, uint8_t to_layer, message_t *msg) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP AMPacket_source(%%d, %%d, 0x%%1x)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, msg);\n");
	fprintf(fp,"\tmsg->conf = call Fennec.getConfId(module_id);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_NETWORK) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sNetworkAMPacket.source(msg);\n\n", mp->lib->full_name);
		}
	}
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sMacAMPacket.source(msg);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"void AMPacket_setDestination(uint16_t module_id, uint8_t to_layer, message_t *msg, am_addr_t addr) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP AMPacket_setDestination(%%d, %%d, 0x%%1x, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, msg, addr);\n");
	fprintf(fp,"\tmsg->conf = call Fennec.getConfId(module_id);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_NETWORK) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sNetworkAMPacket.setDestination(msg, addr);\n\n", mp->lib->full_name);
		}
	}
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sMacAMPacket.setDestination(msg, addr);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"void AMPacket_setSource(uint16_t module_id, uint8_t to_layer, message_t *msg, am_addr_t addr) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP AMPacket_setSource(%%d, %%d, 0x%%1x, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, msg, addr);\n");
	fprintf(fp,"\tmsg->conf = call Fennec.getConfId(module_id);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_NETWORK) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sNetworkAMPacket.setSource(msg, addr);\n\n", mp->lib->full_name);
		}
	}
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sMacAMPacket.setSource(msg, addr);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");



	fprintf(fp,"bool AMPacket_isForMe(uint16_t module_id, uint8_t to_layer, message_t *msg) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP AMPacket_isForMe(%%d, %%d, 0x%%1x)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, msg);\n");
	fprintf(fp,"\tmsg->conf = call Fennec.getConfId(module_id);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_NETWORK) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sNetworkAMPacket.isForMe(msg);\n\n", mp->lib->full_name);
		}
	}
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sMacAMPacket.isForMe(msg);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"am_id_t AMPacket_type(uint16_t module_id, uint8_t to_layer, message_t *msg) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP AMPacket_type(%%d, %%d, 0x%%1x)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, msg);\n");
	fprintf(fp,"\tmsg->conf = call Fennec.getConfId(module_id);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_NETWORK) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sNetworkAMPacket.type(msg);\n\n", mp->lib->full_name);
		}
	}
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sMacAMPacket.type(msg);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"void AMPacket_setType(uint16_t module_id, uint8_t to_layer, message_t *msg, am_id_t t) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP AMPacket_setType(%%d, %%d, 0x%%1x, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, msg, t);\n");
	fprintf(fp,"\tmsg->conf = call Fennec.getConfId(module_id);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_NETWORK) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sNetworkAMPacket.setType(msg, t);\n\n", mp->lib->full_name);
		}
	}
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sMacAMPacket.setType(msg, t);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"am_group_t AMPacket_group(uint16_t module_id, uint8_t to_layer, message_t *msg) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP AMPacket_group(%%d, %%d, 0x%%1x)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, msg);\n");
	fprintf(fp,"\tmsg->conf = call Fennec.getConfId(module_id);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_NETWORK) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sNetworkAMPacket.group(msg);\n\n", mp->lib->full_name);
		}
	}
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sMacAMPacket.group(msg);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"void AMPacket_setGroup(uint16_t module_id, uint8_t to_layer, message_t *msg, am_group_t grp) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP AMPacket_setGroup(%%d, %%d, 0x%%1x, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, msg, grp);\n");
	fprintf(fp,"\tmsg->conf = call Fennec.getConfId(module_id);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_NETWORK) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sNetworkAMPacket.setGroup(msg, grp);\n\n", mp->lib->full_name);
		}
	}
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sMacAMPacket.setGroup(msg, grp);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"void* Packet_getPayload(uint16_t module_id, uint8_t to_layer, message_t *msg, uint8_t len) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP Packet_getPayload(%%d, %%d, 0x%%1x, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, msg, len);\n");
	fprintf(fp,"\tmsg->conf = call Fennec.getConfId(module_id);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_NETWORK) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sNetworkPacket.getPayload(msg, len);\n\n", mp->lib->full_name);
		}
	}
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sMacPacket.getPayload(msg, len);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn NULL;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"uint8_t Packet_maxPayloadLength(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP Packet_maxPayloadLength(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_NETWORK) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sNetworkPacket.maxPayloadLength();\n\n", mp->lib->full_name);
		}
	}
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sMacPacket.maxPayloadLength();\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"am_group_t AMPacket_localGroup(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP Packet_localGroup(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_NETWORK) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sNetworkAMPacket.localGroup();\n\n", mp->lib->full_name);
		}
	}
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sMacAMPacket.localGroup();\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"void Packet_clear(uint16_t module_id, uint8_t to_layer, message_t *msg) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP Packet_clear(%%d, %%d, 0x%%1x)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, msg);\n");
	fprintf(fp,"\tmsg->conf = call Fennec.getConfId(module_id);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_NETWORK) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sNetworkPacket.clear(msg);\n\n", mp->lib->full_name);
		}
	}
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sMacPacket.clear(msg);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"uint8_t Packet_payloadLength(uint16_t module_id, uint8_t to_layer, message_t *msg) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP Packet_payloadLength(%%d, %%d, 0x%%1x)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, msg);\n");
	fprintf(fp,"\tmsg->conf = call Fennec.getConfId(module_id);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_NETWORK) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sNetworkPacket.payloadLength(msg);\n\n", mp->lib->full_name);
		}
	}
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sMacPacket.payloadLength(msg);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"void Packet_setPayloadLength(uint16_t module_id, uint8_t to_layer, message_t *msg, uint8_t len) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP Packet_setPayloadLength(%%d, %%d, 0x%%1x, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, msg, len);\n");
	fprintf(fp,"\tmsg->conf = call Fennec.getConfId(module_id);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_NETWORK) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sNetworkPacket.setPayloadLength(msg, len);\n\n", mp->lib->full_name);
		}
	}
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sMacPacket.setPayloadLength(msg, len);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"error_t PacketAcknowledgements_requestAck(uint16_t module_id, uint8_t to_layer, message_t *msg) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP PacketAcknowledgements_requestAck(%%d, %%d, 1x%%1x)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, msg);\n");
	fprintf(fp,"\tmsg->conf = call Fennec.getConfId(module_id);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_NETWORK) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sNetworkPacketAcknowledgements.requestAck(msg);\n\n", mp->lib->full_name);
		}
	}
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sMacPacketAcknowledgements.requestAck(msg);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn FAIL;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"error_t PacketAcknowledgements_noAck(uint16_t module_id, uint8_t to_layer, message_t *msg) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP PacketAcknowledgements_noAck(%%d, %%d, 0x%%1x)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, msg);\n");
	fprintf(fp,"\tmsg->conf = call Fennec.getConfId(module_id);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_NETWORK) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sNetworkPacketAcknowledgements.noAck(msg);\n\n", mp->lib->full_name);
		}
	}
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sMacPacketAcknowledgements.noAck(msg);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn FAIL;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"bool PacketAcknowledgements_wasAcked(uint16_t module_id, uint8_t to_layer, message_t *msg) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP PacketAcknowledgements_wasAcked(%%d, %%d, 0x%%1x)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, msg);\n");
	fprintf(fp,"\tmsg->conf = call Fennec.getConfId(module_id);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_NETWORK) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sNetworkPacketAcknowledgements.wasAcked(msg);\n\n", mp->lib->full_name);
		}
	}
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sMacPacketAcknowledgements.wasAcked(msg);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	/* Radio Only Interfaces */

	fprintf(fp,"error_t RadioConfig_sync(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP RadioConfig_sync(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sRadioConfig.sync();\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"uint8_t RadioConfig_getChannel(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP RadioConfig_getChannel(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sRadioConfig.getChannel();\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"void RadioConfig_setChannel(uint16_t module_id, uint8_t to_layer, uint8_t channel) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP RadioConfig_setChannel(%%d, %%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, channel);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sRadioConfig.setChannel( channel );\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"uint16_t RadioConfig_getShortAddr(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP RadioConfig_getShortAddr(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sRadioConfig.getShortAddr();\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"void RadioConfig_setShortAddr(uint16_t module_id, uint8_t to_layer, uint16_t address) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP RadioConfig_setShortAddr(%%d, %%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, address);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sRadioConfig.setShortAddr(address);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"uint16_t RadioConfig_getPanAddr(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP RadioConfig_getPanAddr(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sRadioConfig.getPanAddr();\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"void RadioConfig_setPanAddr(uint16_t module_id, uint8_t to_layer, uint16_t address) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP RadioConfig_setPanAddr(%%d, %%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, address);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sRadioConfig.setPanAddr(address);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"void RadioConfig_setAddressRecognition(uint16_t module_id, uint8_t to_layer, bool enableAddressRecognition, bool useHwAddressRecognition) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP RadioConfig_setAddressRecognition(%%d, %%d, %%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, enableAddressRecognition, useHwAddressRecognition);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sRadioConfig.setAddressRecognition(enableAddressRecognition, useHwAddressRecognition);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"bool RadioConfig_isAddressRecognitionEnabled(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP RadioConfig_isAddressRecognicationEnabled(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sRadioConfig.isAddressRecognitionEnabled();\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"bool RadioConfig_isHwAddressRecognitionDefault(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP RadioConfig_isHwAddressRecognicationDefault(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sRadioConfig.isHwAddressRecognitionDefault();\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");



	fprintf(fp,"void RadioConfig_setAutoAck(uint16_t module_id, uint8_t to_layer, bool enableAutoAck, bool hwAutoAck) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP RadioConfig_setAutoAck(%%d, %%d, %%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, enableAutoAck, hwAutoAck);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sRadioConfig.setAutoAck(enableAutoAck, hwAutoAck);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"bool RadioConfig_isHwAutoAckDefault(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP RadioConfig_isHwAutoAckDefault(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sRadioConfig.isHwAutoAckDefault();\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"bool RadioConfig_isAutoAckEnabled(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP RadioConfig_isAutoAckEnabled(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sRadioConfig.isAutoAckEnabled();\n\n", mp->lib->full_name);
		}	
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"error_t RadioPower_startVReg(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP RadioPower_startVReg(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sRadioPower.startVReg();\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"error_t RadioPower_stopVReg(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP RadioPower_stopVReg(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sRadioPower.stopVReg();\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"error_t RadioPower_startOscillator(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP RadioPower_startOscillator(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sRadioPower.startOscillator();\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"error_t RadioPower_stopOscillator(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP RadioPower_stopOscillator(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sRadioPower.stopOscillator();\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"error_t RadioPower_rxOn(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP RadioPower_rxOn(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sRadioPower.rxOn();\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"error_t RadioPower_rfOff(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP RadioPower_rxOff(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sRadioPower.rfOff();\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"error_t ReadRssi_read(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP ReadRssi_read(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sReadRssi.read();\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"error_t RadioResource_request(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP RadioResource_request(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sRadioResource.request();\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn FAIL;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"error_t RadioResource_immediateRequest(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP RadioResource_immediateRequest(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sRadioResource.immediateRequest();\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"error_t RadioResource_release(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP RadioResource_release(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sRadioResource.release();\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"bool RadioResource_isOwner(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP RadioResource_isOwner(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sRadioResource.isOwner();\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"error_t RadioControl_start(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP RadioControl_start(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tlayer_request = F_MAC;\n");
	fprintf(fp,"\treturn call ModuleCtrl.start( call Fennec.getNextModuleId(module_id, to_layer) );\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"error_t RadioControl_stop(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP RadioControl_stop(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tlayer_request = F_MAC;\n");
	fprintf(fp,"\treturn call ModuleCtrl.stop( call Fennec.getNextModuleId(module_id, to_layer) );\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"error_t RadioSend_cancel(uint16_t module_id, uint8_t to_layer, message_t *msg) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP RadioSend_cancel(%%d, %%d, 0x%%1x)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, msg);\n");
	fprintf(fp,"\tmsg->conf = call Fennec.getConfId(module_id);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sRadioSend.cancel(msg);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn FAIL;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"error_t RadioSend_send(uint16_t module_id, uint8_t to_layer, message_t* msg, bool useCca) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP RadioSend_send(%%d, %%d, 0x%%1x, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, msg, useCca);\n");
	fprintf(fp,"\tmsg->conf = call Fennec.getConfId(module_id);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sRadioSend.send(msg, useCca);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn FAIL;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"uint8_t RadioPacket_maxPayloadLength(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP Radiopacket_maxPayloadLength(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sRadioPacket.maxPayloadLength();\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"void* RadioPacket_getPayload(uint16_t module_id, uint8_t to_layer, message_t* msg, uint8_t len) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP RadioPacket_getPayload(%%d, %%d, 0x%%1x, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, msg, len);\n");
	fprintf(fp,"\tmsg->conf = call Fennec.getConfId(module_id);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sRadioPacket.getPayload(msg, len);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn NULL;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"error_t RadioBuffer_load(uint16_t module_id, uint8_t to_layer, message_t* msg) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP RadioBuffer_load(%%d, %%d, 0x%%1x)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, msg);\n");
	fprintf(fp,"\tmsg->conf = call Fennec.getConfId(module_id);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sRadioBuffer.load(msg);\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn FAIL;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");



	fprintf(fp,"bool PacketIndicator_isReceiving(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP PacketIndicator_isReceiving(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sPacketIndicator.isReceiving();\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"bool EnergyIndicator_isReceiving(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP EnergyIndicator_isReceiving(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sEnergyIndicator.isReceiving();\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"bool ByteIndicator_isReceiving(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP ByteIndicator_isReceiving(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn call %sByteIndicator.isReceiving();\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn 0;\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"void sendDone(uint16_t module_id, uint8_t to_layer, message_t* msg, error_t error) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP sendDone(%%d, %%d, 0x%%1x, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, msg, error);\n");
	fprintf(fp,"\tmsg->conf = call Fennec.getConfId(module_id);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_EVENT) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\tsignal %sNetworkAMSend.sendDone(msg, error);\n", mp->lib->full_name);
			fprintf(fp,"\t\treturn;\n\n");
		}
	}
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_APPLICATION) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\tsignal %sNetworkAMSend.sendDone(msg, error);\n", mp->lib->full_name);
			fprintf(fp,"\t\treturn;\n\n");
		}
	}
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_NETWORK) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\tsignal %sMacAMSend.sendDone(msg, error);\n", mp->lib->full_name);
			fprintf(fp,"\t\treturn;\n\n");
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn;\n");
	fprintf(fp,"}\n\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"message_t* receive(uint16_t module_id, uint8_t to_layer, message_t* msg, void* payload, uint8_t len) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP sendDone(%%d, %%d, 0x%%1x, 0x%%1x, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, msg, payload, len);\n");
	fprintf(fp,"\tif (call Fennec.checkPacket(msg, len) != SUCCESS) {\n");
	fprintf(fp,"\t\t return msg;\n");
	fprintf(fp,"\t}\n\n");
	fprintf(fp,"\tmsg->conf = call Fennec.getConfId(module_id);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	fprintf(fp,"\t\treturn msg;\n");
	fprintf(fp,"\t}\n\n");

	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_EVENT) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn signal %sNetworkReceive.receive(msg, payload, len);\n\n", mp->lib->full_name);
		}
	}
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_APPLICATION) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn signal %sNetworkReceive.receive(msg, payload, len);\n\n", mp->lib->full_name);
		}
	}
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_NETWORK) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn signal %sMacReceive.receive(msg, payload, len);\n\n", mp->lib->full_name);
		}
	}

	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn signal %sRadioReceive.receive(msg, payload, len);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn msg;\n\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"message_t* snoop(uint16_t module_id, uint8_t to_layer, message_t* msg, void* payload, uint8_t len) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP sendDone(%%d, %%d, 0x%%1x, 0x%%1x, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, msg, payload, len);\n");
	fprintf(fp,"\tif (call Fennec.checkPacket(msg, len) != SUCCESS) {\n");
	fprintf(fp,"\t\t return msg;\n");
	fprintf(fp,"\t}\n\n");
	fprintf(fp,"\tmsg->conf = call Fennec.getConfId(module_id);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	fprintf(fp,"\t\treturn msg;\n");
	fprintf(fp,"\t}\n\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_EVENT) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn signal %sNetworkSnoop.receive(msg, payload, len);\n\n", mp->lib->full_name);
		}
	}
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_APPLICATION) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn signal %sNetworkSnoop.receive(msg, payload, len);\n\n", mp->lib->full_name);
		}
	}
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_NETWORK) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn signal %sMacSnoop.receive(msg, payload, len);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\tdefault:\n");
	fprintf(fp,"\t\treturn msg;\n\n");
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"void syncDone(uint16_t module_id, uint8_t to_layer, error_t error) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP syncDone(%%d, %%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, error);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn signal %sRadioConfig.syncDone(error);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"void startVRegDone(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP startVRegDone(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn signal %sRadioPower.startVRegDone();\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"void startOscillatorDone(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP startOscillatorDone(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn signal %sRadioPower.startOscillatorDone();\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"void readRssiDone(uint16_t module_id, uint8_t to_layer, error_t error, uint16_t rssi) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP readRssiDone(%%d, %%d, %%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, error, rssi);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn signal %sReadRssi.readDone(error, rssi);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");



	fprintf(fp,"void granted(uint16_t module_id, uint8_t to_layer) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP granted(%%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn signal %sRadioResource.granted();\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"void transmitLoadDone(uint16_t module_id, uint8_t to_layer, message_t *msg, error_t error) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP transmitLoadDone(%%d, %%d, 0x%%1x, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, msg, error);\n");
	fprintf(fp,"\tmsg->conf = call Fennec.getConfId(module_id);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn signal %sRadioBuffer.loadDone(msg, error);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"void transmitSendDone(uint16_t module_id, uint8_t to_layer, message_t *msg, error_t error) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP transmitSendDone(%%d, %%d, 0x%%1x, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, msg, error);\n");
	fprintf(fp,"\tmsg->conf = call Fennec.getConfId(module_id);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn signal %sRadioSend.sendDone(msg, error);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"void radioControlStartDone(uint16_t module_id, uint8_t to_layer, error_t error) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP radioControlStartDone(%%d, %%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, error);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn signal %sRadioControl.startDone(error);\n\n", mp->lib->full_name);
		}		
	}
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");


	fprintf(fp,"void radioControlStopDone(uint16_t module_id, uint8_t to_layer, error_t error) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"FennecEngineP radioControlStopDone(%%d, %%d, %%d)\",\n");
	fprintf(fp,"\t\t\tmodule_id, to_layer, error);\n");
	fprintf(fp,"\tswitch( call Fennec.getNextModuleId(module_id, to_layer) ) {\n");
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp,"\tcase %d:\n", mp->id);
			fprintf(fp,"\t\treturn signal %sRadioControl.stopDone(error);\n\n", mp->lib->full_name);
		}
	}
	fprintf(fp,"\t}\n");
	fprintf(fp,"}\n\n");

	struct paramtype *pt;

	/* Interfaces with Applications */


	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && ((mp->lib->type == TYPE_APPLICATION) || (mp->lib->type == TYPE_EVENT))) {
			fprintf(fp, "\n/* Linking Application %s */\n", mp->lib->full_name);

			fprintf(fp, "\t/* Module Control Interface */\n\n");	

			fprintf(fp, "event void %sControl.startDone(error_t err){\n", mp->lib->full_name);
			fprintf(fp, "\tmodule_startDone(%d, err);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "event void %sControl.stopDone(error_t err) {\n", mp->lib->full_name);
			fprintf(fp, "\tmodule_stopDone(%d, err);\n", mp->id);
			fprintf(fp, "}\n\n");

			if (mp->lib->type == TYPE_EVENT) {
				fprintf(fp, "\t/* Event Interface */\n\n");	
				fprintf(fp, "event void %sEvent.occured(uint16_t oc) {\n", mp->lib->full_name);
				fprintf(fp, "\tcall Fennec.eventOccured(%d, oc);\n", mp->id);
				fprintf(fp, "}\n\n");
			}

			fprintf(fp, "\t/* Parameter Interface */\n\n");	

			/* check if the interface is empty, if it is add dummy call */
			if (mp->lib->params == NULL) {
				fprintf(fp, "command void %sParams.dummy() {}\n", mp->lib->full_name);
			}

			for(pt = mp->lib->params; pt != NULL; pt = pt->child ) {
				fprintf(fp, "command %s %sParams.get_%s() {\n",
						type_name(pt->type), 
						mp->lib->full_name, 
						pt->name);
				fprintf(fp, "\tstruct %s_params_ptr *params = (struct %s_params_ptr*) (call Fennec.getConfParams(%d)).application;\n", 
							mp->lib->full_name, 
							mp->lib->full_name, 
							mp->id);
				fprintf(fp, "\treturn *(params->%s);\n", pt->name);
				fprintf(fp, "}\n\n");

				fprintf(fp, "command error_t %sParams.set_%s(%s new_%s) {\n",
						mp->lib->full_name, 
						pt->name, 
						type_name(pt->type), 
						pt->name);
				fprintf(fp, "\tstruct %s_params_ptr *params = (struct %s_params_ptr*) (call Fennec.getConfParams(%d)).application;\n", 
							mp->lib->full_name, 
							mp->lib->full_name, 
							mp->id);

				fprintf(fp, "\t*params->%s = new_%s;\n", 
						pt->name,
						pt->name);
				fprintf(fp, "\treturn SUCCESS;\n");
		                fprintf(fp, "}\n\n");
		        }

			fprintf(fp, "command error_t %sNetworkAMSend.send(am_addr_t addr, message_t* msg, uint8_t len) {\n",
						mp->lib->full_name);
			fprintf(fp, "\tdbg(\"FennecEngine\", \"FennecEngineP call %sNetworkAMSend.send()\");\n",
						mp->lib->full_name);
			fprintf(fp, "\treturn AMSend_send(%d, F_NETWORK, addr, msg, len);\n", 
						mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "command error_t %sNetworkAMSend.cancel(message_t* msg) {\n",
						mp->lib->full_name);
			fprintf(fp, "\tdbg(\"FennecEngine\", \"FennecEngineP call %sNetworkAMSend.cancel()\");\n",
						mp->lib->full_name);
			fprintf(fp, "\treturn AMSend_cancel(%d, F_NETWORK, msg);\n",
						mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "command uint8_t %sNetworkAMSend.maxPayloadLength() {\n", mp->lib->full_name);
			fprintf(fp, "\tdbg(\"FennecEngine\", \"FennecEngineP call %sNetworkAMSend.maxPayloadLength()\");\n",
						mp->lib->full_name);
			fprintf(fp, "\treturn AMSend_maxPayloadLength(%d, F_NETWORK);\n", mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "command void* %sNetworkAMSend.getPayload(message_t* msg, uint8_t len) {\n", mp->lib->full_name);
			fprintf(fp, "\tdbg(\"FennecEngine\", \"FennecEngineP call %sNetworkAMSend.getPayload()\");\n",
						mp->lib->full_name);
			fprintf(fp, "\treturn AMSend_getPayload(%d, F_NETWORK, msg, len);\n", mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "command am_addr_t %sNetworkAMPacket.address() {\n", mp->lib->full_name);
			fprintf(fp, "\tdbg(\"FennecEngine\", \"FennecEngineP call %sNetworkAMPacket.address()\");\n",
						mp->lib->full_name);
			fprintf(fp, "\treturn AMPacket_address(%d, F_NETWORK);\n", mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "command am_addr_t %sNetworkAMPacket.destination(message_t* msg) {\n", mp->lib->full_name);
			fprintf(fp, "\tdbg(\"FennecEngine\", \"FennecEngineP call %sNetworkAMPacket.destination()\");\n",
						mp->lib->full_name);
			fprintf(fp, "\treturn AMPacket_destination(%d, F_NETWORK, msg);\n", mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "command am_addr_t %sNetworkAMPacket.source(message_t* msg) {\n", mp->lib->full_name);
			fprintf(fp, "\tdbg(\"FennecEngine\", \"FennecEngineP call %sNetworkAMPacket.source()\");\n",
						mp->lib->full_name);
			fprintf(fp, "\treturn AMPacket_source(%d, F_NETWORK, msg);\n", mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "command void %sNetworkAMPacket.setDestination(message_t* msg, am_addr_t addr) {\n", mp->lib->full_name);
			fprintf(fp, "\tdbg(\"FennecEngine\", \"FennecEngineP call %sNetworkAMPacket.setDestination()\");\n",
						mp->lib->full_name);
			fprintf(fp, "\treturn AMPacket_setDestination(%d, F_NETWORK, msg, addr);\n", mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "command void %sNetworkAMPacket.setSource(message_t* msg, am_addr_t addr) {\n", mp->lib->full_name);
			fprintf(fp, "\tdbg(\"FennecEngine\", \"FennecEngineP call %sNetworkAMPacket.setSource()\");\n",
						mp->lib->full_name);
			fprintf(fp, "\treturn AMPacket_setSource(%d, F_NETWORK, msg, addr);\n", mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "command bool %sNetworkAMPacket.isForMe(message_t* msg) {\n", mp->lib->full_name);
			fprintf(fp, "\tdbg(\"FennecEngine\", \"FennecEngineP call %sNetworkAMPacket.isForMe()\");\n",
						mp->lib->full_name);
			fprintf(fp, "\treturn AMPacket_isForMe(%d, F_NETWORK, msg);\n", mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "command am_id_t %sNetworkAMPacket.type(message_t* msg) {\n", mp->lib->full_name);
			fprintf(fp, "\tdbg(\"FennecEngine\", \"FennecEngineP call %sNetworkAMPacket.type()\");\n",
						mp->lib->full_name);
			fprintf(fp, "\treturn AMPacket_type(%d, F_NETWORK, msg);\n",mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "command void %sNetworkAMPacket.setType(message_t* msg, am_id_t t) {\n", mp->lib->full_name);
			fprintf(fp, "\tdbg(\"FennecEngine\", \"FennecEngineP call %sNetworkAMPacket.setType()\");\n",
						mp->lib->full_name);
			fprintf(fp, "\treturn AMPacket_setType(%d, F_NETWORK, msg, t);\n", mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "command am_group_t %sNetworkAMPacket.group(message_t* msg) {\n", mp->lib->full_name);
			fprintf(fp, "\tdbg(\"FennecEngine\", \"FennecEngineP call %sNetworkAMPacket.group()\");\n",
						mp->lib->full_name);
			fprintf(fp, "\treturn AMPacket_group(%d, F_NETWORK, msg);\n", mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "command void %sNetworkAMPacket.setGroup(message_t* msg, am_group_t grp) {\n", mp->lib->full_name);
			fprintf(fp, "\tdbg(\"FennecEngine\", \"FennecEngineP call %sNetworkAMPacket.setGroup()\");\n",
						mp->lib->full_name);
			fprintf(fp, "\treturn AMPacket_setGroup(%d, F_NETWORK, msg, grp);\n", mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "command am_group_t %sNetworkAMPacket.localGroup() {\n", mp->lib->full_name);
			fprintf(fp, "\tdbg(\"FennecEngine\", \"FennecEngineP call %sNetworkAMPacket.localGroup()\");\n",
						mp->lib->full_name);
			fprintf(fp, "\treturn AMPacket_localGroup(%d, F_NETWORK);\n", mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "command void %sNetworkPacket.clear(message_t* msg) {\n", mp->lib->full_name);
			fprintf(fp, "\tdbg(\"FennecEngine\", \"FennecEngineP call %sNetworkPacket.clear()\");\n",
						mp->lib->full_name);
			fprintf(fp, "\treturn Packet_clear(%d, F_NETWORK, msg);\n", mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "command uint8_t %sNetworkPacket.payloadLength(message_t* msg) {\n", mp->lib->full_name);
			fprintf(fp, "\tdbg(\"FennecEngine\", \"FennecEngineP call %sNetworkPacket.payloadLength()\");\n",
						mp->lib->full_name);
			fprintf(fp, "\treturn Packet_payloadLength(%d, F_NETWORK, msg);\n", mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "command void %sNetworkPacket.setPayloadLength(message_t* msg, uint8_t len) {\n", mp->lib->full_name);
			fprintf(fp, "\tdbg(\"FennecEngine\", \"FennecEngineP call %sNetworkPacket.setPayloadLength()\");\n",
						mp->lib->full_name);
			fprintf(fp, "\treturn Packet_setPayloadLength(%d, F_NETWORK, msg, len);\n", mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "command uint8_t %sNetworkPacket.maxPayloadLength() {\n", mp->lib->full_name);
			fprintf(fp, "\tdbg(\"FennecEngine\", \"FennecEngineP call %sNetworkPacket.maxPayloadLength()\");\n",
						mp->lib->full_name);
			fprintf(fp, "\treturn Packet_maxPayloadLength(%d, F_NETWORK);\n", mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "command void* %sNetworkPacket.getPayload(message_t* msg, uint8_t len) {\n", mp->lib->full_name);
			fprintf(fp, "\tdbg(\"FennecEngine\", \"FennecEngineP call %sNetworkPacket.getPayload()\");\n",
						mp->lib->full_name);
			fprintf(fp, "\treturn Packet_getPayload(%d, F_NETWORK, msg, len);\n", mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "async command error_t %sNetworkPacketAcknowledgements.requestAck( message_t* msg ) {\n", mp->lib->full_name);
			fprintf(fp, "\tdbg(\"FennecEngine\", \"FennecEngineP call %sNetworkPacket.requestAck()\");\n",
						mp->lib->full_name);
			fprintf(fp, "\treturn PacketAcknowledgements_requestAck(%d, F_NETWORK, msg);\n", mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "async command error_t %sNetworkPacketAcknowledgements.noAck( message_t* msg ) {\n", mp->lib->full_name);
			fprintf(fp, "\tdbg(\"FennecEngine\", \"FennecEngineP call %sNetworkPacket.noAck()\");\n",
						mp->lib->full_name);
			fprintf(fp, "\treturn PacketAcknowledgements_noAck(%d, F_NETWORK, msg);\n", mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "async command bool %sNetworkPacketAcknowledgements.wasAcked(message_t* msg) {\n", mp->lib->full_name);
			fprintf(fp, "\tdbg(\"FennecEngine\", \"FennecEngineP call %sNetworkPacket.wasAcked()\");\n",
						mp->lib->full_name);
			fprintf(fp, "\treturn PacketAcknowledgements_wasAcked(%d, F_NETWORK, msg);\n", mp->id);
			fprintf(fp, "}\n\n");
		}
	}

  /* Interfaces with Networks */

	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_NETWORK) {
			fprintf(fp, "event void %sControl.startDone(error_t err) {\n", mp->lib->full_name);
			fprintf(fp, "\tmodule_startDone(%d, err);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "event void %sControl.stopDone(error_t err) {\n", mp->lib->full_name);
			fprintf(fp, "\tmodule_stopDone(%d, err);\n", mp->id);
			fprintf(fp, "}\n\n");


			fprintf(fp, "\t/* Parameter Interfaces */\n\n");	

			/* check if the interface is empty, if it is add dummy call */
			if (mp->lib->params == NULL) {
				fprintf(fp, "command void %sParams.dummy() {}\n", mp->lib->full_name);
			}

			for(pt = mp->lib->params; pt != NULL; pt = pt->child ) {
				fprintf(fp, "command %s %sParams.get_%s() {\n",
						type_name(pt->type), 
						mp->lib->full_name, 
						pt->name);
				fprintf(fp, "\tstruct %s_params_ptr *params = (struct %s_params_ptr*) (call Fennec.getConfParams(%d)).network;\n", 
							mp->lib->full_name, 
							mp->lib->full_name, 
							mp->id);
				fprintf(fp, "\treturn *(params->%s);\n", pt->name);
				fprintf(fp, "}\n\n");
				fprintf(fp, "command error_t %sParams.set_%s(%s new_%s) {\n",
						mp->lib->full_name, 
						pt->name, 
						type_name(pt->type), 
						pt->name);
				fprintf(fp, "\tstruct %s_params_ptr *params = (struct %s_params_ptr*) (call Fennec.getConfParams(%d)).network;\n", 
							mp->lib->full_name, 
							mp->lib->full_name, 
							mp->id);

				fprintf(fp, "\t*params->%s = new_%s;\n", 
						pt->name,
						pt->name);
				fprintf(fp, "\treturn SUCCESS;\n");
		                fprintf(fp, "}\n\n");
		        }


			fprintf(fp, "event void %sNetworkAMSend.sendDone(message_t *msg, error_t error) {\n", mp->lib->full_name);
			fprintf(fp, "\tsendDone(%d, F_APPLICATION, msg, error);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "event message_t* %sNetworkReceive.receive(message_t *msg, void* payload, uint8_t len) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn receive(%d, F_APPLICATION, msg, payload, len);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "event message_t* %sNetworkSnoop.receive(message_t *msg, void* payload, uint8_t len) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn snoop(%d, F_APPLICATION, msg, payload, len);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "command error_t %sMacAMSend.send(am_addr_t addr, message_t* msg, uint8_t len) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn AMSend_send(%d, F_MAC, addr, msg, len);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "command error_t %sMacAMSend.cancel(message_t* msg) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn AMSend_cancel(%d, F_MAC, msg);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "command uint8_t %sMacAMSend.maxPayloadLength() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn AMSend_maxPayloadLength(%d, F_MAC);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "command void* %sMacAMSend.getPayload(message_t* msg, uint8_t len) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn AMSend_getPayload(%d, F_MAC, msg, len);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "command am_addr_t %sMacAMPacket.address() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn AMPacket_address(%d, F_MAC);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "command am_addr_t %sMacAMPacket.destination(message_t* msg) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn AMPacket_destination(%d, F_MAC, msg);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "command am_addr_t %sMacAMPacket.source(message_t* msg) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn AMPacket_source(%d, F_MAC, msg);\n", mp->id);
			fprintf(fp, "}\n");
			fprintf(fp, "command void %sMacAMPacket.setDestination(message_t* msg, am_addr_t addr) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn AMPacket_setDestination(%d, F_MAC, msg, addr);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "command void %sMacAMPacket.setSource(message_t* msg, am_addr_t addr) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn AMPacket_setSource(%d, F_MAC, msg, addr);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "command bool %sMacAMPacket.isForMe(message_t* msg) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn AMPacket_isForMe(%d, F_MAC, msg);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "command am_id_t %sMacAMPacket.type(message_t* msg) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn AMPacket_type(%d, F_MAC, msg);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "command void %sMacAMPacket.setType(message_t* msg, am_id_t t) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn AMPacket_setType(%d, F_MAC, msg, t);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "command am_group_t %sMacAMPacket.group(message_t* msg) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn AMPacket_group(%d, F_MAC, msg);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "command void %sMacAMPacket.setGroup(message_t* msg, am_group_t grp) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn AMPacket_setGroup(%d, F_MAC, msg, grp);\n", mp->id);
			fprintf(fp, "}\n");
			fprintf(fp, "command am_group_t %sMacAMPacket.localGroup() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn AMPacket_localGroup(%d, F_MAC);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "command void %sMacPacket.clear(message_t* msg) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn Packet_clear(%d, F_MAC, msg);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "command uint8_t %sMacPacket.payloadLength(message_t* msg) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn Packet_payloadLength(%d, F_MAC, msg);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "command void %sMacPacket.setPayloadLength(message_t* msg, uint8_t len) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn Packet_setPayloadLength(%d, F_MAC, msg, len);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "command uint8_t %sMacPacket.maxPayloadLength() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn Packet_maxPayloadLength(%d, F_MAC);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "command void* %sMacPacket.getPayload(message_t* msg, uint8_t len) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn Packet_getPayload(%d, F_MAC, msg, len);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "async command error_t %sMacPacketAcknowledgements.requestAck( message_t* msg ) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn PacketAcknowledgements_requestAck(%d, F_MAC, msg);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "async command error_t %sMacPacketAcknowledgements.noAck( message_t* msg ) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn PacketAcknowledgements_noAck(%d, F_MAC, msg);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "async command bool %sMacPacketAcknowledgements.wasAcked(message_t* msg) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn PacketAcknowledgements_wasAcked(%d, F_MAC, msg);\n", mp->id);
			fprintf(fp, "}\n\n");
		}
	}

/* Interfaces with Macs */

	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_MAC) {
			fprintf(fp, "event void %sControl.startDone(error_t err) {\n", mp->lib->full_name);
			fprintf(fp, "\tmodule_startDone(%d, err);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "event void %sControl.stopDone(error_t err) {\n", mp->lib->full_name);
			fprintf(fp, "\tmodule_stopDone(%d, err);\n", mp->id);
			fprintf(fp, "}\n\n");


			fprintf(fp, "\t/* Parameter Interfaces */\n\n");	

			/* check if the interface is empty, if it is add dummy call */
			if (mp->lib->params == NULL) {
				fprintf(fp, "command void %sParams.dummy() {}\n", mp->lib->full_name);
			}

			for(pt = mp->lib->params; pt != NULL; pt = pt->child ) {
				fprintf(fp, "command %s %sParams.get_%s() {\n",
						type_name(pt->type), 
						mp->lib->full_name, 
						pt->name);
				fprintf(fp, "\tstruct %s_params_ptr *params = (struct %s_params_ptr*) (call Fennec.getConfParams(%d)).mac;\n", 
							mp->lib->full_name, 
							mp->lib->full_name, 
							mp->id);
				fprintf(fp, "\treturn *(params->%s);\n", pt->name);
				fprintf(fp, "}\n\n");
				fprintf(fp, "command error_t %sParams.set_%s(%s new_%s) {\n",
						mp->lib->full_name, 
						pt->name, 
						type_name(pt->type), 
						pt->name);
				fprintf(fp, "\tstruct %s_params_ptr *params = (struct %s_params_ptr*) (call Fennec.getConfParams(%d)).mac;\n", 
							mp->lib->full_name, 
							mp->lib->full_name, 
							mp->id);
				fprintf(fp, "\t*params->%s = new_%s;\n", 
						pt->name,
						pt->name);
				fprintf(fp, "\treturn SUCCESS;\n");
		                fprintf(fp, "}\n\n");
		        }


			fprintf(fp, "event void %sMacAMSend.sendDone(message_t *msg, error_t error) {\n", mp->lib->full_name);
			fprintf(fp, "\tsendDone(%d, F_NETWORK, msg, error);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "event message_t* %sMacReceive.receive(message_t *msg, void* payload, uint8_t len) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn receive(%d, F_NETWORK, msg, payload, len);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "event message_t* %sMacSnoop.receive(message_t *msg, void* payload, uint8_t len) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn snoop(%d, F_NETWORK, msg, payload, len);\n", mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "command error_t %sRadioConfig.sync() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn RadioConfig_sync(%d, F_RADIO);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "command uint8_t %sRadioConfig.getChannel() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn RadioConfig_getChannel(%d, F_RADIO);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "command void %sRadioConfig.setChannel(uint8_t channel) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn RadioConfig_setChannel(%d, F_RADIO, channel);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "async command uint16_t %sRadioConfig.getShortAddr() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn RadioConfig_getShortAddr(%d, F_RADIO);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "command void %sRadioConfig.setShortAddr(uint16_t address) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn RadioConfig_setShortAddr(%d, F_RADIO, address);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "async command uint16_t %sRadioConfig.getPanAddr() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn RadioConfig_getPanAddr(%d, F_RADIO);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "command void %sRadioConfig.setPanAddr(uint16_t address) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn RadioConfig_setPanAddr(%d, F_RADIO, address);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "command void %sRadioConfig.setAddressRecognition(bool enableAddressRecognition, bool useHwAddressRecognition) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn RadioConfig_setAddressRecognition(%d, F_RADIO, enableAddressRecognition, useHwAddressRecognition);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "async command bool %sRadioConfig.isAddressRecognitionEnabled() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn RadioConfig_isAddressRecognitionEnabled(%d, F_RADIO);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "async command bool %sRadioConfig.isHwAddressRecognitionDefault() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn RadioConfig_isHwAddressRecognitionDefault(%d, F_RADIO);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "command void %sRadioConfig.setAutoAck(bool enableAutoAck, bool hwAutoAck) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn RadioConfig_setAutoAck(%d, F_RADIO, enableAutoAck, hwAutoAck);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "async command bool %sRadioConfig.isAutoAckEnabled() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn RadioConfig_isAutoAckEnabled(%d, F_RADIO);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "async command bool %sRadioConfig.isHwAutoAckDefault() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn RadioConfig_isHwAutoAckDefault(%d, F_RADIO);\n", mp->id);
			fprintf(fp, "}\n\n");


			fprintf(fp, "async command error_t %sRadioPower.startVReg() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn RadioPower_startVReg(%d, F_RADIO);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "async command error_t %sRadioPower.stopVReg() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn RadioPower_stopVReg(%d, F_RADIO);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "async command error_t %sRadioPower.startOscillator() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn RadioPower_startOscillator(%d, F_RADIO);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "async command error_t %sRadioPower.stopOscillator() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn RadioPower_stopOscillator(%d, F_RADIO);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "async command error_t %sRadioPower.rxOn() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn RadioPower_rxOn(%d, F_RADIO);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "async command error_t %sRadioPower.rfOff() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn RadioPower_rfOff(%d, F_RADIO);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "command error_t %sReadRssi.read() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn ReadRssi_read(%d, F_RADIO);\n", mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "async command error_t %sRadioResource.request() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn RadioResource_request(%d, F_RADIO);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "async command error_t %sRadioResource.immediateRequest() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn RadioResource_immediateRequest(%d, F_RADIO);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "async command error_t %sRadioResource.release() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn RadioResource_release(%d, F_RADIO);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "async command error_t %sRadioResource.isOwner() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn RadioResource_isOwner(%d, F_RADIO);\n", mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "command error_t %sRadioControl.start() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn RadioControl_start(%d, F_RADIO);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "command error_t %sRadioControl.stop() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn RadioControl_stop(%d, F_RADIO);\n", mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "async command error_t %sRadioSend.cancel(message_t *msg) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn RadioSend_cancel(%d, F_RADIO, msg);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "async command error_t %sRadioSend.send(message_t* msg, bool useCca) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn RadioSend_send(%d, F_RADIO, msg, useCca);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "async command uint8_t %sRadioPacket.maxPayloadLength() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn RadioPacket_maxPayloadLength(%d, F_RADIO);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "async command void* %sRadioPacket.getPayload(message_t* msg, uint8_t len) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn RadioPacket_getPayload(%d, F_RADIO, msg, len);\n", mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "async command error_t %sRadioBuffer.load(message_t* msg) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn RadioBuffer_load(%d, F_RADIO, msg);\n", mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "async command bool %sPacketIndicator.isReceiving() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn PacketIndicator_isReceiving(%d, F_RADIO);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "async command bool %sEnergyIndicator.isReceiving() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn EnergyIndicator_isReceiving(%d, F_RADIO);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "async command bool %sByteIndicator.isReceiving() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn ByteIndicator_isReceiving(%d, F_RADIO);\n", mp->id);
			fprintf(fp, "}\n\n");
		}
	}

/* Interfaces with Radios */

	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->id > 0 && mp->lib->type == TYPE_RADIO) {
			fprintf(fp, "event void %sControl.startDone(error_t err) {\n", mp->lib->full_name);
			fprintf(fp, "\tmodule_startDone(%d, err);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "event void %sControl.stopDone(error_t err) {\n", mp->lib->full_name);
			fprintf(fp, "\tmodule_stopDone(%d, err);\n", mp->id);
			fprintf(fp, "}\n\n");


			fprintf(fp, "\t/* Parameter Interfaces */\n\n");	

			/* check if the interface is empty, if it is add dummy call */
			if (mp->lib->params == NULL) {
				fprintf(fp, "command void %sParams.dummy() {}\n", mp->lib->full_name);
			}

			for(pt = mp->lib->params; pt != NULL; pt = pt->child ) {
				fprintf(fp, "command %s %sParams.get_%s() {\n",
						type_name(pt->type), 
						mp->lib->full_name, 
						pt->name);
				fprintf(fp, "\tstruct %s_params_ptr *params = (struct %s_params_ptr*) (call Fennec.getConfParams(%d)).radio;\n", 
							mp->lib->full_name, 
							mp->lib->full_name, 
							mp->id);
				fprintf(fp, "\treturn *(params->%s);\n", pt->name);
				fprintf(fp, "}\n\n");
				fprintf(fp, "command error_t %sParams.set_%s(%s new_%s) {\n",
						mp->lib->full_name, 
						pt->name, 
						type_name(pt->type), 
						pt->name);
				fprintf(fp, "\tstruct %s_params_ptr *params = (struct %s_params_ptr*) (call Fennec.getConfParams(%d)).radio;\n", 
							mp->lib->full_name, 
							mp->lib->full_name, 
							mp->id);
				fprintf(fp, "\t*params->%s = new_%s;\n", 
						pt->name,
						pt->name);
				fprintf(fp, "\treturn SUCCESS;\n");
		                fprintf(fp, "}\n\n");
		        }


			fprintf(fp, "event message_t* %sRadioReceive.receive(message_t *msg, void* payload, uint8_t len) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn receive(%d, F_MAC, msg, payload, len);\n", mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "event void %sRadioConfig.syncDone(error_t error) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn syncDone(%d, F_MAC, error);\n", mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "async event void %sRadioPower.startVRegDone() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn startVRegDone(%d, F_MAC);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "async event void %sRadioPower.startOscillatorDone() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn startOscillatorDone(%d, F_MAC);\n", mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "event void %sReadRssi.readDone(error_t error, uint16_t rssi) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn readRssiDone(%d, F_MAC, error, rssi);\n", mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "event void %sRadioResource.granted() {\n", mp->lib->full_name);
			fprintf(fp, "\treturn granted(%d, F_MAC);\n", mp->id);
			fprintf(fp, "}\n\n");

			fprintf(fp, "async event void %sRadioBuffer.loadDone(message_t* msg, error_t error) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn transmitLoadDone(%d, F_MAC, msg, error);\n", mp->id);
			fprintf(fp, "}\n\n");
			fprintf(fp, "async event void %sRadioSend.sendDone(message_t *msg, error_t error) {\n", mp->lib->full_name);
			fprintf(fp, "\treturn transmitSendDone(%d, F_MAC, msg, error);\n", mp->id);
			fprintf(fp, "}\n\n");
		}
	}

	fprintf(fp, "\n}\n");
	fclose(fp);
	free(full_path);
}

