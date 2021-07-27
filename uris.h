#ifndef dracorex_URIS_H
#define dracorex_URIS_H

#include "lv2/lv2plug.in/ns/ext/state/state.h"
#include "lv2/lv2plug.in/ns/ext/log/log.h"

#define LV2_MIDI__MidiEvent "http://lv2plug.in/ns/ext/midi#MidiEvent"
#define dracorex_URI "http://nickbailey.co.nr/plugins/dracorex"

typedef struct {
	LV2_URID midi_Event;
} dracorexURIs;

static inline void
map_dracorex_uris(LV2_URID_Map* map, dracorexURIs* uris)
{
	uris->midi_Event = map->map(map->handle, LV2_MIDI__MidiEvent);
}




#endif  /* dracorex_URIS_H */

