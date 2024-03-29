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
#include "Questlog.h"

namespace Character
{
	Questlog::Questlog() {}

	Questlog::~Questlog() {}

	void Questlog::addstarted(int16_t qid, string qdata)
	{
		started[qid] = qdata;
	}

	void Questlog::addprogress(int16_t qid, int16_t qidl, string qdata)
	{
		progress[qid] = make_pair(qidl, qdata);
	}

	void Questlog::addcompleted(int16_t qid, int64_t time)
	{
		completed[qid] = time;
	}

	bool Questlog::isstarted(int16_t qid)
	{
		return started.count(qid) > 0;
	}

	int16_t Questlog::getlaststarted()
	{
		map<int16_t, string>::iterator qend = started.end();
		qend--;
		return qend->first;
	}
}
