Erik Eckell
Rodrigo Guzmán

Informe de la segunda parte de la tarea 1 de Sistemas operativos y redes

Procesamiento de input

El procesamiento de la entrada empieza en el struct parsecmd (línea 36 en nsh.c), este convierte una cadena de entrada en una lista de comandos que se almacena en command_list. Posteriormente, se eliminan los saltos de línea y se procesan los comandos (línea 52-116). El while (línea 52) crea un nuevo struct, command, para cada comando encontrado. Los argumentos del comando se extraen hasta encontrar un delimitador (;, &, o |). En caso de recibir multiples comandos, estos son separados por los limitadores que son reemplazados por un caracter nulo, para indicar el término del comando actual.

Redirección de entrada y salida

La redirección de entrada y salida se maneja en la función runcmd. Aquí se configuran los descriptores de archivo según los archivos especificados en la entrada. En la línea 138 la sentencia if recibe un archivo para la redirección de entrada (cmd->input_redir) y se abre en modo solo lectura (O_RDONLY). Luego, se cierra el descriptor de entrada estándar (stdin, descriptor 0) y se duplica el descriptor del archivo en el descriptor de entrada estándar. Finalmente, se cierra el descriptor del archivo dado que ya está duplicado.

Al recibir un archivo en la redirección de salida (cmd->output_redir)(línea 158), se abre el archivo en modo escritura (O_WRONLY) con las opciones de crear el archivo si no existe (O_CREATE) y truncar el archivo si existe (O_TRUNC). Después, se cierra el descriptor de salida estándar (stdout, descriptor 1) y se duplica el descriptor del archivo en el descriptor de salida estándar.

Pipes (no se logró implementar correctamente pero de esta manera se planeaba hacerlo)

El pipe tomaría la salida estándar de un proceso y la conectaría directamente con la entrada estándar de otro. En parsecmd, al encontrar el carácter |, se divide el comando en dos y se almacena la relación de pipe entre ellos (cmd->pipe_out ). En runcmd se crea una tubería al detectarla en parsecmd y se redirige la salida estándar (cmd->output_redir) del proceso actual hacia la entrada (cmd->input_redir) de la siguiente tubería recibiendo asi el comando como entrada.
