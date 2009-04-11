/*
 * CAPAWARE 3D Geographical Multilayer Framework Free Software
 * Copyright (C) 2009 Instituto Tecnol�gico de Canarias (ITC) and Universidad de Las Palmas de Gran Canaria (ULPGC)
 *
 * This file is part of CAPAWARE.
 *
 * CAPAWARE is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * CAPAWARE is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this application; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * The CAPAWARE development team
*/
#ifndef _ICALLBACK_
#define _ICALLBACK_

#include "Callable.h"
#include <cpw/Export.h>

namespace cpw
{
	class CPWEXPORT Callable;

	/** 
		\brief Interface for the callback entities
		\ingroup cpw
	*/
	class ICallBack
	{
		public:
			
			/** Destructor initialized empty */
			virtual ~ICallBack() {};

			/** Inserts ...
				\param callable
			*/
			virtual void Insert(const cpw::Callable* callable) {}
			
			/** Inserts ...
				\param callable
			*/
			virtual void Update(cpw::Callable* callable) {}
			
			/** Inserts ...
				\param callable
			*/
			virtual void Delete(cpw::Callable* callable) {}
			
			/** Inserts ...
				\param callable
				\param value
			*/
			virtual void Visualize(cpw::Callable* callable, const bool &value) {}
			
			/** Virtual function to implement the prototype pattern */
			virtual ICallBack* Clone() {return NULL;}

	};
}

#endif
