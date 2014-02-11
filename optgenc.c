#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <expat.h>

#define BUFFSZ 512

//Copyright Adrian McMenamin, 2014
//Licensed for use under GNU GPL v2
//or any later version at your discretion

static void XMLCALL
	starthandler(void *data, const XML_Char *name, const XML_Char **attr)
{
	int i;
	if (strcmp(name, "file") == 0) {
		for (i = 0; attr[i]; i += 2) {
			if (strcmp(attr[i], "path") == 0) {
				printf("File is at %s\n", attr[i + 1]);
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
