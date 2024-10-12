#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

typedef struct _statement {
	char read;
	char state;
	char write;
	char new_state;
	char dir;
	struct _statement* next;

} statement;

statement* prog; /* first statement ptr */

char* tape;
int pos; /* current caret position */
int tape_sz; /* tape capacity */
char state; /* current caret state */

int load_program(FILE* f, int line, statement** stmt) {
	char read, state, write, ns, dir;
	int sc;
	if((sc = fscanf(f, ":%c%c:%c%c%c\n", &read, &state, &write, &ns, &dir)) == 5) {
		if ( dir == '<' || dir == '>') {
			statement* n  = (statement*)malloc(sizeof(statement));
			if ( !n ) return -1;
			*n = (statement){read, state, write, ns, dir, *stmt};
			*stmt = n;
			fprintf(stderr, "statement loaded: '%c', '%c' : '%c' '%c' '%c'\n",
						n->read, n->state, n->write, n->new_state, n->dir); 
			return load_program(f, line+1, stmt);
		}
		fprintf(stderr, "Bad format at line %d\n", line);
		return -2;
	} 
	fprintf(stderr, "Scanned: %d: '%c', '%c', '%c', Program fininshed at line %d\n", sc, read, state, write, line);
	return 0;
}


void free_program(statement* prog) {
	if (prog) {
		statement* n = prog->next;
		free(prog);
		free_program(n);
	}
}

#define POS2ADDR(p)  ((p)>=0?2*(p):-2*(p)-1)
void ensure_size(int p) {
	if (p >= tape_sz || -p >= tape_sz-1)  {
		fprintf(stderr, "Reallocating tape for %d  to %d\n", p, tape_sz);
		tape = (void*)realloc(tape, tape_sz*4);
		memset(tape+tape_sz*2, ' ', tape_sz*2);
		tape_sz*=2;
	}
}


void tape_set(int p, char c) {
	ensure_size(p);
	tape[POS2ADDR(p)] = c;
}

char tape_get(int p) {
	ensure_size(p);
	return tape[POS2ADDR(p)];
}

int load_tape(FILE*  f) {
	/* preallocate tape */
	tape_sz = 1024;
	tape = (void*)calloc(tape_sz, 2);
	memset(tape, ' ', tape_sz*2);
	pos = 0;
	int c;
	while((c = fgetc(f)) != '\n')
		tape_set(pos++, c);
	pos = 0;
	while(fgetc(f) != '^') pos++; /* find caret symbol */
	state = fgetc(f); /* state label should be placed after caret */
	fprintf(stderr, "Caret at pos: %d, state: '%c'\n", pos, state);
	return 0;	
}

void print_tape() {
	int p;
	for( p = pos -20; p < pos+30; p++ )
		putchar(tape_get(p));
	putchar('\n');
	for( p = pos- 20; p <= pos; p++ ) 
		putchar(p == pos?'^':' ');
	putchar(state);
	putchar('\n');
}




int find_statement(statement *stmt, char r) {
	if  (!stmt) return 0;
	if ( stmt->read == r && stmt->state == state ) {
		/* do */
		fprintf(stderr, "Statement found: '%c%c'\n", stmt->read, stmt->state);
		tape_set(pos, stmt->write);
		pos += (stmt->dir=='<')?-1:1;
		state = stmt->new_state;
		print_tape();
		return 1;
	}
	return find_statement(stmt->next, r);
}


void print_program(statement* stmt) {
	if ( stmt ) {
		printf("<'%c', '%c'> -> '%c', '%c', %c\n", stmt->read, stmt->state,
				stmt->write, stmt->new_state, stmt->dir);
		print_program(stmt->next);
	} else {
		printf("\n");
	}

}

void interpret() {
	fprintf(stderr, "Fiding statement for pos %d '%c'\n", pos, tape_get(pos));
	if ( find_statement(prog, tape_get(pos)) ) { 
		interpret();
	}else {
		fprintf(stderr, "Program finished\n");
		print_tape();
	}
}
	
int main(int argc, char** argv) {
	if ( argc > 2 ) {
		prog = NULL;
		FILE* f = fopen(argv[1], "r");
		if ( !f ) {
			fprintf(stderr, "Cannot open program file %s\n", argv[1]);
			exit(1);
		}
		if (load_program(f, 1, &prog)) {
			free_program(prog);
			fprintf(stderr, "Error while loading program\n");
			fclose(f);
			exit(1);
		}
		fclose(f);
		print_program(prog);
		f = fopen(argv[2], "r");
		if ( !f ) {
			fprintf(stderr, "Cannot open tape file %s\n", argv[2]);
			free_program(prog);
			exit(1);
		}
		load_tape(f);
		fclose(f);
		interpret();
		exit(0);
	} else {
		fprintf(stderr, "Usage: %s <program> <tape>\n", argv[0]);
		exit(2);
	}
}



