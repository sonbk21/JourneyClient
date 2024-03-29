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
#include "Tile.h"
#include "nlnx\nx.hpp"

namespace Gameplay
{
	Tile::Tile(node src, string ts)
	{
		node dsrc = nl::nx::map["Tile"][ts][src["u"]][src["no"]];
		texture = Texture(dsrc);
		pos = vector2d<int16_t>(src["x"], src["y"]);
		z = dsrc["z"];
		if (z == 0)
			z = dsrc["zM"];
	}

	Tile::~Tile() {}

	void Tile::draw(vector2d<int16_t> viewpos) const
	{
		using::Graphics::DrawArgument;
		texture.draw(DrawArgument(pos + viewpos));
	}

	uint8_t Tile::getz() const
	{
		return z;
	}
}
