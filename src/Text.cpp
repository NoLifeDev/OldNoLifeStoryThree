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
    uint8_t font[0x200000];
    bool full[0x10000];
    uint8_t buf[0x1000];
    GLfloat red[2] = {0, 1};
    GLfloat green[2] = {0, 1};
    GLfloat blue[2] = {0, 1};
    GLfloat alpha[2] = {0, 1};
    void Text::Init() {
#ifdef _M_X64
        ifstream file("../Unifont.bin");
#else
        ifstream file("Unifont.bin");
#endif
        file.read((char*)full, 0x10000);
        file.read((char*)font, 0x200000);
        glPixelMapfv(GL_PIXEL_MAP_I_TO_R, 2, red);
		glPixelMapfv(GL_PIXEL_MAP_I_TO_G, 2, green);
		glPixelMapfv(GL_PIXEL_MAP_I_TO_B, 2, blue);
        glPixelMapfv(GL_PIXEL_MAP_I_TO_A, 2, alpha);
    }
    Text::Text() {
        tex = 0;
        w = 0;
    }
    Text::~Text() {
        if (tex) glDeleteTextures(1, &tex);
    }
    std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> conv;
    void Text::Set(string s) {
        Set(conv.from_bytes(s));
    }
    void Text::Set(u16string s) {
        if (!tex) glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        w = 0;
        for (int i = 0; i < s.size(); ++i) {
            if(full[s[i]]) w += 2;
            else w += 1;
        }
        for (int i = 0, j = 0; i < s.size(); ++i) {
            char16_t c = s[i];
            bool f = full[c];
            for (int k = 0; k < 16; ++k) {
                buf[j+k*w] = font[(c<<5)+(k<<1)];
                if (f) buf[j+k*w+1] = font[(c<<5)+(k<<1)+1];
            }
            if (f) j += 2;
            else  j += 1;
        }
        w <<= 3;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, 0x10, 0, GL_COLOR_INDEX, GL_BITMAP, buf);
    }
    void Text::Draw(int x, int y) {
        glColor4d(0, 0, 0, 1);
        glBindTexture(GL_TEXTURE_2D, tex);
        glPushMatrix();
        glTranslated(x, y, 0);
        glScaled(w, 0x10, 1);
        glVertexPointer(2, GL_SHORT, 0, 0);
        glTexCoordPointer(2, GL_SHORT, 0, 0);
        glDrawArrays(GL_QUADS, 0, 4);
        glPopMatrix();
    }
}