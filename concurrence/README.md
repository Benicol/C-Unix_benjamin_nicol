# Question d'analyse EXO 1
## Performance : Quelle version est la plus rapide ? Pourquoi ?
LA version multi thread est précisément dix fois plus rapide, (version original = (715ms) et version parallele (71 ms) Cela se justifie car le tableau d'origine est de taille 10, et donc les 10 calculs sont fait en parallèle)
## Mesurez : Différentes valeurs pour MULTIPLICATEUR, jusqu’a à 10000000 et observez la différence
Le temps pris est juste multiplier selon la taille du multiplicateue
## Résultats : Les résultats sont-ils identiques entre les versions ? Pourquoi n’y a-t-il pas de race condition ?
Il y a pas de race condition car ils utilise des données différents 
## Scalabilité : Testez avec différents nombres de threads.