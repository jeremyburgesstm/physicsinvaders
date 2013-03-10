/*
 * This source file is part of one of Jeremy Burgess's samples.
 *
 * Copyright (c) 2013 Jeremy Burgess 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

// STL includes
#include <vector>
#include <string>

// Eigen
#include "Core/EigenIncludes.h"

namespace ComponentModel
{
	class Component;
	class EntityComponentManager;

	/**
	 * \class Entity
	 *
	 * Represents a generic entity (or 'game object') in the entity component model.
	 * Holds transform data as well as a name field for use in lookup/identification.
	 * Holds the list of components which define the behaviour of this entity.
	 */
	class Entity
	{
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		friend class EntityComponentManager; // ECM should be able to set itself.
		friend class Component; // The entity is the path back to the ECM for all components.
	public:
		Entity(void);
		virtual ~Entity(void) {};

		void AddComponent(Component* c);
		void RemoveComponent(Component* c);
		void RemoveAllComponents(EntityComponentManager* ecm);

		template<class T>
		T* GetComponentByType()
		{
			for each (Component* c in m_components)
			{
				T* cAsT = dynamic_cast<T*>(c);
				if (cAsT != nullptr)
				{
					return cAsT;
				}
			}
			return nullptr;
		}

		/// Version of GetComponentByType which does _NOT_ respect polymorphism,
		/// i.e., will only return the component if it is of the type passed exactly.
		template<class T>
		T* GetComponentByTypeFast()
		{
			const type_info& typeInfoT = typeid(T);
			for each (Component* c in m_components)
			{
				const type_info& typeInfoC = typeid(*c);
				if (typeInfoC == typeInfoT)
				{
					return static_cast<T*>(c);
				}
			}
			return nullptr;
		}

		inline void SetPosition(const Eigen::Vector3f& position) { m_position = position; }
		inline const Eigen::Vector3f& GetPosition() const { return m_position; }

		inline void SetOrientation(const Eigen::Quaternionf& orientation) { m_orientation = orientation; }
		inline const Eigen::Quaternionf& GetOrientation() const { return m_orientation; }

		inline void SetScale(float scale) { m_scale = scale; }
		inline float GetScale() const { return m_scale; }

		inline void SetEnabled(bool enabled) { m_enabled = enabled; }
		inline bool GetEnabled() const { return m_enabled; }

		inline const std::string& GetName() const { return m_name; }
		inline void SetName(const std::string& name) { m_name = name; } 

		Eigen::Affine3f GetTransform() const;

		void Destroy();

	private:
		void SetEntityComponentManager(EntityComponentManager* mgr) { m_manager = mgr; }
		EntityComponentManager* GetEntityComponentManager() const { return m_manager; }
		void SetAlive(bool isAlive) { m_isAlive = isAlive; }
		bool GetAlive() const { return m_isAlive; }

	private:
		std::vector<Component*> m_components;
		Eigen::Vector3f m_position;
		Eigen::Quaternionf m_orientation;
		float m_scale;
		EntityComponentManager* m_manager;
		bool m_enabled;
		std::string m_name;
		bool m_isAlive;
	};

};