# MPI parallel computing

Les problèmes traités consistent à faire une série d’altérations aux éléments d’une matrice. Soit la matrice M chaque élément   M i , j représente celui à la ligne i (axe des Y) et la colonne j (axe des X).
La taille de la matrice est fixée à 8 par 8

**Problème 1**
Pour ce problème, la matrice finale est calculée en fonction de la matrice précédente ainsi que du
numéro d’altération k, du numéro de ligne et de colonne. Lors de l’altération k, chaque élément Mk de la matrice est calculé par Mk−1+( i+j)×k.

**Problème 2**
Ce problème est semblable au premier, on doit prendre en compte la matrice précédente ainsi que le numéro d’altération k, le numéro de ligne et la valeur de la colonne précédente. Pour une altération k,
on a Mk =(Mk−1)+(Mk )×k pour tous les éléments sauf ceux de la première colonne qui sont i, j i, j i, j−1
données par (Mk−1)+(i×k).
