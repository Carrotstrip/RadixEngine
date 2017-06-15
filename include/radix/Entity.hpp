#ifndef RADIX_ENTITY_HPP
#define RADIX_ENTITY_HPP

#include <array>
#include <memory>
#include <type_traits>

#include <radix/Transform.hpp>
#include <radix/core/event/Event.hpp>
#include <radix/core/types/TimeDelta.hpp>
#include <radix/env/Util.hpp>

namespace radix {

class World;

using EntityId = uint32_t;

/** \class Entity
 * It is created like this:
 * @snippet source/data/map/XmlMapLoader.cpp Creating an Entity
 */
class Entity : protected Transform {
private:
  friend class EntityManager;

  Entity(Entity&) = delete;
  Entity& operator=(Entity&) = delete;
  Entity(Entity&&) = delete;
  Entity& operator=(Entity&&) = delete;

  /**
   * Entity's name. Used to find a specific entity from the @ref World entity list.
   */
  std::string m_name;

public:
  struct NameChangedEvent : public Event {
    static constexpr StaticEventTypeName TypeName = "radix/Entity:NameChanged";
    const EventTypeName getTypeName() const {
      return TypeName;
    }
    static constexpr StaticEventType Type = TypeNameHash(TypeName);
    const EventType getType() const {
      return Type;
    }

    Entity &entity;
    const std::string oldName;
    NameChangedEvent(Entity &e, const std::string &o) :
      entity(e), oldName(o) {}
  };


  World &world;

  struct CreationParams {
    World &world;
    EntityId id;
    CreationParams(World &world, EntityId id) :
      world(world),
      id(id) {
    }
  };

  Entity(const CreationParams &cp);
  virtual ~Entity();


  /**
   * Entity (instance) identifier.
   * @note IDs are stable and may be used as permanent access keys, but consider using @ref getName
   *       when not referring to a particular unique instance but rather any entity that has.
   */
  EntityId id;

  inline bool operator==(const Entity &o) const {
    return id == o.id;
  }
  inline bool operator!=(const Entity &o) const {
    return id != o.id;
  }


  inline void privSetPosition(const Vector3f &v) {
    position = v;
  }
  inline void privSetScale(const Vector3f &v) {
    scale = v;
  }
  inline void privSetOrientation(const Quaternion &v) {
    orientation = v;
  }

  inline const Vector3f& getPosition() const {
    return position;
  }
  virtual void setPosition(const Vector3f&);

  inline const Vector3f& getScale() const {
    return scale;
  }
  virtual void setScale(const Vector3f&);

  inline const Quaternion& getOrientation() const {
    return orientation;
  }
  virtual void setOrientation(const Quaternion&);


  std::string name() const {
    return m_name;
  }
  void setName(const std::string&);


  virtual std::string fullClassName() = 0;
  virtual std::string className() = 0;

  virtual std::string typeName();

  virtual void tick(TDelta);
};

} /* namespace radix */

#endif /* RADIX_ENTITY_HPP */
