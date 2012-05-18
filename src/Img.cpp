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
    void Img::ExtendedProperty(Node n) {
        char* name = file.ReadTypeString();
        if (strcmp(name, "Property") == 0) {
            file.Skip(2);
            SubProperty(n);
        } else if (strcmp(name, "Canvas") == 0) {
            file.Skip(1);
            uint8_t b = file.Read<uint8_t>();
            if (b == 1) {
                file.Skip(2);
                SubProperty(n);
            }
            Sprite::Create(file, n, offset);
        } else if (strcmp(name, "Shape2D#Vector2D") == 0) {
            n.Reserve(2);
            n.g("x", 0).Set(file.ReadCInt());
            n.g("y", 1).Set(file.ReadCInt());
        } else if (strcmp(name, "Shape2D#Convex2D") == 0) {
            int32_t ec = file.ReadCInt();
            n.Reserve(ec);
            for (int i = 0; i < ec; ++i) ExtendedProperty(n.g(to_cstring(ec), i));
        } else if (strcmp(name, "Sound_DX8") == 0) {
            Sound::Create(file, n, offset);
        } else if (strcmp(name, "UOL") == 0) {
            file.Skip(1);
            n.SetUOL(file.ReadTypeString());
        } else die();
    }
    void Img::SubProperty(Node n) {
        int32_t count = file.ReadCInt();
        n.Reserve(count);
        for (int i = 0; i < count; ++i) {
            char* name = file.ReadTypeString();
            uint8_t a = file.Read<uint8_t>();
            switch (a) {
            case 0x00:
                n.g(name, i).Set(i);
                break;
            case 0x0B:
            case 0x02:
                n.g(name, i).Set(file.Read<uint16_t>());
                break;
            case 0x03:
                n.g(name, i).Set(file.ReadCInt());
                break;
            case 0x04:
                if (file.Read<uint8_t>() == 0x80) n.g(name, i).Set(file.Read<float>());
                else n.g(name, i).Set(0);
                break;
            case 0x05:
                n.g(name, i).Set(file.Read<double>());
                break;
            case 0x08:
                n.g(name, i).Set(file.ReadTypeString());
                break;
            case 0x09:
                {
                    uint32_t p = file.Read<uint32_t>();
                    p += file.Tell();
                    ExtendedProperty(n.g(name, i));
                    file.Seek(p);
                    break;
                }
            default: die();
            }
        }
    }
    void Img::Parse() {
        file.Map(offset, size);
        uint8_t a = file.Read<uint8_t>();
        if (a != 0x73) die();
        bool success = false;
        for (int i = 0; i < 3; ++i) {
            file.Seek(1);
            Key = Keys[i];
            AKey = AKeys[i];
            WKey = WKeys[i];
            string s = file.ReadEncString();
            if (s == "Property") {
                success = true;
                break;
            }
        }
        if (!success) die();
        uint16_t b = file.Read<uint16_t>();
        if (b != 0) die();
        SubProperty(n);
        n.Resolve();
        file.Unmap();
        delete this;
    }
}