/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright � 2015 Daniel Allendorf                                        //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include <cstdint>
#include <map>

namespace Character
{
	using::std::int32_t;
	using::std::int64_t;
	using::std::map;
	// Class that stores all information about the skills of an individual character.
	class Skillbook
	{
	public:
		Skillbook();
		~Skillbook();
		void setskill(int32_t, int32_t, int32_t, int64_t);
		void setcd(int32_t, int32_t);
	private:
		map<int32_t, int32_t> levels;
		map<int32_t, int32_t> masterlevels;
		map<int32_t, int32_t> cooldowns;
		map<int32_t, int64_t> expirations;
	};
}

