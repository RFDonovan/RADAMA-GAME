#include "World.h"

/// class ResourceHolder - template <typename Resource, typename Identifier>

class ResourceHolder {
  // Attributes
public:
  /// before the class:
template <typename Resource, typename Identifier>
  std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
  // Operations
public:
  void load ();
  Resource& get (Identifier id);
  void insertResource ();
};

