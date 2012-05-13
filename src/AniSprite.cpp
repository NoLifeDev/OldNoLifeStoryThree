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
    void AniSprite::operator=(const Node& other) {
        n = other;
        frame = 0;
        delay = 0;
    }
    void AniSprite::Draw(int x, int y, bool flipped, double alpha, double ang, int horz, int vert) {
        Node fn = n[frame];
        if (fn) {
            delay += Time::Delta*1000;
            double d = fn["delay"];
            if (!d) d = 100;
            if (delay > d) {
                ++frame;
                delay -= d;
                fn = n[frame];
                if (!fn) {
                    frame = 0;
                    fn = n[frame];
                }
                d = fn["delay"];
                if (!d) d = 100;
            }
        } else fn = n;
        Sprite s = fn;
        s.Draw(x, y, flipped, alpha, ang, horz, vert);
    }
}