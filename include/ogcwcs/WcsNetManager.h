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
#ifndef _WCSNETMANAGER_
#define _WCSNETMANAGER_


#include <cpw/thread/RequestThread.h>

#include "Export.h"



namespace cpw { 
	
	namespace ogcwcs
	{

		/** \brief Thread that resolves WCS petitions through the net
			\ingroup ogcwcs
		*/
		class OGCWCSEXPORT WcsNetManager: public cpw::RequestThread
		{

		public:

			WcsNetManager(IRequestReceiver *in_thread, int npet);

			~WcsNetManager(void){}

			virtual void Process(cpw::Request &request);

			virtual void PreProcess(){}


		private:



		};
	}

}

#endif