
void n(void)

{
  system("/bin/cat /home/user/level7/.pass");
  return;
}



void m(void *param_1,int param_2,char *param_3,int param_4,int param_5)

{
  puts("Nope");
  return;
}



void main(undefined4 param_1,int param_2)

{
  char *__dest;
  undefined4 *puVar1;
  
  __dest = (char *)malloc(0x40);
  puVar1 = (undefined4 *)malloc(4);
  *puVar1 = m;
  strcpy(__dest,*(char **)(param_2 + 4));
  (*(code *)*puVar1)();
  return;
}