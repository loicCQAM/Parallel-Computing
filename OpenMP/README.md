Votre algorithme devra effectuer une série d’altération sur une matrice de taille 12 par 12. Chaque
élément de cette matrice est représenté par   M k où k est le numéro de l’altération ou le temps, i i,j
est la position sur l’axe des Y et j est la position sur l’axe des X.

**Problème 1:** La matrice finale est calculée en fonction de la matrice précédente ainsi que du numéro d’altération k,Mk
du numéro de ligne et de colonne. Lors de l’altération k, chaque élément i , j de la matrice est
calculé par Mk−1+i+j.

**Problème 2:** La matrice finale est calculée en fonction de la matrice précédente ainsi que de la valeur de la cellule à la
droite de la cellule calculée. Lors de l’altération k, chaque élément
k−1 k parMi,j +Mi,j+1
