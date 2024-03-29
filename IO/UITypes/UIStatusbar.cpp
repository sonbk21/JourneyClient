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
#include "UIStatusbar.h"
#include "UIStatsinfo.h"
#include "IO\UI.h"
#include "IO\Components\MapleButton.h"
#include "nlnx\nx.hpp"

namespace IO
{
	UIStatusbar::UIStatusbar(const Charstats& st) : stats(st) 
	{
		node mainbar = nl::nx::ui["StatusBar2.img"]["mainBar"];
		node chat = nl::nx::ui["StatusBar2.img"]["chat"];

		sprites.push_back(Sprite(mainbar["backgrnd"], vector2d<int16_t>(0, 0)));
		sprites.push_back(Sprite(mainbar["gaugeBackgrd"], vector2d<int16_t>(0, 0)));
		sprites.push_back(Sprite(mainbar["notice"], vector2d<int16_t>(0, 0)));
		sprites.push_back(Sprite(mainbar["lvBacktrnd"], vector2d<int16_t>(0, 0)));
		sprites.push_back(Sprite(mainbar["lvCover"], vector2d<int16_t>(0, 0)));

		expbar = Bar(
			Texture(mainbar.resolve("gauge/exp/0")), 
			Texture(mainbar.resolve("gauge/exp/1")), 
			Texture(mainbar.resolve("gauge/exp/2")), 
			308
			);
		hpbar = Bar(
			Texture(mainbar.resolve("gauge/hp/0")), 
			Texture(mainbar.resolve("gauge/hp/1")), 
			Texture(mainbar.resolve("gauge/hp/2")), 
			137
			);
		mpbar = Bar(
			Texture(mainbar.resolve("gauge/mp/0")), 
			Texture(mainbar.resolve("gauge/mp/1")), 
			Texture(mainbar.resolve("gauge/mp/2")), 
			137
			);

		statset = Charset(mainbar.resolve("gauge/number"), Charset::RIGHT);
		levelset = Charset(mainbar.resolve("lvNumber"), Charset::LEFT);

		joblabel = Textlabel(Textlabel::DWF_12ML, Textlabel::TXC_YELLOW, stats.getjobname(), 0);
		namelabel = Textlabel(Textlabel::DWF_14ML, Textlabel::TXC_WHITE, stats.getname(), 0);

		buttons[BT_WHISPER] = unique_ptr<Button>(new MapleButton(mainbar["BtChat"]));
		buttons[BT_CALLGM] = unique_ptr<Button>(new MapleButton(mainbar["BtClaim"]));

		buttons[BT_CASHSHOP] = unique_ptr<Button>(new MapleButton(mainbar["BtCashShop"]));
		buttons[BT_TRADE] = unique_ptr<Button>(new MapleButton(mainbar["BtMTS"]));
		buttons[BT_MENU] = unique_ptr<Button>(new MapleButton(mainbar["BtMenu"]));
		buttons[BT_OPTIONS] = unique_ptr<Button>(new MapleButton(mainbar["BtSystem"]));

		buttons[BT_CHARACTER] = unique_ptr<Button>(new MapleButton(mainbar["BtCharacter"]));
		buttons[BT_STATS] = unique_ptr<Button>(new MapleButton(mainbar["BtStat"]));
		buttons[BT_QUEST] = unique_ptr<Button>(new MapleButton(mainbar["BtQuest"]));
		buttons[BT_INVENTORY] = unique_ptr<Button>(new MapleButton(mainbar["BtInven"]));
		buttons[BT_EQUIPS] = unique_ptr<Button>(new MapleButton(mainbar["BtEquip"]));
		buttons[BT_SKILL] = unique_ptr<Button>(new MapleButton(mainbar["BtSkill"]));

		position = vector2d<int16_t>(512, 590);
		dimension = vector2d<int16_t>(1366, 80);
		active = true;
	}

	void UIStatusbar::draw(float inter) const
	{
		UIElement::draw(inter);

		int64_t currentexp = stats.getexp();
		int64_t expneeded = stats.getexpneeded();
		uint16_t currenthp = stats.getstat(Character::MS_HP);
		uint16_t currentmp = stats.getstat(Character::MS_MP);
		uint32_t maxhp = stats.gettotal(Character::ES_HP);
		uint32_t maxmp = stats.gettotal(Character::ES_MP);
		float exppercent = static_cast<float>(static_cast<double>(currentexp) / expneeded);

		expbar.draw(position + vector2d<int16_t>(-261, -15), exppercent);
		hpbar.draw(position + vector2d<int16_t>(-261, -31), static_cast<float>(currenthp) / maxhp);
		mpbar.draw(position + vector2d<int16_t>(-90, -31), static_cast<float>(currentmp) / maxmp);

		string expstring = std::to_string(100 * exppercent);
		statset.draw(
			std::to_string(currentexp) + "[" + expstring.substr(0, expstring.find('.') + 3) + "%]",
			position + vector2d<int16_t>(47, -13)
			);
		statset.draw(
			"[" + std::to_string(currenthp) + "/" + std::to_string(maxhp) + "]",
			position + vector2d<int16_t>(-124, -29)
			);
		statset.draw(
			"[" + std::to_string(currentmp) + "/" + std::to_string(maxmp) + "]",
			position + vector2d<int16_t>(47, -29)
			);
		levelset.draw(
			std::to_string(stats.getstat(Character::MS_LEVEL)),
			position + vector2d<int16_t>(-480, -24)
			);

		joblabel.draw(position + vector2d<int16_t>(-435, -22));
		namelabel.draw(position + vector2d<int16_t>(-435, -37));
	}

	void UIStatusbar::buttonpressed(uint16_t id)
	{
		switch (id)
		{
		/*case BT_BAR_SYSOP:
			uinterface.add(UI_SYSTEM);
			break;*/
		case BT_STATS:
			UI::add(ElementStatsinfo(stats));
			break;
		/*case BT_BAR_EQUIPS:
			uinterface.add(UI_EQUIPS);
			break;*/
		}
		buttons[id].get()->setstate(Button::MOUSEOVER);
	}

	rectangle2d<int16_t> UIStatusbar::bounds() const
	{
		return rectangle2d<int16_t>(
			position - vector2d<int16_t>(512, 84), 
			position - vector2d<int16_t>(512, 84) + dimension
			);
	}
}