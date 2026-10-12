Often, in the process of making M4L devices, devs have trouble receiving MIDI from multiple channels in Ableton.
Live only routes one MIDI channel as a listener, so now this tool routes multiple MIDI channels per object to the Max Device. 
It's designed to be used with the native umenu object, so a getdevices message will send the list of devices from the third outlet to umenu.
Select your device to listen to. 
Send that to a [prepend device] object or message with 'device $1'
Now a MIDI instrument can be selected per object for use. 
_Note_ They're MIDI bytes, so you'll need a MIDI parse object.
