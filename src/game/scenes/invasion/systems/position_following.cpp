#include "game/scenes/invasion/systems/position_following.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/position.hpp"
#include "game/scenes/invasion/components/position_following.hpp"

namespace systems {

void PositionFollowing::execute(framework::ExecuteCtx &ctx) {
  auto view = ctx.ecs.view<components::PositionFollowing>();
  std::vector<entt::entity> to_remove;

  for (auto [entity, following] : view.each()) {
    const entt::entity leader{following.leader};

    if (!ctx.ecs.valid(leader) || !ctx.ecs.all_of<components::Position>(leader)) {
      to_remove.push_back(entity);
      continue;
    }

    const auto &leader_position = ctx.ecs.get<components::Position>(leader);
    auto &follower_position = ctx.ecs.get<components::Position>(entity);

    follower_position.x = leader_position.x + following.x_offset;
    follower_position.y = leader_position.y + following.y_offset;
  }

  for (const auto &entity : to_remove) {
    ctx.ecs.remove<components::PositionFollowing>(entity);
  }
}

} // namespace systems
