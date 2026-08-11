#include "framework/scene.hpp"
#include "game/scenes/start_screen/rendering_system.hpp"
#include "game/scenes/start_screen/rendering_system_component.hpp"
#include "game/scenes/start_screen/scene.hpp"
#include <memory>

namespace start_screen {

void Scene::initialize(framework::SceneInitializationContext ctx) {
  ctx.assets.load_images_in_dir_png("invasion");
  ctx.assets.load_audio_wav("invasion/menu_select.wav");

  ctx.systems.add_draw_system(
      std::make_unique<RenderingSystem>(ctx.scene_setter, ctx.audio_player, ctx.renderer)
  );

  ctx.ecs.ctx().insert_or_assign(RenderingSystemComponent{});
}

} // namespace start_screen