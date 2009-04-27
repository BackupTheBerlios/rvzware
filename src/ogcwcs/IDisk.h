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
#ifndef _IDISKWCS_
#define _IDISKWCS_

#include <vector>
#include <string>

#include <ogcwcs/Export.h>

namespace cpw 
{ 
	namespace ogcwcs
	{

		/** 
			\brief Interface for disk management 
			\ingroup ogcwcs
		*/
		class OGCWCSEXPORT IDisk
		{

		public:
			virtual ~IDisk(){}
			virtual int Dir(const std::string &dir, std::vector<std::string> &files) = 0;
			virtual int Save(const std::string &name, const std::string &data) = 0;
			virtual int Load(const std::string &name, std::string &data) = 0;
			virtual int RemoveFile(const std::string &name) = 0;
			virtual int RemoveDir(const std::string &name) = 0;
			virtual int MakeDir(const std::string &name) = 0;
		};

	}

}

#endif