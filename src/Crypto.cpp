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
    const uint8_t ShuffleKey[] = {
	    0xEC, 0x3F, 0x77, 0xA4, 0x45, 0xD0, 0x71, 0xBF, 0xB7, 0x98, 0x20, 0xFC, 0x4B, 0xE9, 0xB3, 0xE1,
	    0x5C, 0x22, 0xF7, 0x0C, 0x44, 0x1B, 0x81, 0xBD, 0x63, 0x8D, 0xD4, 0xC3, 0xF2, 0x10, 0x19, 0xE0,
	    0xFB, 0xA1, 0x6E, 0x66, 0xEA, 0xAE, 0xD6, 0xCE, 0x06, 0x18, 0x4E, 0xEB, 0x78, 0x95, 0xDB, 0xBA,
	    0xB6, 0x42, 0x7A, 0x2A, 0x83, 0x0B, 0x54, 0x67, 0x6D, 0xE8, 0x65, 0xE7, 0x2F, 0x07, 0xF3, 0xAA,
	    0x27, 0x7B, 0x85, 0xB0, 0x26, 0xFD, 0x8B, 0xA9, 0xFA, 0xBE, 0xA8, 0xD7, 0xCB, 0xCC, 0x92, 0xDA,
	    0xF9, 0x93, 0x60, 0x2D, 0xDD, 0xD2, 0xA2, 0x9B, 0x39, 0x5F, 0x82, 0x21, 0x4C, 0x69, 0xF8, 0x31,
	    0x87, 0xEE, 0x8E, 0xAD, 0x8C, 0x6A, 0xBC, 0xB5, 0x6B, 0x59, 0x13, 0xF1, 0x04, 0x00, 0xF6, 0x5A,
	    0x35, 0x79, 0x48, 0x8F, 0x15, 0xCD, 0x97, 0x57, 0x12, 0x3E, 0x37, 0xFF, 0x9D, 0x4F, 0x51, 0xF5,
	    0xA3, 0x70, 0xBB, 0x14, 0x75, 0xC2, 0xB8, 0x72, 0xC0, 0xED, 0x7D, 0x68, 0xC9, 0x2E, 0x0D, 0x62,
	    0x46, 0x17, 0x11, 0x4D, 0x6C, 0xC4, 0x7E, 0x53, 0xC1, 0x25, 0xC7, 0x9A, 0x1C, 0x88, 0x58, 0x2C,
	    0x89, 0xDC, 0x02, 0x64, 0x40, 0x01, 0x5D, 0x38, 0xA5, 0xE2, 0xAF, 0x55, 0xD5, 0xEF, 0x1A, 0x7C,
	    0xA7, 0x5B, 0xA6, 0x6F, 0x86, 0x9F, 0x73, 0xE6, 0x0A, 0xDE, 0x2B, 0x99, 0x4A, 0x47, 0x9C, 0xDF,
	    0x09, 0x76, 0x9E, 0x30, 0x0E, 0xE4, 0xB2, 0x94, 0xA0, 0x3B, 0x34, 0x1D, 0x28, 0x0F, 0x36, 0xE3,
	    0x23, 0xB4, 0x03, 0xD8, 0x90, 0xC8, 0x3C, 0xFE, 0x5E, 0x32, 0x24, 0x50, 0x1F, 0x3A, 0x43, 0x8A,
	    0x96, 0x41, 0x74, 0xAC, 0x52, 0x33, 0xF0, 0xD9, 0x29, 0x80, 0xB1, 0x16, 0xD3, 0xAB, 0x91, 0xB9,
	    0x84, 0x7F, 0x61, 0x1E, 0xCF, 0xC5, 0xD1, 0x56, 0x3D, 0xCA, 0xF4, 0x05, 0xC6, 0xE5, 0x08, 0x49
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
    void NextIV(char* oldiv) {
        uint8_t newIV[4] = {0xF2, 0x53, 0x50, 0xC6};
	    for (auto i = 0; i < 4; i++) {
		    uint8_t input = oldiv[i];
		    uint8_t tableInput = ShuffleKey[input];
		    newIV[0] += ShuffleKey[newIV[1]]-input;
		    newIV[1] -= newIV[2]^tableInput;
		    newIV[2] ^= ShuffleKey[newIV[3]]+input;
		    newIV[3] -= newIV[0]-tableInput;
		    uint32_t& merged = *(uint32_t*)newIV;
		    merged = merged<<3|merged>>0x1D;
	    }
	    for (auto i = 0; i < 4; i++) {
		    oldiv[i] = newIV[i];
	    }
    }
    void Transform(char* iv, char* data, size_t size) {
        uint8_t BigIV[16];
	    for (int i = 0; i < 16; i += 4) memcpy(BigIV+i, iv, 4);
	    AESGen.SetParameters(256, 128);
	    AESGen.StartEncryption(AESKey);
	    uint32_t pos = 0;
	    uint8_t first = 1;
	    int32_t tpos = 0;
	    while (size > pos) {
		    AESGen.SetParameters(256, 128);
		    AESGen.StartEncryption(AESKey);
		    tpos = 1460 - first * 4;
		    if (size > pos + tpos) AESGen.TransformOFB((uint8_t*)data + pos, BigIV, tpos);
		    else AESGen.TransformOFB((uint8_t*)data + pos, BigIV, size - pos);
		    pos += tpos;
		    if (first) first = 0;
	    }
    }
    void Encrypt(char* data, size_t size) {
        int32_t j;
	    uint8_t a, c;
	    for (uint8_t i = 0; i < 3; i++) {
		    a = 0;
		    for (j = size; j > 0; j--) {
			    c = data[size-j];
			    c = _rotl8(c, 3);
			    c += j;
			    c ^= a;
			    a = c;
			    c = _rotr8(c, j);
			    c = ~c;
			    c += 0x48;
			    data[size-j] = c;
		    }
		    a = 0;
		    for (j = size; j > 0; j--) {
			    c = data[j-1];
			    c = _rotl8(c, 4);
			    c += j;
			    c ^= a;
			    a = c;
			    c ^= 0x13;
			    c = _rotr8(c, 3);
			    data[j-1] = c;
		    }
	    }
	    Transform(NLS::Network::SendIV, data, size);
	    NextIV(Network::SendIV);
    }
    void Decrypt(char* data, size_t size) {
	    Transform(Network::RecvIV, data, size);
	    NextIV(Network::RecvIV);
	    int32_t j;
	    uint8_t a, b, c;
	    for (uint8_t i = 0; i < 3; i++) {
		    a = 0;
		    b = 0;
		    for (j = size; j > 0; j--) {
			    c = data[j-1];
			    c = _rotl8(c, 3);
			    c ^= 0x13;
			    a = c;
			    c ^= b;
			    c -= j;
			    c = _rotr8(c, 4);
			    b = a;
			    data[j-1] = c;
		    }
		    a = 0;
		    b = 0;
		    for (j = size; j > 0; j--) {
			    c = data[size-j];
			    c -= 0x48;
			    c = ~c;
			    c = _rotl8(c, j);
			    a = c;
			    c ^= b;
			    c -= j;
			    c = _rotr8(c, 3);
			    b = a;
			    data[size-j] = c;
		    }
	    }
    }
}