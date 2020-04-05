#include "TentGui.h"

#include "GlObject.h"
#include "Light.h"
#include "FrameBuffer.h"
#include "SceneLoader.h"

//#include "imgui/imfilebrowser.h"

#include <vector>

const int TAG_LENGTH = 32;

static void ShowExampleMenuFile()
{
    static bool isFileBrowserOpen = false;

    ImGui::MenuItem("(dummy menu)", NULL, false, false);
    if (ImGui::MenuItem("New"))
    {
        // Prompt user if they want to save file first before creating new scene
    }

    //ImGui::FileBrowser fileDialog;
    //fileDialog.SetTitle("Choose Scene File");
    //fileDialog.SetTypeFilters({".json"});
    //if (ImGui::MenuItem("Open Scene", "Ctrl+O"))
    //{
    //    fileDialog.Open();
    //}

    //fileDialog.Display();
    //if (fileDialog.HasSelected())
    //{
    //    std::cout << "Selected filename: " << fileDialog.GetSelected().string() << '\n';
    //    fileDialog.ClearSelected();
    //}

    if (ImGui::BeginMenu("Open Recent"))
    {
        // TODO
        ImGui::MenuItem("fish_hat.c");
        ImGui::MenuItem("fish_hat.inl");
        ImGui::MenuItem("fish_hat.h");
        if (ImGui::BeginMenu("More.."))
        {
            ImGui::MenuItem("Hello");
            ImGui::MenuItem("Sailor");
            if (ImGui::BeginMenu("Recurse.."))
            {
                ShowExampleMenuFile();
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
    if (ImGui::MenuItem("Save", "Ctrl+S")) {}
    if (ImGui::MenuItem("Save As..")) {}
    ImGui::Separator();
    if (ImGui::BeginMenu("Options"))
    {
        static bool enabled = true;
        ImGui::MenuItem("Enabled", "", &enabled);
        ImGui::BeginChild("child", ImVec2(0, 60), true);
        for (int i = 0; i < 10; i++)
            ImGui::Text("Scrolling Text %d", i);
        ImGui::EndChild();
        static float f = 0.5f;
        static int n = 0;
        static bool b = true;
        ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
        ImGui::InputFloat("Input", &f, 0.1f);
        ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
        ImGui::Checkbox("Check", &b);
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Colors"))
    {
        float sz = ImGui::GetTextLineHeight();
        for (int i = 0; i < ImGuiCol_COUNT; i++)
        {
            const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
            ImVec2 p = ImGui::GetCursorScreenPos();
            ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x+sz, p.y+sz), ImGui::GetColorU32((ImGuiCol)i));
            ImGui::Dummy(ImVec2(sz, sz));
            ImGui::SameLine();
            ImGui::MenuItem(name);
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Disabled", false)) // Disabled
    {
        IM_ASSERT(0);
    }
    if (ImGui::MenuItem("Checked", NULL, true)) {}
    if (ImGui::MenuItem("Quit", "Alt+F4")) {}
}

void TentGui::ShowMainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ShowExampleMenuFile();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Examples"))
        {
            //ImGui::MenuItem("Console", NULL, &show_app_console);
            //ImGui::MenuItem("Log", NULL, &show_app_log);
            //ImGui::MenuItem("Simple layout", NULL, &show_app_layout);
            //ImGui::MenuItem("Property editor", NULL, &show_app_property_editor);
            //ImGui::MenuItem("Long text display", NULL, &show_app_long_text);
            //ImGui::MenuItem("Auto-resizing window", NULL, &show_app_auto_resize);
            //ImGui::MenuItem("Constrained-resizing window", NULL, &show_app_constrained_resize);
            //ImGui::MenuItem("Simple overlay", NULL, &show_app_simple_overlay);
            //ImGui::MenuItem("Manipulating window titles", NULL, &show_app_window_titles);
            //ImGui::MenuItem("Custom rendering", NULL, &show_app_custom_rendering);
            //ImGui::MenuItem("Dockspace", NULL, &show_app_dockspace);
            //ImGui::MenuItem("Documents", NULL, &show_app_documents);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tools"))
        {
            ImGui::MenuItem("Metrics", NULL, &show_app_metrics);
            ImGui::MenuItem("Style Editor", NULL, &show_app_style_editor);
            ImGui::MenuItem("About Dear ImGui", NULL, &show_app_about);
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void TentGui::Init(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
    // Setup Dear ImGui style;
    ImGui::StyleColorsDark();

    // Configuration flags
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void TentGui::RenderStateButtons(GameController& gc)
{
    ImGui::Begin("Game State");
    static int state = static_cast<int>(gc.state); // State in stop state (scene mode)
    ImGui::RadioButton("Play",  &state, 0); ImGui::SameLine();
    ImGui::RadioButton("Stop",  &state, 1); ImGui::SameLine();
    ImGui::RadioButton("Pause", &state, 2);
    gc.state = static_cast<GameState>(state);
    ImGui::End();
}

void TentGui::RenderGUI(ObjectManager& manager)
{
    ShowMainMenuBar();
    ShowSceneHierarchy(manager);
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void TentGui::ShowRenderPasses(const std::vector<FrameBuffer>& renderPasses)
{
    ImGui::Begin("Render Passes Outputs");
    if (ImGui::TreeNode("Render Passes"))
    {
        for (int i = 0; i < renderPasses.size(); ++i)
        {
            ImGui::SetNextItemOpen(true, ImGuiCond_Once);
            if (ImGui::TreeNode((void*)(intptr_t)i, "%s", renderPasses[i].name))
            {
                Texture objectTex = renderPasses[i].texture;
                //float aspectRatio = (float)objectTex.width/objectTex.height;
                //if (ImGui::GetWindowWidth() > ImGui::GetWindowHeight())
                //{
                //    // Scale texture with window
                //    ImGui::Image((void*)(intptr_t)objectTex.ID, ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowWidth()/aspectRatio), ImVec2(0,1), ImVec2(1,0));
                //}
                //else
                //{
                //    ImGui::Image((void*)(intptr_t)objectTex.ID, ImVec2(ImGui::GetWindowHeight()*aspectRatio, ImGui::GetWindowHeight()), ImVec2(0,1), ImVec2(1,0));
                //}

                ImGui::Image((void*)(intptr_t)objectTex.ID, ImVec2((float)1600/4, (float)900/4), ImVec2(0,1), ImVec2(1,0));

                ImGui::Separator();
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }
    ImGui::End();
}


void TentGui::ShowCamera(Camera& cam)
{
    ImGui::Begin("Camera");

    float position[3] = {
        cam.Position.x,
        cam.Position.y,
        cam.Position.z
    };
    ImGui::DragFloat3("Position", position, 0.01f);
    ImGui::DragFloat("Near Plane", &cam.nearPlane, 0.01f);
    ImGui::DragFloat("Far Plane" , &cam.farPlane , 0.01f);

    ImGui::Separator();

    ImGui::End();
}


void TentGui::ShowPrimitiveGenerator(ObjectManager& manager)
{
    static int objectIdx = -1;
    const char* objects[] = {"Cube", "Quad", "Sphere", "Light"};
    static Geometry selectedGeom = NONE;

    ImGui::Begin("Generate Primitive");

    ImGui::Combo("Geometry", &objectIdx, objects, IM_ARRAYSIZE(objects));
    ImGui::SameLine(); HelpMarker("Select a geometry to spawn into the scene.\n");
    selectedGeom = static_cast<Geometry>(objectIdx);

    static char tag[TAG_LENGTH] = "";
    ImGui::InputTextWithHint("Tag", "Enter object tag here", tag, IM_ARRAYSIZE(tag));

    static float position[3] = { 0.0f, 0.0f, 0.0f };
    ImGui::DragFloat3("Position", position, 0.01f);
    ImGui::Spacing();

    static float rotation[3] = { 0.0f, 0.0f, 0.0f };
    ImGui::DragFloat3("Rotation", rotation, 0.01f);
    ImGui::Spacing();

    static float scale[3] = { 1.0f, 1.0f, 1.0f };
    ImGui::DragFloat3("Scale", scale, 0.01f);
    ImGui::Spacing();

    std::string tagString(tag);
    if (ImGui::Button("Generate"))
    {
        manager.LoadObject(selectedGeom, tagString, position, rotation, scale);
    }

    ImGui::End();
}


void TentGui::ShowObjects(ObjectManager& manager)
{
    static int selected = -1;

    ImGui::Text("Scene Objects");
    // TODO
    // Note: the number 105 was just chosen by eye
    // Eventually figure out a way that takes into account the button's width
    ImGui::SameLine(ImGui::GetWindowWidth()-105);
    if (ImGui::Button("Remove Object") && selected != -1)
    {
        manager.RemoveObject(selected);
        selected = -1;
    }

    ImGui::Separator();

    static ImGuiTextFilter filter;
    const char* filterHelp(
                "Filter usage:\n"
                "  \"\"         display all lines\n"
                "  \"xxx\"      display lines containing \"xxx\"\n"
                "  \"xxx,yyy\"  display lines containing \"xxx\" or \"yyy\"\n"
                "  \"-xxx\"     hide lines containing \"xxx\""
            );

    HelpMarker(filterHelp);
    ImGui::SameLine(); filter.Draw();

    ImGui::Separator();

    // TODO light inspector breaks when
    // no longer using i for index
    int i = 0;
    for (auto object : manager.objectList)
    {
        if (filter.PassFilter(object->name.c_str()))
        {
            char tag[TAG_LENGTH];
            //sprintf(tag, "Idx:%d Tag:%s", i, object->name.c_str());
            sprintf(tag, "%s", object->name.c_str());
            if (ImGui::Selectable(tag, selected == i))
            {
                selected = i;
            }
        }
        ++i;
    }

    if (selected != -1)
    {
        ShowInspector(manager.objectList[selected]);
    }
}

void TentGui::ShowMetrics(double frameTime)
{
    if (!show_app_metrics) { return; }

    ImGui::Begin("Metrics");
    static bool animate = true;
    ImGui::Checkbox("Animate", &animate);

    static float values[90] = {};
    static int values_offset = 0;
    static double refresh_time = 0.0;
    if (!animate || refresh_time == 0.0)
        refresh_time = ImGui::GetTime();

    while (refresh_time < ImGui::GetTime()) // Create dummy data at fixed 60 hz rate for the demo
    {
        values[values_offset] = frameTime*1000; // convert to milliseconds;
        values_offset = (values_offset+1) % IM_ARRAYSIZE(values);
        refresh_time += 1.0f/60.0f;
    }

    // Display Frame window
    {
        float average = 0.0f;
        for (int n = 0; n < IM_ARRAYSIZE(values); n++)
            average += values[n];
        average /= (float)IM_ARRAYSIZE(values);

        // Convert average frameTime to FPS
        ImGui::Text("Average FPS: %lf", 1.0/(average/1000.0f));
        char overlay[32];
        sprintf(overlay, "avg: %f ms", average);
        // TODO make the time scale adjustable? Either scale with the average or allow user to configure
        ImGui::PlotLines("Frame times", values, IM_ARRAYSIZE(values), values_offset, overlay, 0.0f, 20.0f, ImVec2(0,80));
    }
    ImGui::End();
}

void EditTransform(GlObject* object, const float *cameraView, float *cameraProjection, float* matrix)
{
    static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
    static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
    static bool useSnap = false;
    static float snap[3] = { 1.f, 1.f, 1.f };
    static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
    static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
    static bool boundSizing = false;
    static bool boundSizingSnap = false;

    // Create transform window in inspector
    ImGui::Begin("Inspector");

//    if (ImGui::IsKeyPressed(90))
//        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
//    if (ImGui::IsKeyPressed(69))
//        mCurrentGizmoOperation = ImGuizmo::ROTATE;
//    if (ImGui::IsKeyPressed(82)) // r Key
//        mCurrentGizmoOperation = ImGuizmo::SCALE;
    if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
        mCurrentGizmoOperation = ImGuizmo::SCALE;
    float matrixTranslation[3], matrixRotation[3], matrixScale[3];
    ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
    ImGui::InputFloat3("Tr", matrixTranslation, 3);
    ImGui::InputFloat3("Rt", matrixRotation, 3);
    ImGui::InputFloat3("Sc", matrixScale, 3);
    ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

    if (mCurrentGizmoOperation != ImGuizmo::SCALE)
    {
        if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
            mCurrentGizmoMode = ImGuizmo::LOCAL;
        ImGui::SameLine();
        if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
            mCurrentGizmoMode = ImGuizmo::WORLD;
    }
//    if (ImGui::IsKeyPressed(83))
//        useSnap = !useSnap;
    // TODO broken
    ImGui::Checkbox("", &useSnap);
    ImGui::SameLine();

    switch (mCurrentGizmoOperation)
    {
        case ImGuizmo::TRANSLATE:
            ImGui::InputFloat3("Snap", &snap[0]);
            break;
        case ImGuizmo::ROTATE:
            ImGui::InputFloat("Angle Snap", &snap[0]);
            break;
        case ImGuizmo::SCALE:
            ImGui::InputFloat("Scale Snap", &snap[0]);
            break;
    }
    ImGui::Checkbox("Bound Sizing", &boundSizing);
    if (boundSizing)
    {
        ImGui::PushID(3);
        ImGui::Checkbox("", &boundSizingSnap);
        ImGui::SameLine();
        ImGui::InputFloat3("Snap", boundsSnap);
        ImGui::PopID();
    }

    ImGuiIO& io = ImGui::GetIO();
    ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
    ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL, boundSizing?bounds:NULL, boundSizingSnap?boundsSnap:NULL);

    ImGui::Separator();
    ImGui::End();

    // Update GlObject transform
    ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
    object->position = glm::make_vec3(matrixTranslation);
    object->rotation = glm::make_vec3(matrixRotation);
    object->scale    = glm::make_vec3(matrixScale);
}


void TentGui::ShowInspector(GlObject* object)
{
    // TODO recompose model matrix to draw
    //object->model = glm::make_mat4x4(mat);

    // Show and be able to modify information on selected object
    ImGui::Begin("Inspector");

    ImGui::Checkbox("", &object->isActive);
    ImGui::SameLine();
    // TODO: cleaner way of doing this somehow?
    char tag[TAG_LENGTH];
    strcpy(tag, object->name.c_str());
    ImGui::InputText("Tag", tag, IM_ARRAYSIZE(tag));
    object->name.assign(tag);

    ImGui::Separator();

    { // Transform Info
        float* matrix = const_cast<float*>(glm::value_ptr(object->GetModelMatrix()));
        float* view = const_cast<float*>(glm::value_ptr(activeCamera->GetViewMatrix()));
        float* proj = const_cast<float*>(glm::value_ptr(activeCamera->GetProjMatrix(1600, 900)));

        EditTransform(object, view, proj, matrix);
    }

    if (object->isLight)
    { // Attenuation factors
        Light* light = static_cast<Light*>(object);
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::TreeNode("Light Settings"))
        {
            float linear = light->linear;
            float quadratic = light->quadratic;
            float* color = glm::value_ptr(light->color);

            ImGui::ColorEdit4("Light Color", color);
            ImGui::DragFloat("Linear", &linear, 0.01f);
            ImGui::DragFloat("Quadratic", &quadratic, 0.01f);

            light->color = glm::make_vec4(color);
            light->linear = linear;
            light->quadratic = quadratic;

            ImGui::TreePop();
        }
    }
    else
    { // Mesh details
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::TreeNode("Mesh Details"))
        {
            Texture objectTex = object->texture;
            static char tex[64];
            strcpy(tex, objectTex.GetName().c_str());
            ImGui::InputText("Texture", tex, IM_ARRAYSIZE(tag));

            // TODO allow loading up new texture and shader
            //if (strcmp(tex, objectTex.GetName().c_str()) != 0)
            //{
            //    std::string texPath;
            //    texPath.assign(tex);
            //    Texture newTexture(tex);
            //    object->texture = newTexture;
            //}

            ImGui::Text("Dim: %dx%d", objectTex.width, objectTex.height);

            // TODO
//            float aspectRatio = (float)objectTex.width/objectTex.height;
//            if (ImGui::GetWindowWidth() > ImGui::GetWindowHeight())
//            {
//                ImGui::Image((void*)(intptr_t)objectTex.ID, ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowWidth()/aspectRatio), ImVec2(0,1), ImVec2(1,0));
//
//            }
//            else
//            {
//                ImGui::Image((void*)(intptr_t)objectTex.ID, ImVec2(ImGui::GetWindowHeight()*aspectRatio, ImGui::GetWindowHeight()), ImVec2(0,1), ImVec2(1,0));
//            }
            ImGui::Image((void*)(intptr_t)objectTex.ID, ImVec2(128, 128), ImVec2(0,1), ImVec2(1,0));

            ImGui::TreePop();
        }
        ImGui::Separator();
    }

    { // TODO what other details to add?

    }

    ImGui::End();
}

ImVec2 AspectRatioResize()
{
    // TODO
    return ImVec2(0, 0);
}

void TentGui::ShowSceneHierarchy(ObjectManager& manager)
{
    ImGui::Begin("Scene Hierarchy");

    ShowPrimitiveGenerator(manager);

    ImGui::Separator();

    ShowObjects(manager);

    ImGui::End();
}


