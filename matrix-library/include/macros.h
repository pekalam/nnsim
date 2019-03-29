#pragma once
//Odczyt zawartosci macierzy w wierszu i, kolumnie j
#define MX(i,j,m) (m).getPrx(i,j)
//Transpozycja macierzy
#define MX_T(i,j,m) (m).getPrx(j,i)
//Traktuje macierz Yx1 jak tablice jednowymiarowa 
#define MX_R1_AS_VEC(i, m) (m).getPrx(i,0)