#include "framework/event_broker.hpp"
#include "framework/player_input_manager.hpp"
#include "framework/renderer.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/position.hpp"
#include "game/scenes/invasion/components/sprite.hpp"
#include "game/scenes/invasion/systems/sprite_rendering.hpp"
#include <entt.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace testing::sprite_rendering_system {

class MockRenderer : public framework::Renderer {
  public:
    // NOLINTBEGIN(modernize-type-traits)
    MOCK_METHOD(void, draw_image, (const framework::DrawImageParams &params), (override));
    MOCK_METHOD(void, draw_rect, (const framework::DrawRectParams &params), (override));
    MOCK_METHOD(void, draw_line, (const framework::DrawLineParams &params), (override));
    // NOLINTEND(modernize-type-traits)
};

struct TestSetup {
    entt::registry ecs;
    framework::EventBroker events;
    framework::PlayerInputManager player_input;
    std::shared_ptr<MockRenderer> renderer;
    systems::SpriteRendering system;

    framework::ExecuteCtx ctx() {
      return framework::ExecuteCtx{
          .ecs = ecs,
          .events = events,
          .player_input = player_input,
      };
    }
};

TestSetup setupTest() {
  auto renderer = std::make_shared<MockRenderer>();

  return TestSetup{
      .ecs = entt::registry(),
      .events = framework::EventBroker(),
      .player_input = framework::PlayerInputManager(),
      .renderer = renderer,
      .system = systems::SpriteRendering(*renderer),
  };
}

TEST(SystemSpriteRendering, ExecuteWithSpriteComponentEntitiesShouldRenderTheseEntities) {
  auto setup = setupTest();
  auto entity = setup.ecs.create();
  setup.ecs.emplace<components::Sprite>(
      entity,
      components::Sprite{
          .src_id = "1",
          .src_x = 5,
          .src_y = 6,
          .src_w = 7,
          .src_h = 8,
          .dst_w = 9,
          .dst_h = 10,
      }
  );
  setup.ecs.emplace<components::Position>(
      entity, components::Position{.x = 1, .y = 2, .w = 3, .h = 4, .z = 100}
  );
  auto entity_two = setup.ecs.create();
  setup.ecs.emplace<components::Sprite>(
      entity_two,
      components::Sprite{
          .src_id = "2",
          .src_x = 51,
          .src_y = 61,
          .src_w = 71,
          .src_h = 81,
          .dst_w = 91,
          .dst_h = 101,
      }
  );
  setup.ecs.emplace<components::Position>(
      entity_two, components::Position{.x = 11, .y = 21, .w = 31, .h = 41, .z = 101}
  );
  auto ctx = setup.ctx();

  auto expected_draw = framework::DrawImageParams{
      .src_id = "1",
      .src_x = 5,
      .src_y = 6,
      .src_width = 7,
      .src_height = 8,
      .dst_x = 1,
      .dst_y = 2,
      .dst_width = 9,
      .dst_height = 10,
  };
  EXPECT_CALL(*setup.renderer, draw_image(expected_draw)).Times(1);
  auto expected_draw_two = framework::DrawImageParams{
      .src_id = "2",
      .src_x = 51,
      .src_y = 61,
      .src_width = 71,
      .src_height = 81,
      .dst_x = 11,
      .dst_y = 21,
      .dst_width = 91,
      .dst_height = 101,
  };
  EXPECT_CALL(*setup.renderer, draw_image(expected_draw_two)).Times(1);

  setup.system.execute(ctx);
}

} // namespace testing::sprite_rendering_system