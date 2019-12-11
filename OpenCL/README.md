Conception d'un programme qui pourra simuler le transfert de chaleur d'une plaque chauffante de façon séquentielle, selon la fonction:

Sur une matrice de dimensions U[m, n, np], la formule discrétisée d’Euler à appliquer sur chaque cellule de la matrice à chaque itération est la suivante :

**U(i, j,k+1)=(1− 4td)∗U(i, j,k)+(td)∗[U(i−1, j,k)+U(i+1, j,k)+U(i, j−1,k)+U(i, j+1,k)]**

De plus, la matrice au temps k = 0 doit être initialisée grâce à l’équation suivante :

**U(i, j,0)=i∗(m−i−1)∗(2.0∗i/m)∗j∗(n− j−1)∗(1.0∗j/n)**
