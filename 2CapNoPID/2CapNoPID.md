## Pseudo Code pour le Voiture d'évitement d'obstacles

| Le code (Gauche) | Le code (Droite) | On va faire      |
| ---------------- | ---------------- | ---------------- |
| 1                | 1                | Allez ~!         |
| 1                | 0                | Droite!          |
| 0                | 1                | Gauche!          |
| 0                | 0                | Arreter! (Error) |

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
        if(sensor [0] == 0 && sensor [1] == 0){
            error = 0 // Arreter
        } else if (sensor [0] == 0 && sensor [1] == 1){
            error = -1 // Gauche
        } else if (sensor [0] == 1 && sensor [1] == 0){
            error = 1 // Droite
        } else if (sensor [0] == 1 && sensor [1] == 1){
            error = 0 // Aller
        }
        return error
    }
    Controler le moteur (){
        - Gauche()
        - Droite()
        - Aller()
        - _stop()
    }
}
```
