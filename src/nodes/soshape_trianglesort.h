/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#ifndef COIN_SOSHAPE_TRIANGLESORT_H
#define COIN_SOSHAPE_TRIANGLESORT_H


#include <Inventor/lists/SbList.h>
#include <Inventor/SoPrimitiveVertex.h>

class SoState;
class SoPrimitiveVertex;
class SoMaterialBundle;

class soshape_trianglesort {
public:
  soshape_trianglesort();
  ~soshape_trianglesort();

  void beginShape(SoState * state);
  void triangle(SoState * state,
                const SoPrimitiveVertex * v1,
                const SoPrimitiveVertex * v2,
                const SoPrimitiveVertex * v3);
  void endShape(SoState * state, SoMaterialBundle & mb);

  typedef struct {
    int idx : 31;
    unsigned int backface : 1;
    float dist;
  } sorted_triangle;

private:

  SbList <SoPrimitiveVertex> * pvlist;
  SbList <sorted_triangle> * trianglelist;
};

#endif // !COIN_SOSHAPE_TRIANGLESORT_H


