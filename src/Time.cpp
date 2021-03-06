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
        double Jitter;
        steady_clock Clock;
        steady_clock::time_point Last;
        steady_clock::time_point Start;
        vector<double> Deltas;
#ifdef NLS_WINDOWS
        LARGE_INTEGER last;
        LARGE_INTEGER freq;
#endif
        void Init() {
            Start = Clock.now();
            Last = Start;
            Deltas.resize(View::Width, 0);
#ifdef NLS_WINDOWS
            QueryPerformanceCounter(&last);
            QueryPerformanceFrequency(&freq);
#endif
        }
        void Loop() {
            steady_clock::time_point Now = Clock.now();
            Delta = duration_cast<duration<double, ratio<1, 1>>>(Now-Last).count();
            Total = duration_cast<duration<double, ratio<1, 1>>>(Now-Start).count();
#ifdef NLS_WINDOWS
            LARGE_INTEGER now;
            QueryPerformanceCounter(&now);
            Delta = double(now.QuadPart-last.QuadPart)/freq.QuadPart;
            last = now;
#endif
            double fd = min(1., Delta*10);
            FPS = fd/max(Delta, 0.0001) + (1-fd)*FPS;
            Jitter = fd*abs(Deltas[0]-Deltas[1]) + (1-fd)*Jitter;
            Last = Now;
            move_backward(Deltas.begin(), Deltas.end()-1, Deltas.end());
            Deltas[0] = Delta;
        }
    }
}