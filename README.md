# semaforo_c_SO

### primer paso

debe asegurarse de tener instalado del comando `make`  en el odenador

```
sudo apt-get -y install make
```
## el proyecto esta basado en el un problema de aeropuerto, especificamente con las cintas

### condiciones 

1. En el aeropuerto existen 5000 mostradores donde los pasajeros entregan su equipaje y 500 cintas transportadoras que clasifican el equipaje según su destino.

2. Igualmente, hay 250 áreas de almacenamiento temporal que corresponden a zonas donde se almacena el equipaje en espera de ser cargado en los aviones.

3. Un detalle importante es que los aviones pueden ser de diferentes tipos y con capacidad de carga variable, por lo que el equipaje debe ser transportado por los pasillos de embarque hasta los aviones, los cuales serán cargados de acuerdo al límite de capacidad.

4. Luego en la terminal de llegada, el equipaje es trasladado a la cinta donde los pasajeros recogerán su equipaje después del aterrizaje.

## El equipaje se clasifica como: 

1) `facturado`: Equipaje que es entregado en el mostrador de facturación y transportado en la bodega del avión. 

2) `mano`: Equipaje que el pasajero lleva consigo a bordo del avión. 

3) `facturado`: Equipaje que requiere un manejo especial, como instrumentos musicales, equipos deportivos o animales. 

4) `sobredimensionado`: Equipaje que excede las dimensiones permitidas. 

## Reglas de Clasificación 

1) `Destino`: El equipaje se clasifica según su destino final (ciudad, país). 

2) `Vuelo`: El equipaje se asigna al vuelo correspondiente. 

3) `Prioridad`: Algunos equipajes pueden tener prioridad en el manejo, como los de conexión o los de pasajeros con necesidades especiales. 

4) `Fragilidad`: El equipaje frágil requiere un manejo especial para evitar daños.

## Tiempos de Procesamiento 

1) `Facturación`: Tiempo que tarda un agente en facturar una pieza de equipaje. 

2) `Clasificación`: Tiempo que tarda una cinta transportadora en clasificar una pieza de equipaje. 

3) `Transporte`: Tiempo que tarda un equipaje en ser transportado desde un punto a otro del aeropuerto. 

4) `Carga`: Tiempo que tarda en cargar un equipaje en un avión. 

5) `Descarga`: Tiempo que tarda en descargar un equipaje de un avión. 

## Se tiene los siguientes procesos: mostrador, cinta, área de almacenamiento, avión y cinta de recogida. 

# Se requiere contabilizar:  

1) Tiempo de procesamiento de cada equipaje 

2) Porcentaje de equipajes perdidos 

3) Nivel de utilización de los recursos. 

4) La simulación deberá prestar atención para evitar las situaciones de interbloqueo, inanición y espera activa. Se debe procurar un elevado grado de concurrencia entre los procesos. Asimismo, para que la simulación sea inteligible, todos los procesos deberán emitir mensajes informativos por pantalla cada vez que su situación se modifique. De esta forma, la salida permitirá tener acceso a toda la información, para que se pueda verificar el comportamiento.
