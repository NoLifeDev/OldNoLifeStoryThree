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
    namespace View {
        int Width;
        int Height;
        int X, Y;
        int CX, CY;
        double vx = 0, vy = 0;
        double tx = 0, ty = 0;
        void Update() {
            double c1 = Time::Delta*1000;
            double c2 = Time::Delta*3;
            double c3 = 10;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) tx -= c1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) tx += c1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) ty -= c1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) ty += c1;
            double dx = tx-vx;
            double dy = ty-vy;
            vx += dx*c2;
            vy += dy*c2;
            CX = vx;
            CY = vy;
            X = CX-Width/2;
            Y = CY-Height/2;
            glLoadIdentity();
            glTranslated(-X, -Y, 0);
            
        }
    }
}