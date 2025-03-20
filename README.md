# Tp_IoT_2022_LoRa_Bluetooth
## BAHAIN Alexandre & DELAGARDE Augustin

### Connection en WiFi
Il a fallu modifier le fichier sdkconfig pour paramétrer le WiFi car sur Windows l'éditeur graphique ne fonctionne pas. 



### Envoi d'un message MQTT
Il faut s'inscrire à un topic et envoyer le message de son choix. Dans notre cas le topic est tp/alban.
MQTT est utile quand on veut envoyer des petits montants de données, comme par exemple les données d'un capteur de températeur.



### LoRa
Pour communiquer en LoRa, il faut paramétrer 6 pins : 
SCK, MISO, MOSI, NSS, NRESET, DIO0
Il faut aussi choisir un Spreading Factor, une bande passante, une fréquence, un coding rate.



### Réalisation éffectués


On écoute sur le topic venans du groupe de dorian "SALAD"

on a récupéré le message "POTATO", qu'on garde dans une variable token.

On envoie par intervalle de 5 seconde en laura, les 8 chracteres de la variable token.

Dorian a pu observé le message en lora