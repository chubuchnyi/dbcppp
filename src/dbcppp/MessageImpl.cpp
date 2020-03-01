
#include "MessageImpl.h"

using namespace dbcppp;

uint64_t MessageImpl::getId() const
{
	return _id;
}
const std::string& MessageImpl::getName() const
{
	return _name;
}
uint64_t MessageImpl::getMessageSize() const
{
	return _message_size;
}
const std::string& MessageImpl::getTransmitter() const
{
	return _transmitter;
}
bool MessageImpl::hasMessageTransmitter(const std::string& name) const
{
	return _message_transmitters.find(name) != _message_transmitters.end();
}
std::vector<const std::string*> MessageImpl::getMessageTransmitters() const
{
	std::vector<const std::string*> result;
	for (auto& n : _message_transmitters)
	{
		result.emplace_back(&n);
	}
	return result;
}
const Signal* MessageImpl::getSignalByName(const std::string& name) const
{
	const Signal* result = nullptr;
	auto iter = _signals.find(name);
	if (iter != _signals.end())
	{
		result = &iter->second;
	}
	return result;
}
std::vector<std::pair<std::string, const Signal*>> MessageImpl::getSignals() const
{
	std::vector<std::pair<std::string, const Signal*>> result;
	for (auto& s : _signals)
	{
		result.emplace_back(s.first, &s.second);
	}
	return result;
}
const Attribute* MessageImpl::getAttributeValueByName(const std::string& name) const
{
	const Attribute* result = nullptr;
	auto iter = _attribute_values.find(name);
	if (iter != _attribute_values.end())
	{
		result = &iter->second;
	}
	return result;
}
std::vector<std::pair<std::string, const Attribute*>> MessageImpl::getAttributeValues() const
{
	std::vector<std::pair<std::string, const Attribute*>> result;
	for (auto& av : _attribute_values)
	{
		result.emplace_back(av.first, &av.second);
	}
	return result;
}
const std::string& MessageImpl::getComment() const
{
	return _comment;
}
Signal* MessageImpl::addSignal(
	const std::string& name,
	Signal::ByteOrder byte_order,
	Signal::ValueType value_type,
	uint64_t bit_size, uint64_t start_bit,
	uint64_t message_size)
{
	Signal* result = nullptr;
	if (_signals.find(name) == _signals.end())
	{
		SignalImpl sig(byte_order, value_type, bit_size, start_bit, Signal::ExtendedValueType::Float, message_size);
		sig._name = name;
		if (sig.getError() == SignalImpl::ErrorCode::NoError)
		{
			_signals[name] = std::move(sig);
			result = &_signals[name];
		}
	}
	return result;
}
void MessageImpl::removeSignal(const std::string& name)
{
	_signals.erase(name);
}

void Message::serializeToStream(std::ostream& os) const
{
	os << "BO_ " << getId() << " " << getName() << ": " << getMessageSize() << " " << getTransmitter();
	for (const auto& s : getSignals())
	{
		os << "\n ";
		s.second->serializeToStream(os);
	}
}
