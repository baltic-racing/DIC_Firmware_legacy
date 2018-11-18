# TY19_DIC

Lenkraddisplay für TY19.

Hardware:
 - Microcontroller AT90CAN
 - Display: Electronic Assembly W204-XLG
 
Das Display wird im 8-bit Modus verwendet.

Dies ist das neue Displaylayout:
```
COOL.     125 C  ###
OEL 1.0 B  25 C  # #
                 # #
K=1  E=0         ###
```

Zusätzlich ist ein zweite Displaymodus für Testzwecke vorgesehen:

```

```

Relevante Dateien:
 - `main.c` - Hauptprogrammcode des Displaycontrollers

Submodule:
 - `CAN_LIB` - Can Bus Bibliothek
 
Was ist noch zu tun:
 - Neustrukturierung und _Dokumentierung_ der `dsp_*`-Funktionen