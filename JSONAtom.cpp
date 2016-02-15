/*
 * This file is part of rSON
 * Copyright © 2012-2013 Rachel Mant (dx-mon@users.sourceforge.net)
 *
 * rSON is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * rSON is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "internal.h"

JSONAtom::JSONAtom() : type(JSON_TYPE_NULL)
{
}

JSONAtom::JSONAtom(JSONAtomType atomType) : type(atomType)
{
}

JSONAtom::~JSONAtom()
{
}

JSONAtomType JSONAtom::getType() const
{
	return type;
}

JSONAtom::operator JSONAtom *() const
{
	return const_cast<JSONAtom *>(this);
}

void *JSONAtom::asNull() const
{
	if (type != JSON_TYPE_NULL)
		throw JSONTypeError(type, JSON_TYPE_NULL);
	return nullptr;
}

bool JSONAtom::asBool() const
{
	if (type != JSON_TYPE_BOOL)
		throw JSONTypeError(type, JSON_TYPE_BOOL);
	return *((JSONBool *)this);
}

int JSONAtom::asInt() const
{
	if (type != JSON_TYPE_INT)
		throw JSONTypeError(type, JSON_TYPE_INT);
	return *((JSONInt *)this);
}

double JSONAtom::asFloat() const
{
	if (type != JSON_TYPE_FLOAT)
		throw JSONTypeError(type, JSON_TYPE_FLOAT);
	return *((JSONFloat *)this);
}

const char *JSONAtom::asString() const
{
	if (type != JSON_TYPE_STRING)
		throw JSONTypeError(type, JSON_TYPE_STRING);
	return *((JSONString *)this);
}

JSONObject *JSONAtom::asObject() const
{
	if (type != JSON_TYPE_OBJECT)
		throw JSONTypeError(type, JSON_TYPE_OBJECT);
	return (JSONObject *)this;
}

JSONObject &JSONAtom::asObjectRef() const
{
	return *asObject();
}

JSONArray *JSONAtom::asArray() const
{
	if (type != JSON_TYPE_ARRAY)
		throw JSONTypeError(type, JSON_TYPE_ARRAY);
	return (JSONArray *)this;
}

JSONArray &JSONAtom::asArrayRef() const
{
	return *asArray();
}
