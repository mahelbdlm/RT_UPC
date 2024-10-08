Código de las prácticas de RT (grado de ingeniería electrónica de telecomunicaciones) de la UPC versión 2024.

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


P6: 
Para utilizar el socket, primero se debe ejecutar el servidor.

Luego, ejecutar el cliente de la manera siguiente: ```./ej4_client localhost```.


Compilación P7: No olvidar el flag ```-pthread```

---
Deliverables: 10  
Summary: 10  
Project: 10  
