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
  \class SoSearchAction SoSearchAction.h Inventor/actions/SoSearchAction.h
  \brief The SoSearchAction class provides methods for searching through scene graphs.
  \ingroup actions

  Nodes can be searched for by pointer, type, and name, or a
  combination of those criteria.  Types can be interpreted as exact
  types, or the type can match nodes derived from it.  Every single
  node can be searched, or normal traversal rules can be followed when
  searching (this is especially important to note with regard to
  switch nodes).

  When using more than one of the setNode(), setType() and setName()
  calls, note that the action will search for node(s) with an \c "AND"
  combination of the given search criteria.
*/

#include <Inventor/actions/SoSearchAction.h>

#include <Inventor/actions/SoSubActionP.h>
#include <Inventor/nodes/SoNode.h>

// *************************************************************************

/*!
  \enum SoSearchAction::LookFor

  Specify the search criterion. This can be a bitwise combination of
  the available values.
*/

/*!
  \enum SoSearchAction::Interest

  Values used when specifiying what node(s) we are interested in: the
  first one found, the last one or all of them.
*/


SO_ACTION_SOURCE(SoSearchAction);


// Overridden from parent class.
void
SoSearchAction::initClass(void)
{
  SO_ACTION_INIT_CLASS(SoSearchAction, SoAction);
}


/*!
  Initializes internal settings with default values. With the default
  settings, the SoSearchAction will ignore all nodes.
*/
SoSearchAction::SoSearchAction(void)
  : lookfor(0), interest(FIRST), searchall(FALSE),
    node(NULL), type(SoType::badType()), name(""),
    path(NULL) // paths(0)
{
  SO_ACTION_CONSTRUCTOR(SoSearchAction);

  static int first = 1;
  if (first) {
    first = 0;
    SO_ACTION_ADD_METHOD_INTERNAL(SoNode, SoNode::searchS);
  }
}

/*!
  Destructor.
*/
SoSearchAction::~SoSearchAction()
{
  this->reset(); // clears everything
}

// *************************************************************************

/*!
  Sets the \a node pointer to search for.

  The action is also automatically configured to set the search
  "interest" to LookFor \c NODE, so there is no need to call
  SoSearchAction::setFind() in anything but special cases. This is
  also the case for the setType() and setName() calls.
*/
void
SoSearchAction::setNode(SoNode * const node)
{
  this->node = node;
  this->lookfor |= NODE;
}

/*!
  Returns the node the SoSearchAction instance is configured
  to search for.
*/
SoNode *
SoSearchAction::getNode(void) const
{
  return this->node;
}

/*!
  Configures the SoSearchAction instance to search for nodes of the
  given \a type, and nodes of classes derived from the given \a type
  if \a chkderived is \c TRUE.
*/
void
SoSearchAction::setType(const SoType type, const SbBool chkderived)
{
  this->type = type;
  this->chkderived = chkderived;
  this->lookfor |= TYPE;
}

/*!
  Returns the node type which is searched for, and whether derived
  classes of that type also returns a match.
*/
SoType
SoSearchAction::getType(SbBool & chkderived) const
{
  chkderived = this->chkderived;
  return this->type;
}

/*!
  Configures the SoSearchAction instance to search for nodes with the
  given \a name.
*/
void
SoSearchAction::setName(const SbName name)
{
  this->name = name;
  this->lookfor |= NAME;
}

/*!
  Returns the name the SoSearchAction instance is configured to search
  for.
*/
SbName
SoSearchAction::getName(void) const
{
  return this->name;
}

/*!
  Configures what to search for in the scene graph.  \a what is a
  bitmask of LookFor flags.

  Default find configuration is to ignore all nodes, but the setFind()
  configuration is updated automatically when any one of
  SoSearchAction::setNode(), SoSearchAction::setType() or
  SoSearchAction::setName() is called.
*/
void
SoSearchAction::setFind(const int what)
{
  this->lookfor = what;
}

/*!
  Returns the search configuration of the action instance.
*/
int
SoSearchAction::getFind(void) const
{
  return this->lookfor;
}

/*!
  Configures whether only the first, the last, or all the searching
  matches are of interest.  Default configuration is \c FIRST.
*/
void
SoSearchAction::setInterest(const Interest interest)
{
  this->interest = interest;
}

/*!
  Returns whether only the first, the last, or all the searching
  matches will be saved.
*/
SoSearchAction::Interest
SoSearchAction::getInterest(void) const
{
  return this->interest;
}

/*!
  Specifies whether normal graph traversal should be done (\a
  searchall is \c FALSE) (which is the default setting), or if every
  single node should be searched (\a searchall is \c TRUE).

  If the \a searchall flag is \c TRUE, even nodes considered "hidden"
  by other actions are searched (like for instance the disabled
  children of SoSwitch nodes).
*/
void
SoSearchAction::setSearchingAll(const SbBool searchall)
{
  this->searchall = searchall;
}

/*!
  Returns the traversal method configuration of the action.
*/
SbBool
SoSearchAction::isSearchingAll(void) const
{
  return this->searchall;
}

/*!
  Returns the path to the node of interest that matched the search
  criterions. If no match was found, \c NULL is returned.

  Note that if \c ALL matches are of interest, the result of a search
  action must be fetched through SoSearchAction::getPaths().

*/
SoPath *
SoSearchAction::getPath(void) const
{
  return this->path;
}

/*!
  Returns a path list of all nodes that matched the search criterions.

  Note that if interest were only \c FIRST or \c LAST,
  SoSearchAction::getPath() should be used instead of this method.
*/
SoPathList &
SoSearchAction::getPaths(void)
{
  return this->paths;
}

/*!
  Resets all the SoSearchAction internals back to their default
  values.
*/
void
SoSearchAction::reset(void)
{
  this->lookfor = 0;
  this->interest = SoSearchAction::FIRST;
  this->searchall = FALSE;
  this->chkderived = TRUE;
  this->node = NULL;
  this->type = SoType::badType();
  this->name = SbName("");
  if (this->path) this->path->unref();
  this->path = NULL;
  this->paths.truncate(0);
}

/*!
  \internal

  Marks the SoSearchAction instance as terminated.
*/
void
SoSearchAction::setFound(void)
{
  this->setTerminated(TRUE);
}

/*!
  \internal

  Returns whether the search action was terminated.
*/

SbBool
SoSearchAction::isFound(void) const
{
  return this->hasTerminated();
}

/*!
  \internal

  Sets the path, or adds the path to the path list, depending on the
  interest configuration.  The path is not copied, so it can not be
  modified after being added without side effects.
*/
void
SoSearchAction::addPath(SoPath * const path)
{
  assert(! this->isFound()); // shouldn't try to add path if found

  switch (this->interest) {
  case FIRST:
    assert(! this->path); // should be NULL
    this->path = path;
    this->path->ref();
    this->setFound();
    break;

  case LAST:
    if (this->path)
      this->path->unref(); // should delete it if possible
    this->path = path;
    this->path->ref();
    break;

  case ALL:
    this->paths.append(path);
    break;

  default:
    assert(FALSE && "Interest setting is invalid");
    break;
  }
}

// *************************************************************************

/*!
  Overloaded from superclass to initialize internal data.
*/
void
SoSearchAction::beginTraversal(SoNode * node)
{
  this->paths.truncate(0);
  if (this->path) this->path->unref();
  this->path = NULL;

  // begin traversal at root node
  this->traverse(node);
}
