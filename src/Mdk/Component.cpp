/** This file is part of smp-mdk
 *
 * Copyright (C) 2017 Juan R. Garcia Blanco
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Mdk/Component.h"

using namespace ::Smp::Mdk;

Component::Component(void)
{
}

Component::Component(
        ::Smp::String8 name,
        ::Smp::String8 description,
        ::Smp::IComposite* parent)
    throw (::Smp::InvalidObjectName) :
        Object(name, description)
{
    _SetParent(parent);
}

Component::~Component(void)
{
    this->_parent = NULL;
}

::Smp::IComposite* Component::GetParent(void) const
{
    return this->_parent;
}

void Component::_SetParent(::Smp::IComposite* parent)
{
    this->_parent = parent;
}