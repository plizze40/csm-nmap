#include "bytebuffer.h"

ByteBuffer::ByteBuffer()
:m_readPos(0)
{
}

ByteBuffer::~ByteBuffer()
{
}

void ByteBuffer::write_int8(int8_t value)
{
	write(&value, sizeof(value));
}

void ByteBuffer::write_uint8(uint8_t value)
{
	write(&value, sizeof(value));
}

void ByteBuffer::write_int16(int16_t value)
{
	write(&value, sizeof(value));
}

void ByteBuffer::write_uint16(uint16_t value)
{
	write(&value, sizeof(value));
}

void ByteBuffer::write_int32(int32_t value)
{
	write(&value, sizeof(value));
}

void ByteBuffer::write_uint32(uint32_t value)
{
	write(&value, sizeof(value));
}

void ByteBuffer::write_int64(int64_t value)
{
	write(&value, sizeof(value));
}

void ByteBuffer::write_uint64(uint64_t value)
{
	write(&value, sizeof(value));
}

void ByteBuffer::write_float(float value)
{
	write(&value, sizeof(value));
}

void ByteBuffer::write_double(double value)
{
	write(&value, sizeof(value));
}

void ByteBuffer::write_string(const std::string& str)
{
	write_uint16(str.size());
	write_string_fixed(str);
}

void ByteBuffer::write_string(const char* str)
{
	std::size_t length = strlen(str);
	write_uint16(length);

	if (length > 0) {
		write(str, length);
	}
}

void ByteBuffer::write_string_fixed(const std::string& str)
{
	if (!str.empty()) 
		write(str.data(), str.size());
}

int8_t ByteBuffer::read_int8()
{
	int8_t retval;
	read(&retval, sizeof(retval));

	return retval;
}

uint8_t ByteBuffer::read_uint8()
{
	uint8_t retval;
	read(&retval, sizeof(retval));

	return retval;
}

int16_t ByteBuffer::read_int16()
{
	int16_t retval;
	read(&retval, sizeof(retval));

	return retval;
}

uint16_t ByteBuffer::read_uint16()
{
	uint16_t retval;
	read(&retval, sizeof(retval));

	return retval;
}

int32_t ByteBuffer::read_int32()
{
	int32_t retval;
	read(&retval, sizeof(retval));

	return retval;
}

uint32_t ByteBuffer::read_uint32()
{
	uint32_t retval;
	read(&retval, sizeof(retval));

	return retval;
}

int64_t ByteBuffer::read_int64()
{
	int64_t retval;
	read(&retval, sizeof(retval));

	return retval;
}

uint64_t ByteBuffer::read_uint64()
{
	uint64_t retval;
	read(&retval, sizeof(retval));

	return retval;
}

float ByteBuffer::read_float()
{
	float retval;
	read(&retval, sizeof(retval));

	return retval;
}

double ByteBuffer::read_double()
{
	double retval;
	read(&retval, sizeof(retval));

	return retval;
}

std::string ByteBuffer::read_string()
{
	std::size_t length = read_uint16();

	return read_string_fixed(length);
}

std::string ByteBuffer::read_string_fixed(std::size_t size)
{
	if (size == 0)
		return std::string();

	std::vector<char> str(size);
	read(&str[0], size);

	return {str.begin(), str.end()};
}

void ByteBuffer::write(const void* src, std::size_t size)
{
	auto ptr = reinterpret_cast<const uint8_t*>(src);
	m_data.insert(m_data.end(), ptr, ptr + size);
}

void ByteBuffer::read(void* dest, std::size_t size)
{
	if ((size + m_readPos) > get_size()) {
		throw std::runtime_error("Subscript out of range");
	}

	memcpy(dest, &m_data[m_readPos], size);
	m_readPos += size;
}

std::size_t ByteBuffer::get_size() const
{
	return m_data.size();
}

std::size_t ByteBuffer::get_readpos() const
{
	return m_readPos;
}

const void* ByteBuffer::get_data() const
{
	if(m_data.empty())
		return nullptr;
	return m_data.data();
}
 