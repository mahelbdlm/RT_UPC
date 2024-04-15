Ejemplo de compilación P1 y P2: 
```
gcc ej1.c -o ej1
```

Ejemplo de compilación P3 
```
gcc ej1.c -o ej1 -pthread
```
No se debe olvidar de añadir ```-pthread```

Ejemplo de compilación P4
Algunos apartados necesitan compilar con  ```-lrt``` (los que utilizan mqueue)

