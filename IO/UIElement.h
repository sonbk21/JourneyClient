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
#include "Components\Button.h"
#include "Cursor.h"
#include "Graphics\Sprite.h"
#include <memory>

namespace IO
{
	using std::vector;
	using std::unique_ptr;
	using Graphics::Sprite;

	// Base class for all types of user interfaces on screen.
	class UIElement
	{
	public:
		virtual ~UIElement() {}

		void togglehide() { active = !active; }
		void makeactive() { active = true; }
		void deactivate() { active = false; }
		bool isactive() const { return active; }

		virtual void draw(float inter) const;
		virtual void update();
		virtual void buttonpressed(uint16_t) = 0;
		virtual Cursor::Mousestate sendmouse(bool, vector2d<int16_t>);
		virtual rectangle2d<int16_t> bounds() const;

	protected:
		bool active;
		vector2d<int16_t> position;
		vector2d<int16_t> dimension;
		map<uint16_t, unique_ptr<Button>> buttons;
		vector<Sprite> sprites;
	};
}