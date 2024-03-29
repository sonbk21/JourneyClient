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
#include "UIElement.h"

namespace IO
{
	void UIElement::draw(float inter) const
	{
		using::Graphics::DrawArgument;
		for (auto& sprit : sprites)
		{
			sprit.draw(DrawArgument(position), inter);
		}

		for (auto& btit : buttons)
		{
			btit.second->draw(position);
		}
	}

	void UIElement::update()
	{
		for (auto& sprit : sprites)
		{
			sprit.update();
		}
	}

	Cursor::Mousestate UIElement::sendmouse(bool down, vector2d<int16_t> pos)
	{
		Cursor::Mousestate ret = down ? Cursor::MST_CLICKING : Cursor::MST_IDLE;

		for (auto& btit : buttons)
		{
			if (btit.second->isactive() && btit.second->bounds(position).contains(pos))
			{
				if (btit.second->getstate() == Button::NORMAL)
				{
					btit.second->setstate(Button::MOUSEOVER);
					ret = Cursor::MST_CANCLICK;
				}
				else if (btit.second->getstate() == Button::MOUSEOVER)
				{
					if (down)
					{
						btit.second->setstate(Button::PRESSED);
						buttonpressed(btit.first);
					}
					else
					{
						ret = Cursor::MST_CANCLICK;
					}
				}
			}
			else if (btit.second->getstate() == Button::MOUSEOVER)
			{
				btit.second->setstate(Button::NORMAL);
			}
		}

		return ret;
	}

	rectangle2d<int16_t> UIElement::bounds() const
	{
		return rectangle2d<int16_t>(position, position + dimension);
	}
}