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
#include "Foothold.h"

namespace Gameplay
{
	Foothold::Foothold(node src, int8_t l)
	{
		id = static_cast<uint16_t>(stoi(src.name()));
		layer = l;
		prev = src["prev"];
		next = src["next"];
		horizontal = vector2d<int16_t>(src["x1"], src["x2"]);
		vertical = vector2d<int16_t>(src["y1"], src["y2"]);
	}

	Foothold::Foothold()
	{
		id = 0;
		layer = 0;
		prev = 0;
		next = 0;
	}

	Foothold::~Foothold() {}

	float Foothold::getslope() const
	{ 
		return iswall() ? 0 : static_cast<float>(getvdelta()) / gethdelta(); 
	}

	float Foothold::resolvex(float x) const
	{ 
		return isfloor() ? vertical.x() : getslope() * (x - horizontal.x()) + vertical.x(); 
	}
}
