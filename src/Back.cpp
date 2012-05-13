//////////////////////////////////////////////////////////////////////////
// Copyright 2012 Peter Atechian (Retep998)                             //
//////////////////////////////////////////////////////////////////////////
// This file is part of NoLifeStory.                                    //
//                                                                      //
// NoLifeStory is free software: you can redistribute it and/or modify  //
// it under the terms of the GNU General Public License as published by //
// the Free Software Foundation, either version 3 of the License, or    //
// (at your option) any later version.                                  //
//                                                                      //
// NoLifeStory is distributed in the hope that it will be useful,       //
// but WITHOUT ANY WARRANTY; without even the implied warranty of       //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        //
// GNU General Public License for more details.                         //
//                                                                      //
// You should have received a copy of the GNU General Public License    //
// along with NoLifeStory.  If not, see <http://www.gnu.org/licenses/>. //
//////////////////////////////////////////////////////////////////////////
#include "Global.h"

namespace NLS {
    Back::Back(Node n) {
        x = n["x"];
        y = n["y"];
        rx = n["rx"];
        ry = n["ry"];
        cx = n["cx"];
        cy = n["cy"];
        type = n["type"];
        string bs = n["bS"];
        string no = n["no"];
        Node d = (int)n["ani"]?Base["Map"]["Back"][bs]["ani"][no]:Base["Map"]["Back"][bs]["back"][no];
        spr = d;
    }
    void Back::Draw() {
        int ox = (100+rx)*View::CX/100;
	    int oy = (100+ry)*View::CY/100;
        switch (type) {
        case 0:
            spr.Draw(x+ox, y+oy, false, 1, 0);
            break;
        case 1:
            spr.Draw(x+ox, y+oy, false, 1, 0, cx);
            break;
        case 2:
            spr.Draw(x+ox, y+oy, false, 1, 0, -1, cy);
            break;
        case 3:
            spr.Draw(x+ox, y+oy, false, 1, 0, cx, cy);
            break;
        case 4:
            spr.Draw(x+ox+Time::Total*rx*5, y+oy, false, 1, 0, cx);
            break;
        case 5:
            spr.Draw(x+ox, y+oy+Time::Total*rx*5, false, 1, 0, -1, cy);
            break;
        case 6:
            spr.Draw(x+ox+Time::Total*rx*5, y+oy, false, 1, 0, cx, cy);
            break;
        case 7:
            spr.Draw(x+ox, y+oy+Time::Total*rx*5, false, 1, 0, cx, cy);
            break;
        }
    }
}