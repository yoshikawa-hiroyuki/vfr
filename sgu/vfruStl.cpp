#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <deque>

#include "vfruStl.h"
#include "vfruGetLine.h"
#include "utilEndian.h"
#include "utilPath.h"

#include "vfrGroup.h"
#include "vfrTriangles.h"

#undef BUFFLEN
#define BUFFLEN 256


inline static void OUT_ERR(std::ostream& os, std::string file,
			   std::string msg, std::string buf =std::string("")) {
  if ( ! os.good() ) return;
  os << "ReadStl: " << file << ": " << msg << std::endl;
  if ( buf.empty() ) return;
  os << "       + " << buf.substr(0, 64); 
  if ( buf.size() > 64 ) os << "...";
  os << std::endl << std::flush;
}


//=================== check ascii file ===================
bool vfruStl::IsAscii(const char* path) {
  char buff[BUFFLEN];

  std::ifstream sf(path);
  if ( ! sf ) return false;
  VFR::GetLine(sf, buff, BUFFLEN);
  if ( sf.eof() ) return false;
  if ( strstr(buff, "solid") )
    return true;
  return false;
}

//=================== read ascii file ===================
vfrNode* vfruStl::ReadSla(const char* path) {
  using namespace std;

  deque<Facet> fl;
  char buff[BUFFLEN];
  char ds[64];
  string base_name;

  //// OPEN INFILE ////
  ifstream sf(path);
  if ( ! sf )  {
    OUT_ERR(cerr, path, "Sla open failed");
    return NULL;
  }
#ifdef WINDOWS
  base_name = CES::BaseName(path, ".stl", '\\');
#else
  base_name = CES::BaseName(path, ".stl");
#endif
  if ( base_name.empty() ) base_name = "sla";

  //// READ INFILE ////
  register int vcount = 0;
  Facet face;

  while ( ! sf.eof() ) {
    VFR::GetLine(sf, buff, BUFFLEN);

    // beginning of the face
    if ( strstr(buff, "facet normal") || strstr(buff, "FACET NORMAL") ) {
      float nv[3];
      if ( sscanf(buff, "%s %s %f %f %f",
		  ds, ds, &nv[0], &nv[1], &nv[2]) < 5 ) {
	OUT_ERR(cerr, path, "imcomplete facet normal", buff);
        continue;
      }
      memcpy(face.nv, nv, sizeof(float)*3);
      continue;
    }

    if ( strstr(buff, "outer loop") || strstr(buff, "OUTER LOOP") ) {
      vcount = 0;
      continue;
    }

    // vertex
    if ( strstr(buff, "vertex") || strstr(buff, "VERTEX") ) {
      float v[3];
      if ( sscanf(buff, "%s %f %f %f", ds, &v[0], &v[1], &v[2]) < 4 ) {
	OUT_ERR(cerr, path, "imcomplete vertex", buff);
        continue;
      }
      if ( vcount < 0 ) continue;
      if ( vcount < 2 ) {
        memcpy(face.p[vcount++], v, sizeof(float)*3);
      } else if ( vcount == 2 ) {
        memcpy(face.p[vcount++], v, sizeof(float)*3);
        fl.push_back(face);
      } else {
        memcpy(face.p[1], face.p[2], sizeof(float)*3);
        memcpy(face.p[2], v, sizeof(float)*3);
        fl.push_back(face);
        vcount++;
      }
      continue;
    } // End of 'vertex' line

    // end of the face
    if ( strstr(buff, "endloop") || strstr(buff, "ENDLOOP") ) {
      vcount = -1;
      continue;
    }

  } // End of while(1)

  if ( vcount == 0 || vcount == 1 ) {
    OUT_ERR(cerr, path, "imcomplete facet", buff);
  }

  // create Node
  vfrTriangles* pTria = new vfrTriangles(base_name);
  if ( ! pTria ) return NULL;
  int nVtx = fl.size() * 3;
  pTria->alcVerts(nVtx);
  pTria->alcNormals(fl.size());
  pTria->setNormalMode(AT_PER_FACE);

  register int i;
  for ( i = 0; i < fl.size(); i++ ) {
    pTria->setVert(i*3  , fl[i].p[0], (i==0)?TRUE:FALSE);
    pTria->setVert(i*3+1, fl[i].p[1], FALSE);
    pTria->setVert(i*3+2, fl[i].p[2]);
    pTria->setNormal(i, fl[i].nv);
  } // end of for(i)

  pTria->generateBbox();
  return pTria;
}

//=================== read binary file ===================
vfrNode* vfruStl::ReadSlb(const char* path, const bool bigEdn) {
  using namespace std;

  deque<Facet> fl;
  string base_name;

  int nFacet;
  register int i;
  Facet face;
  union {
    char cb[50];
    float fb[4][3];
  } ub;
  unsigned short* p_ext = (unsigned short*)(&ub.cb[48]);

  // endian conversion
  bool eCvt;
  if ( CES::BigEndianSys() ) {
    eCvt = (! bigEdn);
  } else {
    eCvt = bigEdn;
  }

  // open file
  FILE* fp = fopen(path, "rb");
  if ( ! fp ) {
    OUT_ERR(cerr, path, "Slb open failed");
    return NULL;
  }
#ifdef WINDOWS
  base_name = CES::BaseName(path, ".stl", '\\');
#else
  base_name = CES::BaseName(path, ".stl");
#endif
  if ( base_name.empty() ) base_name = "slb";

  // skip header (80bytes);
  if ( fseek(fp, 80, SEEK_SET) != 0 ) {
    OUT_ERR(cerr, path, "invalid file");
    return NULL;
  }

  // main loop
  while ( 1 ) {
    // read #of facet
    if ( fread(&nFacet, 4, 1, fp) < 1 ) break;
    if ( eCvt ) BSWAP32(nFacet);
    if ( nFacet > 500000000 || nFacet < 0 ) {
      OUT_ERR(cerr, path, "invalid file(not Slb?)");
      return NULL;
    }

    // facet loop
    for ( i = 0; i < nFacet; i++ ) {
      // read to buffer
      fread(ub.cb, 1, 50, fp);
      if ( feof(fp) ) break;

      // endian convert
      if ( eCvt ) {
	BSWAPVEC(ub.fb[0], 12);
	SBSWAPVEC(p_ext, 1);
      }

      // copy to face
      memcpy(face.nv, ub.fb[0], sizeof(float)*3);
      memcpy(face.p, ub.fb[1], sizeof(float)*9);
      face.ext = (*p_ext);

      // push to list
      fl.push_back(face);

    } // end of for(i)

  } // end of while(1)

  // create Node
  vfrTriangles* pTria = new vfrTriangles(base_name);
  if ( ! pTria ) return NULL;
  int nVtx = fl.size() * 3;
  if ( ! pTria->alcVerts(nVtx) ||
       ! pTria->alcNormals(fl.size()) ) {
    delete pTria;
    return NULL;
  }
  pTria->setNormalMode(AT_PER_FACE);

  for ( i = 0; i < fl.size(); i++ ) {
    pTria->setVert(i*3  , fl[i].p[0], FALSE);
    pTria->setVert(i*3+1, fl[i].p[1], FALSE);
    pTria->setVert(i*3+2, fl[i].p[2], FALSE);
    pTria->setNormal(i, fl[i].nv);
  } // end of for(i)

  if ( pTria->alcIndices(fl.size()) ) {
    for ( i = 0; i < fl.size(); i++ )
      pTria->setIndice(i, (const int)fl[i].ext);
  }

  pTria->generateBbox();
  return pTria;
}

