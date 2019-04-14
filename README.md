# TY19_DIC

Lenkraddisplay für TY19.

Hardware:
 - Microcontroller AT90CAN
 - Display: Electronic Assembly W204-XLG
 
Das Display wird im 8-bit Modus verwendet.

Aktuell findet eine Neustrukturierung statt, Status:
 - [ ] Produktionsdisplay
   - [x] Kühler
   - [x] Öltemperatur
   - [x] Öldruck
   - [ ] K
   - [x] Fehlerlevel
 - [ ] Debugdisplay
   - [ ] Bremsdruck
   - [ ] Bremstemperatur
   - [x] TPS
   - [x] Öltemperatur
   - [ ] Sli (?)

Dies ist das neue Displaylayout (20x4):
```
COOL.     125 C  ###
OEL 1.0 B  25 C  # #
                 # #
K=1  E=0         ###
```

Zusätzlich ist ein zweite Displaymodus für Testzwecke vorgesehen:

```
BPFR=  B VB=  . V
BPRE=  B TPS=   ?
BTL=   C OiT=   C
BTR=   C Sli=   ?
```

Relevante Dateien:
 - `main.c` - Hauptprogrammcode des Displaycontrollers

Submodule:
 - `CAN_LIB` - Can Bus Bibliothek
 
Was ist noch zu tun:
 - Neustrukturierung und _Dokumentierung_ der `dsp_*`-Funktionen
 
 Fuse Settings AT90CAN
Extended  	0xF7
High		0x9F
Low		0xCF
