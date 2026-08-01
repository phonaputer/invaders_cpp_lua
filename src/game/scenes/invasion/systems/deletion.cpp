#include "game/scenes/invasion/systems/deletion.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/to_be_deleted.hpp"

namespace systems {

void Deletion::execute(framework::ExecuteCtx &ctx) {
  auto view = ctx.ecs.view<components::ToBeDeleted>();

  ctx.ecs.destroy(view.begin(), view.end());
}

} // namespace systems