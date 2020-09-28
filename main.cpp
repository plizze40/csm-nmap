#include <fstream>
#include <iostream>
#include <filesystem>
#include "bytebuffer.h"
#include "base64.h"
#include <nlohmann/json.hpp>

struct MapHeader 
{
	int32_t		num_bloq;
	int32_t		num_layers[3];
	int32_t		num_triggers;
	int32_t		num_lights;
	int32_t		num_particles;
	int32_t		num_npcs;
	int32_t		num_objs;
	int32_t		num_te;
};

struct Pos
{
	int x;
	int y;
};

struct PosAndValue
{
	int x;
	int y;
	int value;
};

struct PosAndTwoValue
{
	int x;
	int y;
	int value;
	int othervalue;
};

struct PosAndThreeValue
{
	int x;
	int y;
	int value;
	int othervalue;
	int secondvalue;
};

struct MapDat
{
	std::string		map_name;
	int				battle_mode;
	int				backup_mode;
	std::string		restrict_mode;
	std::string		music_number;
	std::string		zone;
	std::string		terrain;
	std::string		ambient;
	int				base_light;
	int				letter_grh;
	int				extra1;
	int				extra2;
	std::string		extra3;

};

static inline std::string rtrim(std::string s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
		}).base(), s.end());
	return s;
}


void load_file(ByteBuffer& buffer, const std::string& filename)
{
	std::ifstream rFile(filename, std::ios::in | std::ios::binary);
	if (!rFile.good())
		exit(1);

	rFile.seekg(0, std::ios::end);
	auto size = rFile.tellg();
	rFile.seekg(0, std::ios::beg);

	std::vector<char> d(size);
	rFile.read(&d[0], size);

	buffer.write(d.data(), size);
}

void load_map(int map) 
{
	std::string filename = "data/resources/maps/mapa" + std::to_string(map) + ".csm";
	ByteBuffer mapBuffer;
	ByteBuffer output;

	load_file(mapBuffer, filename);

	//header
	MapHeader mapheader;
	mapBuffer.read(&mapheader, sizeof(mapheader));

	//dim
	mapBuffer.read_double();

	//mapdata
	MapDat mapdat;
	mapdat.map_name			= rtrim(mapBuffer.read_string_fixed(64));
	mapdat.battle_mode		= mapBuffer.read_uint8();
	mapdat.backup_mode		= mapBuffer.read_uint8();
	mapdat.restrict_mode	= rtrim(mapBuffer.read_string_fixed(4));
	mapdat.music_number		= rtrim(mapBuffer.read_string_fixed(16));
	mapdat.zone				= rtrim(mapBuffer.read_string_fixed(16));
	mapdat.terrain			= rtrim(mapBuffer.read_string_fixed(16));
	mapdat.ambient			= rtrim(mapBuffer.read_string_fixed(16));
	mapdat.base_light		= mapBuffer.read_int32();
	mapdat.letter_grh		= mapBuffer.read_int32();
	mapdat.extra1			= mapBuffer.read_int32();
	mapdat.extra2			= mapBuffer.read_int32();
	mapdat.extra3			= rtrim(mapBuffer.read_string_fixed(32));
   
	std::vector<int32_t> layer1(100 * 100);
	mapBuffer.read(&layer1[0], layer1.size());

	std::vector<Pos> blocks;
	for (int i = 0; i < mapheader.num_bloq; i++) {
		Pos blocked_tile;
		blocked_tile.x = mapBuffer.read_int16();
		blocked_tile.y = mapBuffer.read_int16();

		blocks.push_back(blocked_tile);
	}

	std::vector<PosAndValue> layer2;
	for (int i = 0; i < mapheader.num_layers[0]; i++) {
		PosAndValue tile;
		tile.x = mapBuffer.read_int16();
		tile.y = mapBuffer.read_int16();
		tile.value = mapBuffer.read_int32();

		layer2.push_back(tile);
	}

	std::vector<PosAndValue> layer3;
	for (int i = 0; i < mapheader.num_layers[1]; i++) {
		PosAndValue tile;
		tile.x = mapBuffer.read_int16();
		tile.y = mapBuffer.read_int16();
		tile.value = mapBuffer.read_int32();

		layer3.push_back(tile);
	}

	std::vector<PosAndValue> layer4;
	for (int i = 0; i < mapheader.num_layers[2]; i++) {
		PosAndValue tile;
		tile.x = mapBuffer.read_int16();
		tile.y = mapBuffer.read_int16();
		tile.value = mapBuffer.read_int32();

		layer4.push_back(tile);
	}

	std::vector<PosAndValue> triggers;
	for (int i = 0; i < mapheader.num_triggers; i++) {
		PosAndValue trigger;
		trigger.x = mapBuffer.read_int16();
		trigger.y = mapBuffer.read_int16();
		trigger.value = mapBuffer.read_int16();

		triggers.push_back(trigger);
	}

	std::vector<PosAndValue> particles;
	for (int i = 0; i < mapheader.num_particles; i++) {
		PosAndValue particle; 
		particle.x = mapBuffer.read_int16();
		particle.y = mapBuffer.read_int16();
		particle.value = mapBuffer.read_int32();

		particles.push_back(particle);
	}

	std::vector<PosAndTwoValue> lights;
	for (int i = 0; i < mapheader.num_lights; i++) {
		PosAndTwoValue light;
		light.x = mapBuffer.read_int16();
		light.y = mapBuffer.read_int16();
		light.value = mapBuffer.read_int32();
		light.othervalue = mapBuffer.read_uint8();

		lights.push_back(light);
	}

	std::vector<PosAndTwoValue> objects;
	for (int i = 0; i < mapheader.num_objs; i++) {
		PosAndTwoValue obj; 
		obj.x = mapBuffer.read_int16();
		obj.y = mapBuffer.read_int16();
		obj.value = mapBuffer.read_int16();
		obj.othervalue = mapBuffer.read_int16();

		objects.push_back(obj);
	}

	std::vector<PosAndValue> npcs;
	for (int i = 0; i < mapheader.num_npcs; i++) {
		PosAndValue npc;
		npc.x = mapBuffer.read_int16();
		npc.y = mapBuffer.read_int16();
		npc.value = mapBuffer.read_int16();

		npcs.push_back(npc);
	}

	std::vector<PosAndThreeValue> tiles_te;
	for (int i = 0; i < mapheader.num_te; i++) {
		PosAndThreeValue te;
		te.x = mapBuffer.read_int16();
		te.y = mapBuffer.read_int16();
		te.value = mapBuffer.read_int16();
		te.othervalue = mapBuffer.read_int16();
		te.secondvalue = mapBuffer.read_int16();

		tiles_te.push_back(te);
	} 
	
	//mapheader
	output.write_uint8(std::numeric_limits<uint8_t>::max());
	output.write_string(mapdat.map_name);
	output.write_string(mapdat.restrict_mode);
	output.write_string(mapdat.music_number);
	output.write_string(mapdat.zone);
	output.write_string(mapdat.terrain);
	output.write_string(mapdat.ambient);
	output.write_int32(mapdat.battle_mode);
	output.write_int32(mapdat.backup_mode);
	output.write_int32(mapdat.base_light);
	output.write_int32(mapdat.letter_grh);

	output.write(layer1.data(), layer1.size() * sizeof(std::int32_t));

	//block
	output.write_uint32(blocks.size());
	for (const auto& b : blocks) {
		output.write_uint8(b.x);
		output.write_uint8(b.y);
	}

	//layer2
	output.write_uint32(layer2.size());
	for (const auto& b : layer2) {
		output.write_uint8(b.x);
		output.write_uint8(b.y);
		output.write_uint32(b.value);
	}

	//layer3
	output.write_uint32(layer3.size());
	for (const auto& b : layer3) {
		output.write_uint8(b.x);
		output.write_uint8(b.y);
		output.write_uint32(b.value);
	}

	//layer4
	output.write_uint32(layer4.size());
	for (const auto& b : layer4) {
		output.write_uint8(b.x);
		output.write_uint8(b.y);
		output.write_uint32(b.value);
	}

	//triggers
	output.write_uint32(triggers.size());
	for (const auto& b : triggers) {
		output.write_uint8(b.x);
		output.write_uint8(b.y);
		output.write_uint16(b.value);
	}

	//particles
	output.write_uint32(particles.size());
	for (const auto& b : particles) {
		output.write_uint8(b.x);
		output.write_uint8(b.y);
		output.write_uint32(b.value);
	}

	//lights
	output.write_uint32(lights.size());
	for (const auto& b : lights) {
		output.write_uint8(b.x);
		output.write_uint8(b.y);
		output.write_uint32(b.value);
		output.write_uint8(b.othervalue);
	}

	//objects
	output.write_uint32(objects.size());
	for (const auto& b : objects) {
		output.write_uint8(b.x);
		output.write_uint8(b.y);
		output.write_uint16(b.value);
		output.write_uint16(b.othervalue);
	}

	//npcs
	output.write_uint32(npcs.size());
	for (const auto& b : npcs) {
		output.write_uint8(b.x);
		output.write_uint8(b.y);
		output.write_uint16(b.value); 
	}

	//tiles_te
	output.write_uint32(tiles_te.size());
	for (const auto& b : tiles_te) {
		output.write_uint8(b.x);
		output.write_uint8(b.y);

		output.write_uint16(b.value);
		output.write_uint8(b.othervalue);
		output.write_uint8(b.secondvalue);
	}

	std::ofstream wFile("output/map" + std::to_string(map) + ".nmap");
	wFile.write(reinterpret_cast<const char*>(output.get_data()), output.get_size());
}




int main(int arg, char **args)
{	    
	for(int i = 1; i <= 851; ++i)
		load_map(i);


	return 0;
} 