#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <expat.h>
#include "redblack.h"

#define BUFFSZ 512
#define BITSHIFT 12
long instructioncnt;
void* redblacktree;
char* outputprefix;
char writeoutname[BUFFSZ];


//Copyright Adrian McMenamin, 2014
//Licensed for use under GNU GPL v2
//or any later version at your discretion

static void XMLCALL
	refhandler(void *data, const XML_Char *name, const XML_Char **attr)
{
	int i;
	long page, address, threadno;
	if (strcmp(name, "instruction") == 0 || strcmp(name, "load") == 0 ||
		strcmp(name, "store") == 0 || strcmp(name, "modify") == 0) {
		instructioncnt++;
		for (i = 0; attr[i]; i += 2) {
			if (strcmp(attr[i], "thread") == 0) {
				threadno = strtol(attr[i + 1], NULL, 10);
				sprintf(writeoutname, "%s%lu.bin",
					outputprefix, threadno);
			}				
			if (strcmp(attr[i], "address") == 0) {
				address = strtol(attr[i + 1], NULL, 16);
				page = address >> BITSHIFT;
				insertinstruction(page, instructioncnt,
					redblacktree, getroot(redblacktree));
			}
		}
	}

}

static void XMLCALL
	starthandler(void *data, const XML_Char *name, const XML_Char **attr)
{
	FILE *referenceXML;
	char buffer[BUFFSZ];
	int i;
	int done;
	int len;

	if (strcmp(name, "file") == 0) {
		for (i = 0; attr[i]; i += 2) {
			if (strcmp(attr[i], "path") == 0) {
				XML_Parser refparse =
					XML_ParserCreate("UTF-8");
				if (!refparse) {
					fprintf(stderr,
						"Could not create parser\n");
					return;
				}
				XML_SetStartElementHandler(refparse,
						refhandler);
				referenceXML = fopen(attr[i + 1], "r");
				if (referenceXML == NULL) {
					fprintf(stderr,
						"Could not open %s\n",
						attr[i + 1]);
					XML_ParserFree(refparse);
					return;
				}
				instructioncnt = 0;
				redblacktree = createtree();
				do {
					len = fread(buffer, 1, sizeof(buffer),
						referenceXML);
					done = len < sizeof(buffer);
					if (XML_Parse(refparse, buffer, len, 0)
							== 0) {
						enum XML_Error errcde =
							XML_GetErrorCode(refparse);
						printf("ERROR: %s\n",
							XML_ErrorString(errcde));
						printf(
					"Error at column number %lu\n",
					XML_GetCurrentColumnNumber(refparse));
						printf(
					"Error at line number %lu\n",
					XML_GetCurrentLineNumber(refparse));
					}
				} while(!done);
				printf("Now writing %s ... ", writeoutname);
				FILE *pageout = fopen(writeoutname, "w");
				writeinorder(redblacktree, pageout);
				printf("complete.\n");
				removetree(redblacktree);
				fclose(pageout);
			}
		}
	}
}

int main(int argc, char *argv[])
{
	FILE* inXML;
	char data[BUFFSZ]; 
	size_t len = 0;
	int done;

	strcpy(outputprefix, argv[2]);

	XML_Parser p_ctrl = XML_ParserCreate("UTF-8");
	if (!p_ctrl) {
		fprintf(stderr, "Could not create parser\n");
		exit(-1);
	}

	XML_SetStartElementHandler(p_ctrl, starthandler);
	inXML = fopen(argv[1], "r");
	if (inXML == NULL) {
		fprintf(stderr, "Could not open %s\n", argv[1]);
		XML_ParserFree(p_ctrl);
		exit(-1);
	}

	do {
		len = fread(data, 1, sizeof(data), inXML);
		done = len < sizeof(data);

		if (XML_Parse(p_ctrl, data, len, 0) == 0) {
			enum XML_Error errcde = XML_GetErrorCode(p_ctrl);
			printf("ERROR: %s\n", XML_ErrorString(errcde));
			printf("Error at column number %lu\n", XML_GetCurrentColumnNumber(p_ctrl));
			printf("Error at line number %lu\n", XML_GetCurrentLineNumber(p_ctrl));
		}
	} while(!done);

	XML_ParserFree(p_ctrl);
	fclose(inXML);
	return 0;
}
