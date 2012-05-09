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
    uint8_t *Key = 0;
    uint8_t *AKey = 0;
    uint16_t *WKey = 0;
    int16_t EncVersion;
    uint16_t Version = 0;
    uint32_t VersionHash;
    uint8_t Keys[3][0x10000];
    uint16_t WKeys[3][0x8000];
    uint8_t AKeys[3][0x10000];
    AES AESGen;
    const uint8_t GMSKeyIV[4] = {
        0x4D, 0x23, 0xC7, 0x2B
    };
    const uint8_t KMSKeyIV[4] = {
        0xB9, 0x7D, 0x63, 0xE9
    };
    const uint8_t AESKey[] = {
        0x13, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,	0x06, 0x00, 0x00, 0x00, 0xB4, 0x00, 0x00, 0x00,
        0x1B, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00,	0x33, 0x00, 0x00, 0x00, 0x52, 0x00, 0x00, 0x00
    };
    void GenKey(const uint8_t* IV, uint8_t* key) {
        uint8_t big[16];
        for (int i = 0; i < 16; i += 4) {
            memcpy(big+i, IV, 4);
        }
        AESGen.SetParameters(256, 128);
        AESGen.StartEncryption(AESKey);
        AESGen.EncryptBlock(big, key);
        for (int i = 16; i < 0x10000; i += 16) {
            AESGen.EncryptBlock(key+i-16, key+i);
        }
    }
    void GenKeys() {
        memset(Keys[0], 0, 0x10000);
        GenKey(GMSKeyIV, Keys[1]);
        GenKey(KMSKeyIV, Keys[2]);
        uint8_t amask = 0xAA;
        for (int i = 0; i < 0x10000; ++i, ++amask) {
            AKeys[0][i] = amask^Keys[0][i];
            AKeys[1][i] = amask^Keys[1][i];
            AKeys[2][i] = amask^Keys[2][i];
        }
        uint16_t wmask = 0xAAAA;
        for (int i = 0; i < 0x8000; ++i, ++wmask) {
            WKeys[0][i] = wmask^*(uint16_t*)(Keys[0]+2*i);
            WKeys[1][i] = wmask^*(uint16_t*)(Keys[1]+2*i);
            WKeys[2][i] = wmask^*(uint16_t*)(Keys[2]+2*i);
        }
        Key = Keys[0];
        AKey = AKeys[0];
        WKey = WKeys[0];
    }
}