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
    vector<Sprite::Data*> Loaded;
    class Sprite::Data {
    public:
        void Parse() {
            static uint8_t Buf1[0x1000000];
            static uint8_t Buf2[0x1000000];
            uint8_t* Src = Buf1;
            uint8_t* Dest = Buf2;
            auto DecompressBlock = [&](uint32_t len) {
                static z_stream strm;
                strm.next_in = Src;
                strm.avail_in = len;
                strm.opaque = nullptr;
                strm.zfree = nullptr;
                strm.zalloc = nullptr;
                inflateInit(&strm);
                strm.next_out = Dest;
                strm.avail_out = 0x1000000;
                int err = inflate(&strm, Z_FINISH);
                if (err != Z_BUF_ERROR) die();
                inflateEnd(&strm);
                swap(Src, Dest);
            };
            auto Decompress = [&](uint32_t len) {
                if (Src[0] == 0x78 && Src[1] == 0x9C) DecompressBlock(len);
                else {
                    uint32_t p, i;
                    for (p = 0, i = 0; i < len-1;) {
                        uint32_t blen = *(uint32_t*)&Src[i];
                        i += 4;
                        if (i+blen > len) die();
                        for (uint32_t j = 0; j < blen; j++) Dest[p+j] = Src[i+j]^Key[j];
                        i += blen;
                        p += blen;
                    }
                    swap(Src, Dest);
                    DecompressBlock(p);
                }
            };
            file.Map(offset, length);
            void* orig = file.ReadBin(length);
            memcpy(Src, orig, length);
            Decompress(length);
            int32_t f = format+format2;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            switch (f) {
            case 1:
                glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_BGRA, GL_UNSIGNED_SHORT_4_4_4_4_REV, Src);
                break;
            case 2:
                glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, Src);
                break;
            case 513:
                glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_BGRA, GL_UNSIGNED_SHORT_5_6_5_REV, Src);
                break;
            case 517:
                glTexImage2D(GL_TEXTURE_2D, 0, 4, width>>4, height>>4, 0, GL_BGRA, GL_UNSIGNED_SHORT_5_6_5_REV, Src);
                break;
            default:
                die();
            }
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            file.Unmap();
            Loaded.push_back(this);
        }
		GLuint texture;
		int32_t width;
		int32_t height;
		int32_t originx;
		int32_t originy;
        int32_t format;
        int32_t length;
        uint32_t offset;
        MapFile file;
        uint8_t format2;
    };
    Sprite Sprite::Blank() {
        Sprite s;
        s.data = nullptr;
        return s;
    }
    void Sprite::Unload() {
        for (Data* d : Loaded) {
            glDeleteTextures(1, &d->texture);
            d->texture = 0;
        }
        Loaded.clear();
    }
    void Sprite::Create(MapFile& file, Node n, uint32_t off) {
        Data* data = new Data();
        data->texture = 0;
        data->width = file.ReadCInt();
        data->height = file.ReadCInt();
        data->originx = n["origin"]["x"];
        data->originy = n["origin"]["y"];
        data->format = file.ReadCInt();
        data->format2 = file.Read<uint8_t>();
        file.Skip(4);
        data->length = file.Read<int32_t>();
        if (data->length <= 0) die();
        data->offset = file.Tell()+off+1;
        data->file = file;
        Sprite s;
        s.data = data;
        n.Set(s);
    }
    GLuint vbo;
    hash<Sprite::Data*> h;
    void Sprite::Draw(int x, int y, bool flipped, double alpha, double ang, int horz, int vert) {
        if (!data) return;
        if (horz < 0 && vert < 0)
        if (x+data->width-data->originx < View::X || x-data->originx > View::X+View::Width ||
            y+data->height-data->originy < View::Y || y-data->originy > View::Y+View::Height) return;
        if (!data->texture) data->Parse();
        glBindTexture(GL_TEXTURE_2D, data->texture);
        glVertexPointer(2, GL_SHORT, 0, 0);
        glTexCoordPointer(2, GL_SHORT, 0, 0);
        glColor4d(1, 1, 1, alpha);
        auto Single = [&](double x, double y) {
            glPushMatrix();
            glTranslated(x, y, 0);
            glRotated(ang, 0, 0, 1);
            glTranslated(-data->originx, -data->originy, 0);
            glScaled(data->width, data->height, 1);
            if (flipped) glScaled(-1, 1, 1);
            glDrawArrays(GL_QUADS, 0, 4);
            glPopMatrix();
        };
        if (!horz) horz = data->width;
        if (!vert) vert = data->height;
        if (horz < 0 && vert < 0) {
            Single(x, y);
        } else if (horz < 0) {
            for (int iy = (y+data->originy-View::Y)%vert-data->originy+View::Y-vert; iy-data->originy<View::Y+View::Height; iy += vert) {
                Single(x, iy);
            }
        } else if (vert < 0) {
            for (int ix = (x+data->originx-View::X)%horz-data->originx+View::X-horz; ix-data->originx<View::X+View::Width; ix += horz) {
                Single(ix, y);
            }
        } else {
            for (int iy = (y+data->originy-View::Y)%vert-data->originy+View::Y-vert; iy-data->originy<View::Y+View::Height; iy += vert) {
                for (int ix = (x+data->originx-View::X)%horz-data->originx+View::X-horz; ix-data->originx<View::X+View::Width; ix += horz) {
                    Single(ix, iy);
                }
            }
        }
    }
    void Sprite::Init() {
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        GLushort data[] = {0, 0, 1, 0, 1, 1, 0, 1};
        glBufferData(GL_ARRAY_BUFFER, 16, data, GL_STATIC_DRAW);
        glVertexPointer(2, GL_SHORT, 0, 0);
        glTexCoordPointer(2, GL_SHORT, 0, 0);
    }
}