#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    // Convertit le deuxième argument en entier
    int input_value = atoi(argv[2]);

    // Vérifie si l'entrée est égale à 423
    if (input_value == 423) {
        // Duplique la chaîne "/bin/sih"
        char *shell_path = strdup("/bin/sh");

        // Obtient les UID et GID effectifs
        uid_t effective_uid = geteuid();
        gid_t effective_gid = getegid();

        // Définit les UID et GID effectifs du processus
        setresgid(effective_gid, effective_gid, effective_gid);
        setresuid(effective_uid, effective_uid, effective_uid);

        // Exécute le shell
        execv(shell_path, &shell_path);
    } else {
        // Affiche un message d'erreur si l'entrée n'est pas 423
        fwrite("No !\n", 1, 5, stderr);
    }

    return 0;
}
ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff