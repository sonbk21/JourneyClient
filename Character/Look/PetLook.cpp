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
#include "PetLook.h"
#include "Program\Constants.h"
#include "nlnx\nx.hpp"
#include "nlnx\node.hpp"

namespace Character
{
	const string stancenames[4] = 
	{
		"stand0", "move", "hang", "fly"
	};

	PetLook::PetLook(int32_t iid, string nm, int32_t uqid,
		vector2d<int16_t> pos, uint8_t st, int32_t fhid) {

		itemid = iid;
		name = nm;
		uniqueid = uqid;
		flip = st % 2 == 1;
		if (flip)
			stance = static_cast<Stance>(st - 1);
		else
			stance = static_cast<Stance>(st);

		setposition(pos.x(), pos.y());
		phobj.fhid = static_cast<uint16_t>(fhid);

		namelabel = Textlabel(Textlabel::DWF_14MC, Textlabel::TXC_WHITE, name, 0);
		namelabel.setback(Textlabel::TXB_NAMETAG);

		using nl::node;
		node src = nl::nx::item["Pet"][std::to_string(iid) + ".img"];
		for (node sub : src)
		{
			string stname = sub.name();
			if (stname != "info")
				animations[stname] = Animation(sub);
		}
	}

	PetLook::PetLook()
	{
		itemid = 0;
		name = "";
		uniqueid = 0;
		stance = Stance::STAND;
	}

	void PetLook::draw(vector2d<int16_t> viewpos, float inter) const
	{
		using Graphics::DrawArgument;
		vector2d<int16_t> absp = phobj.getposition(inter) + viewpos;

		string stname = stancenames[stance / 2];
		if (animations.count(stname))
			animations.at(stname).draw(DrawArgument(absp, flip), inter);

		namelabel.draw(absp);
	}

	void PetLook::update(const Physics& physics, vector2d<int16_t> charpos)
	{
		static const float PETWALKFORCE = 0.35f;
		static const float PETFLYFORCE = 0.2f;

		vector2d<int16_t> curpos = phobj.getposition(1.0f);
		switch (stance)
		{
		case STAND:
		case MOVE:
			if ((charpos - curpos).length() > 150)
			{
				setposition(charpos.x(), charpos.y());
			}
			else
			{
				if (charpos.x() - curpos.x() > 50.0f)
				{
					phobj.hforce = PETWALKFORCE;
					flip = true;
					setstance(MOVE);
				}
				else if (charpos.x() - curpos.x() < -50.0f)
				{
					phobj.hforce = -PETWALKFORCE;
					flip = false;
					setstance(MOVE);
				}
				else
				{
					phobj.hforce = 0.0f;
					setstance(STAND);
				}
			}
			phobj.type = PhysicsObject::NORMAL;
			break;
		case HANG:
			setposition(charpos.x(), charpos.y());
			phobj.type = PhysicsObject::CLIMBING;
			break;
		case FLY:
			if ((charpos - curpos).length() > 250)
			{
				setposition(charpos.x(), charpos.y());
			}
			else
			{
				if (charpos.x() - curpos.x() > 50.0f)
				{
					phobj.hforce = PETFLYFORCE;
					flip = true;
				}
				else if (charpos.x() - curpos.x() < -50.0f)
				{
					phobj.hforce = -PETFLYFORCE;
					flip = false;
				}
				else
				{
					phobj.hforce = 0.0f;
				}

				if (charpos.y() - curpos.y() > 50.0f)
					phobj.vforce = PETFLYFORCE;
				else if (charpos.y() - curpos.y() < -50.0f)
					phobj.vforce = -PETFLYFORCE;
				else
					phobj.vforce = 0.0f;
			}
			phobj.type = PhysicsObject::FLYING;
			break;
		}

		physics.moveobject(phobj);

		string stname = stancenames[stance / 2];
		if (animations.count(stname))
			animations.at(stname).update(Constants::TIMESTEP / 2);
	}

	void PetLook::setposition(int16_t x, int16_t y)
	{
		phobj.fx = static_cast<float>(x);
		phobj.fy = static_cast<float>(y);
	}

	void PetLook::setstance(Stance st)
	{
		bool tempflip = st % 2 == 1;
		Stance newstance;
		if (tempflip)
			newstance = static_cast<Stance>(st - 1);
		else
			newstance = static_cast<Stance>(st);

		if (stance != newstance)
		{
			stance = newstance;

			string stname = stancenames[stance / 2];
			if (animations.count(stname))
				animations[stname].reset();
		}
	}

	int32_t PetLook::getiid() const
	{
		return itemid;
	}

	PetLook::Stance PetLook::getstance() const
	{
		return stance;
	}
}