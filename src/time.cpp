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
    namespace Time {
        double Delta = 0;
        double Total = 0;
        double FPS = 0;
        steady_clock Clock;
        steady_clock::time_point Last;
        void Init() {
            Last = Clock.now();
        }
        void Loop() {
            //sleep_until(Last + milliseconds(8));
            steady_clock::time_point Now = Clock.now();
            steady_clock::duration dif = Now - Last;
            Delta = duration_cast<duration<double, ratio<1, 1>>>(dif).count();
            Total = duration_cast<duration<double, ratio<1, 1>>>(Now.time_since_epoch()).count();
            double fd = min(1., Delta*10);
            FPS = fd/max(Delta, 0.0001) + (1-fd)*FPS;
            Last = Now;
        }
    }
}