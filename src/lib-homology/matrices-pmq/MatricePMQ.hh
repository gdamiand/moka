#ifndef MATRICEPMQ_HH
#define MATRICEPMQ_HH

#include "Matrice.hh"

class MatricePMQ
{
private :
  Matrice *P;
  Matrice *Pinv;
  Matrice *Q;
  Matrice *Qinv;
  Matrice *M;


public :
  MatricePMQ(const MatricePMQ & source);
  MatricePMQ(int i,int j);
  ~MatricePMQ();

  /// Return the memory size (in bytes) required for the matrixes.
  unsigned long size() const;

  /// Return true if the matrix is valid, ie if the allocation of
  /// matrixes was successful.
  bool valid();
   
  void affichePMQ();
  void inverseLignePMQ(int i ,int j);
  void inverseColonnePMQ(int i,int j);
  
  void multiplyLignePMQ(int i,int coeff);
  void multiplyColonnePMQ(int i,int coeff);
  
  void addLignePMQ(int i,int j,int coeff);
  void addColonnePMQ(int i,int j,int coeff);
  void deplacePMQ(coord c,int pos);
  void setValPMQ(int i,int j,int val);
  int getValPMQ(int i,int j);
  Matrice* getP();
  Matrice* getPinv();
  Matrice* getM();
  Matrice* getQ();
  Matrice* getQinv();
  
  void zeroOnLignePMQ(int pos);
  void zeroOnColonnePMQ(int pos);
  void smithForm();
};

#endif
