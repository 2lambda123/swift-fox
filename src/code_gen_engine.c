/*
 * Copyright (c) 2010-2014, Columbia University.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  - Neither the name of the Columbia University nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL COLUMBIA UNIVERSITY BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
  * Swift Fox compiler
  *
  * @author: Marcin K Szczodrak
  */


#include "code_gen.h"
#include "utils.h"

void generateFennecEngineC() {

	char *full_path = get_sfc_path("", "FennecEngineC.nc");
	FILE *fp = fopen(full_path, "w");
	int i;
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

	fprintf(fp, "components FennecC;\n");
  	fprintf(fp, "FennecEngineP.Fennec -> FennecC;\n\n");

	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->lib->used && mp->type == TYPE_AM) {
			fprintf(fp, "#define UQ_%s_QUEUE \"unique_%s_queue\"\n",
					mp->id_name,
					mp->lib->name);
			fprintf(fp, "components %sC;\n",
					mp->lib->name);
			fprintf(fp, "components new AMQueueCtrlP(uniqueCount(UQ_%s_QUEUE)) as %sSendQueueP;\n",
					mp->id_name,
					mp->lib->name);
			fprintf(fp, "FennecEngineP.SplitControl[%s] -> %sC.SplitControl;\n",
					mp->id_name,
					mp->lib->name);
			fprintf(fp, "%sC.%sParams -> FennecEngineP.%sParams;\n", 
					mp->lib->name,
					mp->lib->name,
					mp->lib->name);
			fprintf(fp, "%sC.AMQueueControl -> %sSendQueueP;\n", 
					mp->lib->name,
					mp->lib->name);
			fprintf(fp, "%sSendQueueP.AMSend -> %sC.AMSend;\n",
                                        mp->lib->name,
                                        mp->lib->name);
			fprintf(fp, "%sSendQueueP.AMPacket -> %sC.AMPacket;\n",
                                        mp->lib->name,
                                        mp->lib->name);
			fprintf(fp, "%sSendQueueP.Packet -> %sC.Packet;\n",
                                        mp->lib->name,
                                        mp->lib->name);
			fprintf(fp, "\n");
                }
        }
        fprintf(fp, "\n");

	for( i = 0; i < conf_id_counter; i++ ) {
  		fprintf(fp, "/* Process %s */\n\n", 
					conftab[i].conf->id->name);
 		fprintf(fp, "components new %sC(%s) as %s_%s;\n",
					conftab[i].conf->app->lib->name,
					conftab[i].conf->id_name,
					conftab[i].conf->id->name,
					conftab[i].conf->app->lib->name);
      		fprintf(fp, "FennecEngineP.SplitControl[%s] -> %s_%s;\n",
					conftab[i].conf->app_id_name,
					conftab[i].conf->id->name,
					conftab[i].conf->app->lib->name);
      		fprintf(fp, "FennecEngineP.%sParams[%s] <- %s_%s.%sParams;\n",
					conftab[i].conf->app->lib->name,
					conftab[i].conf->id_name,
					conftab[i].conf->id->name,
					conftab[i].conf->app->lib->name, 
					conftab[i].conf->app->lib->name);

		fprintf(fp, "\n");

		fprintf(fp, "components new %sC(%s) as %s_%s;\n",
					conftab[i].conf->net->lib->name,
					conftab[i].conf->id_name,
					conftab[i].conf->id->name,
					conftab[i].conf->net->lib->name);
      		fprintf(fp, "FennecEngineP.SplitControl[%s] -> %s_%s;\n",
					conftab[i].conf->net_id_name,
					conftab[i].conf->id->name,
					conftab[i].conf->net->lib->name); 
      		fprintf(fp, "FennecEngineP.%sParams[%s] <- %s_%s.%sParams;\n", 
					conftab[i].conf->net->lib->name, 
					conftab[i].conf->id_name,
					conftab[i].conf->id->name,
					conftab[i].conf->net->lib->name, 
					conftab[i].conf->net->lib->name);

		fprintf(fp, "\n");

      	
		if (conftab[i].conf->app->lib->type == TYPE_EVENT) {
			fprintf(fp, "%s_%s.Event -> FennecC.Event;\n\n",
					conftab[i].conf->id->name,
					conftab[i].conf->app->lib->name);
		}

     		fprintf(fp, "%s_%s.SubAMSend -> %s_%s.AMSend;\n",
					conftab[i].conf->id->name,
					conftab[i].conf->app->lib->name,
					conftab[i].conf->id->name,
					conftab[i].conf->net->lib->name);
      		fprintf(fp, "%s_%s.SubReceive -> %s_%s.Receive;\n",
					conftab[i].conf->id->name,
					conftab[i].conf->app->lib->name,
					conftab[i].conf->id->name,
					conftab[i].conf->net->lib->name);
      		fprintf(fp, "%s_%s.SubSnoop -> %s_%s.Snoop;\n",
					conftab[i].conf->id->name,
					conftab[i].conf->app->lib->name,
					conftab[i].conf->id->name,
					conftab[i].conf->net->lib->name);
      		fprintf(fp, "%s_%s.SubPacket -> %s_%s.Packet;\n",
					conftab[i].conf->id->name,
					conftab[i].conf->app->lib->name,
					conftab[i].conf->id->name,
					conftab[i].conf->net->lib->name);
      		fprintf(fp, "%s_%s.SubAMPacket -> %s_%s.AMPacket;\n",
					conftab[i].conf->id->name,
					conftab[i].conf->app->lib->name,
					conftab[i].conf->id->name,
					conftab[i].conf->net->lib->name);
      		fprintf(fp, "%s_%s.SubPacketAcknowledgements -> %s_%s.PacketAcknowledgements;\n",
					conftab[i].conf->id->name,
					conftab[i].conf->app->lib->name, 
					conftab[i].conf->id->name,
					conftab[i].conf->net->lib->name);
      		fprintf(fp, "%s_%s.SubPacketLinkQuality -> %s_%s.PacketLinkQuality;\n",
					conftab[i].conf->id->name,
					conftab[i].conf->app->lib->name, 
					conftab[i].conf->id->name,
					conftab[i].conf->net->lib->name);
      		fprintf(fp, "%s_%s.SubPacketTransmitPower -> %s_%s.PacketTransmitPower;\n",
					conftab[i].conf->id->name,
					conftab[i].conf->app->lib->name, 
					conftab[i].conf->id->name,
					conftab[i].conf->net->lib->name);
      		fprintf(fp, "%s_%s.SubPacketRSSI -> %s_%s.PacketRSSI;\n",
					conftab[i].conf->id->name,
					conftab[i].conf->app->lib->name, 
					conftab[i].conf->id->name,
					conftab[i].conf->net->lib->name);

		fprintf(fp, "\n");

		fprintf(fp, "components new AMQueueEntryP(%s) as %sAMQueueEntryP;\n",
					conftab[i].conf->id_name,
					conftab[i].conf->id->name);
      		fprintf(fp, "%s_%s.SubAMSend -> %sAMQueueEntryP.AMSend;\n",
					conftab[i].conf->id->name,
					conftab[i].conf->net->lib->name,
					conftab[i].conf->id->name);
      		fprintf(fp, "%sAMQueueEntryP.AMPacket -> %sC.AMPacket;\n",
					conftab[i].conf->id->name,
					conftab[i].conf->am->lib->name);
      		fprintf(fp, "%sAMQueueEntryP.Send -> %sSendQueueP.Send[unique(UQ_%s_QUEUE)];\n",
					conftab[i].conf->id->name,
					conftab[i].conf->am->lib->name,
					conftab[i].conf->am->id_name);
      		fprintf(fp, "%s_%s.SubReceive -> %sC.Receive[%s];\n",
					conftab[i].conf->id->name,
					conftab[i].conf->net->lib->name, 
					conftab[i].conf->am->lib->name,
					conftab[i].conf->id_name);
      		fprintf(fp, "%s_%s.SubSnoop -> %sC.Snoop[%s];\n",
					conftab[i].conf->id->name,
					conftab[i].conf->net->lib->name,
					conftab[i].conf->am->lib->name,
					conftab[i].conf->id_name);
      		fprintf(fp, "%s_%s.SubAMPacket -> %sC.AMPacket;\n",
					conftab[i].conf->id->name,
					conftab[i].conf->net->lib->name,
					conftab[i].conf->am->lib->name);
      		fprintf(fp, "%s_%s.SubPacket -> %sC.Packet;\n",
					conftab[i].conf->id->name,
					conftab[i].conf->net->lib->name,
					conftab[i].conf->am->lib->name);
      		fprintf(fp, "%s_%s.SubPacketAcknowledgements -> %sC.PacketAcknowledgements;\n",
					conftab[i].conf->id->name,
					conftab[i].conf->net->lib->name,
					conftab[i].conf->am->lib->name);
      		fprintf(fp, "%s_%s.SubLinkPacketMetadata -> %sC.LinkPacketMetadata;\n",
					conftab[i].conf->id->name,
					conftab[i].conf->net->lib->name,
					conftab[i].conf->am->lib->name);
      		fprintf(fp, "%s_%s.LowPowerListening -> %sC.LowPowerListening;\n",
					conftab[i].conf->id->name,
					conftab[i].conf->net->lib->name,
					conftab[i].conf->am->lib->name);
      		fprintf(fp, "%s_%s.RadioChannel -> %sC.RadioChannel;\n",
					conftab[i].conf->id->name,
					conftab[i].conf->net->lib->name,
					conftab[i].conf->am->lib->name);
      		fprintf(fp, "%s_%s.SubPacketLinkQuality -> %sC.PacketLinkQuality;\n",
					conftab[i].conf->id->name,
					conftab[i].conf->net->lib->name,
					conftab[i].conf->am->lib->name);
      		fprintf(fp, "%s_%s.SubPacketTransmitPower -> %sC.PacketTransmitPower;\n",
					conftab[i].conf->id->name,
					conftab[i].conf->net->lib->name,
					conftab[i].conf->am->lib->name);
      		fprintf(fp, "%s_%s.SubPacketRSSI -> %sC.PacketRSSI;\n",
					conftab[i].conf->id->name,
					conftab[i].conf->net->lib->name,
					conftab[i].conf->am->lib->name);

		fprintf(fp, "\n");
	}

	fprintf(fp, "}\n");
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

	fprintf(fp, "/* Swift Fox generated code for Fennec Fox Computation module */\n");
  	fprintf(fp, "#include <Fennec.h>\n");
  	fprintf(fp, "module FennecEngineP {\n\n");
  	fprintf(fp, "provides interface ModuleCtrl;\n");

  	fprintf(fp, "uses interface Leds;\n");
  	fprintf(fp, "uses interface Fennec;\n\n");


	fprintf(fp, "uses interface SplitControl[module_t module_id];\n");

	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->lib->used) {
			if (mp->type == TYPE_AM) {
	      			fprintf(fp, "provides interface %sParams;\n", 
						mp->lib->name);
	                } else {
      				fprintf(fp, "provides interface %sParams[process_t process_id];\n", 
					mp->lib->name);
			}
		}
        }

	fprintf(fp,"}\n\n");
	fprintf(fp,"implementation {\n\n");


	fprintf(fp,"command error_t ModuleCtrl.start(module_t module_id) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"[-] FennecEngine ModuleCtrl.start(%%d)\", module_id);\n");
	fprintf(fp,"\treturn call SplitControl.start[module_id]();\n");
	fprintf(fp,"}\n\n");

	fprintf(fp,"command error_t ModuleCtrl.stop(module_t module_id) {\n");
	fprintf(fp,"\tdbg(\"FennecEngine\", \"[-] FennecEngine ModuleCtrl.stop(%%d)\", module_id);\n");
	fprintf(fp,"\treturn call SplitControl.stop[module_id]();\n");
	fprintf(fp,"}\n\n");

	struct paramtype *pt;

	/* Interfaces with Computations */

	fprintf(fp, "event void SplitControl.startDone[module_t module_id](error_t error) {\n");
	fprintf(fp, "\tdbg(\"FennecEngine\", \"[-] FennecEngine SplitControl.startDone[%%d](%%d)\", module_id, error);\n");
	fprintf(fp, "\tsignal ModuleCtrl.startDone(error);\n");
	fprintf(fp, "}\n\n");
	fprintf(fp, "event void SplitControl.stopDone[module_t module_id](error_t error) {\n");
	fprintf(fp, "\tdbg(\"FennecEngine\", \"[-] FennecEngine SplitControl.stopDone[%%d](%%d)\", module_id, error);\n");
	fprintf(fp, "\tsignal ModuleCtrl.stopDone(error);\n");
	fprintf(fp, "}\n\n");

	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->lib->used) {
			/* check if the interface is empty, if it is add dummy call */
			if (mp->lib->params == NULL) {
				if (mp->type == TYPE_AM) {
					fprintf(fp, "async command void %sParams.dummy() {}\n\n",
						mp->lib->name);
				} else {
					fprintf(fp, "command void %sParams.dummy[process_t process_id]() {}\n\n",
						mp->lib->name);
				}
			} else {
				for (pt = mp->lib->params; pt != NULL; pt = pt->child ) {
					if (mp->type == TYPE_AM) {
						fprintf(fp, "async command %s %sParams.get_%s() {\n",
							type_name(pt->type), 
							mp->lib->name, 
							pt->name);
						fprintf(fp, "\tprocess_t process_id = call Fennec.getProcessIdFromAM(%s);\n",
							mp->id_name);
					} else {
						fprintf(fp, "command %s %sParams.get_%s[process_t process_id]() {\n",
							type_name(pt->type), 
							mp->lib->name, 
							pt->name);
					}

					if ((mp->type == TYPE_APPLICATION) || (mp->type == TYPE_EVENT)) {
						fprintf(fp, "\treturn *((struct %s_params_ptr*)(processes[process_id].application_params))->%s;\n",
							mp->lib->name,
							pt->name);
					}
					if (mp->type == TYPE_NETWORK) {
						fprintf(fp, "\treturn *((struct %s_params_ptr*)(processes[process_id].network_params))->%s;\n",
							mp->lib->name,
							pt->name);
					}
					if (mp->type == TYPE_AM) {
						fprintf(fp, "\treturn *((struct %s_params_ptr*)(processes[process_id].am_params))->%s;\n",
							mp->lib->name,
							pt->name);
					}

					fprintf(fp, "}\n\n");

					if (mp->type == TYPE_AM) {
						fprintf(fp, "async command void %sParams.set_%s(%s new_%s) {\n",
							mp->lib->name, 
							pt->name, 
							type_name(pt->type), 
							pt->name);
						fprintf(fp, "\tprocess_t process_id = call Fennec.getProcessIdFromAM(%s);\n",
							mp->id_name);
					} else {
						fprintf(fp, "command void %sParams.set_%s[process_t process_id](%s new_%s) {\n",
							mp->lib->name, 
							pt->name, 
							type_name(pt->type), 
							pt->name);
					}

					if ((mp->type == TYPE_APPLICATION) || (mp->type == TYPE_EVENT)) {
						fprintf(fp, "\t*((struct %s_params_ptr*)(processes[process_id].application_params))->%s = new_%s;\n",
							mp->lib->name,
							pt->name,
							pt->name);
					}
					if (mp->type == TYPE_NETWORK) {
						fprintf(fp, "\t*((struct %s_params_ptr*)(processes[process_id].network_params))->%s = new_%s;\n",
							mp->lib->name,
							pt->name,
							pt->name);
					}
					if (mp->type == TYPE_AM) {
						fprintf(fp, "\t*((struct %s_params_ptr*)(processes[process_id].am_params))->%s = new_%s;\n",
							mp->lib->name,
							pt->name,
							pt->name);
					}
					fprintf(fp, "}\n\n");
				}
			}
	        }
	}

	fprintf(fp, "default command error_t SplitControl.start[module_t module_id]() { return FAIL; }\n");
	fprintf(fp, "default command error_t SplitControl.stop[module_t module_id]() { return FAIL; }\n");

	fprintf(fp, "\n}\n");
	fclose(fp);
	free(full_path);
}

