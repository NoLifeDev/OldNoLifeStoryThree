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

namespace NLS {
    extern uint8_t *Key;
    extern uint8_t *AKey;
    extern uint16_t *WKey;
    extern int16_t EncVersion;
    extern uint32_t VersionHash;
    extern uint8_t Keys[3][0x10000];
    extern uint8_t AKeys[3][0x10000];
    extern uint16_t WKeys[3][0x8000];
    const uint32_t OffsetKey = 0x581C3F6D;
    void GenKeys();
    void Encrypt(char*, size_t);
    void Decrypt(char*, size_t);
}