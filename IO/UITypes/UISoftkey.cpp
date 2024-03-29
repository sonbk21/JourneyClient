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
#include "UISoftKey.h"
#include "IO\UI.h"
#include "Net\Session.h"
#include "IO\Components\MapleButton.h"
#include "Net\Packets\SelectCharPackets83.h"
#include "nlnx\nx.hpp"
#include "nlnx\node.hpp"

namespace IO
{
	const uint8_t NUM_KEYS = 10;

	UISoftkey::UISoftkey(SkType t)
	{
		type = t;

		node src = nl::nx::ui["Login.img"]["Common"]["SoftKey"];
		sprites.push_back(Sprite(src["backgrnd"]));
		sprites.push_back(Sprite(src["backgrnd2"]));
		sprites.push_back(Sprite(src["backgrnd3"]));

		buttons[BT_NEXT] = unique_ptr<Button>(new MapleButton(src["BtNext"]));
		buttons[BT_BACK] = unique_ptr<Button>(new MapleButton(src["BtDel"]));
		buttons[BT_OK] = unique_ptr<Button>(new MapleButton(src["BtOK"], vector2d<int16_t>(72, 235)));
		buttons[BT_CANCEL] = unique_ptr<Button>(new MapleButton(src["BtCancel"], vector2d<int16_t>(13, 235)));

		node keys = src["BtNum"];
		buttons[BT_0] = unique_ptr<Button>(new MapleButton(keys["0"]));
		buttons[BT_1] = unique_ptr<Button>(new MapleButton(keys["1"]));
		buttons[BT_2] = unique_ptr<Button>(new MapleButton(keys["2"]));
		buttons[BT_3] = unique_ptr<Button>(new MapleButton(keys["3"]));
		buttons[BT_4] = unique_ptr<Button>(new MapleButton(keys["4"]));
		buttons[BT_5] = unique_ptr<Button>(new MapleButton(keys["5"]));
		buttons[BT_6] = unique_ptr<Button>(new MapleButton(keys["6"]));
		buttons[BT_7] = unique_ptr<Button>(new MapleButton(keys["7"]));
		buttons[BT_8] = unique_ptr<Button>(new MapleButton(keys["8"]));
		buttons[BT_9] = unique_ptr<Button>(new MapleButton(keys["9"]));

		buttons[BT_OK]->setstate(Button::DISABLED);

		entry = Textlabel(Textlabel::DWF_12ML, Textlabel::TXC_BLACK, "", 0);

		shufflekeys();

		position = vector2d<int16_t>(330, 160);
		dimension = vector2d<int16_t>(140, 280);
		active = true;
	}

	void UISoftkey::draw(float inter) const
	{
		UIElement::draw(inter);

		entry.draw(position + vector2d<int16_t>(15, 41));
	}

	void UISoftkey::buttonpressed(uint16_t id)
	{
		using::std::string;
		string entered = entry.gettext();

		switch (id)
		{
		case BT_0:
		case BT_1:
		case BT_2:
		case BT_3:
		case BT_4:
		case BT_5:
		case BT_6:
		case BT_7:
		case BT_8:
		case BT_9:
			if (entered.size() < 8)
			{
				entered.append(std::to_string(id));
				shufflekeys();
			}
			buttons[id]->setstate(Button::NORMAL);
			break;
		case BT_BACK:
			if (entered.size() > 0)
			{
				entered.pop_back();
			}
			buttons[id]->setstate(Button::NORMAL);
			break;
		case BT_CANCEL:
			active = false;
			break;
		case BT_OK:
			if (entered.size() > 5)
			{
				UI::disable();

				int32_t cid = Net::Session::getlogin().getcharid();
				switch (type)
				{
				case CHARSELECT:
					using Net::SelectCharPicPacket83;
					Net::Session::dispatch(SelectCharPicPacket83(entered, cid));
					break;
				case REGISTER:
					using Net::RegisterPicPacket83;
					Net::Session::dispatch(RegisterPicPacket83(cid, entered));
					break;
				}
				active = false;
			}
			break;
		}

		switch (entered.size())
		{
		case 5:
			buttons[BT_OK]->setstate(Button::DISABLED);
			break;
		case 6:
			buttons[BT_OK]->setstate(Button::NORMAL);
			break;
		case 7:
			if (entry.getlength() == 8)
			{
				for (uint8_t i = 0; i < NUM_KEYS; i++)
				{
					buttons[i]->setstate(Button::NORMAL);
				}
			}
			break;
		case 8:
			for (uint8_t i = 0; i < NUM_KEYS; i++)
			{
				buttons[i]->setstate(Button::DISABLED);
			}
			break;
		}

		entry.settext(entered, 0);
	}

	void UISoftkey::shufflekeys()
	{
		using::std::vector;
		vector<uint8_t> reserve;
		for (uint8_t i = 0; i < NUM_KEYS; i++)
		{
			reserve.push_back(i);
		}
		for (uint8_t i = 0; i < NUM_KEYS; i++)
		{
			size_t rand = random.nextint(reserve.size() - 1);
			vector2d<int16_t> pos = keypos(reserve[rand]);
			buttons[BT_0 + i]->setposition(pos);
			reserve.erase(reserve.begin() + rand);
		}
	}

	vector2d<int16_t> UISoftkey::keypos(uint8_t num) const
	{
		return vector2d<int16_t>(12 + (num % 3) * 39, 94 + (num / 3) * 35);
	}
}
