#include <stdio.h>
#include <expat.h>

//Copyright Adrian McMenamin, 2014
//Licensed for use under GNU GPL v2
//or any later version at your discretion

static void XMLCALL
	start(void* data, XML_Char* name, XML_Char** attr)
{
	int i;

	if (strcmp(name, "thread") == 0) {
		for (i = 0; attr[i]; i += 2) {
			if (strcmp(attr[i], "path") == 0) {
				printf("File is at %s\n", attr[i + 1]);
			}
		}
	}
}		

int main(int argc, char* agrv[])
{
	FILE* inXML;
	ssize_t read;
	char* line = NULL;
	size_t len = 0;

	XML_Parser p_ctrl = XML_Parser(NULL);
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

	while ((read = getline(&line, &len, inXML)) != -1) {
		XML_Parse(p_ctrl, line, len, false);
		free(line);
	}

	XML_ParserFree(p_ctrl);

	return 1;
}
