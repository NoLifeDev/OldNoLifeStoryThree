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
    namespace Game {
        bool Over = false;
        void Init() {
            freopen("log.log", "w", stdout);
            Graphics::Init();
            Sound::Init();
            WZ::AddPath("");
            WZ::AddPath("..");
            WZ::Init();
            Network::Init();
            Map::Init();
            Time::Init();
        }
        void Loop() {
            Time::Loop();
            Network::Loop();
            Graphics::Loop();
            Map::Loop();
        }
        void Unload() {
        }
        void Play() {
            Init();
            while (!Over) Loop();
            Unload();
        }
    }
}