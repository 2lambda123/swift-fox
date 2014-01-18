/** 
    Swift Fox Compiler
    Copyright (C) 2009-2013 Marcin Szczodrak

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

/**
creates ff_defaults.h file
the file stores default parameters for application module
*/
void generateDefaultParams() {
        char *full_path = get_sfc_path("", "ff_defaults.h");
        FILE *fp = fopen(full_path, "w");
	struct paramvalue *pv;
	struct paramtype *pt;
	struct modtab *mp;
	int i;

        if (fp == NULL) {
                fprintf(stderr, "You do not have a permission to write \
                                                into file: %s\n", full_path);
                exit(1);
        }

	fprintf(fp, "#ifndef _FF_DEFAULTS_H_\n");
	fprintf(fp, "#define _FF_DEFAULTS_H_\n\n");
	fprintf(fp, "#include <Fennec.h>\n");
	fprintf(fp, "#include \"ff_globals.h\"\n");

	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if (mp->lib != NULL && mp->lib->path && mp->lib->used) {
			fprintf(fp, "#include \"%sParams.h\"\n", 
						mp->lib->name);
		}
	}
	fprintf(fp, "\n");

	/** 
	generate structure with default parameters for each module of a given configuration 
	*/
	for( i = 0; i < conf_id_counter; i++ ) {
		/** 
		application default parameter values 
		*/
		fprintf(fp, "struct %s_params %s_params = {\n",
        	        conftab[i].conf->app->lib->name,
			conftab[i].conf->app_id_name);


                for (pv = conftab[i].conf->app_params; pv != NULL; ) {
                       	fprintf(fp, "\t%f", pv->num_value);

                        pv = pv->child;
                        if (pv == NULL)
                                fprintf(fp, "\n");
                        else
                                fprintf(fp, ",\n");
                }
                fprintf(fp, "};\n\n");

                /** 
		network default parameter values 
		*/
                fprintf(fp, "struct %s_params %s_params = {\n",
                        conftab[i].conf->net->lib->name,
			conftab[i].conf->net_id_name);

                for (pv = conftab[i].conf->net_params; pv != NULL; ) {
                        fprintf(fp, "\t%f", pv->num_value);

                        pv = pv->child;
                        if (pv == NULL)
                                fprintf(fp, "\n");
                        else
                                fprintf(fp, ",\n");
                }
                fprintf(fp, "};\n\n");

		/** 
		mac default parameter values 
		*/
                fprintf(fp, "struct %s_params %s_params = {\n",
                        conftab[i].conf->mac->lib->name,
			conftab[i].conf->mac_id_name);

                for (pv = conftab[i].conf->mac_params; pv != NULL; ) {
                        fprintf(fp, "\t%f", pv->num_value);
                        
                        pv = pv->child;         
                        if (pv == NULL)
                                fprintf(fp, "\n");
                        else    
                                fprintf(fp, ",\n");
                }               
                fprintf(fp, "};\n\n");

		/** 
		radio default parameter values 
		*/
                fprintf(fp, "struct %s_params %s_params = {\n",
                        conftab[i].conf->radio->lib->name,
			conftab[i].conf->radio_id_name);

                for (pv = conftab[i].conf->radio_params; pv != NULL; ) {
                        fprintf(fp, "\t%f", pv->num_value);
                                                
                        pv = pv->child;         
                        if (pv == NULL)
                                fprintf(fp, "\n");
                        else    
                                fprintf(fp, ",\n");
                }
                fprintf(fp, "};\n\n");
	}



	/** 
	generate structure with default parameters for each module of a given configuration 
	*/
	for( i = 0; i < conf_id_counter; i++ ) {
		/** 
		application default parameter values 
		*/
		fprintf(fp, "struct %s_params_ptr %s_ptr = {\n",
        	        conftab[i].conf->app->lib->name,
			conftab[i].conf->app_id_name);

                for (pv = conftab[i].conf->app_params, pt = conftab[i].conf->app->lib->params;
			pv != NULL && pt != NULL; pv = pv->child) {
			if (pv->value == NULL) {
				/* this is fixed number so point to module's struct params */
                        	fprintf(fp, "\t&%s_params.%s",
					conftab[i].conf->app_id_name,
					pt->name);
			} else {
				/* this is global variable */
				fprintf(fp, "\t%s", pv->value->name);

			}

                        pt = pt->child;
                        if (pt == NULL)
                                fprintf(fp, "\n");
                        else
                                fprintf(fp, ",\n");
                }
                fprintf(fp, "};\n\n");

                /** 
		network default parameter values 
		*/
                fprintf(fp, "struct %s_params_ptr %s_ptr = {\n",
                        conftab[i].conf->net->lib->name,
			conftab[i].conf->net_id_name);

                for (pv = conftab[i].conf->net_params, pt = conftab[i].conf->net->lib->params;
			pv != NULL && pt != NULL; pv = pv->child) {
			if (pv->value == NULL) {
				/* this is fixed number so point to module's struct params */
                        	fprintf(fp, "\t&%s_params.%s",
					conftab[i].conf->net_id_name,
					pt->name);
			} else {
				/* this is global variable */
				fprintf(fp, "\t%s", pv->value->name);

			}

                        pt = pt->child;
                        if (pt == NULL)
                                fprintf(fp, "\n");
                        else
                                fprintf(fp, ",\n");
                }
                fprintf(fp, "};\n\n");

		/** 
		mac default parameter values 
		*/
                fprintf(fp, "struct %s_params_ptr %s_ptr = {\n",
                        conftab[i].conf->mac->lib->name,
			conftab[i].conf->mac_id_name);

                for (pv = conftab[i].conf->mac_params, pt = conftab[i].conf->mac->lib->params;
			pv != NULL && pt != NULL; pv = pv->child) {
			if (pv->value == NULL) {
				/* this is fixed number so point to module's struct params */
                        	fprintf(fp, "\t&%s_params.%s",
					conftab[i].conf->mac_id_name,
					pt->name);
			} else {
				/* this is global variable */
				fprintf(fp, "\t%s", pv->value->name);

			}

                        pt = pt->child;
                        if (pt == NULL)
                                fprintf(fp, "\n");
                        else
                                fprintf(fp, ",\n");
                }
                fprintf(fp, "};\n\n");

		/** 
		radio default parameter values 
		*/
                fprintf(fp, "struct %s_params_ptr %s_ptr = {\n",
                        conftab[i].conf->radio->lib->name,
			conftab[i].conf->radio_id_name);

                for (pv = conftab[i].conf->radio_params, pt = conftab[i].conf->radio->lib->params;
			pv != NULL && pt != NULL; pv = pv->child) {
			if (pv->value == NULL) {
				/* this is fixed number so point to module's struct params */
                        	fprintf(fp, "\t&%s_params.%s",
					conftab[i].conf->radio_id_name,
					pt->name);
			} else {
				/* this is global variable */
				fprintf(fp, "\t%s", pv->value->name);

			}

                        pt = pt->child;
                        if (pt == NULL)
                                fprintf(fp, "\n");
                        else
                                fprintf(fp, ",\n");
                }
                fprintf(fp, "};\n\n");
	}

	fprintf(fp, "#endif\n\n");
	fclose(fp);
	free(full_path);
}
