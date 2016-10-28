#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#ifndef USE_DL
#  define USE_DL 1
#endif

#include "scheme.h"
#include "scheme-private.h"

scheme *sc;

char* parameters[] = {
"ftp-server", "localhost",
"ftp-port", "21",
"ftp-user", "anonymous",
"ftp-pass", "ftp@localhost"};

#ifndef CONFIG_FILE
#  define CONFIG_FILE "/etc/cups/ftpbackend.conf"
#endif

// util funcs
// 2 elements every loop
void foreach2 (void** array, int arrlen, void (*call_back)(void*, void*)) {
	assert (array);
	assert (*array);
	assert (call_back);
	assert (arrlen % 2 == 0);

	int i;
	for (i=0; i<arrlen; i+=2) {
		(*call_back) (array[i], array[i+1]);
	}
}

	
void init_parameters (scheme *sc) {
	assert (sc);
	int i;

	for (i=0; i< sizeof parameters / sizeof parameters[0]; i+=2) {
		scheme_define (sc, sc->global_env,
			mk_symbol (sc, parameters[i]),
			mk_string (sc, parameters[i+1]));
	}

}

// init scheme engine
void start_scheme (scheme **scptr) {
        *scptr = scheme_init_new();
        assert (*scptr);

        scheme_set_input_port_file (*scptr, stdin);
        scheme_set_output_port_file (*scptr, stdout);
#ifdef USE_REGEX
	// use re extension
	init_re (*scptr);
#endif
#ifdef USE_TSX
	// use tsx extension
	init_tsx (*scptr);
#endif
	init_parameters (*scptr);
}

void stop_scheme (scheme **sc) {
	assert (*sc);
        scheme_deinit (*sc);
	*sc = NULL;
}
	

void display2 (void *arg1, void *arg2) {
	printf ("%s: %s\n", arg1, arg2);
}
	
//void load_config (void);

//void connect_server (server serv);
//void upload_file (connection con);

int main(int argc, char ** argv) {
	char buf[256];
        pointer p1;
	int i;

	if (access (CONFIG_FILE, R_OK) != 0) {
		perror ("cannot load file " CONFIG_FILE);
		return 1;
	}

	start_scheme (&sc);

	FILE * fp = fopen (CONFIG_FILE, "r");
	assert (fp);

#ifndef NDEBUG
	puts ("Parameters default value:");
	foreach2 ((void**) parameters, sizeof parameters/sizeof parameters[0], &display2);
#endif
	scheme_load_named_file (sc, fp, CONFIG_FILE);
#ifndef NDEBUG
	puts ("Parameters read in Scheme:");
	for (i=0; i< sizeof parameters/sizeof parameters[0]; i+=2) {
		snprintf(buf, 256, "(display \"%s: \") (display %s) (newline)",
			parameters[i], parameters[i]);
		scheme_load_string (sc, buf);
	}
	
	puts ("");
	puts ("Parameters read in C:");
	for (i=0; i< sizeof parameters/sizeof parameters[0]; i+=2) {
		pointer p = variable_ref (sc, sc->global_env, (mk_symbol (sc, parameters[i])));
		if (is_string (p))
			printf ("%s: %s\n", parameters[i], string_value (p));
		else 
			printf ("%s: UNKNOWN\n", parameters[i]); 
	}
#endif
	
	stop_scheme (&sc);

        return 0;
}

