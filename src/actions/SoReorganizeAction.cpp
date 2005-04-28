/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

/*!
  \class SoReorganizeAction Inventor/include/SoReorganizeAction.h
  \brief The SoReorganizeAction class ...

  Note. This is work-in-progress. pederb, 2005-04-05.

*/

#include <Inventor/SbName.h>
#include <Inventor/actions/SoReorganizeAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoVertexShape.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/elements/SoTextureImageElement.h>
#include <Inventor/VRMLnodes/SoVRMLShape.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/caches/SoPrimitiveVertexCache.h>
#include <Inventor/elements/SoMultiTextureEnabledElement.h>
#include <Inventor/elements/SoTextureEnabledElement.h>
#include <Inventor/elements/SoTexture3EnabledElement.h>
#include <Inventor/elements/SoTextureCoordinateElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/elements/SoMultiTextureCoordinateElement.h>
#include <Inventor/SbColor4f.h>
#include <string.h>
#include <assert.h>

#include <coindefs.h> // COIN_STUB()

class SoReorganizeActionP {
 public:
  SoReorganizeActionP(SoReorganizeAction * masterin) 
    : master(masterin),
      gennormals(TRUE),
      gentexcoords(TRUE),
      gentristrips(FALSE),
      genvp(FALSE),
      matchidx(TRUE),
      cbaction(SbViewportRegion(640, 480)),
      pvcache(NULL)
  {
    cbaction.addTriangleCallback(SoVertexShape::getClassTypeId(), triangle_cb, this);
    cbaction.addPreCallback(SoVertexShape::getClassTypeId(),
                            pre_shape_cb, this);
    cbaction.addPostCallback(SoVertexShape::getClassTypeId(),
                             post_shape_cb, this);
    
  } 
  SoReorganizeAction * master;
  SbBool gennormals;
  SbBool gentexcoords;
  SbBool gentristrips;
  SbBool genvp;
  SbBool matchidx;
  SbList <SbBool> needtexcoords;
  int lastneeded;
  int numtriangles;
  SbBool hastexture;
  SbColor4f diffusecolor;

  SoCallbackAction cbaction;
  SoSearchAction sa;
  SoPrimitiveVertexCache * pvcache;

  static SoCallbackAction::Response pre_shape_cb(void * userdata, SoCallbackAction * action, const SoNode * node);
  static SoCallbackAction::Response post_shape_cb(void * userdata, SoCallbackAction * action, const SoNode * node);
  static void triangle_cb(void * userdata, SoCallbackAction * action,
                          const SoPrimitiveVertex * v1,
                          const SoPrimitiveVertex * v2,
                          const SoPrimitiveVertex * v3);

  SbBool initShape(SoCallbackAction * action);
  void replaceNode(SoFullPath * path);
};



#define PRIVATE(obj) obj->pimpl

SO_ACTION_SOURCE(SoReorganizeAction);

// Override from parent class.
void
SoReorganizeAction::initClass(void)
{
  SO_ACTION_INTERNAL_INIT_CLASS(SoReorganizeAction, SoSimplifyAction);
}


/*!
  A constructor.
*/

SoReorganizeAction::SoReorganizeAction(SoSimplifier * simplifier)
{
  PRIVATE(this) = new SoReorganizeActionP(this);
  SO_ACTION_CONSTRUCTOR(SoReorganizeAction);
}

/*!
  The destructor.
*/

SoReorganizeAction::~SoReorganizeAction(void)
{
  delete PRIVATE(this);
}

SoSeparator * 
SoReorganizeAction::getSimplifiedSceneGraph(void) const
{
  return NULL;
}

void 
SoReorganizeAction::generateNormals(SbBool onoff)
{
  PRIVATE(this)->gennormals = onoff;
}

SbBool 
SoReorganizeAction::areNormalGenerated(void) const
{
  return PRIVATE(this)->gennormals;
}

void 
SoReorganizeAction::generateTriangleStrips(SbBool onoff)
{
  PRIVATE(this)->gentristrips = onoff;
}

SbBool 
SoReorganizeAction::areTriangleStripGenerated(void) const
{
  return PRIVATE(this)->gentristrips;
}

void 
SoReorganizeAction::generateTexCoords(SbBool onoff)
{
  PRIVATE(this)->gentexcoords = onoff;
}

SbBool 
SoReorganizeAction::areTexCoordsGenerated(void) const
{
  return PRIVATE(this)->gentexcoords;
}

void 
SoReorganizeAction::generateVPNodes(SbBool onoff)
{
  PRIVATE(this)->genvp = onoff;
}

SbBool 
SoReorganizeAction::areVPNodesGenerated(void)
{
  return PRIVATE(this)->genvp;
}

void 
SoReorganizeAction::matchIndexArrays(SbBool onoff)
{
  PRIVATE(this)->matchidx = onoff;
}

SbBool 
SoReorganizeAction::areIndexArraysMatched(void) const
{
  return PRIVATE(this)->matchidx;
}

SoSimplifier * 
SoReorganizeAction::getSimplifier(void) const
{
  return NULL;
}

void 
SoReorganizeAction::apply(SoNode * root)
{
  PRIVATE(this)->sa.setType(SoVertexShape::getClassTypeId());
  PRIVATE(this)->sa.setSearchingAll(TRUE);
  PRIVATE(this)->sa.setInterest(SoSearchAction::ALL);
  PRIVATE(this)->sa.apply(root);
  SoPathList & pl = PRIVATE(this)->sa.getPaths();
  for (int i = 0; i < pl.getLength(); i++) {
    this->apply(pl[i]);
  }
  PRIVATE(this)->sa.reset();
}

void 
SoReorganizeAction::apply(SoPath * path)
{
  PRIVATE(this)->cbaction.apply(path);
  PRIVATE(this)->replaceNode((SoFullPath*) path);
}

void 
SoReorganizeAction::apply(const SoPathList & pathlist, SbBool obeysrules)
{
  for (int i = 0; i < pathlist.getLength(); i++) {
    this->apply(pathlist[i]);
  }
}

void 
SoReorganizeAction::startReport(const char * msg)
{
  COIN_STUB();
}

void 
SoReorganizeAction::finishReport(void)
{
  COIN_STUB();
}

// Documented in superclass.
void
SoReorganizeAction::beginTraversal(SoNode * /* node */)
{
  assert(0 && "should never get here");
}


SoCallbackAction::Response 
SoReorganizeActionP::pre_shape_cb(void * userdata, SoCallbackAction * action, const SoNode * node)
{
  SoReorganizeActionP * thisp = (SoReorganizeActionP*) userdata;
  thisp->numtriangles = 0;
  return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response 
SoReorganizeActionP::post_shape_cb(void * userdata, SoCallbackAction * action, const SoNode * node)
{
  SoReorganizeActionP * thisp = (SoReorganizeActionP*) userdata;

#if 0 // debug
  fprintf(stderr,"shape: %s, numtri: %d, pvcache: %p\n",
          node->getTypeId().getName().getString(),
          thisp->numtriangles,
          thisp->pvcache);
#endif // debug
  return SoCallbackAction::CONTINUE;
}
  
void 
SoReorganizeActionP::triangle_cb(void * userdata, SoCallbackAction * action,
                                 const SoPrimitiveVertex * v1,
                                 const SoPrimitiveVertex * v2,
                                 const SoPrimitiveVertex * v3) 
{
  SoReorganizeActionP * thisp = (SoReorganizeActionP*) userdata;
  
  if (thisp->numtriangles == 0) {
    if (thisp->initShape(action)) {
      assert(thisp->pvcache == NULL);
      thisp->pvcache = new SoPrimitiveVertexCache(action->getState());
      thisp->pvcache->ref();

    }
  }

  thisp->numtriangles++;
  if (thisp->pvcache) {
    thisp->pvcache->addTriangle(v1, v2, v3);
  }
}

SbBool
SoReorganizeActionP::initShape(SoCallbackAction * action)
{
  SoState * state = action->getState();
  SbBool canrenderasvertexarray = TRUE;

  unsigned int shapeflags = SoShapeStyleElement::get(state)->getFlags();

  SbBool texture0enabled =
    SoTextureEnabledElement::get(state) != FALSE;

  this->hastexture = texture0enabled;

  int lastenabled;
  const SbBool * enabledunits = 
    SoMultiTextureEnabledElement::getEnabledUnits(state, lastenabled);

  this->needtexcoords.truncate(0);
  this->needtexcoords.append(FALSE);

  if (shapeflags & 
      (SoShapeStyleElement::BUMPMAP|
       SoShapeStyleElement::BBOXCMPLX|
       SoShapeStyleElement::INVISIBLE|
       SoShapeStyleElement::BIGIMAGE)) {
    canrenderasvertexarray = FALSE;
  }


  if (canrenderasvertexarray && texture0enabled) {
    const SoTextureCoordinateElement * celem =
      (const SoTextureCoordinateElement *) SoTextureCoordinateElement::getInstance(state);
    switch (celem->getType()) {
    case SoTextureCoordinateElement::DEFAULT:
    case SoTextureCoordinateElement::EXPLICIT:
      this->needtexcoords[0] = TRUE;
      break;
    case SoTextureCoordinateElement::TEXGEN:
      // don't need texcoords for unit0
      break;
    case SoTextureCoordinateElement::FUNCTION:
      this->needtexcoords[0] = TRUE;
    default:
      canrenderasvertexarray = FALSE;
      break;
    }
  }
  
  if (canrenderasvertexarray && enabledunits) {
    const SoMultiTextureCoordinateElement * melem =
      SoMultiTextureCoordinateElement::getInstance(state);
    for (int i = 1; i <= lastenabled; i++) {
      this->needtexcoords.append(FALSE);
      if (enabledunits[i]) {
        // FIXME: multitexturing is not supported yet, since it's not
        // supported by SoVertexProperty. We might fix this later by
        // inserting new nodes though. pederb, 2005-04-28
        canrenderasvertexarray = FALSE; 

        this->hastexture = TRUE;
        switch (melem->getType(i)) {
        case SoTextureCoordinateElement::DEFAULT:
        case SoTextureCoordinateElement::EXPLICIT:
          this->needtexcoords[i] = TRUE;
          break;
        case SoTextureCoordinateElement::TEXGEN:
          // don't need texcoords for unit i
          break;
        case SoTextureCoordinateElement::FUNCTION:
          this->needtexcoords[i] = TRUE;
          break;
        default:
          canrenderasvertexarray = FALSE;
          break;
        }
      }
    }
  }

  if (canrenderasvertexarray) {
    SbColor diffuse = SoLazyElement::getDiffuse(state, 0);
    float transp = SoLazyElement::getTransparency(state, 0); 
    this->diffusecolor = SbColor4f(diffuse, 1.0f - transp);
  }

  return canrenderasvertexarray;
}

void 
SoReorganizeActionP::replaceNode(SoFullPath * path)
{
  if (this->pvcache == NULL) return;

  if (this->pvcache->colorPerVertex()) {
    // color-per-vertex not supported yet because of endian issues
    this->pvcache->unref();
    this->pvcache = NULL;
    return;
  }

  SoIndexedFaceSet * ifs = new SoIndexedFaceSet;
  SoVertexProperty * vp = new SoVertexProperty;
  vp->normalBinding = SoVertexProperty::PER_VERTEX_INDEXED;
  vp->materialBinding = SoVertexProperty::OVERALL;
  vp->orderedRGBA = this->diffusecolor.getPackedValue();
  ifs->ref();
  ifs->vertexProperty = vp;
  
  int numv = this->pvcache->getNumVertices();

  if (this->hastexture) {
    vp->texCoord.setNum(numv);
    SbVec2f * dst = vp->texCoord.startEditing();
    const SbVec4f * src = this->pvcache->getTexCoordArray();
    
    for (int i = 0; i < numv; i++) {
      SbVec4f tmp = src[i];
      if (tmp[3] != 0.0f) {
        tmp[0] /= tmp[3];
        tmp[1] /= tmp[3];
      }
      dst[i][0] = tmp[0];
      dst[i][1] = tmp[1];
    }
    vp->texCoord.finishEditing();
  }

  vp->materialBinding = SoVertexProperty::OVERALL;
  vp->vertex.setValues(0, numv,
                       this->pvcache->getVertexArray());
  vp->normal.setValues(0, numv,
                       this->pvcache->getNormalArray());
  
  ifs->normalIndex.setNum(0);
  ifs->materialIndex.setNum(0);
  ifs->textureCoordIndex.setNum(0);
  
  int numtri = this->pvcache->getNumIndices() / 3;
  const int32_t * indices = this->pvcache->getIndices();
  ifs->coordIndex.setNum(numtri * 4);
  int32_t * ptr = ifs->coordIndex.startEditing();

  for (int i = 0; i < numtri; i++) {
    *ptr++ = indices[i*3];
    *ptr++ = indices[i*3+1];
    *ptr++ = indices[i*3+2];
    *ptr++ = -1;
  }
  ifs->coordIndex.finishEditing();
  
  SoNode * parent = path->getNodeFromTail(1);
  int idx = path->getIndexFromTail(0);
    
  path->pop();
  if (parent->isOfType(SoGroup::getClassTypeId())) {
    SoGroup * g = (SoGroup*)parent;
    g->replaceChild(idx, ifs);
  }
  else if (parent->isOfType(SoVRMLShape::getClassTypeId())) {
    SoVRMLShape * vs = (SoVRMLShape*) parent;
    vs->geometry = ifs;
  }
  path->push(idx);
  ifs->unrefNoDelete();

  this->pvcache->unref();
  this->pvcache = NULL;
}
