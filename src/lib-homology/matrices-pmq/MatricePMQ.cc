#include "Matrice.hh"
#include "MatricePMQ.hh"
#include <iostream>
#include <cassert>

MatricePMQ::MatricePMQ(int i,int j)
{
  P=new Matrice(i,i);
  Pinv=new Matrice(i,i);
  Q=new Matrice(j,j);
  Qinv=new Matrice(j,j);
  M=new Matrice(i,j);

  assert(P!=NULL);
  assert(Pinv!=NULL);
  assert(Q!=NULL);
  assert(Qinv!=NULL);
  assert(M!=NULL);
  
  
  if ( valid() )
    {
      for(int n=0;n<i;n++)
	{
	  Pinv->setVal(n,n,1);
	  P->setVal(n,n,1);
	}
      
      for(int n=0;n<j;n++)
	{
	  Q->setVal(n,n,1);
	  Qinv->setVal(n,n,1);      
	}
    }
}

bool MatricePMQ::valid()
{ return P->valid() && Pinv->valid() && Q->valid() && Qinv->valid() && M->valid(); }

// AIE AIE AIE !!!
// MatricePMQ::MatricePMQ(const MatricePMQ & source)
// {
//   Pinv=source.Pinv;
//   P=source.P;
//   Q=source.Q;
//   Qinv=source.Qinv;  
//   M=source.M;
// }

MatricePMQ::~MatricePMQ()
{
  delete P;
  delete Pinv;
  delete Q;
  delete Qinv;
  delete M;
}

unsigned long MatricePMQ::size() const
{ return P->size()+Pinv->size()+Q->size()+Qinv->size()+M->size(); }

void MatricePMQ::deplacePMQ(coord c,int pos)
{
  M->inverseLigne(c.i,pos);
  Pinv->inverseLigne(c.i,pos);
  P->inverseColonne(c.i,pos);
  M->inverseColonne(c.j,pos);
  Q->inverseColonne(c.j,pos);
  Qinv->inverseLigne(c.j,pos);
}

void MatricePMQ::smithForm()
{
  if ( !valid() ) return;
  
  int  a=M->getnbli();
  int  b=M->getnbcol();
  int min=a;
  int coeff;
  int indice;
  coord c1;
  coord c2;
  coord c3;
  coord cc;

  if(min>b){
    min=b;}

  for(int i=0;i<min;i++)
    {      
      //on amene le plus petit element en position (i,i)
      do{
	cc=M->getMinElemNonZero(i);	
		
	if(!no(cc))
	  {   
	    deplacePMQ(cc,i);
	    c1=M->verifyDiviseLigne(i);	
	    if(!no(c1))
	      {   
		coeff=M->getVal(c1.i,c1.j)/M->getVal(i,i);
		addColonnePMQ(c1.j,i,-coeff);
	      }
	    else
	      {
		c2=M->verifyDiviseColonne(i);
		if(!no(c2))
		  {
		    coeff=M->getVal(c2.i,c2.j)/M->getVal(i,i);
		    addLignePMQ(c2.i,i,-coeff);
		  }
		else
		  {
		    c3=M->verifyOthers(i);
		    if(!no(c3))
		      {
			coeff=M->getVal(c3.i,c3.j)/M->getVal(i,i);
			addLignePMQ(c3.i,i,-coeff);
			addLignePMQ(i,c3.i,1);
		      }	    
		    else
		      {
			zeroOnLignePMQ(i);
			zeroOnColonnePMQ(i);	  
			if(M->getVal(i,i)<0)
			  {
			    multiplyColonnePMQ(i,-1);
			  }
			c1.i=-1;
			c1.j=-1;
			c2.i=-1;
			c2.j=-1;
			c3.i=-1;
			c3.j=-1;
		      }
		  }
	      }
	  }
	else
	  {
	    c1.i=-1;
	    c1.j=-1;
	    c2.i=-1;
	    c2.j=-1;
	    c3.i=-1;
	    c3.j=-1;

	  }
      }
      while(!(no(c1)&&no(c2)&&no(c3)));
    }


  for(int i=0;i<b/2;i++)
    {
      inverseColonnePMQ(i,b-i-1);
    }

  indice=0;
  for(int i=0;i<min;i++)
    {
      if(M->getVal(i,b-1-i)!=0)
	indice++;
	
      else
	break;  
    }

  for(int i=0;i<indice/2;i++)
    {
      inverseLignePMQ(i,indice-i-1);
    }
}

void MatricePMQ::zeroOnLignePMQ(int pos){
  
  int coeff;
  int a;
  int b;
  for(int i=pos+1;i<M->getnbcol();i++)
    {
      a=M->getVal(pos,i);
      b=M->getVal(pos,pos);
      coeff=a/b;
      addColonnePMQ(i,pos,-coeff);
     
    }
}

void MatricePMQ::zeroOnColonnePMQ(int pos)
{  
  int coeff;
  int a;
  int b;

  for(int i=pos+1;i<M->getnbli();i++)
    {
      a=M->getVal(i,pos);
      b=M->getVal(pos,pos);
      coeff=a/b;
      addLignePMQ(i,pos,-coeff);
    }

}


Matrice * MatricePMQ::getPinv()
{
  return Pinv;
}

Matrice * MatricePMQ::getP()
{
  return P;
}

Matrice * MatricePMQ::getM()
{
  return M;
}

Matrice * MatricePMQ::getQ() {
  return Q;
}

Matrice * MatricePMQ::getQinv()
{
  return Qinv;
}

void MatricePMQ::inverseLignePMQ(int i,int j)
{
  M->inverseLigne(i,j);
  P->inverseColonne(i,j);
  Pinv->inverseLigne(i,j);
}

void MatricePMQ::inverseColonnePMQ(int i,int j)
{
  M->inverseColonne(i,j);
  Q->inverseColonne(i,j);
  Qinv->inverseLigne(i,j);
}

void MatricePMQ::multiplyLignePMQ(int i,int coeff)
{
  M->multiplyLigne(i,coeff);
  P->multiplyColonne(i,coeff);
  Pinv->multiplyLigne(i,coeff);
}

void MatricePMQ::multiplyColonnePMQ(int i,int coeff)
{
  M->multiplyColonne(i,coeff);
  Q->multiplyColonne(i,coeff);
  Qinv->multiplyLigne(i,coeff);
}

void MatricePMQ::addLignePMQ(int i,int j,int coeff)
{
  M->addLigne(i,j,coeff);
  P->addColonne(j,i,-coeff);
  Pinv->addLigne(i,j,coeff);
}

void MatricePMQ::addColonnePMQ(int i,int j,int coeff)
{
  M->addColonne(i,j,coeff);
  Q->addColonne(i,j,coeff);
  Qinv->addLigne(j,i,-coeff);
}

void MatricePMQ::setValPMQ(int i,int j,int val)
{
  M->setVal(i,j,val);
}

int MatricePMQ::getValPMQ(int i,int j)
{
  return  M->getVal(i,j);
}

void MatricePMQ::affichePMQ()
{
  std::cout << "Pinv" << std::endl;
  Pinv->affiche();
  std::cout << "P" << std::endl;
  P->affiche();
  std::cout << "M" << std::endl;
  M->affiche();
  std::cout << "Q" << std::endl;
  Q->affiche();
  std::cout << "Qinv" << std::endl;
  Qinv->affiche();

}
