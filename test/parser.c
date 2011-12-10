#include "parser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct Value values[30];
int values_index = 0;

void declaration(char * id) {
	struct Value value;

	value.id = (char *) malloc(strlen(id));
	strcpy(value.id, id);
	value.value = 0;

	values[values_index] = value;
	values_index++;
}
int allocation(char * id, double value) {
	struct Value var;
	int i;

	for(i = 0; i<values_index; i++) {
		var = values[i];
		if(strcmp(id, var.id) == 0) {
			var.value = value;
			values[i] = var;
			return 1;
		}
	}
	return 0;
}
double get_value(char * id) {
	struct Value var;
        int i;

        for(i = 0; i<values_index; i++) {
                var = values[i];
                if(strcmp(id, var.id) == 0) {
                        return var.value;
                }
        }
}
