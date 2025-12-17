# Question d'analyse EXO 1
## Performance : Quelle version est la plus rapide ? Pourquoi ?
LA version multi thread est précisément dix fois plus rapide, (version original = (715ms) et version parallele (71 ms) Cela se justifie car le tableau d'origine est de taille 10, et donc les 10 calculs sont fait en parallèle)
## Mesurez : Différentes valeurs pour MULTIPLICATEUR, jusqu’a à 10000000 et observez la différence
Le temps pris est juste multiplier selon la taille du multiplicateue
## Résultats : Les résultats sont-ils identiques entre les versions ? Pourquoi n’y a-t-il pas de race condition ?
Il y a pas de race condition car ils utilise des données différents 
## Scalabilité : Testez avec différents nombres de threads.
Le fait de modifié le nombre de thread (par rapport à la taille du tableau) permet d'aller plus vite mais pas de manière linéaire, cela aide de moins en moins
## Combien de coeur a votre machine virtuel ? vous pouvez regarder dans /proc/cpuinfo
12 Coeur physique, 24 virtuel
## Experimentations suggestés EXO 2
1. 1. LE plus de thread il y a, le plus le resultat est incorrecte, avec  1 thread c'est en effet toujours correcte
2. Oui, le plus il y a d'itérations, le plus le resultat est incorrecte
3. Avec l'optimisation o2 Le resultatest toujours correcte et beaucoup plus rapide (instantanée)