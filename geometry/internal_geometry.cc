#include "drake/geometry/internal_geometry.h"

#include <memory>

#include "drake/common/drake_assert.h"
#include "drake/geometry/make_mesh_for_deformable.h"

namespace drake {
namespace geometry {
namespace internal {

using math::RigidTransform;
using std::move;

InternalGeometry::InternalGeometry(
    SourceId source_id, std::unique_ptr<Shape> shape, FrameId frame_id,
    GeometryId geometry_id, std::string name, RigidTransform<double> X_FG)
    : shape_spec_(std::move(shape)),
      id_(geometry_id),
      name_(std::move(name)),
      source_id_(source_id),
      frame_id_(frame_id),
      X_PG_(move(X_FG)),
      X_FG_(X_PG_),
      parent_geometry_id_(std::nullopt) {}

InternalGeometry::InternalGeometry(SourceId source_id,
                                   std::unique_ptr<Shape> shape,
                                   FrameId frame_id, GeometryId geometry_id,
                                   std::string name, double resolution_hint)
    : shape_spec_(std::move(shape)),
      id_(geometry_id),
      name_(std::move(name)),
      source_id_(source_id),
      frame_id_(frame_id),
      X_PG_(RigidTransform<double>::Identity()),
      X_FG_(RigidTransform<double>::Identity()),
      parent_geometry_id_(std::nullopt) {
  MeshBuilderForDeformable mesh_builder;
  reference_mesh_ = mesh_builder.Build(*shape_spec_, resolution_hint);
}

bool InternalGeometry::has_role(Role role) const {
  switch (role) {
    case Role::kProximity:
      return has_proximity_role();
    case Role::kIllustration:
      return has_illustration_role();
    case Role::kPerception:
      return has_perception_role();
    case Role::kUnassigned:
      return !(has_proximity_role() || has_perception_role() ||
               has_illustration_role());
  }
  DRAKE_UNREACHABLE();
}

}  // namespace internal
}  // namespace geometry
}  // namespace drake
