#include "ContactTag.h"

namespace uc {

ContactTag::ContactTag()  : _id(0),  _name(""),
        _value(""),  _inputType(CTIT_SINGLE_LINE_TEXT), _userId(0),
        _scope(CTS_DEFAULT), _type(CTT_BASIC), _state(0){
}

ContactTag::ContactTag (const ContactTag& tag) : _id(tag._id),  _name(tag._name),
        _value(tag._value),  _inputType(tag._inputType), _userId(tag._userId),
        _scope(tag._scope), _type(tag._type), _state(tag._state){
}

ContactTag& ContactTag::operator=(const ContactTag& other) {
    if (this == &other) {
        return *this;
    }

    _id = other._id;
    _name = other._name;
    _value = other._value;
    _inputType = other._inputType;
    _userId = other._userId;
    _scope = other._scope;
    _type = other._type;
    _state = other._state;

    return *this;
}

void ContactTag::Update(const ContactTag& tag) {
    _name = tag._name;
    _value = tag._value;
    _scope = tag._scope;
    _type = tag._type;
    _state = tag._state;
}

}

