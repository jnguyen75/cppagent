// -*- c++ -*-
/* $Id: init.cc 54 2003-11-03 09:27:33Z murrayc $ */

/* Copyright (C) 2003 The glibmm Development Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <glibmm/wrap.h>
#include <glibmm/error.h>

namespace Glib
{

void init()
{
  Glib::wrap_register_init();
  Glib::Error::register_init(); //also calls Glib::wrap_init();  
}

} // namespace Glib
