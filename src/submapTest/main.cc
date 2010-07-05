#include <iostream>
#include <cstdlib>
#include <cstring>
#include "g-map-vertex.hh"
#include "chrono.hh"
using namespace std;
using namespace GMap3d;

// Test if motifFile is a submap of each map given in indexFile (which
// is a text file composed of a list of filename), repTarget being the
// directory containing all the files.
void testSeveralSubmap(const char* motifFile, const char* repTarget,
		       const char* indexFile, bool infoBase=false)
{
  ifstream stream;
  stream.open(indexFile);

  CGMapVertex m1;
  m1.load(motifFile);
  //m1.importOff(motifFile);

  /*  std::string file=std::string(motifFile)+".txt";
  //  m1.save(file.c_str());
  m1.exportOff3D(file.c_str());*/

  std::cout<<"Motif "<<motifFile<<": "<<std::endl;
  m1.displayCharacteristics(std::cout)<<std::endl;

  CGMapVertex* m2=NULL;

  unsigned int nbMatch=0;
  unsigned int res = 0;
  CChrono c;
  
  while ( stream.good() )
    {
      string txt;
      stream>>txt;
      if ( stream.good() )
	{
	  delete m2;
	  m2 = new CGMapVertex;
	  m2->load((string(repTarget) + txt).c_str());
	  // m2->importOff((string(repTarget) + txt).c_str());
	  if ( infoBase ) m2->displayCharacteristics(std::cout)<<std::endl;
	  nbMatch=0;

	  /*	  if ( infoBase )
	    {
	      std::string file=(string(repTarget) + txt)+".txt";
	      m2->exportOff3D(file.c_str());
	      //  m2->save(file.c_str());
	      }*/

	  c.start();
	  //	  if ( (index=m1.findMotif(m2,&nbMatch))!=-1 )
	  if ( (res=m1.countNumberOfMotifs(m2,&nbMatch))>0 )
	    {
	      cout<<motifFile<<" MATCHE avec "<<txt<<" ("<<res<<" fois)"<<nbMatch<<endl;
	      // m1.freeDirectInfo(index);
	    }
	  else cout<<txt<<"... no   "<<nbMatch<<endl;
	  c.stop();
	}
    }

  c.display("Search time");
  stream.close();
}

// Test if each motif in listMotifs is a submap of each map given in
// indexFile (which is a text file composed of a list of filename),
// repTarget being the directory containing all the files.
void testSeveralSubmapSeveralMotif(const char* listMotifs,
				   const char* repTarget,
				   const char* indexFile)
{
  ifstream stream;
  bool first=true;
  stream.open(listMotifs);
  while ( stream.good() )
    {
      string txt;
      stream>>txt;
      if ( stream.good() )
	{
	  testSeveralSubmap(txt.c_str(), repTarget, indexFile, first);
	  first = false;
	}
    }
}

  // Display the number of times the map contained in smallFile is a
// submap of the one contained in bigFile.
void testSubgraph(char* smallFile, char* bigFile)
{
  CGMapVertex smallmap;
  if ( !smallmap.load(smallFile) )
    {
      cout<<"Problème de chargement du fichier "<<smallFile<<endl;
      exit(EXIT_FAILURE);      
    }
    
  
  CGMapVertex bigmap;
  if ( !bigmap.importOff(bigFile) )
    {
      cout<<"Problème d'import OFF du fichier "<<bigFile<<endl;
      exit(EXIT_FAILURE);      
    }
  
  cout<<"Nombre de motifs trouvés : "
      <<smallmap.countNumberOfMotifs(&bigmap)<<endl;
}

int main(int argc, char** argv)
{
  if ( ( argc>1 && (!strcmp(argv[1],"-?") || !strcmp(argv[1],"-h")) ) ||
       (!(argc==3 && strcmp(argv[1],"-l")) &&
	!(argc==5 && !strcmp(argv[1],"-l")) &&
       !(argc==5 && !strcmp(argv[1],"-L"))) )
    {
      cout<<"Usage1 : a.out smallgraph.mok biggraph.off2D"<<endl
	  <<"        compte le nombre de fois que la carte smallgraph.mok "
	  <<"est présente dans la carte associée à biggraph.off2D"<<endl;
      cout<<"Usage2 : a.out -l motifFile.mok directory filelist.txt"<<endl
	  <<"        cherche la présence de motifFile.mok dans chaque carte "
	  <<"dont le nom est donné dans le fichier filelist.txt, "
	  <<"ces fichiers se trouvant dans le répertoire directory"<<endl;
      cout<<"Usage3 : a.out -L motiflist.txt directory filelist.txt"<<endl
	  <<"        cherche la présence de chaque motif donné dans motiflist.txt "
	  <<"dans chaque carte dont le nom est donné dans le fichier filelist.txt, "
	  <<"ces fichiers se trouvant dans le répertoire directory"<<endl;
      exit(EXIT_FAILURE);
    }

  if (!strcmp(argv[1],"-l"))
    testSeveralSubmap(argv[2],argv[3],argv[4]);
  else if (!strcmp(argv[1],"-L"))
    testSeveralSubmapSeveralMotif(argv[2],argv[3],argv[4]);
  else
    testSubgraph(argv[1],argv[2]);

  exit(EXIT_SUCCESS);
}
