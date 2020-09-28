#pragma once
#include <string>
#include <cstring>
#include <vector>
#include <stdexcept>

class ByteBuffer
{
public:
	ByteBuffer();
	~ByteBuffer();
	
	void write_int8(int8_t value);
	void write_uint8(uint8_t value);

	void write_int16(int16_t value);
	void write_uint16(uint16_t value);

	void write_int32(int32_t value);
	void write_uint32(uint32_t value);

	void write_int64(int64_t value);
	void write_uint64(uint64_t value);

	void write_float(float value);
	void write_double(double value);

	void write_string(const std::string& str);
	void write_string(const char* str);
	void write_string_fixed(const std::string& str);

	int8_t read_int8();
	uint8_t read_uint8();

	int16_t read_int16();
	uint16_t read_uint16();

	int32_t read_int32();
	uint32_t read_uint32();

	int64_t read_int64();
	uint64_t read_uint64();

	float read_float();
	double read_double();

	std::string read_string();
	std::string read_string_fixed(std::size_t size);

	void write(const void* src, std::size_t size);
	void read(void* dest, std::size_t size);

	std::size_t get_size() const;
	std::size_t get_readpos() const;
	const void* get_data() const;
private:
	std::vector<uint8_t> m_data;
	std::size_t			 m_readPos;
};

