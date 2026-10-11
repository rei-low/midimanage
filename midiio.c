//
//  midiio.c
//  ALL_BUILD
//
//  Created by Braxton Clark on 2/17/26.
//

#include "ext.h"
#include "ext_obex.h"
#include <CoreMIDI/CoreMIDI.h>


typedef struct _midiio {
    t_object ob;
    void *midi_outlet;
    void *device_outlet;
    MIDIClientRef client;
    MIDIPortRef inPort;
    MIDIEndpointRef source;
    
    void *qelem;
    unsigned char midi_buffer[256];
    int midi_buffer_len;
} t_midiio;

void *midiio_new(t_symbol *s, long argc, t_atom *argv);
void midiio_free(t_midiio *x);
void midiio_assist(t_midiio *x, void *b, long m, long a, char *s);
void midiio_bang(t_midiio *x);
void midiio_qfn(t_midiio *x);
void midiio_getdevices(t_midiio *x);
void midiio_device(t_midiio *x, long index);

static t_class *midiio_class = NULL;

void ext_main(void *r)
{
    t_class *c = class_new("midiio",
                          (method)midiio_new,
                          (method)midiio_free,
                          (long)sizeof(t_midiio),
                          0L,
                          A_GIMME,
                          0);
    
    class_addmethod(c, (method)midiio_bang, "bang", 0);
    class_addmethod(c, (method)midiio_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)midiio_getdevices, "getdevices", 0);
    class_addmethod(c, (method)midiio_device, "device", A_LONG, 0);
    
    class_register(CLASS_BOX, c);
    midiio_class = c;
}

void midiio_read_proc(const MIDIPacketList *pktlist, void *readProcRefCon, void *srcConnRefCon)
{
    t_midiio *x = (t_midiio *)readProcRefCon;
    const MIDIPacket *packet = &pktlist->packet[0];
    
    for(unsigned int i = 0; i < pktlist->numPackets; i++){
        for(unsigned int j = 0; j < packet->length; j++){
            if (x->midi_buffer_len < 256){  //prevent overflow of char
                x->midi_buffer[x->midi_buffer_len++] = packet ->data[j];
            } // send each byte
        }
        packet = MIDIPacketNext(packet);
    }
    qelem_set(x->qelem);
}

void midiio_getdevices(t_midiio *x){
    
    outlet_anything(x->device_outlet, gensym("clear"), 0, NULL);
    
    ItemCount numSources = MIDIGetNumberOfSources();
    for (ItemCount i = 0; i < numSources; i++){
        MIDIEndpointRef source = MIDIGetSource(i);
        
        CFStringRef name = NULL;
        OSStatus status = MIDIObjectGetStringProperty(source, kMIDIPropertyName, &name);
        
        if (status == noErr && name != NULL){
            char deviceName [256];
            CFStringGetCString(name, deviceName, sizeof(deviceName), kCFStringEncodingUTF8);
            
            t_atom argv[2];
            atom_setlong(&argv[0], i);
            atom_setsym(&argv[1], gensym(deviceName));
            outlet_anything(x->device_outlet, gensym("append"), 2, argv);
            
            CFRelease(name);
        }
    }
    
}

void midiio_device(t_midiio *x, long index){
    ItemCount numSources = MIDIGetNumberOfSources();
    
    if (index < 0 || index >= numSources){
        object_error((t_object *)x, "Invalid device index: %ld", index);
        return;
    }
    
    
    if (x->inPort && x->source){
        MIDIPortDisconnectSource(x->inPort, x->source);
    }
    
    x->source = MIDIGetSource(index);
    OSStatus status = MIDIPortConnectSource(x->inPort, x->source, NULL);
    
    if(status == noErr){
        CFStringRef name = NULL;
        MIDIObjectGetStringProperty(x->source, kMIDIPropertyName, &name);
        if (name){
            char deviceName[256];
            CFStringGetCString(name, deviceName, sizeof(deviceName), kCFStringEncodingUTF8);
            post("amt.midiio: connected to %s", deviceName);
            CFRelease(name);
        }
    } else{
        object_error((t_object *)x, "Failed to connect to device %ld: %d", index, (int)status);
    }
    
}

void midiio_qfn(t_midiio *x)
{
    for (int i = 0; i < x->midi_buffer_len; i++) {
        outlet_int(x->midi_outlet, x->midi_buffer[i]);
    }
    x->midi_buffer_len = 0;
}
void *midiio_new(t_symbol *s, long argc, t_atom *argv)
{
    t_midiio *x = (t_midiio *)object_alloc(midiio_class);
    
    if (x) {
        x->device_outlet = outlet_new(x, NULL);
        x-> midi_outlet = outlet_new(x, NULL);
        x->source = 0;
        x->midi_buffer_len = 0;
        
        x->qelem = qelem_new(x, (method)midiio_qfn);
        
        OSStatus status = MIDIClientCreate(CFSTR("Max Midi IO Client"), NULL, NULL, &x->client);
        if (status != noErr) {
            object_error((t_object *)x, "Failed to create midi client: %d", (int)status);
            return x;
        }
        
        status = MIDIInputPortCreate(x->client, CFSTR("Input Port"), midiio_read_proc, x, &x->inPort);
        if (status != noErr){
            object_error((t_object *)x, "Failed to create input port: %d", (int)status);
            return x;
        }
        
        ItemCount numSources = MIDIGetNumberOfSources();
        if (numSources > 0){
            x->source = MIDIGetSource(0);
            status = MIDIPortConnectSource(x->inPort, x->source, NULL);
            
            if (status == noErr){
                CFStringRef name = NULL;
                MIDIObjectGetStringProperty(x->source, kMIDIPropertyName, &name);
                if (name){
                    char deviceName[256];
                    CFStringGetCString(name, deviceName, sizeof(deviceName), kCFStringEncodingUTF8);
                    post("midiio: connected to %s", deviceName);
                    CFRelease(name);
                }
            } else {
                object_error((t_object *)x, "Failed to connect to MIDI source: %d", (int)status);
            }
        } else {
            post("midiio: no MIDI sources available");
        }
    }
    return (x);
}
void midiio_free(t_midiio *x)
{
    if (x->qelem){
        qelem_free(x->qelem);
    }
    if (x->inPort) {
        if (x->source) {
            MIDIPortDisconnectSource(x->inPort, x->source);
        }
        MIDIPortDispose(x->inPort);  // ← ADD THIS
    }
    
    if (x->client) {
        MIDIClientDispose(x->client);
    }
}

void midiio_assist(t_midiio *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_INLET) {
        sprintf(s, "getdevices, device <index>");
    }
    else {
        if (a == 0){
            sprintf(s, "MIDI data out");}
        else{
            sprintf(s, "Device list out");
        }
    }
}

void midiio_bang(t_midiio *x)
{
    post("midiio: bang received");
}
