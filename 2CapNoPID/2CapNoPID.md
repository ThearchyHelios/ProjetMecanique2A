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

        Utiliser le valeur obtenu et combiner avec le table
    }

    Controller le moteur (){
        - Gauche()
        - Droite()
        - Aller()
        - _stop()
    }
}
```
