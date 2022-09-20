//
// vfruWfObj
//     utility class for Wavefront Obj data
//
#ifdef WINDOWS
#include "stdafx.h"
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <sstream>

#include "utilPath.h"
#include "vfruGetLine.h"
#include "vfruWfObj.h"
#include "vfruTessel.h"

#include "vfrGroup.h"
#include "vfrTriangles.h"


// operators
inline std::istream&
operator >>(std::istream& is, vfruWfObj::V3& v) {
  return is >> v.xyz[0] >> v.xyz[1] >> v.xyz[2];
}

inline std::istream&
operator >>(std::istream& is, vfruWfObj::V2& v) {
  return is >> v.uv[0] >> v.uv[1];
}

inline std::istream&
operator >>(std::istream& is, vfruWfObj::I3& v) {
  is >> v.idx[0];
  if ( is.get() == '/' ) {
    register int pc = is.peek();
    if ( pc == '/' ) { // case of '//'
      is.get();
      if ( isdigit(is.peek()) )
	is >> v.idx[2];
      return is;
    }
    else if ( ! isdigit(pc) )
      return is;
    is >> v.idx[1];
    if ( is.get() == '/' ) {
      if ( isdigit(is.peek()) )
	is >> v.idx[2];
    }
  }
  return is;
}


//=================== error message ===================
inline static void OUT_ERR(std::ostream& os, std::string file,
			   std::string msg, std::string buf =std::string("")) {
  if ( ! os.good() ) return;
  os << "ReadObj: " << file << ": " << msg << std::endl;
  if ( buf.empty() ) return;
  os << "       + " << buf.substr(0, 64); 
  if ( buf.size() > 64 ) os << "...";
  os << std::endl << std::flush;
}


//=================== read file ===================
vfrNode* vfruWfObj::ReadObj(const char* path,
			    std::deque<vfruWfMtl>* pml,
			    std::deque<std::string>* pmpl)
{
  using namespace std;

  string buff;
  register int i, j;
  register int curMtl = -1;
  register int curGrp = 0;

  deque<V3> m_vl;
  deque<V3> m_nl;
  deque<V2> m_tl;
  deque<I3> m_vlIdx;
  deque<I3> m_nlIdx;
  deque<I3> m_tlIdx;
  deque<int> m_mtlPos;
  deque<vfruWfMtl> m_ml;
  deque<int> m_grpPos;
  deque<string> m_grp;
  deque<int> m_grpNumVtx;
  m_grpNumVtx.push_back(0);
  deque<string> sMtlPathList;
  string base_name;

  //// OPEN INFILE ////
  ifstream sf(path);
  if ( ! sf ) {
    OUT_ERR(cerr, path, "open failed");
    return NULL;
  }
#ifdef WINDOWS
  base_name = CES::BaseName(path, ".obj", '\\');
#else
  base_name = CES::BaseName(path, ".obj");
#endif
  if ( base_name.empty() ) base_name = "obj";
  m_grp.push_back(base_name + string("_0"));

  //// READ INFILE ////
  i = 0;
  string keyword, token;
  while ( ! sf.eof() ) {
    VFR::GetLine(sf, buff);
    if ( buff.size() < 1 ) continue;

    // continued line
    while ( buff[buff.size() -1] == '\\' ) {
      buff[buff.size() -1] = ' ';
      string buff2;
      VFR::GetLine(sf, buff2);
      if ( sf.eof() ) break;
      buff.append(buff2);
    }

    // string stream from read buff
    istringstream iss(buff);

    // get keyword
    iss >> keyword;
    if ( keyword.empty() ) continue;
    if ( keyword[0] == '#' ) continue;

    // keyword is mtllib
    if ( keyword == "mtllib" ) {
      iss >> token;
      if ( token.empty() ) {
	OUT_ERR(cerr, path, "imcomplete mtllib", buff);
        continue;
      }
      bool mtlHasRead = false;
      deque<string>::iterator it;
      for ( it = sMtlPathList.begin(); it != sMtlPathList.end(); it++) {
        if ( token == (*it) ) {
          mtlHasRead = true;
          break;
        }
      }
      if ( ! mtlHasRead ) {
	if ( vfruWfMtl::ReadMtl(m_ml, token.c_str(), path) >= 0 )
	  sMtlPathList.push_back(token);
	else
	  OUT_ERR(cerr, path, "can't read mtl file", buff);
      }
      continue;
    }

    // keyword is usemtl
    if ( keyword == "usemtl" ) {
      iss >> token;
      if ( token.empty() ) {
	OUT_ERR(cerr, path, "imcomplete usemtl", buff);
        continue;
      }
      int nm = 0;
      deque<vfruWfMtl>::iterator it;
      for ( it = m_ml.begin(); it != m_ml.end(); it++, nm++) {
        if ( token == it->m_name )
          break;
      }
      if ( nm >= m_ml.size() ) {
	//OUT_ERR(cerr, path, "mtl not found", buff);
        curMtl = -1;
      } else {
        curMtl = nm;
      }
      continue;
    }

    // keyword is 'v' --- vertex ---
    if ( keyword == "v" ) {
      V3 vert;
      iss >> vert;
      m_vl.push_back(vert);
      continue;
    } // End of if keyword is 'v'

    // keyword is 'vn' --- vertex normal ---
    if ( keyword == "vn" ) {
      V3 norm;
      iss >> norm;
      m_nl.push_back(norm);
      continue;
    } // End of if keyword is 'vn'

    // keyword is 'vt' --- texture coord ---
    if ( keyword == "vt" ) {
      V2 texc;
      iss >> texc;
      m_tl.push_back(texc);
      continue;
    } // End of if keyword is 'vt'

    // keyword is 'f' --- face ---
    if ( keyword == "f" ) {
      deque<I3> idxLst;
      I3 wkf, face;

      iss >> wkf;
      while ( 1 ) {
	if ( wkf.idx[0] > 0 ) face.idx[0] = wkf.idx[0] -1;
	else if ( wkf.idx[0] < 0 ) face.idx[0] = m_vl.size() + wkf.idx[0];
	else face.idx[0] = -1;

	if ( wkf.idx[1] > 0 ) face.idx[1] = wkf.idx[1] -1;
	else if ( wkf.idx[1] < 0 ) face.idx[1] = m_tl.size() + wkf.idx[1];
	else face.idx[1] = -1;

	if ( wkf.idx[2] > 0 ) face.idx[2] = wkf.idx[2] -1;
	else if ( wkf.idx[2] < 0 ) face.idx[2] = m_nl.size() + wkf.idx[2];
	else face.idx[2] = -1;

	idxLst.push_back(face);

	if ( iss.eof() ) break;
	iss >> wkf;
      } // end of while

      if ( idxLst.size() < 3 ) {
	OUT_ERR(cerr, path, "imcomplete face", buff);
      }
      else if ( idxLst.size() == 3 ) {
	face.idx[0] = idxLst[0].idx[0];
	face.idx[1] = idxLst[1].idx[0];
	face.idx[2] = idxLst[2].idx[0];
	if ( ! face.IsValid() ) {
	  OUT_ERR(cerr, path, "imcomplete face", buff);
	  continue;
	}
	m_vlIdx.push_back(face);
	face.idx[0] = idxLst[0].idx[1];
	face.idx[1] = idxLst[1].idx[1];
	face.idx[2] = idxLst[2].idx[1];
	m_tlIdx.push_back(face);
	face.idx[0] = idxLst[0].idx[2];
	face.idx[1] = idxLst[1].idx[2];
	face.idx[2] = idxLst[2].idx[2];
	m_nlIdx.push_back(face);
	m_grpPos.push_back(curGrp);
	m_grpNumVtx[curGrp] += 1;
	m_mtlPos.push_back(curMtl);
      }
      else { // over 3 vertices, do tesselation
	Vector3F* pvtx = new Vector3F[idxLst.size()];
	register int idx;
	for ( idx = 0; idx < idxLst.size(); idx++ )
	  pvtx[idx] = m_vl[idxLst[idx].idx[0]].xyz;

	vfruTessel tessel;
	deque<Vector3I> tessedLst
	  = tessel.Triangulate(pvtx, idxLst.size());
	delete[] pvtx;

	if ( tessedLst.size() > 0 ) { // tesselation succeed
	  for ( idx = 0; idx < tessedLst.size(); idx ++ ) {
	    face.idx[0] = idxLst[tessedLst[idx].m_v[0]].idx[0];
	    face.idx[1] = idxLst[tessedLst[idx].m_v[1]].idx[0];
	    face.idx[2] = idxLst[tessedLst[idx].m_v[2]].idx[0];
	    if ( ! face.IsValid() ) {
	      OUT_ERR(cerr, path, "imcomplete face", buff);
	      break;
	    }
	    m_vlIdx.push_back(face);
	    face.idx[0] = idxLst[tessedLst[idx].m_v[0]].idx[1];
	    face.idx[1] = idxLst[tessedLst[idx].m_v[1]].idx[1];
	    face.idx[2] = idxLst[tessedLst[idx].m_v[2]].idx[1];
	    m_tlIdx.push_back(face);
	    face.idx[0] = idxLst[tessedLst[idx].m_v[0]].idx[2];
	    face.idx[1] = idxLst[tessedLst[idx].m_v[1]].idx[2];
	    face.idx[2] = idxLst[tessedLst[idx].m_v[2]].idx[2];
	    m_nlIdx.push_back(face);
	    m_grpPos.push_back(curGrp);
	    m_grpNumVtx[curGrp] += 1;
	    m_mtlPos.push_back(curMtl);
	  } // end of for(idx)
	}
	else { // tesselation failed
	  I3 texc, norm;
	  face.idx[0] = idxLst[0].idx[0];
	  face.idx[1] = idxLst[1].idx[0];
	  face.idx[2] = idxLst[2].idx[0];
	  if ( ! face.IsValid() ) {
	    OUT_ERR(cerr, path, "imcomplete face", buff);
	    continue;
	  }
	  m_vlIdx.push_back(face);
	  texc.idx[0] = idxLst[0].idx[1];
	  texc.idx[1] = idxLst[1].idx[1];
	  texc.idx[2] = idxLst[2].idx[1];
	  m_tlIdx.push_back(texc);
	  norm.idx[0] = idxLst[0].idx[2];
	  norm.idx[1] = idxLst[1].idx[2];
	  norm.idx[2] = idxLst[2].idx[2];
	  m_nlIdx.push_back(norm);
	  m_grpPos.push_back(curGrp);
	  m_grpNumVtx[curGrp] += 1;
	  m_mtlPos.push_back(curMtl);

	  for ( idx = 3; idx < idxLst.size(); idx++ ) {
	    face.idx[1] = face.idx[2];
	    face.idx[2] = idxLst[idx].idx[0];
	    if ( ! face.IsValid() ) {
	      OUT_ERR(cerr, path, "imcomplete face", buff);
	      break;
	    }
	    m_vlIdx.push_back(face);
	    texc.idx[1] = texc.idx[2];
	    texc.idx[2] = idxLst[idx].idx[1];
	    m_tlIdx.push_back(texc);
	    norm.idx[1] = norm.idx[2] ;
	    norm.idx[2] = idxLst[idx].idx[2];
	    m_nlIdx.push_back(norm);
	    m_grpPos.push_back(curGrp);
	    m_grpNumVtx[curGrp] += 1;
	    m_mtlPos.push_back(curMtl);
	  } // end of for(idx)
	}
      }

      continue;
    } // End of if keyword is 'f'

    // keyword is 'g|o' --- group ---
    if ( keyword == "g" || keyword == "o" ) {
      char nmbuff[128];
      token = "";
      iss >> token;
      if ( token.empty() ) {
	sprintf(nmbuff, "%s_%zd", base_name.c_str(), m_grp.size());
	token = nmbuff;
      }
      deque<string>::iterator itg;
      while ( 1 ) {
	for ( itg = m_grp.begin(); itg != m_grp.end(); itg++ )
	  if ( *itg == token ) {
	    sprintf(nmbuff, "%s_%zd", token.c_str(), m_grp.size());
	    token = nmbuff;
	    break;
	  }
	if ( itg == m_grp.end() ) break;
      } // end of while(1)
      m_grp.push_back(token);
      m_grpNumVtx.push_back(0);
      curGrp ++;

      continue;
    } // End of if keyword is 'g|o'

  } // End of while(1)


  //// CREATE SCENE GRAPH ////

  // allocate root node
  vfrGroup* root = new vfrGroup("vfruWfObj");
  if ( ! root ) {
    OUT_ERR(cerr, path, "can't allocate memory for root node");
    return NULL;
  }

  // allocate child nodes
  register bool mlExist = ( m_ml.size() > 0 );
  register bool nlExist = ( m_nl.size() > 0 );
  for ( i = 0; i < m_grp.size(); i++ ) {
    vfrTriangles* pnode = new vfrTriangles(m_grp[i], TRUE);
    if ( ! pnode ) {
      OUT_ERR(cerr, path, "warn: can't allocate memory for some node");
      continue;
    }
    if ( nlExist )
      pnode->alcPools(3 * m_grpNumVtx[i], 3 * m_grpNumVtx[i], 1, -1);
    else
      pnode->alcPools(3 * m_grpNumVtx[i], -1, 1, -1);
    pnode->setNormalMode(AT_PER_VERTEX);
#if 0 // don't allocate colors for each faces
    if ( mlExist ) {
      pnode->alcColors(m_grpNumVtx[i]);
      pnode->setColorMode(AT_PER_FACE);
    }
#endif
    root->addChild(pnode);
  } // end of for(i)

  // set verts...
  for ( i = 0; i < m_vlIdx.size(); i++ ) {
    vfrNode* pnode = root->getChild(m_grpPos[i]);
    if ( ! pnode ) continue;
    j = pnode->getNumVerts();
    pnode->alcVerts(j+3);
    vector3* pv = pnode->getVerts();
    memcpy(pv[j  ], m_vl[m_vlIdx[i].idx[0]].xyz, sizeof(vector3));
    memcpy(pv[j+1], m_vl[m_vlIdx[i].idx[1]].xyz, sizeof(vector3));
    memcpy(pv[j+2], m_vl[m_vlIdx[i].idx[2]].xyz, sizeof(vector3));

    if ( nlExist && m_nlIdx[i].IsValid() ) {
      pnode->alcNormals(j+3);
      pv = pnode->getNormals();
      memcpy(pv[j  ], m_nl[m_nlIdx[i].idx[0]].xyz, sizeof(vector3));
      memcpy(pv[j+1], m_nl[m_nlIdx[i].idx[1]].xyz, sizeof(vector3));
      memcpy(pv[j+2], m_nl[m_nlIdx[i].idx[2]].xyz, sizeof(vector3));
    }

    if ( mlExist && m_mtlPos[i] >= 0 ) {
      // don't set colors for each faces
      //pnode->setColor3(j/3, m_ml[m_mtlPos[i]].m_diffuse);
      pnode->setColor3(0, m_ml[m_mtlPos[i]].m_diffuse);
      pnode->alcIndices(1);
      pnode->setIndice(0, m_mtlPos[i]);
    }
  } // end of for(i)

  // omit empty nodes
  for ( i = 0; i < root->getNumChildren(); i++ ) {
    vfrNode* pobj = root->getChild(i);
    if ( ! pobj ) continue;
    if ( pobj->getNumVerts() < 1 ) {
      root->remChild(pobj);
      i--;
    }
  } // end of for(i)

  // check normals and bbox
  j = root->getNumChildren();
  for ( i = 0; i < j; i++ ) {
    vfrNode* pobj = root->getChild(i);
    if ( ! pobj ) continue;
    pobj->generateBbox();
    if ( pobj->getNumNormals() < 3 ) {
      pobj->generateNormals();
    }
  } // end of for(i)

  if ( pml ) *pml = m_ml;
  if ( pmpl ) *pmpl = sMtlPathList;

  root->notice();
  return root;
}


// STATIC
//  path_tok : mtl path in .obj file
//  ref_path : .obj path
int vfruWfMtl::ReadMtl(std::deque<vfruWfMtl>& ml,
		       const char* path_tok, const char* ref_path)
{
  static char buff[256];
  const char* _DELIM = " \t\r\n";
  FILE* fp = NULL;

  if ( ! path_tok || strlen(path_tok) < 1 ) return -1;
  fp = fopen(path_tok, "r");
  if ( ! fp ) {
    if ( path_tok[0] != '\0' && ref_path && strlen(ref_path) > 0 ) {
#ifdef WINDOWS
      std::string xpath = CES::DirName(ref_path, '\\');
#else
      std::string xpath = CES::DirName(ref_path);
#endif
      if ( xpath.length() > 0 ) {
	xpath = xpath + '/';
	xpath = xpath + path_tok;
	fp = fopen(xpath.c_str(), "r");
      }
    }
    if ( ! fp ) return -1;
  }

  vfruWfMtl mtl;
  char *keyword, *token;
  int state = 0;
  while ( fgets(buff, 256, fp) ) {
    keyword = strtok(buff, _DELIM);
    if ( ! keyword ) continue;
    if ( strlen(keyword) < 1 ) continue;

    if ( ! strncmp(keyword, "newmtl", 6) ) {
      if ( state )
        ml.push_back(mtl);
      mtl.reset();
      token = strtok(NULL, _DELIM);
      if ( token && strlen(token) > 0 )
        mtl.m_name = std::string(token);
      state = 1;
      continue;
    }
    else if ( ! strncmp(keyword, "Ns", 2) ) {
      if ( ! state ) {state = 0; break;}
      token = strtok(NULL, _DELIM);
      if ( token && strlen(token) > 0 )
        mtl.m_shininess = (float)atof(token);
      else {state = 0; break;}
      continue;
    }
    else if ( ! strncmp(keyword, "Ka", 2) ) {
      if ( ! state ) {state = 0; break;}
      token = strtok(NULL, _DELIM);
      if ( token && strlen(token) > 0 ) 
        mtl.m_ambent[0] = (float)atof(token);
      else {state = 0; break;}
      token = strtok(NULL, _DELIM);
      if ( token && strlen(token) > 0 )
        mtl.m_ambent[1] = (float)atof(token);
      else {state = 0; break;}
      token = strtok(NULL, _DELIM);
      if ( token && strlen(token) > 0 )
        mtl.m_ambent[2] = (float)atof(token);
      else {state = 0; break;}
      continue;
    }
    else if ( ! strncmp(keyword, "Kd", 2) ) {
      if ( ! state ) {state = 0; break;}
      token = strtok(NULL, _DELIM);
      if ( token && strlen(token) > 0 )
        mtl.m_diffuse[0] = (float)atof(token);
      else {state = 0; break;}
      token = strtok(NULL, _DELIM);
      if ( token && strlen(token) > 0 )
        mtl.m_diffuse[1] = (float)atof(token);
      else {state = 0; break;}
      token = strtok(NULL, _DELIM);
      if ( token && strlen(token) > 0 )
        mtl.m_diffuse[2] = (float)atof(token);
      else {state = 0; break;}
      continue;
    }
    else if ( ! strncmp(keyword, "Ks", 2) ) {
      if ( ! state ) {state = 0; break;}
      token = strtok(NULL, _DELIM);
      if ( token && strlen(token) > 0 )
        mtl.m_specular[0] = (float)atof(token);
      else {state = 0; break;}
      token = strtok(NULL, _DELIM);
      if ( token && strlen(token) > 0 )
        mtl.m_specular[1] = (float)atof(token);
      else {state = 0; break;}
      token = strtok(NULL, _DELIM);
      if ( token && strlen(token) > 0 )
        mtl.m_specular[2] = (float)atof(token);
      else {state = 0; break;}
      continue;
    }
    else if ( ! strncmp(keyword, "map_Kd", 6) ) {
      if ( ! state ) {state = 0; break;}
      token = strtok(NULL, _DELIM);
      if ( token && strlen(token) > 0 )
        mtl.m_texture = std::string(token);
      else {state = 0; break;}
      continue;
    }
  } // end of while()

  fclose(fp);

  if ( state ) {
    ml.push_back(mtl);
  }

  return ml.size();
}

// EOF
