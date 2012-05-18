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
    class Sound::Data {
    public:
        int32_t len;
        void* data;
        uint32_t offset;
        MapFile file;
        HSTREAM stream;
    };
    void Sound::Init() {
        BASS_Init(-1, 44100, 0, Graphics::Window->getSystemHandle(), 0);
    }
    void Sound::Create(MapFile& file, Node n, uint32_t off) {
        Data* data = new Data();
        file.Skip(1);
        data->len = file.ReadCInt();
        file.ReadCInt();
        data->offset = file.Tell()+off+82;
        data->data = nullptr;
        data->stream = 0;
        data->file = file;
        Sound s;
        s.data = data;
        n.Set(s);
    }
    Sound Sound::Blank() {
        Sound s;
        s.data = nullptr;
        return s;
    }
    void Sound::Load() {
        data->file.Map(data->offset, data->len);
        void* d = data->file.ReadBin(data->len);
        data->data = malloc(data->len);
        memcpy(data->data, d, data->len);
    }
    void Sound::Play() {
        if (!data) return;
        if (!data->data) Load();
        if (!data->stream) data->stream = BASS_StreamCreateFile(true, data->data, 0, data->len, BASS_SAMPLE_FLOAT);
        else BASS_ChannelFlags(data->stream, 0, BASS_SAMPLE_LOOP);
        BASS_ChannelPlay(data->stream, true);
    }
    void Sound::Loop() {
        if (!data) return;
        if (!data->data) Load();
        if (!data->stream) data->stream = BASS_StreamCreateFile(true, data->data, 0, data->len, BASS_SAMPLE_FLOAT|BASS_SAMPLE_LOOP);
        else BASS_ChannelFlags(data->stream, BASS_SAMPLE_LOOP, BASS_SAMPLE_LOOP);
        BASS_ChannelPlay(data->stream, false);
    }
    void Sound::Stop() {
        if (!data) return;
        BASS_ChannelStop(data->stream);
    }
    
    bool Sound::operator==(Sound other) {
        return data == other.data;
    }
}