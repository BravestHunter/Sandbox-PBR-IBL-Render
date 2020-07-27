#include "Example/application.h"

#include <string>
#include <limits>

#include "Sandbox/common/graphics/opengl/opengl_binder.h"
#include "Sandbox/graphics/model_loader.h"
#include "Sandbox/common/graphics/opengl/opengl_model_component.h"
#include "Sandbox/common/graphics/opengl/opengl_hdr_skybox.h"

#include "Sandbox/graphics/basic_meshes.h"
#include "Sandbox/graphics/point_light.h"

int GraphicsApplication::RunExample1(int width, int height)
{
  // systems configuration
  ConfigurateSystems(width, height);

  // constructing modules
  auto engine_module = std::make_shared<sandbox_common::SandboxEngine>();
  auto interrupt_module = std::make_shared<InterruptApplicationModule>();
  auto scene_module = std::make_shared<sandbox_common::SceneModule>();
  auto graphics_module = std::make_shared<sandbox_common::OpenglRenderModule>(width, height);
  auto camera_movement_module = std::make_shared<sandbox_common::SandboxCameraMovementModule>();

  engine_module->AddChild(scene_module);
  engine_module->AddChild(interrupt_module);
  scene_module->AddChild(graphics_module);
  graphics_module->AddChild(camera_movement_module);

  engine_module->Construct();
  interrupt_module->Construct();
  scene_module->Construct();
  graphics_module->Construct();
  camera_movement_module->Construct();

  // scene configuration
  sandbox_graphics::GraphicsModel axis_model = CreateAxisModel();
  sandbox_graphics::GraphicsModel triangle_model1;
  sandbox_graphics::GraphicsModel triangle_model2;
  sandbox_graphics::GraphicsModel box_model1;
  sandbox_graphics::GraphicsModel box_model2;
  sandbox_graphics::GraphicsModel box_model3;
  sandbox_graphics::ModelLoader::LoadGltfModel(triangle_model1, RESOURCE_DIRECTORY + std::string("resources/models/gltf/TriangleWithoutIndices/glTF/TriangleWithoutIndices.gltf"));
  sandbox_graphics::ModelLoader::LoadGltfModel(triangle_model2, RESOURCE_DIRECTORY + std::string("resources/models/gltf/Triangle/glTF/Triangle.gltf"));
  sandbox_graphics::ModelLoader::LoadGltfModel(box_model1, RESOURCE_DIRECTORY + std::string("resources/models/gltf/Box/glTF/Box.gltf"));
  sandbox_graphics::ModelLoader::LoadGltfModel(box_model2, RESOURCE_DIRECTORY + std::string("resources/models/gltf/BoxInterleaved/glTF/BoxInterleaved.gltf"));
  sandbox_graphics::ModelLoader::LoadGltfModel(box_model3, RESOURCE_DIRECTORY + std::string("resources/models/gltf/BoxVertexColors/glTF/BoxVertexColors.gltf"));
  sandbox_common::OpenglModel opengl_axis_model;
  sandbox_common::OpenglModel opengl_triangle_model1;
  sandbox_common::OpenglModel opengl_triangle_model2;
  sandbox_common::OpenglModel opengl_box_model1;
  sandbox_common::OpenglModel opengl_box_model2;
  sandbox_common::OpenglModel opengl_box_model3;
  sandbox_common::OpenglBinder::BindModel(opengl_axis_model, axis_model);
  sandbox_common::OpenglBinder::BindModel(opengl_triangle_model1, triangle_model1);
  sandbox_common::OpenglBinder::BindModel(opengl_triangle_model2, triangle_model2);
  sandbox_common::OpenglBinder::BindModel(opengl_box_model1, box_model1);
  sandbox_common::OpenglBinder::BindModel(opengl_box_model2, box_model2);
  sandbox_common::OpenglBinder::BindModel(opengl_box_model3, box_model3);

  sandbox_common::Entity axes_entity;
  sandbox_common::Entity triangle_ent1;
  sandbox_common::Entity triangle_ent2;
  sandbox_common::Entity box_ent1;
  sandbox_common::Entity box_ent2;
  sandbox_common::Entity box_ent3;
  scene_module->CreateEntity(axes_entity);
  scene_module->CreateEntity(triangle_ent1);
  scene_module->CreateEntity(triangle_ent2);
  scene_module->CreateEntity(box_ent1);
  scene_module->CreateEntity(box_ent2);
  scene_module->CreateEntity(box_ent3);
  scene_module->RegisterComponent<sandbox_common::OpenglModelComponent>();
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(axes_entity, { opengl_axis_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(triangle_ent1, { opengl_triangle_model1, sandbox_graphics::Transformation({glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(triangle_ent2, { opengl_triangle_model2, sandbox_graphics::Transformation({glm::vec3(5.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(box_ent1, { opengl_box_model1, sandbox_graphics::Transformation({glm::vec3(-5.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(box_ent2, { opengl_box_model2, sandbox_graphics::Transformation({glm::vec3(0.0f, 5.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(box_ent3, { opengl_box_model3, sandbox_graphics::Transformation({glm::vec3(0.0f, -5.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f)}) });

  // renderer configuration
  graphics_module->SetLightingSimulationMode(false);

  // run
  int run_result = engine_module->Run();

  sandbox_common::OpenglBinder::UnbindModel(opengl_axis_model);
  sandbox_common::OpenglBinder::UnbindModel(opengl_triangle_model1);
  sandbox_common::OpenglBinder::UnbindModel(opengl_triangle_model2);
  sandbox_common::OpenglBinder::UnbindModel(opengl_box_model1);
  sandbox_common::OpenglBinder::UnbindModel(opengl_box_model2);
  sandbox_common::OpenglBinder::UnbindModel(opengl_box_model3);

  // destructing modules
  engine_module->Destruct();
  interrupt_module->Destruct();
  scene_module->Destruct();
  graphics_module->Destruct();
  camera_movement_module->Destruct();

  sandbox_platform::PlatformProvider::GetPlatform()->GetPlatformWindow().lock()->CloseWindow();

  return run_result;
}

int GraphicsApplication::RunExample2(int width, int height)
{
  // systems configuration
  ConfigurateSystems(width, height);

  // constructing modules
  auto engine_module = std::make_shared<sandbox_common::SandboxEngine>();
  auto interrupt_module = std::make_shared<InterruptApplicationModule>();
  auto scene_module = std::make_shared<sandbox_common::SceneModule>();
  auto graphics_module = std::make_shared<sandbox_common::OpenglRenderModule>(width, height);
  auto camera_movement_module = std::make_shared<sandbox_common::SandboxCameraMovementModule>();

  engine_module->AddChild(scene_module);
  engine_module->AddChild(interrupt_module);
  scene_module->AddChild(graphics_module);
  graphics_module->AddChild(camera_movement_module);

  engine_module->Construct();
  interrupt_module->Construct();
  scene_module->Construct();
  graphics_module->Construct();
  camera_movement_module->Construct();

  // scene configuration
  sandbox_graphics::GraphicsModel axis_model = CreateAxisModel();
  sandbox_graphics::GraphicsModel duck_model;
  sandbox_graphics::GraphicsModel box_model;
  sandbox_graphics::ModelLoader::LoadGltfModel(duck_model, RESOURCE_DIRECTORY + std::string("resources/models/gltf/Duck/glTF/Duck.gltf"));
  sandbox_graphics::ModelLoader::LoadGltfModel(box_model, RESOURCE_DIRECTORY + std::string("resources/models/gltf/BoxTextured/glTF/BoxTextured.gltf"));
  sandbox_common::OpenglModel opengl_axis_model;
  sandbox_common::OpenglModel opengl_duck_model;
  sandbox_common::OpenglModel opengl_box_model;
  sandbox_common::OpenglBinder::BindModel(opengl_axis_model, axis_model);
  sandbox_common::OpenglBinder::BindModel(opengl_duck_model, duck_model);
  sandbox_common::OpenglBinder::BindModel(opengl_box_model, box_model);

  sandbox_common::Entity axes_entity;
  sandbox_common::Entity duck_ent;
  sandbox_common::Entity box_ent;
  scene_module->CreateEntity(axes_entity);
  scene_module->CreateEntity(duck_ent);
  scene_module->CreateEntity(box_ent);
  scene_module->RegisterComponent<sandbox_common::OpenglModelComponent>();
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(axes_entity, { opengl_axis_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(duck_ent, { opengl_duck_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.01f, 0.01f, 0.01f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(box_ent, { opengl_box_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 2.5f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f)}) });

  // renderer configuration
  graphics_module->SetLightingSimulationMode(false);

  // run
  int run_result = engine_module->Run();

  sandbox_common::OpenglBinder::UnbindModel(opengl_axis_model);
  sandbox_common::OpenglBinder::UnbindModel(opengl_duck_model);
  sandbox_common::OpenglBinder::UnbindModel(opengl_box_model);

  // destructing modules
  engine_module->Destruct();
  interrupt_module->Destruct();
  scene_module->Destruct();
  graphics_module->Destruct();
  camera_movement_module->Destruct();

  sandbox_platform::PlatformProvider::GetPlatform()->GetPlatformWindow().lock()->CloseWindow();

  return run_result;
}

int GraphicsApplication::RunExample3(int width, int height)
{
  // systems configuration
  ConfigurateSystems(width, height);

  // constructing modules
  auto engine_module = std::make_shared<sandbox_common::SandboxEngine>();
  auto interrupt_module = std::make_shared<InterruptApplicationModule>();
  auto scene_module = std::make_shared<sandbox_common::SceneModule>();
  auto graphics_module = std::make_shared<sandbox_common::OpenglRenderModule>(width, height);
  auto wrap_module = std::make_shared<ModelWrapModule>();
  auto camera_movement_module = std::make_shared<sandbox_common::SandboxCameraMovementModule>();
  auto render_constrol_module = std::make_shared<OpenglRenderControlModule>();

  engine_module->AddChild(scene_module);
  engine_module->AddChild(interrupt_module);
  scene_module->AddChild(graphics_module);
  scene_module->AddChild(wrap_module);
  graphics_module->AddChild(camera_movement_module);
  graphics_module->AddChild(render_constrol_module);

  engine_module->Construct();
  interrupt_module->Construct();
  scene_module->Construct();
  graphics_module->Construct();
  wrap_module->Construct();
  camera_movement_module->Construct();
  render_constrol_module->Construct();

  // scene configuration
  sandbox_graphics::GraphicsModel axis_model = CreateAxisModel();
  sandbox_graphics::GraphicsModel avocado_model;
  sandbox_graphics::GraphicsModel box_model;
  sandbox_graphics::ModelLoader::LoadGltfModel(avocado_model, RESOURCE_DIRECTORY + std::string("resources/models/gltf/Avocado/glTF/Avocado.gltf"));
  box_model.meshes.push_back(sandbox_graphics::BasicMeshes::Cube());
  sandbox_common::OpenglModel opengl_axis_model;
  sandbox_common::OpenglModel opengl_avocado_model;
  sandbox_common::OpenglModel opengl_box_model;
  sandbox_common::OpenglBinder::BindModel(opengl_axis_model, axis_model);
  sandbox_common::OpenglBinder::BindModel(opengl_avocado_model, avocado_model);
  sandbox_common::OpenglBinder::BindModel(opengl_box_model, box_model);
  sandbox_common::OpenglHdrSkybox skybox;
  sandbox_common::OpenglBinder::BindHdrSkybox(skybox, RESOURCE_DIRECTORY + std::string("resources/skyboxes/stadium_center/Stadium_Center_3k.hdr"));

  sandbox_common::Entity axis_entity;
  sandbox_common::Entity skybox_ent;
  sandbox_common::Entity box_ent1;
  sandbox_common::Entity box_ent2;
  sandbox_common::Entity avocado_ent;
  sandbox_common::Entity point_light_ent1;
  sandbox_common::Entity point_light_ent2;
  scene_module->CreateEntity(axis_entity);
  scene_module->CreateEntity(skybox_ent);
  scene_module->CreateEntity(box_ent1);
  scene_module->CreateEntity(box_ent2);
  scene_module->CreateEntity(avocado_ent);
  scene_module->CreateEntity(point_light_ent1);
  scene_module->CreateEntity(point_light_ent2);
  scene_module->RegisterComponent<sandbox_common::OpenglModelComponent>();
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(axis_entity, { opengl_axis_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(box_ent1, { opengl_box_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 5.0f, 5.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.2f, 0.2f, 0.2f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(box_ent2, { opengl_box_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 0.0f, 5.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.2f, 0.2f, 0.2f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(avocado_ent, { opengl_avocado_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(3.14f, 3.14f, 0.0f)), glm::vec3(70.0f, 70.0f, 70.0f)}) });
  scene_module->RegisterComponent<sandbox_graphics::PointLight>();
  scene_module->AddComponent<sandbox_graphics::PointLight>(point_light_ent1, { glm::vec3(0, 0, 5.0f), glm::vec3(100.0f, 100.0f, 100.0f) });
  scene_module->AddComponent<sandbox_graphics::PointLight>(point_light_ent2, { glm::vec3(0, 5.0f, 5.0f), glm::vec3(100.0f, 100.0f, 100.0f) });
  scene_module->RegisterComponent<sandbox_common::OpenglHdrSkybox>();
  scene_module->AddComponent<sandbox_common::OpenglHdrSkybox>(skybox_ent, skybox);

  // wrap configuration
  wrap_module->SetEntity(avocado_ent);

  // renderer configuration
  graphics_module->SetSkyboxUsingMode(true);
  graphics_module->SetSkyboxEntity(skybox_ent);

  // run
  int run_result = engine_module->Run();

  sandbox_common::OpenglBinder::UnbindModel(opengl_axis_model);
  sandbox_common::OpenglBinder::UnbindModel(opengl_avocado_model);
  sandbox_common::OpenglBinder::UnbindModel(opengl_box_model);
  sandbox_common::OpenglBinder::UnbindHdrSkybox(skybox);

  // destructing modules
  engine_module->Destruct();
  interrupt_module->Destruct();
  scene_module->Destruct();
  graphics_module->Destruct();
  wrap_module->Destruct();
  camera_movement_module->Destruct();
  render_constrol_module->Destruct();

  sandbox_platform::PlatformProvider::GetPlatform()->GetPlatformWindow().lock()->CloseWindow();

  return run_result;
}

int GraphicsApplication::RunExample4(int width, int height)
{
  // systems configuration
  ConfigurateSystems(width, height);

  // constructing modules
  auto engine_module = std::make_shared<sandbox_common::SandboxEngine>();
  auto interrupt_module = std::make_shared<InterruptApplicationModule>();
  auto scene_module = std::make_shared<sandbox_common::SceneModule>();
  auto graphics_module = std::make_shared<sandbox_common::OpenglRenderModule>(width, height);
  auto wrap_module = std::make_shared<ModelWrapModule>();
  auto camera_movement_module = std::make_shared<sandbox_common::SandboxCameraMovementModule>();
  auto render_constrol_module = std::make_shared<OpenglRenderControlModule>();

  engine_module->AddChild(scene_module);
  engine_module->AddChild(interrupt_module);
  scene_module->AddChild(graphics_module);
  scene_module->AddChild(wrap_module);
  graphics_module->AddChild(camera_movement_module);
  graphics_module->AddChild(render_constrol_module);

  engine_module->Construct();
  interrupt_module->Construct();
  scene_module->Construct();
  graphics_module->Construct();
  wrap_module->Construct();
  camera_movement_module->Construct();
  render_constrol_module->Construct();

  // scene configuration
  sandbox_graphics::GraphicsModel axis_model = CreateAxisModel();
  sandbox_graphics::GraphicsModel fish_model;
  sandbox_graphics::GraphicsModel box_model;
  sandbox_graphics::ModelLoader::LoadGltfModel(fish_model, RESOURCE_DIRECTORY + std::string("resources/models/gltf/WaterBottle/glTF/WaterBottle.gltf"));
  box_model.meshes.push_back(sandbox_graphics::BasicMeshes::Cube());
  sandbox_common::OpenglModel opengl_axis_model;
  sandbox_common::OpenglModel opengl_fish_model;
  sandbox_common::OpenglModel opengl_box_model;
  sandbox_common::OpenglBinder::BindModel(opengl_axis_model, axis_model);
  sandbox_common::OpenglBinder::BindModel(opengl_fish_model, fish_model);
  sandbox_common::OpenglBinder::BindModel(opengl_box_model, box_model);
  sandbox_common::OpenglHdrSkybox skybox;
  sandbox_common::OpenglBinder::BindHdrSkybox(skybox, RESOURCE_DIRECTORY + std::string("resources/skyboxes/malibu_overlook/Malibu_Overlook_3k.hdr"));

  sandbox_common::Entity axis_ent;
  sandbox_common::Entity skybox_ent;
  sandbox_common::Entity box_ent1;
  sandbox_common::Entity box_ent2;
  sandbox_common::Entity fish_ent;
  sandbox_common::Entity point_light_ent1;
  sandbox_common::Entity point_light_ent2;
  scene_module->CreateEntity(axis_ent);
  scene_module->CreateEntity(skybox_ent);
  scene_module->CreateEntity(box_ent1);
  scene_module->CreateEntity(box_ent2);
  scene_module->CreateEntity(fish_ent);
  scene_module->CreateEntity(point_light_ent1);
  scene_module->CreateEntity(point_light_ent2);
  scene_module->RegisterComponent<sandbox_common::OpenglModelComponent>();
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(axis_ent, { opengl_axis_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(box_ent1, { opengl_box_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 5.0f, 5.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.2f, 0.2f, 0.2f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(box_ent2, { opengl_box_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 0.0f, 5.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.2f, 0.2f, 0.2f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(fish_ent, { opengl_fish_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(3.14f, 3.14f, 0.0f)), glm::vec3(10.0f, 10.0f, 10.0f)}) });
  scene_module->RegisterComponent<sandbox_graphics::PointLight>();
  scene_module->AddComponent<sandbox_graphics::PointLight>(point_light_ent1, { glm::vec3(0, 0, 5.0f), glm::vec3(5.0f, 5.0f, 5.0f) });
  scene_module->AddComponent<sandbox_graphics::PointLight>(point_light_ent2, { glm::vec3(0, 5.0f, 5.0f), glm::vec3(5.0f, 5.0f, 5.0f) });
  scene_module->RegisterComponent<sandbox_common::OpenglHdrSkybox>();
  scene_module->AddComponent<sandbox_common::OpenglHdrSkybox>(skybox_ent, skybox);

  // wrap configuration
  wrap_module->SetEntity(fish_ent);

  // renderer configuration
  graphics_module->SetSkyboxUsingMode(true);
  graphics_module->SetSkyboxEntity(skybox_ent);

  // run
  int run_result = engine_module->Run();

  sandbox_common::OpenglBinder::UnbindModel(opengl_axis_model);
  sandbox_common::OpenglBinder::UnbindModel(opengl_fish_model);
  sandbox_common::OpenglBinder::UnbindModel(opengl_box_model);
  sandbox_common::OpenglBinder::UnbindHdrSkybox(skybox);

  // destructing modules
  engine_module->Destruct();
  interrupt_module->Destruct();
  scene_module->Destruct();
  graphics_module->Destruct();
  wrap_module->Destruct();
  camera_movement_module->Destruct();
  render_constrol_module->Destruct();

  sandbox_platform::PlatformProvider::GetPlatform()->GetPlatformWindow().lock()->CloseWindow();

  return run_result;
}

int GraphicsApplication::RunExample5(int width, int height)
{
  // systems configuration
  ConfigurateSystems(width, height);

  // constructing modules
  auto engine_module = std::make_shared<sandbox_common::SandboxEngine>();
  auto interrupt_module = std::make_shared<InterruptApplicationModule>();
  auto scene_module = std::make_shared<sandbox_common::SceneModule>();
  auto graphics_module = std::make_shared<sandbox_common::OpenglRenderModule>(width, height);
  auto wrap_module = std::make_shared<ModelWrapModule>();
  auto camera_movement_module = std::make_shared<sandbox_common::SandboxCameraMovementModule>();
  auto render_constrol_module = std::make_shared<OpenglRenderControlModule>();

  engine_module->AddChild(scene_module);
  engine_module->AddChild(interrupt_module);
  scene_module->AddChild(graphics_module);
  scene_module->AddChild(wrap_module);
  graphics_module->AddChild(camera_movement_module);
  graphics_module->AddChild(render_constrol_module);

  engine_module->Construct();
  interrupt_module->Construct();
  scene_module->Construct();
  graphics_module->Construct();
  wrap_module->Construct();
  camera_movement_module->Construct();
  render_constrol_module->Construct();

  // scene configuration
  sandbox_graphics::GraphicsModel axis_model = CreateAxisModel();
  sandbox_graphics::GraphicsModel helmet_model;
  sandbox_graphics::GraphicsModel box_model;
  sandbox_graphics::ModelLoader::LoadGltfModel(helmet_model, RESOURCE_DIRECTORY + std::string("resources/models/gltf/DamagedHelmet/glTF/DamagedHelmet.gltf"));
  box_model.meshes.push_back(sandbox_graphics::BasicMeshes::Cube());
  sandbox_common::OpenglModel opengl_axis_model;
  sandbox_common::OpenglModel opengl_helmet_model;
  sandbox_common::OpenglModel opengl_box_model;
  sandbox_common::OpenglBinder::BindModel(opengl_axis_model, axis_model);
  sandbox_common::OpenglBinder::BindModel(opengl_helmet_model, helmet_model);
  sandbox_common::OpenglBinder::BindModel(opengl_box_model, box_model);
  sandbox_common::OpenglHdrSkybox skybox;
  sandbox_common::OpenglBinder::BindHdrSkybox(skybox, RESOURCE_DIRECTORY + std::string("resources/skyboxes/milkyway/Milkyway_small.hdr"));

  sandbox_common::Entity axis_ent;
  sandbox_common::Entity skybox_ent;
  sandbox_common::Entity box_ent1;
  sandbox_common::Entity box_ent2;
  sandbox_common::Entity helmet_ent;
  sandbox_common::Entity point_light_ent1;
  sandbox_common::Entity point_light_ent2;
  scene_module->CreateEntity(axis_ent);
  scene_module->CreateEntity(skybox_ent);
  scene_module->CreateEntity(box_ent1);
  scene_module->CreateEntity(box_ent2);
  scene_module->CreateEntity(helmet_ent);
  scene_module->CreateEntity(point_light_ent1);
  scene_module->CreateEntity(point_light_ent2);
  scene_module->RegisterComponent<sandbox_common::OpenglModelComponent>();
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(axis_ent, { opengl_axis_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(box_ent1, { opengl_box_model, sandbox_graphics::Transformation({glm::vec3(0, 0, 5.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.2f, 0.2f, 0.2f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(box_ent2, { opengl_box_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 5.0f, 5.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.2f, 0.2f, 0.2f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(helmet_ent, { opengl_helmet_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(2.5f, 2.5f, 2.5f)}) });
  scene_module->RegisterComponent<sandbox_graphics::PointLight>();
  scene_module->AddComponent<sandbox_graphics::PointLight>(point_light_ent1, { glm::vec3(0, 0, 5.0f), glm::vec3(100.0f, 100.0f, 100.0f) });
  scene_module->AddComponent<sandbox_graphics::PointLight>(point_light_ent2, { glm::vec3(0, 5.0f, 5.0f), glm::vec3(100.0f, 100.0f, 100.0f) });
  scene_module->RegisterComponent<sandbox_common::OpenglHdrSkybox>();
  scene_module->AddComponent<sandbox_common::OpenglHdrSkybox>(skybox_ent, skybox);

  // wrap configuration
  wrap_module->SetEntity(helmet_ent);

  // renderer configuration
  graphics_module->SetSkyboxUsingMode(true);
  graphics_module->SetSkyboxEntity(skybox_ent);

  // run
  int run_result = engine_module->Run();

  sandbox_common::OpenglBinder::UnbindModel(opengl_axis_model);
  sandbox_common::OpenglBinder::UnbindModel(opengl_helmet_model);
  sandbox_common::OpenglBinder::UnbindModel(opengl_box_model);
  sandbox_common::OpenglBinder::UnbindHdrSkybox(skybox);

  // destructing modules
  engine_module->Destruct();
  interrupt_module->Destruct();
  scene_module->Destruct();
  graphics_module->Destruct();
  wrap_module->Destruct();
  camera_movement_module->Destruct();
  render_constrol_module->Destruct();

  sandbox_platform::PlatformProvider::GetPlatform()->GetPlatformWindow().lock()->CloseWindow();

  return run_result;
}

int GraphicsApplication::RunExample6(int width, int height)
{
  // systems configuration
  ConfigurateSystems(width, height);

  // constructing modules
  auto engine_module = std::make_shared<sandbox_common::SandboxEngine>();
  auto interrupt_module = std::make_shared<InterruptApplicationModule>();
  auto scene_module = std::make_shared<sandbox_common::SceneModule>();
  auto graphics_module = std::make_shared<sandbox_common::OpenglRenderModule>(width, height);
  auto wrap_module = std::make_shared<ModelWrapModule>();
  auto camera_movement_module = std::make_shared<sandbox_common::SandboxCameraMovementModule>();
  auto render_constrol_module = std::make_shared<OpenglRenderControlModule>();

  engine_module->AddChild(scene_module);
  engine_module->AddChild(interrupt_module);
  scene_module->AddChild(graphics_module);
  scene_module->AddChild(wrap_module);
  graphics_module->AddChild(camera_movement_module);
  graphics_module->AddChild(render_constrol_module);

  engine_module->Construct();
  interrupt_module->Construct();
  scene_module->Construct();
  graphics_module->Construct();
  wrap_module->Construct();
  camera_movement_module->Construct();
  render_constrol_module->Construct();

  // scene configuration
  sandbox_graphics::GraphicsModel axis_model = CreateAxisModel();
  sandbox_graphics::GraphicsModel helmet_model;
  sandbox_graphics::GraphicsModel box_model;
  sandbox_graphics::ModelLoader::LoadGltfModel(helmet_model, RESOURCE_DIRECTORY + std::string("resources/models/gltf/FlightHelmet/glTF/FlightHelmet.gltf"));
  box_model.meshes.push_back(sandbox_graphics::BasicMeshes::Cube());
  sandbox_common::OpenglModel opengl_axis_model;
  sandbox_common::OpenglModel opengl_helmet_model;
  sandbox_common::OpenglModel opengl_box_model;
  sandbox_common::OpenglBinder::BindModel(opengl_axis_model, axis_model);
  sandbox_common::OpenglBinder::BindModel(opengl_helmet_model, helmet_model);
  sandbox_common::OpenglBinder::BindModel(opengl_box_model, box_model);
  sandbox_common::OpenglHdrSkybox skybox;
  sandbox_common::OpenglBinder::BindHdrSkybox(skybox, RESOURCE_DIRECTORY + std::string("resources/skyboxes/serpentine_valley/Serpentine_Valley_3k.hdr"));

  sandbox_common::Entity axes_ent;
  sandbox_common::Entity skybox_ent;
  sandbox_common::Entity box_ent1;
  sandbox_common::Entity box_ent2;
  sandbox_common::Entity helmet_ent;
  sandbox_common::Entity point_light_ent1;
  sandbox_common::Entity point_light_ent2;
  scene_module->CreateEntity(skybox_ent);
  scene_module->CreateEntity(axes_ent);
  scene_module->CreateEntity(box_ent1);
  scene_module->CreateEntity(box_ent2);
  scene_module->CreateEntity(helmet_ent);
  scene_module->CreateEntity(point_light_ent1);
  scene_module->CreateEntity(point_light_ent2);
  scene_module->RegisterComponent<sandbox_common::OpenglModelComponent>();
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(axes_ent, { opengl_axis_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(box_ent1, { opengl_box_model, sandbox_graphics::Transformation({glm::vec3(0, 0, 5.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.2f, 0.2f, 0.2f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(box_ent2, { opengl_box_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 5.0f, 5.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.2f, 0.2f, 0.2f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(helmet_ent, { opengl_helmet_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(6.0f, 6.0f, 6.0f)}) });
  scene_module->RegisterComponent<sandbox_graphics::PointLight>();
  scene_module->AddComponent<sandbox_graphics::PointLight>(point_light_ent1, { glm::vec3(0, 0, 5.0f), glm::vec3(10.0f, 10.0f, 10.0f) });
  scene_module->AddComponent<sandbox_graphics::PointLight>(point_light_ent2, { glm::vec3(0, 5.0f, 5.0f), glm::vec3(10.0f, 10.0f, 10.0f) });
  scene_module->RegisterComponent<sandbox_common::OpenglHdrSkybox>();
  scene_module->AddComponent<sandbox_common::OpenglHdrSkybox>(skybox_ent, skybox);

  // renderer configuration
  graphics_module->SetSkyboxUsingMode(true);
  graphics_module->SetSkyboxEntity(skybox_ent);

  // wrap configuration
  wrap_module->SetEntity(helmet_ent);

  // run
  int run_result = engine_module->Run();

  sandbox_common::OpenglBinder::UnbindModel(opengl_axis_model);
  sandbox_common::OpenglBinder::UnbindModel(opengl_helmet_model);
  sandbox_common::OpenglBinder::UnbindModel(opengl_box_model);
  sandbox_common::OpenglBinder::UnbindHdrSkybox(skybox);

  // destructing modules
  engine_module->Destruct();
  interrupt_module->Destruct();
  scene_module->Destruct();
  graphics_module->Destruct();
  wrap_module->Destruct();
  camera_movement_module->Destruct();
  render_constrol_module->Destruct();

  sandbox_platform::PlatformProvider::GetPlatform()->GetPlatformWindow().lock()->CloseWindow();

  return run_result;
}

int GraphicsApplication::RunExample7(int width, int height)
{
  // systems configuration
  ConfigurateSystems(width, height);

  // constructing modules
  auto engine_module = std::make_shared<sandbox_common::SandboxEngine>();
  auto interrupt_module = std::make_shared<InterruptApplicationModule>();
  auto scene_module = std::make_shared<sandbox_common::SceneModule>();
  auto graphics_module = std::make_shared<sandbox_common::OpenglRenderModule>(width, height);
  auto wrap_module = std::make_shared<ModelWrapModule>();
  auto camera_movement_module = std::make_shared<sandbox_common::SandboxCameraMovementModule>();
  auto render_constrol_module = std::make_shared<OpenglRenderControlModule>();

  engine_module->AddChild(scene_module);
  engine_module->AddChild(interrupt_module);
  scene_module->AddChild(graphics_module);
  scene_module->AddChild(wrap_module);
  graphics_module->AddChild(camera_movement_module);
  graphics_module->AddChild(render_constrol_module);

  engine_module->Construct();
  interrupt_module->Construct();
  scene_module->Construct();
  graphics_module->Construct();
  wrap_module->Construct();
  camera_movement_module->Construct();
  render_constrol_module->Construct();

  // scene configuration
  sandbox_graphics::GraphicsModel axis_model = CreateAxisModel();
  sandbox_graphics::GraphicsModel lantern_model;
  sandbox_graphics::GraphicsModel box_model;
  sandbox_graphics::ModelLoader::LoadGltfModel(lantern_model, RESOURCE_DIRECTORY + std::string("resources/models/gltf/BoomBox/glTF/BoomBox.gltf"));
  box_model.meshes.push_back(sandbox_graphics::BasicMeshes::Cube());
  sandbox_common::OpenglModel opengl_axis_model;
  sandbox_common::OpenglModel opengl_lantern_model;
  sandbox_common::OpenglModel opengl_box_model;
  sandbox_common::OpenglBinder::BindModel(opengl_axis_model, axis_model);
  sandbox_common::OpenglBinder::BindModel(opengl_lantern_model, lantern_model);
  sandbox_common::OpenglBinder::BindModel(opengl_box_model, box_model);
  sandbox_common::OpenglHdrSkybox skybox;
  sandbox_common::OpenglBinder::BindHdrSkybox(skybox, RESOURCE_DIRECTORY + std::string("resources/skyboxes/subway_lights/20_Subway_Lights_3k.hdr"));

  sandbox_common::Entity axis_ent;
  sandbox_common::Entity skybox_ent;
  sandbox_common::Entity box_ent1;
  sandbox_common::Entity box_ent2;
  sandbox_common::Entity lantern_ent;
  sandbox_common::Entity point_light_ent1;
  sandbox_common::Entity point_light_ent2;
  scene_module->CreateEntity(axis_ent);
  scene_module->CreateEntity(skybox_ent);
  scene_module->CreateEntity(box_ent1);
  scene_module->CreateEntity(box_ent2);
  scene_module->CreateEntity(lantern_ent);
  scene_module->CreateEntity(point_light_ent1);
  scene_module->CreateEntity(point_light_ent2);
  scene_module->RegisterComponent<sandbox_common::OpenglModelComponent>();
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(axis_ent, { opengl_axis_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(box_ent1, { opengl_box_model, sandbox_graphics::Transformation({glm::vec3(0, 0, 5.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.2f, 0.2f, 0.2f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(box_ent2, { opengl_box_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 5.0f, 5.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.2f, 0.2f, 0.2f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(lantern_ent, { opengl_lantern_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(3.14f, 3.14f, 0.0f)), glm::vec3(200.0f, 200.0f, 200.0f)}) });
  scene_module->RegisterComponent<sandbox_graphics::PointLight>();
  scene_module->AddComponent<sandbox_graphics::PointLight>(point_light_ent1, { glm::vec3(0, 0, 5.0f), glm::vec3(40.0f, 40.0f, 40.0f) });
  scene_module->AddComponent<sandbox_graphics::PointLight>(point_light_ent2, { glm::vec3(0, 5.0f, 5.0f), glm::vec3(40.0f, 40.0f, 40.0f) });
  scene_module->RegisterComponent<sandbox_common::OpenglHdrSkybox>();
  scene_module->AddComponent<sandbox_common::OpenglHdrSkybox>(skybox_ent, skybox);

  // wrap configuration
  wrap_module->SetEntity(lantern_ent);

  // renderer configuration
  graphics_module->SetSkyboxUsingMode(true);
  graphics_module->SetSkyboxEntity(skybox_ent);

  // run
  int run_result = engine_module->Run();

  sandbox_common::OpenglBinder::UnbindModel(opengl_axis_model);
  sandbox_common::OpenglBinder::UnbindModel(opengl_lantern_model);
  sandbox_common::OpenglBinder::UnbindModel(opengl_box_model);
  sandbox_common::OpenglBinder::UnbindHdrSkybox(skybox);

  // destructing modules
  engine_module->Destruct();
  interrupt_module->Destruct();
  scene_module->Destruct();
  graphics_module->Destruct();
  wrap_module->Destruct();
  camera_movement_module->Destruct();
  render_constrol_module->Destruct();

  sandbox_platform::PlatformProvider::GetPlatform()->GetPlatformWindow().lock()->CloseWindow();

  return run_result;
}

int GraphicsApplication::RunExample8(int width, int height)
{
  // systems configuration
  ConfigurateSystems(width, height);

  // constructing modules
  auto engine_module = std::make_shared<sandbox_common::SandboxEngine>();
  auto interrupt_module = std::make_shared<InterruptApplicationModule>();
  auto scene_module = std::make_shared<sandbox_common::SceneModule>();
  auto graphics_module = std::make_shared<sandbox_common::OpenglRenderModule>(width, height);
  auto wrap_module = std::make_shared<ModelWrapModule>();
  auto camera_movement_module = std::make_shared<sandbox_common::SandboxCameraMovementModule>();
  auto render_constrol_module = std::make_shared<OpenglRenderControlModule>();

  engine_module->AddChild(scene_module);
  engine_module->AddChild(interrupt_module);
  scene_module->AddChild(graphics_module);
  scene_module->AddChild(wrap_module);
  graphics_module->AddChild(camera_movement_module);
  graphics_module->AddChild(render_constrol_module);

  engine_module->Construct();
  interrupt_module->Construct();
  scene_module->Construct();
  graphics_module->Construct();
  wrap_module->Construct();
  camera_movement_module->Construct();
  render_constrol_module->Construct();

  // scene configuration
  sandbox_graphics::GraphicsModel axis_model = CreateAxisModel();
  sandbox_graphics::GraphicsModel spheres_model;
  sandbox_graphics::GraphicsModel box_model;
  sandbox_graphics::ModelLoader::LoadGltfModel(spheres_model, RESOURCE_DIRECTORY + std::string("resources/models/gltf/MetalRoughSpheres/glTF/MetalRoughSpheres.gltf"));
  box_model.meshes.push_back(sandbox_graphics::BasicMeshes::Cube());
  sandbox_common::OpenglModel opengl_axis_model;
  sandbox_common::OpenglModel opengl_spheres_model;
  sandbox_common::OpenglModel opengl_box_model;
  sandbox_common::OpenglBinder::BindModel(opengl_axis_model, axis_model);
  sandbox_common::OpenglBinder::BindModel(opengl_spheres_model, spheres_model);
  sandbox_common::OpenglBinder::BindModel(opengl_box_model, box_model);
  sandbox_common::OpenglHdrSkybox skybox;
  sandbox_common::OpenglBinder::BindHdrSkybox(skybox, RESOURCE_DIRECTORY + std::string("resources/skyboxes/helipad_afternoon/LA_Downtown_Afternoon_Fishing_3k.hdr"));

  sandbox_common::Entity axes_ent;
  sandbox_common::Entity skybox_ent;
  sandbox_common::Entity box_ent1;
  sandbox_common::Entity box_ent2;
  sandbox_common::Entity spheres_ent;
  sandbox_common::Entity point_light_ent1;
  sandbox_common::Entity point_light_ent2;
  scene_module->CreateEntity(axes_ent);
  scene_module->CreateEntity(skybox_ent);
  scene_module->CreateEntity(box_ent1);
  scene_module->CreateEntity(box_ent2);
  scene_module->CreateEntity(spheres_ent);
  scene_module->CreateEntity(point_light_ent1);
  scene_module->CreateEntity(point_light_ent2);
  scene_module->RegisterComponent<sandbox_common::OpenglModelComponent>();
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(axes_ent, { opengl_axis_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(spheres_ent, { opengl_spheres_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.1f, 0.1f, 0.1f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(box_ent1, { opengl_box_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 5.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.2f, 0.2f, 0.2f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(box_ent2, { opengl_box_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 5.0f, 5.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.2f, 0.2f, 0.2f)}) });
  scene_module->RegisterComponent<sandbox_graphics::PointLight>();
  scene_module->AddComponent<sandbox_graphics::PointLight>(point_light_ent1, { glm::vec3(0, 0, 5.0f), glm::vec3(10.0f, 10.0f, 10.0f) });
  scene_module->AddComponent<sandbox_graphics::PointLight>(point_light_ent2, { glm::vec3(0, 5.0f, 5.0f), glm::vec3(10.0f, 10.0f, 10.0f) });
  scene_module->RegisterComponent<sandbox_common::OpenglHdrSkybox>();
  scene_module->AddComponent<sandbox_common::OpenglHdrSkybox>(skybox_ent, skybox);
  scene_module->RegisterComponent<sandbox_graphics::PointLight>();
  scene_module->AddComponent<sandbox_graphics::PointLight>(point_light_ent1, { glm::vec3(0, 0, 5.0f), glm::vec3(100.0f, 100.0f, 100.0f) });
  scene_module->AddComponent<sandbox_graphics::PointLight>(point_light_ent2, { glm::vec3(0, 5.0f, 5.0f), glm::vec3(100.0f, 100.0f, 100.0f) });
  scene_module->RegisterComponent<sandbox_common::OpenglHdrSkybox>();
  scene_module->AddComponent<sandbox_common::OpenglHdrSkybox>(skybox_ent, skybox);

  // renderer configuration
  graphics_module->SetSkyboxUsingMode(true);
  graphics_module->SetSkyboxEntity(skybox_ent);

  // wrap configuration
  wrap_module->SetEntity(spheres_ent);

  // run
  int run_result = engine_module->Run();

  sandbox_common::OpenglBinder::UnbindModel(opengl_axis_model);
  sandbox_common::OpenglBinder::UnbindModel(opengl_spheres_model);
  sandbox_common::OpenglBinder::UnbindModel(opengl_box_model);
  sandbox_common::OpenglBinder::UnbindHdrSkybox(skybox);

  // destructing modules
  engine_module->Destruct();
  interrupt_module->Destruct();
  scene_module->Destruct();
  graphics_module->Destruct();
  wrap_module->Destruct();
  camera_movement_module->Destruct();
  render_constrol_module->Destruct();

  sandbox_platform::PlatformProvider::GetPlatform()->GetPlatformWindow().lock()->CloseWindow();

  return run_result;
}

int GraphicsApplication::RunExample9(int width, int height)
{
  // systems configuration
  ConfigurateSystems(width, height);

  // constructing modules
  auto engine_module = std::make_shared<sandbox_common::SandboxEngine>();
  auto interrupt_module = std::make_shared<InterruptApplicationModule>();
  auto scene_module = std::make_shared<sandbox_common::SceneModule>();
  auto graphics_module = std::make_shared<sandbox_common::OpenglRenderModule>(width, height);
  auto wrap_module = std::make_shared<ModelWrapModule>();
  auto camera_movement_module = std::make_shared<sandbox_common::SandboxCameraMovementModule>();
  auto render_constrol_module = std::make_shared<OpenglRenderControlModule>();

  engine_module->AddChild(scene_module);
  engine_module->AddChild(interrupt_module);
  scene_module->AddChild(graphics_module);
  scene_module->AddChild(wrap_module);
  graphics_module->AddChild(camera_movement_module);
  graphics_module->AddChild(render_constrol_module);

  engine_module->Construct();
  interrupt_module->Construct();
  scene_module->Construct();
  graphics_module->Construct();
  wrap_module->Construct();
  camera_movement_module->Construct();
  render_constrol_module->Construct();

  // scene configuration
  sandbox_graphics::GraphicsModel axis_model = CreateAxisModel();
  sandbox_graphics::GraphicsModel spheres_model;
  sandbox_graphics::GraphicsModel box_model;
  sandbox_graphics::ModelLoader::LoadGltfModel(spheres_model, RESOURCE_DIRECTORY + std::string("resources/models/gltf/MetalRoughSpheresNoTextures/glTF/MetalRoughSpheresNoTextures.gltf"));
  box_model.meshes.push_back(sandbox_graphics::BasicMeshes::Cube());
  sandbox_common::OpenglModel opengl_axis_model;
  sandbox_common::OpenglModel opengl_spheres_model;
  sandbox_common::OpenglModel opengl_box_model;
  sandbox_common::OpenglBinder::BindModel(opengl_axis_model, axis_model);
  sandbox_common::OpenglBinder::BindModel(opengl_spheres_model, spheres_model);
  sandbox_common::OpenglBinder::BindModel(opengl_box_model, box_model);
  sandbox_common::OpenglHdrSkybox skybox;
  sandbox_common::OpenglBinder::BindHdrSkybox(skybox, RESOURCE_DIRECTORY + std::string("resources/skyboxes/helipad_afternoon/LA_Downtown_Afternoon_Fishing_3k.hdr"));

  sandbox_common::Entity axes_ent;
  sandbox_common::Entity skybox_ent;
  sandbox_common::Entity box_ent1;
  sandbox_common::Entity box_ent2;
  sandbox_common::Entity spheres_ent;
  sandbox_common::Entity point_light_ent1;
  sandbox_common::Entity point_light_ent2;
  scene_module->CreateEntity(axes_ent);
  scene_module->CreateEntity(skybox_ent);
  scene_module->CreateEntity(box_ent1);
  scene_module->CreateEntity(box_ent2);
  scene_module->CreateEntity(spheres_ent);
  scene_module->CreateEntity(point_light_ent1);
  scene_module->CreateEntity(point_light_ent2);
  scene_module->RegisterComponent<sandbox_common::OpenglModelComponent>();
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(axes_ent, { opengl_axis_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(spheres_ent, { opengl_spheres_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(400.0f, 400.0f, 400.0f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(box_ent1, { opengl_box_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 0.0f, 5.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.2f, 0.2f, 0.2f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(box_ent2, { opengl_box_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 5.0f, 5.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.2f, 0.2f, 0.2f)}) });
  scene_module->RegisterComponent<sandbox_graphics::PointLight>();
  scene_module->AddComponent<sandbox_graphics::PointLight>(point_light_ent1, { glm::vec3(0, 0, 5.0f), glm::vec3(10.0f, 10.0f, 10.0f) });
  scene_module->AddComponent<sandbox_graphics::PointLight>(point_light_ent2, { glm::vec3(0, 5.0f, 5.0f), glm::vec3(10.0f, 10.0f, 10.0f) });
  scene_module->RegisterComponent<sandbox_common::OpenglHdrSkybox>();
  scene_module->AddComponent<sandbox_common::OpenglHdrSkybox>(skybox_ent, skybox);

  // renderer configuration
  graphics_module->SetSkyboxUsingMode(true);
  graphics_module->SetSkyboxEntity(skybox_ent);

  // wrap configuration
  wrap_module->SetEntity(spheres_ent);

  // run
  int run_result = engine_module->Run();

  sandbox_common::OpenglBinder::UnbindModel(opengl_axis_model);
  sandbox_common::OpenglBinder::UnbindModel(opengl_spheres_model);
  sandbox_common::OpenglBinder::UnbindModel(opengl_box_model);
  sandbox_common::OpenglBinder::UnbindHdrSkybox(skybox);

  // destructing modules
  engine_module->Destruct();
  interrupt_module->Destruct();
  scene_module->Destruct();
  graphics_module->Destruct();
  wrap_module->Destruct();
  camera_movement_module->Destruct();
  render_constrol_module->Destruct();

  sandbox_platform::PlatformProvider::GetPlatform()->GetPlatformWindow().lock()->CloseWindow();

  return run_result;
}

int GraphicsApplication::RunExample10(int width, int height)
{
  // systems configuration
  ConfigurateSystems(width, height);

  // constructing modules
  auto engine_module = std::make_shared<sandbox_common::SandboxEngine>();
  auto interrupt_module = std::make_shared<InterruptApplicationModule>();
  auto scene_module = std::make_shared<sandbox_common::SceneModule>();
  auto graphics_module = std::make_shared<sandbox_common::OpenglRenderModule>(width, height);
  auto wrap_module = std::make_shared<ModelWrapModule>();
  auto camera_movement_module = std::make_shared<sandbox_common::SandboxCameraMovementModule>();
  auto render_constrol_module = std::make_shared<OpenglRenderControlModule>();

  engine_module->AddChild(scene_module);
  engine_module->AddChild(interrupt_module);
  scene_module->AddChild(graphics_module);
  scene_module->AddChild(wrap_module);
  graphics_module->AddChild(camera_movement_module);
  graphics_module->AddChild(render_constrol_module);

  engine_module->Construct();
  interrupt_module->Construct();
  scene_module->Construct();
  graphics_module->Construct();
  wrap_module->Construct();
  camera_movement_module->Construct();
  render_constrol_module->Construct();

  // scene configuration
  sandbox_graphics::GraphicsModel axis_model = CreateAxisModel();
  sandbox_graphics::GraphicsModel lantern_model;
  sandbox_graphics::GraphicsModel box_model;
  sandbox_graphics::ModelLoader::LoadGltfModel(lantern_model, RESOURCE_DIRECTORY + std::string("resources/models/gltf/SciFiHelmet/glTF/SciFiHelmet.gltf"));
  box_model.meshes.push_back(sandbox_graphics::BasicMeshes::Cube());
  sandbox_common::OpenglModel opengl_axis_model;
  sandbox_common::OpenglModel opengl_lantern_model;
  sandbox_common::OpenglModel opengl_box_model;
  sandbox_common::OpenglBinder::BindModel(opengl_axis_model, axis_model);
  sandbox_common::OpenglBinder::BindModel(opengl_lantern_model, lantern_model);
  sandbox_common::OpenglBinder::BindModel(opengl_box_model, box_model);
  sandbox_common::OpenglHdrSkybox skybox;
  sandbox_common::OpenglBinder::BindHdrSkybox(skybox, RESOURCE_DIRECTORY + std::string("resources/skyboxes/stadium_center/Stadium_Center_3k.hdr"));

  sandbox_common::Entity axis_ent;
  sandbox_common::Entity skybox_ent;
  sandbox_common::Entity box_ent1;
  sandbox_common::Entity box_ent2;
  sandbox_common::Entity lantern_ent;
  sandbox_common::Entity point_light_ent1;
  sandbox_common::Entity point_light_ent2;
  scene_module->CreateEntity(axis_ent);
  scene_module->CreateEntity(skybox_ent);
  scene_module->CreateEntity(box_ent1);
  scene_module->CreateEntity(box_ent2);
  scene_module->CreateEntity(lantern_ent);
  scene_module->CreateEntity(point_light_ent1);
  scene_module->CreateEntity(point_light_ent2);
  scene_module->RegisterComponent<sandbox_common::OpenglModelComponent>();
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(axis_ent, { opengl_axis_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(box_ent1, { opengl_box_model, sandbox_graphics::Transformation({glm::vec3(0, 0, 5.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.2f, 0.2f, 0.2f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(box_ent2, { opengl_box_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 5.0f, 5.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.2f, 0.2f, 0.2f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(lantern_ent, { opengl_lantern_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f)}) });
  scene_module->RegisterComponent<sandbox_graphics::PointLight>();
  scene_module->AddComponent<sandbox_graphics::PointLight>(point_light_ent1, { glm::vec3(0, 0, 5.0f), glm::vec3(40.0f, 40.0f, 40.0f) });
  scene_module->AddComponent<sandbox_graphics::PointLight>(point_light_ent2, { glm::vec3(0, 5.0f, 5.0f), glm::vec3(40.0f, 40.0f, 40.0f) });
  scene_module->RegisterComponent<sandbox_common::OpenglHdrSkybox>();
  scene_module->AddComponent<sandbox_common::OpenglHdrSkybox>(skybox_ent, skybox);

  // wrap configuration
  wrap_module->SetEntity(lantern_ent);

  // renderer configuration
  graphics_module->SetSkyboxUsingMode(true);
  graphics_module->SetSkyboxEntity(skybox_ent);

  // run
  int run_result = engine_module->Run();

  sandbox_common::OpenglBinder::UnbindModel(opengl_axis_model);
  sandbox_common::OpenglBinder::UnbindModel(opengl_lantern_model);
  sandbox_common::OpenglBinder::UnbindModel(opengl_box_model);
  sandbox_common::OpenglBinder::UnbindHdrSkybox(skybox);

  // destructing modules
  engine_module->Destruct();
  interrupt_module->Destruct();
  scene_module->Destruct();
  graphics_module->Destruct();
  wrap_module->Destruct();
  camera_movement_module->Destruct();
  render_constrol_module->Destruct();

  sandbox_platform::PlatformProvider::GetPlatform()->GetPlatformWindow().lock()->CloseWindow();

  return run_result;
}

int GraphicsApplication::RunExample11(int width, int height)
{
  // systems configuration
  ConfigurateSystems(width, height);

  // constructing modules
  auto engine_module = std::make_shared<sandbox_common::SandboxEngine>();
  auto interrupt_module = std::make_shared<InterruptApplicationModule>();
  auto scene_module = std::make_shared<sandbox_common::SceneModule>();
  auto graphics_module = std::make_shared<sandbox_common::OpenglRenderModule>(width, height);
  auto wrap_module = std::make_shared<ModelWrapModule>();
  auto camera_movement_module = std::make_shared<sandbox_common::SandboxCameraMovementModule>();
  auto render_constrol_module = std::make_shared<OpenglRenderControlModule>();

  engine_module->AddChild(scene_module);
  engine_module->AddChild(interrupt_module);
  scene_module->AddChild(graphics_module);
  scene_module->AddChild(wrap_module);
  graphics_module->AddChild(camera_movement_module);
  graphics_module->AddChild(render_constrol_module);

  engine_module->Construct();
  interrupt_module->Construct();
  scene_module->Construct();
  graphics_module->Construct();
  wrap_module->Construct();
  camera_movement_module->Construct();
  render_constrol_module->Construct();

  // scene configuration
  sandbox_graphics::GraphicsModel axis_model = CreateAxisModel();
  sandbox_graphics::GraphicsModel lantern_model;
  sandbox_graphics::GraphicsModel box_model;
  sandbox_graphics::ModelLoader::LoadGltfModel(lantern_model, RESOURCE_DIRECTORY + std::string("resources/models/bathroom_cube/scene.gltf"));
  box_model.meshes.push_back(sandbox_graphics::BasicMeshes::Cube());
  sandbox_common::OpenglModel opengl_axis_model;
  sandbox_common::OpenglModel opengl_lantern_model;
  sandbox_common::OpenglModel opengl_box_model;
  sandbox_common::OpenglBinder::BindModel(opengl_axis_model, axis_model);
  sandbox_common::OpenglBinder::BindModel(opengl_lantern_model, lantern_model);
  sandbox_common::OpenglBinder::BindModel(opengl_box_model, box_model);
  sandbox_common::OpenglHdrSkybox skybox;
  sandbox_common::OpenglBinder::BindHdrSkybox(skybox, RESOURCE_DIRECTORY + std::string("resources/skyboxes/stadium_center/Stadium_Center_3k.hdr"));

  sandbox_common::Entity axis_ent;
  sandbox_common::Entity skybox_ent;
  sandbox_common::Entity box_ent1;
  sandbox_common::Entity box_ent2;
  sandbox_common::Entity lantern_ent;
  sandbox_common::Entity point_light_ent1;
  sandbox_common::Entity point_light_ent2;
  scene_module->CreateEntity(axis_ent);
  scene_module->CreateEntity(skybox_ent);
  scene_module->CreateEntity(box_ent1);
  scene_module->CreateEntity(box_ent2);
  scene_module->CreateEntity(lantern_ent);
  scene_module->CreateEntity(point_light_ent1);
  scene_module->CreateEntity(point_light_ent2);
  scene_module->RegisterComponent<sandbox_common::OpenglModelComponent>();
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(axis_ent, { opengl_axis_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(box_ent1, { opengl_box_model, sandbox_graphics::Transformation({glm::vec3(0, 0, 5.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.2f, 0.2f, 0.2f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(box_ent2, { opengl_box_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 5.0f, 5.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.2f, 0.2f, 0.2f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(lantern_ent, { opengl_lantern_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.01f, 0.01f, 0.01f)}) });
  scene_module->RegisterComponent<sandbox_graphics::PointLight>();
  scene_module->AddComponent<sandbox_graphics::PointLight>(point_light_ent1, { glm::vec3(0, 0, 5.0f), glm::vec3(40.0f, 40.0f, 40.0f) });
  scene_module->AddComponent<sandbox_graphics::PointLight>(point_light_ent2, { glm::vec3(0, 5.0f, 5.0f), glm::vec3(40.0f, 40.0f, 40.0f) });
  scene_module->RegisterComponent<sandbox_common::OpenglHdrSkybox>();
  scene_module->AddComponent<sandbox_common::OpenglHdrSkybox>(skybox_ent, skybox);

  // wrap configuration
  wrap_module->SetEntity(lantern_ent);

  // renderer configuration
  graphics_module->SetSkyboxUsingMode(true);
  graphics_module->SetSkyboxEntity(skybox_ent);

  // run
  int run_result = engine_module->Run();

  sandbox_common::OpenglBinder::UnbindModel(opengl_axis_model);
  sandbox_common::OpenglBinder::UnbindModel(opengl_lantern_model);
  sandbox_common::OpenglBinder::UnbindModel(opengl_box_model);
  sandbox_common::OpenglBinder::UnbindHdrSkybox(skybox);

  // destructing modules
  engine_module->Destruct();
  interrupt_module->Destruct();
  scene_module->Destruct();
  graphics_module->Destruct();
  wrap_module->Destruct();
  camera_movement_module->Destruct();
  render_constrol_module->Destruct();

  sandbox_platform::PlatformProvider::GetPlatform()->GetPlatformWindow().lock()->CloseWindow();

  return run_result;
}

int GraphicsApplication::RunExample12(int width, int height)
{
  // systems configuration
  ConfigurateSystems(width, height);

  // constructing modules
  auto engine_module = std::make_shared<sandbox_common::SandboxEngine>();
  auto interrupt_module = std::make_shared<InterruptApplicationModule>();
  auto scene_module = std::make_shared<sandbox_common::SceneModule>();
  auto graphics_module = std::make_shared<sandbox_common::OpenglRenderModule>(width, height);
  auto wrap_module = std::make_shared<ModelWrapModule>();
  auto camera_movement_module = std::make_shared<sandbox_common::SandboxCameraMovementModule>();
  auto render_constrol_module = std::make_shared<OpenglRenderControlModule>();

  engine_module->AddChild(scene_module);
  engine_module->AddChild(interrupt_module);
  scene_module->AddChild(graphics_module);
  scene_module->AddChild(wrap_module);
  graphics_module->AddChild(camera_movement_module);
  graphics_module->AddChild(render_constrol_module);

  engine_module->Construct();
  interrupt_module->Construct();
  scene_module->Construct();
  graphics_module->Construct();
  wrap_module->Construct();
  camera_movement_module->Construct();
  render_constrol_module->Construct();

  // scene configuration
  sandbox_graphics::GraphicsModel axis_model = CreateAxisModel();
  sandbox_graphics::GraphicsModel lantern_model;
  sandbox_graphics::GraphicsModel box_model;
  sandbox_graphics::ModelLoader::LoadGltfModel(lantern_model, RESOURCE_DIRECTORY + std::string("resources/models/orb/scene.gltf"));
  box_model.meshes.push_back(sandbox_graphics::BasicMeshes::Cube());
  sandbox_common::OpenglModel opengl_axis_model;
  sandbox_common::OpenglModel opengl_lantern_model;
  sandbox_common::OpenglModel opengl_box_model;
  sandbox_common::OpenglBinder::BindModel(opengl_axis_model, axis_model);
  sandbox_common::OpenglBinder::BindModel(opengl_lantern_model, lantern_model);
  sandbox_common::OpenglBinder::BindModel(opengl_box_model, box_model);
  sandbox_common::OpenglHdrSkybox skybox;
  sandbox_common::OpenglBinder::BindHdrSkybox(skybox, RESOURCE_DIRECTORY + std::string("resources/skyboxes/stadium_center/Stadium_Center_3k.hdr"));

  sandbox_common::Entity axis_ent;
  sandbox_common::Entity skybox_ent;
  sandbox_common::Entity box_ent1;
  sandbox_common::Entity box_ent2;
  sandbox_common::Entity lantern_ent;
  sandbox_common::Entity point_light_ent1;
  sandbox_common::Entity point_light_ent2;
  scene_module->CreateEntity(axis_ent);
  scene_module->CreateEntity(skybox_ent);
  scene_module->CreateEntity(box_ent1);
  scene_module->CreateEntity(box_ent2);
  scene_module->CreateEntity(lantern_ent);
  scene_module->CreateEntity(point_light_ent1);
  scene_module->CreateEntity(point_light_ent2);
  scene_module->RegisterComponent<sandbox_common::OpenglModelComponent>();
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(axis_ent, { opengl_axis_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(box_ent1, { opengl_box_model, sandbox_graphics::Transformation({glm::vec3(0, 0, 5.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.2f, 0.2f, 0.2f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(box_ent2, { opengl_box_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 5.0f, 5.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.2f, 0.2f, 0.2f)}) });
  scene_module->AddComponent<sandbox_common::OpenglModelComponent>(lantern_ent, { opengl_lantern_model, sandbox_graphics::Transformation({glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f)}) });
  scene_module->RegisterComponent<sandbox_graphics::PointLight>();
  scene_module->AddComponent<sandbox_graphics::PointLight>(point_light_ent1, { glm::vec3(0, 0, 5.0f), glm::vec3(40.0f, 40.0f, 40.0f) });
  scene_module->AddComponent<sandbox_graphics::PointLight>(point_light_ent2, { glm::vec3(0, 5.0f, 5.0f), glm::vec3(40.0f, 40.0f, 40.0f) });
  scene_module->RegisterComponent<sandbox_common::OpenglHdrSkybox>();
  scene_module->AddComponent<sandbox_common::OpenglHdrSkybox>(skybox_ent, skybox);

  // wrap configuration
  wrap_module->SetEntity(lantern_ent);

  // renderer configuration
  graphics_module->SetSkyboxUsingMode(true);
  graphics_module->SetSkyboxEntity(skybox_ent);

  // run
  int run_result = engine_module->Run();

  sandbox_common::OpenglBinder::UnbindModel(opengl_axis_model);
  sandbox_common::OpenglBinder::UnbindModel(opengl_lantern_model);
  sandbox_common::OpenglBinder::UnbindModel(opengl_box_model);
  sandbox_common::OpenglBinder::UnbindHdrSkybox(skybox);

  // destructing modules
  engine_module->Destruct();
  interrupt_module->Destruct();
  scene_module->Destruct();
  graphics_module->Destruct();
  wrap_module->Destruct();
  camera_movement_module->Destruct();
  render_constrol_module->Destruct();

  sandbox_platform::PlatformProvider::GetPlatform()->GetPlatformWindow().lock()->CloseWindow();

  return run_result;
}

void GraphicsApplication::ConfigurateSystems(int width, int height)
{
  auto platform = sandbox_platform::PlatformProvider::GetPlatform();

  auto glfw_window = std::static_pointer_cast<sandbox_platform::GlfwWindow>(platform->GetPlatformWindow().lock());
  glfw_window->Init(width, height, "Sandbox Example");
  glfw_window->MakeCurrent();

  auto glfw_input = std::static_pointer_cast<sandbox_platform::GlfwInput>(platform->GetPlatformInput().lock());
  glfw_input->SetAllInputHandlers(glfw_window);
}

sandbox_graphics::GraphicsModel GraphicsApplication::CreateAxisModel()
{
  sandbox_graphics::GraphicsModel model;
  model.meshes.push_back(sandbox_graphics::BasicMeshes::Axes());

  return model;
}