#include "MatrixOS.h"
namespace MatrixOS::MIDI
{
    void Init(void)
    {
        // ClearAllHandler();
    }

    // void (* handler)(MidiPacket) = nullptr;

    // void SetHandler(void (*new_handler)(MidiPacket))
    // {
    //     handler = new_handler;
    // }

    // void ClearHandler(Status status)
    // {
    //     handlers[status] = nullptr;
    // }

    // void ClearAllHandler(void)
    // {
    //     for(uint16_t i = 0; i < HandlerCount; i++)
    //     {
    //        handlers[i] = nullptr;  
    //     }
    // }

    // void CallHandler(Status status, uint32_t value1, uint32_t value2, uint32_t value3)
    // {
    //     if(handlers[status])
    //     {
    //         switch(status)
    //         {
    //             case NoteOn:
    //             case NoteOff:
    //             case AfterTouch:
    //             case ControlChange:
    //                 ((void (*)(uint8_t, uint8_t, uint8_t))(handlers[status]))(value1, value2, value3);
    //                 break;
    //             case ProgramChange:
    //             case ChannelPressure:
    //                 ((void (*)(uint8_t, uint8_t))(handlers[status]))(value1, value2);
    //                 break;
    //             case PitchChange:
    //                 ((void (*)(uint8_t, uint16_t))(handlers[status]))(value1, value2);
    //                 break;
    //             case SongSelect:
    //                 ((void (*)(uint8_t))(handlers[status]))(value1);
    //                 break;
    //             case SongPosition:
    //                 ((void (*)(uint16_t))(handlers[status]))(value1);
    //                 break;
    //             case TuneRequest:
    //             case Sync:
    //             case Start:
    //             case Continue:
    //             case Stop:
    //             case ActiveSense:
    //             case Reset:
    //                 ((void (*)())(handlers[status]))();
    //                 break;
    //             case SysexData:
    //             case SysexEnd:
    //                 //TODO: Need to determain if sysex is system level or application level
    //                 break;
    //         }
    //     }
    // }

    uint32_t Available()
    {
        // MatrixOS::USB::CDC::Println("Midi Available");
        uint32_t packets = 0;
        packets += tud_midi_available();

        #ifdef DEVICE_MIDI
        packets += Device::MIDI::Available();
        #endif

        return packets;
    }

    MidiPacket Get()
    {
        if(tud_midi_available())
        {
            return GetUSB();
        }

        #ifdef DEVICE_MIDI
        else if(Device::MIDI::Available())
        {
            return Device::MIDI::Get();
        }
        #endif
        
        return MidiPacket(0, None);
    }

    MidiPacket GetUSB()
    {
        uint8_t packet[4];
        if(tud_midi_packet_read(packet))
        {
            return DispatchUSBPacket(packet);
        }
        return MidiPacket(1, None);
    }



    MidiPacket DispatchUSBPacket(uint8_t rawPacket[4])
    {
        uint8_t packet[4];
        uint8_t port = 1;
        memcpy(packet, rawPacket, 4);
        // MatrixOS::Logging::LogDebug("USB MIDI Packet", "%#02X %#02X %#02X %#02X", packet[0], packet[1], packet[2], packet[3]);
        switch (packet[0]) 
        {
            // case CIN_SYSEX:
            //     CallHandler(SysexData, packet[1]);
            //     CallHandler(SysexData, packet[2]);
            //     CallHandler(SysexData, packet[3]);
            // break;
            // case CIN_SYSEX_ENDS_IN_1:
            //     CallHandler(SysexData, packet[1]);
            //     CallHandler(SysexEnd);
            //     break;
            // case CIN_SYSEX_ENDS_IN_2:
            //     CallHandler(SysexData, packet[1]);
            //     CallHandler(SysexData, packet[2]);
            //     break;
            // case CIN_SYSEX_ENDS_IN_3:
            //     CallHandler(SysexData, packet[1]);
            //     CallHandler(SysexData, packet[2]);
            //     CallHandler(SysexData, packet[3]);
            //     CallHandler(SysexEnd);
            //     break;
            case CIN_3BYTE_SYS_COMMON:
                if (packet[1] == MIDIv1_SONG_POSITION_PTR) 
                    return MidiPacket(port, SongPosition, 2, &packet[2]);
                break;

            case CIN_2BYTE_SYS_COMMON:
                switch (packet[1]) {
                    case MIDIv1_SONG_SELECT:
                        return MidiPacket(port, SongSelect, 1, &packet[2]);
                    case MIDIv1_MTC_QUARTER_FRAME:
                        // reference library doesnt handle quarter frame.
                        break;
                }
                break;
            case CIN_NOTE_OFF:
                // packet[1] = MIDIv1_VOICE_CHANNEL(packet[1]); //Strip status value, leaving only channel
                return MidiPacket(port, NoteOff, 3, &packet[1]);
            case CIN_NOTE_ON:
                // packet[1] = MIDIv1_VOICE_CHANNEL(packet[1]);
                return MidiPacket(port, NoteOn, 3, &packet[1]);
            case CIN_AFTER_TOUCH:
                // packet[1] = MIDIv1_VOICE_CHANNEL(packet[1]);
                return MidiPacket(port, AfterTouch, 3, &packet[1]);
            case CIN_CONTROL_CHANGE:
                // packet[1] = MIDIv1_VOICE_CHANNEL(packet[1]);
                return MidiPacket(port, ControlChange, 3, &packet[1]);
            case CIN_PROGRAM_CHANGE:
                // packet[1] = MIDIv1_VOICE_CHANNEL(packet[1]);
                return MidiPacket(port, ProgramChange, 2, &packet[1]);
            case CIN_CHANNEL_PRESSURE:
                // packet[1] = MIDIv1_VOICE_CHANNEL(packet[1]);
                return MidiPacket(port, ChannelPressure, 2, &packet[1]);
            case CIN_PITCH_WHEEL:
                // packet[1] = MIDIv1_VOICE_CHANNEL(packet[1]);
                return MidiPacket(port, PitchChange, 3, &packet[1]);
            case CIN_1BYTE:
                switch (packet[1]) 
                {
                    case MIDIv1_CLOCK:
                        return MidiPacket(port, Sync);
                    case MIDIv1_TICK:
                        return MidiPacket(port, Tick);
                    case MIDIv1_START:
                        return MidiPacket(port, Start);
                    case MIDIv1_CONTINUE:
                        return MidiPacket(port, Continue);
                    case MIDIv1_STOP:
                        return MidiPacket(port, Stop);
                    case MIDIv1_ACTIVE_SENSE:
                        return MidiPacket(port, ActiveSense);
                    case MIDIv1_RESET:
                        return MidiPacket(port, Reset);
                    case MIDIv1_TUNE_REQUEST:
                        return MidiPacket(port, TuneRequest);
                }
                break;
        }
        return MidiPacket(port, None);
    }

    void SendPacketUSB(MidiPacket midiPacket)
    {
        if(midiPacket.port == 0 || midiPacket.port == 1)
        {
            tud_midi_stream_write(0, midiPacket.data, midiPacket.Length());
            // MatrixOS::USB::CDC::Println("Sent Midi Packet");
        }
    }

    void SendPacket(MidiPacket midiPacket)
    {
        SendPacketUSB(midiPacket);

        #ifdef DEVICE_MIDI
        Device::MIDI::Send(midiPacket);
        #endif
    }

    void SendNoteOff(uint8_t channel, uint8_t note, uint8_t velocity)
    {
        uint8_t packet[3] = { (uint8_t)(MIDIv1_NOTE_OFF | (channel & 0x0f)), note, velocity};
        tud_midi_stream_write(0, packet, 3);

    }

    void SendNoteOn(uint8_t channel, uint8_t note, uint8_t velocity)
    {
        uint8_t packet[3] = { (uint8_t)(MIDIv1_NOTE_ON | (channel & 0x0f)), note, velocity};
        tud_midi_stream_write(0, packet, 3);
    }

    void SendAfterTouch(uint8_t channel, uint8_t note, uint8_t velocity)
    {   
        uint8_t packet[3] = { (uint8_t)(MIDIv1_AFTER_TOUCH | (channel & 0x0f)), note, velocity};
        tud_midi_stream_write(0, packet, 3);
    }

    void SendControlChange(uint8_t channel, uint8_t controller, uint8_t value)
    {
        uint8_t packet[3] = { (uint8_t)(MIDIv1_CONTROL_CHANGE | (channel & 0x0f)), controller, value};
        tud_midi_stream_write(0, packet, 3);
    }

    void SendProgramChange(uint8_t channel, uint8_t program)
    {
        uint8_t packet[2] = { (uint8_t)(MIDIv1_PROGRAM_CHANGE | (channel & 0x0f)), program};
        tud_midi_stream_write(0, packet, 2);
    }

    void SendChannelPressure(uint8_t channel, uint8_t velocity)
    {
        uint8_t packet[2] = { (uint8_t)(MIDIv1_CHANNEL_PRESSURE | (channel & 0x0f)), velocity};
        tud_midi_stream_write(0, packet, 2);
    }

    void SendPitchChange(uint8_t channel, uint16_t pitch)
    {
        uint8_t packet[3] = { (uint8_t)(MIDIv1_PITCH_WHEEL | (channel & 0x0f)), (uint8_t)(pitch & 0x07F), (uint8_t)((pitch>>7) & 0x7f)};
        tud_midi_stream_write(0, packet, 3);
    }

    void SendSongPosition(uint16_t position)
    {
        uint8_t packet[3] = { MIDIv1_SONG_POSITION_PTR, (uint8_t)(position & 0x07F), (uint8_t)((position>>7) & 0x7f)};
        tud_midi_stream_write(0, packet, 3);
    }

    void SendSongSelect(uint8_t song)
    {
        uint8_t packet[2] = { MIDIv1_SONG_SELECT, song};
        tud_midi_stream_write(0, packet, 2);
    }

    void SendTuneRequest(void)
    {
        uint8_t packet[1] = { MIDIv1_TUNE_REQUEST};
        tud_midi_stream_write(0, packet, 1);
    }

    void SendSync(void)
    {
        uint8_t packet[1] = { MIDIv1_CLOCK };
        tud_midi_stream_write(0, packet, 1);
    }

    void SendStart(void)
    {
        uint8_t packet[1] = { MIDIv1_START};
        tud_midi_stream_write(0, packet, 1);
    }

    void SendContinue(void)
    {
        uint8_t packet[1] = { MIDIv1_CONTINUE};
        tud_midi_stream_write(0, packet, 1);
    }

    void SendStop(void)
    {
        uint8_t packet[1] = { MIDIv1_STOP};
        tud_midi_stream_write(0, packet, 1);
    }

    void SendActiveSense(void)
    {
        uint8_t packet[1] = { MIDIv1_ACTIVE_SENSE};
        tud_midi_stream_write(0, packet, 1);
    }

    void SendReset(void)
    {
        uint8_t packet[1] = { MIDIv1_RESET};
        tud_midi_stream_write(0, packet, 1);
    }
}