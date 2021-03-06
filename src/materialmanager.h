/*
 * This file is part of the Open Stackup distribution (https://github.com/waj334/openstackup).
 * Copyright (c) 2021 Justin A. Wilson.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "enums.h"
#include "manager.h"
#include "material.h"

#include <QList>
#include <QMetaType>

class MaterialManager : public Manager<MaterialManager>
{
public:
	using MaterialList = QList<Material>;

	explicit MaterialManager();
	~MaterialManager();

	const Material get(const QString& name) const;
	
	MaterialList& materials();
	const MaterialList& materials() const;

	static QString typeString(const MaterialClass& type);

private:
	MaterialList m_materials;
};