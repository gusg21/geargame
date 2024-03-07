#if !defined(GG_ACTOR_SPEC_H)
#define GG_ACTOR_SPEC_H

#include "vector.h"

#define ACTOR_SPEC_MAX_SCRIPT_NAME_LEN 64
// NOTE: Re-defined from actor.h
#define ACTOR_MAX_NAME_LEN 32

// Everything you need to construct a new actor
typedef struct gg_actor_spec {
    char name[ACTOR_MAX_NAME_LEN];
    gg_v2_t initial_pos;
    char script_asset_name[ACTOR_SPEC_MAX_SCRIPT_NAME_LEN];
} gg_actor_spec_t;

void ActorSpec_LoadFromJSON(gg_actor_spec_t* spec, const char* path);
void ActorSpec_Destroy(gg_actor_spec_t* spec);

#endif  // GG_ACTOR_SPEC_H
