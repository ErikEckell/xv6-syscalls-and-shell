//
// test program for the shell lab.
// run it in xv6 like this:
// $ testsh nsh
//

#include "types.h"
#include "user.h"
#include "fcntl.h"

#define MAX_CMDS 10
#define MAX_ARGS 100

#define O_TRUNC 0x400 // Definición manual para O_APPEND

// Estructura para almacenar un comando parseado
struct command {
    char *argv[MAX_ARGS];  // Lista de argumentos
    char *input_redir;     // Archivo para redirección de entrada
    char *output_redir;    // Archivo para redirección de salida
    int pipe_in;           // Descriptor de archivo para entrada de tubería
    int pipe_out;          // Descriptor de archivo para salida de tubería
};

// Estructura para almacenar una lista de comandos
struct command_list {
    struct command *cmds[MAX_CMDS];
    int cmd_count;
};

// Declaración de funciones
int myfork(void);
void runcmd(struct command_list *cmd_list);

// Función que parsea la entrada en varios comandos separados por ';', '&', y tuberías '|'
struct command_list* parsecmd(char *input) {
    static struct command_list cmd_list;
    int i = 0;

    // Inicializa la estructura del comando
    for (i = 0; i < MAX_CMDS; i++) {
        cmd_list.cmds[i] = 0;
    }
    cmd_list.cmd_count = 0;

    // Eliminar el salto de línea al final, si existe
    int len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    while (*input != '\0' && cmd_list.cmd_count < MAX_CMDS) {
        struct command *cmd = malloc(sizeof(struct command));
        if (!cmd) {
            printf(2, "Error: no se pudo asignar memoria\n");
            exit();
        }

        for (int j = 0; j < MAX_ARGS; j++) {
            cmd->argv[j] = 0;
        }
        cmd->input_redir = 0;
        cmd->output_redir = 0;
        cmd->pipe_in = -1;
        cmd->pipe_out = -1;

        int arg_count = 0;
        int in_redir = 0;
        int out_redir = 0;

        while (*input != '\0' && *input != ';' && *input != '&') {
            // Saltar espacios
            while (*input == ' ' || *input == '\t') {
                *input++ = '\0';
            }

            // Redirección de entrada
            if (*input == '<') {
                in_redir = 1;
                *input++ = '\0';
                continue;
            }

            // Redirección de salida
            if (*input == '>') {
                out_redir = 1;
                *input++ = '\0';
                continue;
            }

            if (*input != '\0') {
                if (in_redir) {
                    cmd->input_redir = input;  // Archivo de entrada
                    in_redir = 0;
                } else if (out_redir) {
                    cmd->output_redir = input; // Archivo de salida
                    out_redir = 0;
                } else {
                    cmd->argv[arg_count++] = input;
                }
            }

            // Avanzar hasta el siguiente delimitador
            while (*input != '\0' && *input != ' ' && *input != '\t' && *input != ';' && *input != '&') {
                input++;
            }
        }

        cmd->argv[arg_count] = 0; // Finalizar el arreglo de argumentos
        cmd_list.cmds[cmd_list.cmd_count++] = cmd;

        // Manejo de delimitadores
        if (*input == ';' || *input == '&') {
            *input++ = '\0';
        }
    }

    return &cmd_list;
}



void runcmd(struct command_list *cmd_list) {
    int i;

    for (i = 0; i < cmd_list->cmd_count; i++) {
        struct command *cmd = cmd_list->cmds[i];

        if (cmd->argv[0] == 0) {
            continue; // No hay comando que ejecutar
        }

        int pid = myfork();
        if (pid == 0) {
            // Proceso hijo

            // Manejo de redirección de entrada
            if (cmd->input_redir) {
                int fd_in = open(cmd->input_redir, O_RDONLY);
                if (fd_in < 0) {
                    printf(2, "Error: no se pudo abrir el archivo de entrada %s\n", cmd->input_redir);
                    exit();
                }
                // Redirigir stdin (entrada estándar)
                close(0);  // Cerramos stdin (descriptor 0)
                dup(fd_in);  // Duplicamos el fd_in en 0 (stdin)
                close(fd_in);  // Cerramos el descriptor original
            }

            // Manejo de redirección de salida
            if (cmd->output_redir) {
                int fd_out = open(cmd->output_redir, O_WRONLY | O_CREATE | O_TRUNC);
                if (fd_out < 0) {
                    printf(2, "Error: no se pudo abrir el archivo de salida %s\n", cmd->output_redir);
                    exit();
                }
                // Redirigir stdout (salida estándar)
                close(1);  // Cerramos stdout (descriptor 1)
                dup(fd_out);  // Duplicamos el fd_out en 1 (stdout)
                close(fd_out);  // Cerramos el descriptor original
            }

            // Ejecutar el comando
            if (exec(cmd->argv[0], cmd->argv) < 0) {
                printf(1, "Error: no se pudo ejecutar el comando %s\n", cmd->argv[0]);
                exit();
            }
        } else {
            // Proceso padre: esperar a que el hijo termine
            wait();
        }
    }

    // Esperar a que todos los procesos hijos terminen
    while (wait() > 0);
}

int stringcmp(const char *p, const char *q, uint n) {
    while (n > 0 && *p && *p == *q) {
        n--, p++, q++;
    }
    if (n == 0)
        return 0;
    return (uchar)*p - (uchar)*q;
}

int getcmd(char *buf, int nbuf) {
    printf(2, "Ingresar comando: ");
    memset(buf, 0, nbuf);
    gets(buf, nbuf);
    if (buf[0] == 0) // EOF
        return -1;
    return 0;
}

int main(void) {
    static char buf[100];
    int fd;

    while ((fd = open("console", O_RDWR)) >= 0) {
        if (fd >= 3) {
            close(fd);
            break;
        }
    }

    while (getcmd(buf, sizeof(buf)) >= 0) {
        if (myfork() == 0) {
            struct command_list *cmd_list = parsecmd(buf);
            runcmd(cmd_list);
            exit(); // Terminar el proceso hijo
        }
        wait();
    }
    exit();
}

void mypanic(char *s) {
    printf(2, "%s\n", s);
    exit();
}

int myfork(void) {
    int pid;

    pid = fork();
    if (pid == -1)
        mypanic("fork");
    return pid;
}
