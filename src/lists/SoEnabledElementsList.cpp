/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoEnabledElementsList SoEnabledElementsList.h Inventor/lists/SoEnabledElementsList.h
  \brief The SoEnabledElementsList class is a container for type info for element types that are enabled in actions.
  \ingroup actions

  This class is probably not interesting for the application
  programmer.
*/

// Metadon doc
/*�
  FIXME: doesn't handle post-initialization changes very well (fixing
  this also needs some tempering to be done with SoAction, I think.)
  20000305 mortene.
 */

#include <Inventor/lists/SoEnabledElementsList.h>
#include <assert.h>


#ifndef DOXYGEN_SKIP_THIS

class SoEnabledElementsListP {
public:
  int prevmerge;
  SoTypeList elements;
  SoEnabledElementsList * parent;
};

static int enable_counter = 0;

#endif // DOXYGEN_SKIP_THIS

#undef THIS
#define THIS this->pimpl

/*!
  Constructor.
*/
SoEnabledElementsList::SoEnabledElementsList(SoEnabledElementsList * const parentlist)
{
  THIS = new SoEnabledElementsListP;

  THIS->prevmerge = 0;
  THIS->parent = parentlist;
}

/*!
  Destructor.
*/
SoEnabledElementsList::~SoEnabledElementsList()
{
  delete THIS;
}

/*!
  Return the list of enabled elements.
*/
const SoTypeList &
SoEnabledElementsList::getElements(void) const
{
  // check if we need a new merge
  if (THIS->prevmerge != enable_counter) {
    int storedcounter = enable_counter;
    SoEnabledElementsList * plist = (SoEnabledElementsList*) THIS->parent;
    while (plist) {
      ((SoEnabledElementsList*)this)->merge(*plist);
      plist = plist->pimpl->parent;
    }
    // use and restore old counter since it might change during merge
    ((SoEnabledElementsList*)this)->pimpl->prevmerge =
      enable_counter = storedcounter;
  }
  return THIS->elements;
}

/*!
  Add an \a elementtype to the list of enabled elements if it's not
  enabled already.
*/
void
SoEnabledElementsList::enable(const SoType elementtype, const int stackindex)
{
  while (stackindex >= THIS->elements.getLength())
    THIS->elements.append(SoType::badType());

  SoType currtype = THIS->elements[stackindex];
  if (currtype.isBad() ||
      (elementtype != currtype && elementtype.isDerivedFrom(currtype))) {
    THIS->elements.set(stackindex, elementtype);
    // increment to detect when a new merge is needed
    enable_counter++;
  }
}

/*!
  Enables all the elements from the \a eel list that is enabled in
  this instance.
*/
void
SoEnabledElementsList::merge(const SoEnabledElementsList & eel)
{
  SoType bad = SoType::badType();
  const int num = eel.pimpl->elements.getLength();
  for (int i = 0; i < num; i++) {
    if (eel.pimpl->elements[i] != bad) this->enable(eel.pimpl->elements[i], i);
  }
}

/*!
  Return the current setting of the global counter used to determine
  when lists are out of date.  It is incremented whenever a new
  element is added to a list.
*/
int
SoEnabledElementsList::getCounter(void)
{
  return enable_counter;
}

#undef THIS
