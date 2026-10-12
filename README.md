# midiio

A Max external for macOS that receives MIDI from any connected device, one device per object.

In Max for Live, a device only receives the MIDI that Live routes to its track. `midiio` connects straight to a MIDI source through CoreMIDI, so a device can listen to any controller or port on the system. Use several `midiio` objects to listen to several devices at once.

Universal binary (Apple Silicon and Intel), signed and notarized.

## Install

1. Download `midiio.zip` from [Releases](https://github.com/rei-low/midimanage/releases) and unzip it.
2. Put `midiio.mxo` and `midiio.maxhelp` anywhere in Max's search path, for example `~/Documents/Max 8/Library` or `~/Documents/Max 9/Library`.
3. Restart Max, create a `[midiio]` object, and right-click it → **Open midiio Help**.

## Usage

1. Send `getdevices`. The right outlet sends `clear`, then `append <index> <name>` for each MIDI source, which fills a `[umenu]` directly.
2. Choose a source with `device <index>`. From a `[umenu]`, route its left outlet through `[prepend device]`.
3. The left outlet sends raw MIDI bytes one at a time. Connect it to `[midiparse]` to get notes, CCs and other messages.

On creation, `midiio` connects to the first available source.

| Message | What it does |
| --- | --- |
| `getdevices` | Lists the available MIDI sources out the right outlet |
| `device <index>` | Listens to the source at that index |

| Outlet | Output |
| --- | --- |
| Left | Raw MIDI bytes (ints 0–255) |
| Right | `clear` / `append <index> <name>` for `[umenu]` |

Device indices change when devices are connected or disconnected, so send `getdevices` again after plugging something in.

### Testing without hardware

The help patch includes a note generator that sends through `[noteout]`. To hear it loop back into `midiio`, enable the IAC Driver (Audio MIDI Setup → Window → Show MIDI Studio → IAC Driver → **Device is online**) and select the IAC bus in both `[noteout]` and the `midiio` device menu.

## Build from source

Requires the [Max SDK](https://github.com/Cycling74/max-sdk) and CMake. `CMakeLists.txt` expects `max-sdk-base` two folders up, so clone this repo into a subfolder of `max-sdk/source`, for example `max-sdk/source/projects/midimanage`. Then:

```bash
cmake -S . -B build -G Xcode -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" -DC74_LIBRARY_OUTPUT_DIRECTORY="$PWD"
cmake --build build --config Release
```

`midiio.mxo` is written to the repo folder.

## Limitations

- macOS only. `midiio` is built on CoreMIDI.
- Input only. Use Max's `[noteout]` / `[midiout]` for sending.

## Author

Braxton Clark (Rei Low) · [Atlanta Music Technology](https://braxtonclark.squarespace.com)
