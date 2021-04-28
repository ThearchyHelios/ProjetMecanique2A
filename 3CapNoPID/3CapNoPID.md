## Pseudo Code pour le Voiture d'évitement d'obstacles

| Le code (Gauche) | Le code (Droite) | Le code (Avant) | On va faire                           |
| ---------------- | ---------------- | --------------- | ------------------------------------- |
| 1                | 1                | 0               | Aller                                 |
| 0                | 0                | 1               | Arret (ERROR Then Aller)              |
| 1                | 1                | 1               | Arret (Aller)                         |
| 1                | 0                | 1               | Grande Droite                         |
| 1                | 0                | 0               | Petit Droite                          |
| 0                | 1                | 1               | Grande Gauche                         |
| 0                | 1                | 0               | Petit Gauche                          |
| 0                | 0                | 0               | Discuter Sharp Gauche ou Sharp Droite |

```cpp
Main Frame{
    Loop{
        Get Values()
        Calculer PID() // Pour obtenir la vitesse de rotation

        // Mais il y a juste 2 capteur et donc c'est un PEU difficile
        // pour obtenir le PID

        Controler le moteur()
    }
    Get Values(){
        sensor [0] = Le capteur gauche
        sensor [1] = Le capteur droit
        sensor [2] = Le capteur avant

        Utiliser le valeur obtenu et combiner avec le table

    Controller le moteur (){
        - PetitGauche()
        - GrandeGauche()
        - SharpGauche()
        - PetitDroite()
        - GrandeDroite()
        - SharpDroite()
        - Aller()
        - _stop()
    }
}
```
