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

/*!
  \class SoMotion3Event SoMotion3Event.h Inventor/events/SoMotion3Event.h
  \brief The SoMotion3Event class contains information about 3D movement
  events.
  \ingroup events

  Motion3 events are generated by devices capable of "3D motion", for
  instance spaceballs. Instances of this class contains information about
  these devices' translations and rotations in all 3 dimensions.

  \sa SoEvent, SoLocation2Event
  \sa SoEventCallback, SoHandleEventAction */

#include <Inventor/events/SoMotion3Event.h>
#include <Inventor/SbName.h>
#include <assert.h>


SO_EVENT_SOURCE(SoMotion3Event);

/*!
  Initialize the type information data.
 */
void
SoMotion3Event::initClass(void)
{
  SO_EVENT_INIT_CLASS(SoMotion3Event, SoEvent);
}

/*!
  Constructor. Resets values for translation and rotation information.
 */
SoMotion3Event::SoMotion3Event(void)
{
  this->translation.setValue(0.0f, 0.0f, 0.0f);
  this->rotation = SbRotation::identity();
}

/*!
  Destructor.
 */
SoMotion3Event::~SoMotion3Event()
{
}

/*!
  Sets the translation value of this event. Method is used by the
  window specific device class(es) generating the event.

  \sa getTranslation()
 */
void
SoMotion3Event::setTranslation(const SbVec3f & t)
{
  this->translation = t;
}

/*!
  Returns translation value. Note that translation are specified relative
  to former events, i.e. the values given here are \e not accumulated
  global coordinates.

  \sa getTime()
 */
const SbVec3f &
SoMotion3Event::getTranslation(void) const
{
  return this->translation;
}

/*!
  Sets the rotation value of this event. Method is used by the
  window specific device class(es) generating the event.
 */
void
SoMotion3Event::setRotation(const SbRotation & r)
{
  this->rotation = r;
}

/*!
  Returns rotational value. Note that rotations are specified relative
  to former events, i.e. the values given here are \e not accumulated
  rotations.

  \sa getTime()
 */
const SbRotation &
SoMotion3Event::getRotation(void) const
{
  return this->rotation;
}
