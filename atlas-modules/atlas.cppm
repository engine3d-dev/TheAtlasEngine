export module atlas;

export import core;

export import atlas.graphics_api;
export import atlas.logger;

export import atlas.drivers;

// atlas.subsystem_type_name - this is for anything that is globally needed throughout TheAtlasEngine,like loggers, utilities, etc.
// import :internal_stuff - is used for implementation-specific details such as (implementing API-agnostic implementation specific to subsystems)
// Example: Like API-agnostic renderer and API-agnostic physics system.

export namespace atlas {
};