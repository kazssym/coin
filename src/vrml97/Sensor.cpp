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

#include <Inventor/VRMLnodes/SoVRMLSensor.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>

SO_NODE_ABSTRACT_SOURCE(SoVRMLSensor);

void
SoVRMLSensor::initClass(void)
{
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoVRMLSensor, SO_VRML97_NODE_TYPE);
}

SoVRMLSensor::SoVRMLSensor(void)
{
  SO_NODE_CONSTRUCTOR(SoVRMLSensor);

  SO_VRMLNODE_ADD_EVENT_OUT(isActive);
  SO_VRMLNODE_ADD_EXPOSED_FIELD(enabled, (TRUE));
}

SoVRMLSensor::~SoVRMLSensor()
{
}

