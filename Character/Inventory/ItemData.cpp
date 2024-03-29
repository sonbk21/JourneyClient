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
#include "ItemData.h"
#include "nlnx\nx.hpp"

namespace Character
{
	ItemData::ItemData(int32_t id)
	{
		using nl::node;
		node src;
		node strsrc;

		int32_t prefix = id / 1000000;
		switch (prefix)
		{
		case 1:
			category = geteqcategory(id);
			src = nl::nx::character[category]["0" + std::to_string(id) + ".img"];
			strsrc = nl::nx::string["Eqp.img"]["Eqp"][category][std::to_string(id)];
			break;
		case 2:
			category = "Consume";
			src = nl::nx::item["Consume"]["0" + std::to_string(id / 10000) + ".img"]["0" + std::to_string(id)]["info"];
			strsrc = nl::nx::string["Consume.img"][std::to_string(id)];
			break;
		case 3:
			category = "Install";
			src = nl::nx::item["Install"]["0" + std::to_string(id / 10000) + ".img"]["0" + std::to_string(id)]["info"];
			strsrc = nl::nx::string["Ins.img"][std::to_string(id)];
			break;
		case 4:
			category = "Etc";
			src = nl::nx::item["Etc"]["0" + std::to_string(id / 10000) + ".img"]["0" + std::to_string(id)]["info"];
			strsrc = nl::nx::string["Etc.img"]["Etc"][std::to_string(id)];
			break;
		case 5:
			category = "Cash";
			src = nl::nx::item["Cash"]["0" + std::to_string(id / 10000) + ".img"]["0" + std::to_string(id)]["info"];
			strsrc = nl::nx::string["Cash.img"][std::to_string(id)];
			break;
		}

		if (src.size() > 0)
		{
			icons[false] = Texture(src["icon"]);
			icons[true] = Texture(src["iconRaw"]);

			name = strsrc["name"];
			desc = strsrc["desc"];

			loaded = true;
		}
		else
		{
			loaded = false;
		}

		itemid = id;
	}

	ItemData::ItemData()
	{
		loaded = false;
		itemid = 0;
		name = "";
		desc = "";
	}

	ItemData::~ItemData() {}

	string ItemData::geteqcategory(int32_t id) const
	{
		static const string categorynames[15] =
		{
			"Cap", "Accessory", "Accessory", "Accessory", "Coat", "Longcoat",
			"Pants", "Shoes", "Glove", "Shield", "Cape", "Ring", "Accessory",
			"Accessory", "Accessory"
		};

		int32_t prefix = (id / 10000) - 100;
		if (prefix >= 0 && prefix < 15)
			return categorynames[prefix];
		else if (prefix >= 30 && prefix <= 70)
			return "Weapon";
		else
			return "";
	}

	const string& ItemData::getcategory() const
	{
		return category;
	}

	bool ItemData::isloaded() const
	{
		return loaded;
	}

	int32_t ItemData::getid() const
	{
		return itemid;
	}

	const string& ItemData::getname() const
	{
		return name;
	}

	const string& ItemData::getdesc() const
	{
		return desc;
	}

	const Texture& ItemData::geticon(bool raw) const
	{
		return icons.at(raw);
	}
}
