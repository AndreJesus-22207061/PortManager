# Port-Manager

Projeto no âmbito da disciplina de Linguagens de Programação I

Criadores: André Jesus / Tomás Nave

![lena](https://github.com/TomasNave-a22208623/Port-Manager/assets/127102331/86a37fcc-0932-4dd7-a33c-f70a0792d57a)


Objetivo:

O objetivo deste Projeto era desenvolver um simulador de um estaleiro de embarcações de carga utilizando a linguagem de programação C

Exemplos:

````

% ./portmanager
+---- MENU
| move          [-g grua] [-d ponto] [-p pilha] [-D ponto] [-P pilha] [-n numero_de_contentores]
| show          [-d ponto] [-e embarc]
| where         [embarc]
| navigate      [-e embarc] [-d ponto]
| load          [-e embarc] [-p pilha] [-c contentor:peso]
| weight        [embarc]
| save          [filename]
| help 
| quit 
+----
>navigate -e LENA -d 1
SUCCESS: operation concluded
>navigate -e XPTO -d 0
SUCCESS: operation concluded
>load -e LENA -p 0 -c AA0:500
SUCCESS: operation concluded
>load -e LENA -p 0 -c AA0:500
ERROR: invalid command
>show LENA
ERROR: invalid command
>show -e LENA
d1 LENA
        p0 1 AA0:500 

>load -e LENA -p 0 -c AA1:500
SUCCESS: operation concluded
>load -e LENA -p 0 -c AA2:500
SUCCESS: operation concluded
>load -e LENA -p 0 -c AA3:500
SUCCESS: operation concluded
>load -e LENA -p 0 -c AA4:500
SUCCESS: operation concluded
>load -e LENA -p 0 -c AA1:500
ERROR: invalid command
>load -e LENA -p 0 -c AA6:500
SUCCESS: operation concluded
>load -e LENA -p 0 -c AA7:500
SUCCESS: operation concluded
>load -e LENA -p 0 -c AA8:500
SUCCESS: operation concluded
>load -e LENA -p 0 -c AA9:500
SUCCESS: operation concluded
>load -e LENA -p 0 -c AB0:500
SUCCESS: operation concluded
>load -e LENA -p 0 -c AB1:500
SUCCESS: operation concluded
>load -e LENA -p 0 -c AB2:500
SUCCESS: operation concluded
>load -e LENA -p 0 -c AB3:500
SUCCESS: operation concluded
>load -e LENA -p 0 -c AB4:500
SUCCESS: operation concluded
>load -e LENA -p 0 -c AB1:500
ERROR: invalid command
>load -e LENA -p 0 -c AB6:500
SUCCESS: operation concluded
>load -e LENA -p 0 -c AB7:500
SUCCESS: operation concluded
>load -e LENA -p 0 -c AB8:500
SUCCESS: operation concluded
>load -e LENA -p 0 -c AB9:500
SUCCESS: operation concluded
>show -e LENA
d1 LENA
        p0 18 AA0:500 AA1:500 AA2:500 AA3:500 AA4:500 AA6:500 AA7:500 AA8:500 AA9:500 AB0:500 AB1:500 AB2:500 AB3:500 AB4:500 AB6:500 AB7:500 AB8:500 AB9:500 

>weight LENA
LENA 9000
>where LENA
d1 LENA
>move -g A -d 1 -p 0 -D 1 -P 1 -n 5
SUCCESS: operation concluded
>show -e LENA
d1 LENA
        p0 13 AA0:500 AA1:500 AA2:500 AA3:500 AA4:500 AA6:500 AA7:500 AA8:500 AA9:500 AB0:500 AB1:500 AB2:500 AB3:500 
        p1 5 AB9:500 AB8:500 AB7:500 AB6:500 AB4:500 

>move -g B -d 1 -p 0 -D 1 -P 2 -n 6
SUCCESS: operation concluded
>show -e LENA
d1 LENA
        p0 7 AA0:500 AA1:500 AA2:500 AA3:500 AA4:500 AA6:500 AA7:500 
        p1 5 AB9:500 AB8:500 AB7:500 AB6:500 AB4:500 
        p2 6 AB0:500 AB1:500 AB2:500 AB3:500 AA8:500 AA9:500 

>
```
