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
#include "AbstractItemHandler83.h"
#include "IO\UITypes\UIStatusbar.h"
#include "IO\UI.h"
#include "Gameplay\Stage.h"
#include "Program\Configuration.h"

#include "Journey.h"
#ifdef JOURNEY_USE_OPENGL
#include "Graphics\GraphicsGL.h"
#else
#include "Graphics\GraphicsD2D.h"
#endif

namespace Net
{
	using Character::Inventory;
	using Character::Skillbook;
	using Character::Questlog;
	using Character::Monsterbook;
	using Character::Telerock;

	// Handler for a packet which either contains all character information on first login,
	// or warps the player to a different map.
	class SetfieldHandler83 : public AbstractItemHandler83
	{
		void handle(InPacket& recv) const override
		{
			int32_t channel = recv.readint();
			int8_t mode1 = recv.readbyte();
			int8_t mode2 = recv.readbyte();
			if (mode1 == 0 && mode2 == 0)
			{
				changemap(recv, channel);
			}
			else
			{
				setfield(recv);
			}
		}

		void changemap(InPacket& recv, int32_t channel) const
		{
			recv.skip(3);

			// Spawn information.
			int32_t mapid = recv.readint();
			int8_t portalid = recv.readbyte();

			// Timestamp
			recv.skip(8);

			Gameplay::Stage::getplayer().getstats().setportal(portalid);
			if (channel == Session::getlogin().getchannelid())
			{
				// Change map.
#ifdef JOURNEY_USE_OPENGL
				Graphics::GraphicsGL::clear();
#else
				using::Graphics::GraphicsD2D;
				GraphicsD2D::clear();
#endif

				Gameplay::Stage::loadmap(mapid);
				Gameplay::Stage::respawn();
				IO::UI::enable();
			}
			else
			{
				// Change channel.
			}
		}

		void setfield(InPacket& recv) const
		{
			recv.skip(23);

			int32_t cid = recv.readint();
			bool loaded = Gameplay::Stage::loadplayer(cid);
			if (loaded)
			{
				using::Character::Player;
				Player& player = Gameplay::Stage::getplayer();

				StatsEntry trash(recv);
				recv.readbyte(); // 'buddycap'
				if (recv.readbool())
				{
					string lname = recv.readascii();
				}

				parseinventory(recv, player.getinvent());
				parseskillbook(recv, player.getskills());
				parsequestlog(recv, player.getquests());
				parseminigame(recv);
				parsering1(recv);
				parsering2(recv);
				parsering3(recv);
				parsetelerock(recv, player.gettrock());
				parsemonsterbook(recv, player.getmonsterbook());
				parsenewyear(recv);
				parseareainfo(recv);

				recv.skip(10);

				player.recalcstats(true);

				using IO::Element;
				IO::UI::remove(Element::CHARSELECT);
				IO::UI::remove(Element::SOFTKEYBOARD);

#ifdef JOURNEY_USE_OPENGL
				Graphics::GraphicsGL::clear();
#else
				using::Graphics::GraphicsD2D;
				GraphicsD2D::clear();
#endif

				//parent.getui()add(UI_QUICKSLOTS);
				using::IO::ElementStatusbar;
				IO::UI::add(ElementStatusbar(player.getstats()));
				//parent.getui()add(UI_CHATBAR);
				//parent.getui()add(UI_INVENTORY);
				//parent.getui()getbase()->setactive(true);

				//Game::getcache()->getsounds()->play(MSN_GAMEIN);
				int32_t mapid = player.getstats().getmapid();
				Gameplay::Stage::loadmap(mapid);
				Gameplay::Stage::respawn();

				IO::UI::enable();
				IO::UI::enablegamekeys(true);
			}
		}

		void parseinventory(InPacket& recv, Inventory& invent) const
		{
			invent.setmeso(recv.readint());
			invent.setslots(Inventory::EQUIP, recv.readbyte());
			invent.setslots(Inventory::USE, recv.readbyte());
			invent.setslots(Inventory::SETUP, recv.readbyte());
			invent.setslots(Inventory::ETC, recv.readbyte());
			invent.setslots(Inventory::CASH, recv.readbyte());

			recv.skip(8);

			for (size_t i = 0; i < 3; i++)
			{
				Inventory::InvType inv = (i == 0) ? Inventory::EQUIPPED : Inventory::EQUIP;
				int16_t pos = recv.readshort();
				while (pos != 0)
				{
					int16_t slot = (i == 1) ? -pos : pos;
					parseitem(recv, inv, slot, invent);
					pos = recv.readshort();
				}
			}
			recv.skip(2);

			Inventory::InvType toparse[4] =
			{ 
				Inventory::USE, Inventory::SETUP, Inventory::ETC, Inventory::CASH
			};

			for (size_t i = 0; i < 4; i++)
			{
				Inventory::InvType inv = toparse[i];
				int8_t pos = recv.readbyte();
				while (pos != 0)
				{
					parseitem(recv, inv, pos, invent);
					pos = recv.readbyte();
				}
			}
		}

		void parseskillbook(InPacket& recv, Skillbook& skills) const
		{
			int16_t size = recv.readshort();
			for (int16_t i = 0; i < size; i++)
			{
				int32_t skillid = recv.readint();
				int32_t level = recv.readint();
				int64_t expiration = recv.readlong();
				bool fourthtjob = ((skillid % 100000) / 10000 == 2);
				int32_t masterlevel = fourthtjob ? recv.readint() : -1;
				skills.setskill(skillid, level, masterlevel, expiration);
			}
			size = recv.readshort();
			for (int16_t i = 0; i < size; i++)
			{
				int32_t skillid = recv.readint();
				int32_t cooldown = recv.readshort();
				skills.setcd(skillid, cooldown);
			}
		}

		void parsequestlog(InPacket& recv, Questlog& quests) const
		{
			int16_t size = recv.readshort();
			for (int16_t i = 0; i < size; i++)
			{
				int16_t qid = recv.readshort();
				string qdata = recv.readascii();
				if (quests.isstarted(qid))
				{
					int16_t qidl = quests.getlaststarted();
					quests.addprogress(qidl, qid, qdata);
					i--;
				}
				else
				{
					quests.addstarted(qid, qdata);
				}
			}
			map<int16_t, int64_t> completed;
			size = recv.readshort();
			for (int16_t i = 0; i < size; i++)
			{
				int16_t qid = recv.readshort();
				int64_t time = recv.readlong();
				quests.addcompleted(qid, time);
			}
		}

		void parsemonsterbook(InPacket& recv, Monsterbook& monsterbook) const
		{
			int32_t cover = recv.readint();
			monsterbook.setcover(cover);
			recv.skip(1);
			int16_t size = recv.readshort();
			for (int16_t i = 0; i < size; i++)
			{
				int16_t cid = recv.readshort();
				int8_t mblv = recv.readbyte();
				monsterbook.addcard(cid, mblv);
			}
		}

		void parsering1(InPacket& recv) const
		{
			int16_t rsize = recv.readshort();
			for (int16_t i = 0; i < rsize; i++)
			{
				recv.readint();
				recv.readpadascii(13);
				recv.readint();
				recv.readint();
				recv.readint();
				recv.readint();
			}
		}

		void parsering2(InPacket& recv) const
		{
			int16_t rsize = recv.readshort();
			for (int16_t i = 0; i < rsize; i++)
			{
				recv.readint();
				recv.readpadascii(13);
				recv.readint();
				recv.readint();
				recv.readint();
				recv.readint();
				recv.readint();
			}
		}

		void parsering3(InPacket& recv) const
		{
			int16_t rsize = recv.readshort();
			for (int16_t i = 0; i < rsize; i++)
			{
				recv.readint();
				recv.readint();
				recv.readint();
				recv.readshort();
				recv.readint();
				recv.readint();
				recv.readpadascii(13);
				recv.readpadascii(13);
			}
		}

		void parseminigame(InPacket& recv) const
		{
			int16_t mgsize = recv.readshort();
			for (int16_t i = 0; i < mgsize; i++)
			{
			}
		}

		void parsetelerock(InPacket& recv, Telerock& trock) const
		{
			for (size_t i = 0; i < 5; i++)
			{
				int32_t mapid = recv.readint();
				trock.addlocation(mapid);
			}
			for (size_t i = 0; i < 10; i++)
			{
				int32_t mapid = recv.readint();
				trock.addviplocation(mapid);
			}
		}

		void parsenewyear(InPacket& recv) const
		{
			int16_t nysize = recv.readshort();
			for (int16_t i = 0; i < nysize; i++)
			{
			}
		}

		void parseareainfo(InPacket& recv) const
		{
			map<int16_t, string> areainfo;
			int16_t arsize = recv.readshort();
			for (int16_t i = 0; i < arsize; i++)
			{
				int16_t area = recv.readshort();
				areainfo[area] = recv.readascii();
			}
		}
	};
}