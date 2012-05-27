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
    namespace UI {
        Text log[0x10];
        Text text;
        u16string str;
        void Init() {

        }
        void DrawRect(int x, int y, int w, int h) {
            glBindTexture(GL_TEXTURE_2D, 0);
            glPushMatrix();
            glTranslated(x, y, 0);
            glScaled(w, h, 1);
            glDrawArrays(GL_QUADS, 0, 4);
            glPopMatrix();
        }
        void Loop() {
            glLoadIdentity();
            glColor4d(0, 0, 0, 0.7);
            DrawRect(0, View::Height-272, 0x100, 272);
            glColor4d(1, 1, 1, 1);
            text.Draw(0, View::Height-16);
            int y = View::Height-16;
            for (int i = 0; i < 0x10; ++i) {
                y -= 16;
                log[i].Draw(0, y);
            }
        }
        std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> conv;
        void Key(char16_t c) {
            switch (c) {
            case 13:
                Log(str);
#define Check(x) if (s.size() < x) break;
                if (str[0] == '/') {
                    string com = conv.to_bytes(str.substr(1));
                    str.clear();
                    stringstream ss(com);
                    vector<string> s;
                    while (!ss.eof()) {
                        string st;
                        ss >> st;
                        s.push_back(st);
                    }
                    Check(1);
                    if (s[0] == "login") {
                        Check(3);
                        Network::Send::Login(s[1], s[2]);
                    } else if (s[0] == "load") {
                        Check(2);
                        Map::Load(s[1]);
                    }
                } else str.clear();
                break;
            default:
                str += c;
            }
            text.Set(str);
        }
        void Log(string s) {
            Log(conv.from_bytes(s));
        }
        void Log(u16string s) {
            log[0xf].~Text();
            memmove(log+1, log, 0xf*sizeof(Text));
            new(log) Text;
            log[0].Set(s);
        }
    }
}