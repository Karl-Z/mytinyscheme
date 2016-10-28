#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#include "scheme.h"
#include "scheme-private.h"

int main(int argc, char ** argv) {
        scheme *sc;
        pointer p1;

        sc = scheme_init_new();
        assert (sc);
        scheme_set_input_port_file (sc, stdin);
        scheme_set_output_port_file (sc, stdout);


        if (argc > 1) {
                if (access (argv[1], R_OK) != 0) {
                        perror ("cannot load file");
                        return 1;
                        // exit (1);
                }

                FILE * fp = fopen (argv[1], "r");
                assert (fp);
                pointer symvar = mk_symbol (sc, "param1");
                scheme_define (sc, sc->global_env, symvar,
                        mk_string(sc, "default value"));
                puts ("param1=default value");

                scheme_load_named_file (sc, fp, argv[1]);
                p1 = variable_ref (sc, sc->global_env, symvar);
                if (p1 != sc->NIL) {
                        printf ("param1=%s\n", string_value(p1));
                }

        }

        scheme_deinit (sc);
        return 0;
}

