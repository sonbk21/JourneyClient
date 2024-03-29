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
#include "Graphics\Texture.h"
#include "Util\vector2d.h"
#include "nlnx\node.hpp"
#include <cstdint>
#include <string>

namespace Gameplay
{
	using std::int32_t;
	using std::string;
	using nl::node;
	using Util::vector2d;
	using Graphics::Texture;

	// Represents a tile on a map.
	class Tile
	{
	public:
		Tile(node src, string tileset);
		// Empty destructor.
		~Tile();

		// Draw the tile.
		void draw(vector2d<int16_t> viewpos) const;
		// Returns depth of the tile.
		uint8_t getz() const;

	private:
		Texture texture;
		vector2d<int16_t> pos;
		uint8_t z;
	};
}

