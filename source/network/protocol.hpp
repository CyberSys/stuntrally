#pragma once
/**
 * This file describes the protocol that is used for network communication.
 * It contains structures and serializations that are transmitted.
 *
 * Implementation notes:
 *   - First member must be uint8_t packet_type
 *   - Serialization is basically a naïve memcpy, so use only primitive types
 */

#include <stdint.h>
#include "enet-wrapper.hpp"

namespace protocol {

const unsigned DEFAULT_PORT = 4243;

/**
 * This enum contains all possible message types.
 * It will be transmitted as 8-bit unsigned int.
 */
enum PacketType {
	HANDSHAKE = 0,
	PING,
	PONG,
	REQUEST_PEER_INFO,
	PEER_INFO,
	TEXT_MESSAGE,       // Text string that should be displayed somewhere
	STATE_UPDATE,
	GAME_LIST,          // Client requests master server to list games
	GAME_ACCEPTED,      // Master server sends response for newly accepted games
	GAME_STATUS         // An available game (either client updates, or server reports)
};


/**
 * Contains information about one game that is available for joining.
 */
struct GameInfo: public net::SimpleSerializer<GameInfo> {
	uint8_t packet_type;
	uint32_t id;        // Set by server
	uint32_t address;   // Set by server
	uint16_t port;      // Set by server
	uint32_t timestamp; // Set by server
	uint8_t players;    // Set by client
	char name[32];      // Set by client
	char track[32];     // Set by client

	bool operator==(const GameInfo& other) { return id == other.id; }
	bool operator!=(const GameInfo& other) { return !(*this == other); }
};

typedef std::map<uint32_t, protocol::GameInfo> GameList;


/**
 * Contains peer/player information.
 * These structs are passed around to create the complete network topography.
 */
struct PeerInfo: public net::SimpleSerializer<PeerInfo> {
	uint8_t packet_type;
	uint32_t address;
	uint16_t port;
	char name[16];

	bool operator==(const PeerInfo& other) { return address == other.address && port == other.port; }
	bool operator!=(const PeerInfo& other) { return !(*this == other); }
};


/**
 * Text message meant to be displayed for user, e.g. chat.
 */
struct TextMessage: public net::SimpleSerializer<TextMessage> {
	uint8_t packet_type;
	uint32_t length;
	uint8_t message[256]; //FIXME
};


/**
 * Contains the car state.
 */
struct CarStatePackage {
	float x, y, z; // FIXME: Should we use some vector types?
	float vx, vy, vz;
	float rotx, roty, rotz; // FIXME: What's the best way to transmit orientation?

	CarStatePackage(/* TODO: Parameters. */) {}
};

}
