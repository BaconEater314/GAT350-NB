using namespace neu;

int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();
    
    SDL_Event e;
    bool quit = false;

    //Initialize scene
    auto scene = std::make_unique<Scene>();
    scene->Load("Scenes/scene01.json");
    scene->Start();

    //Initialize editor
    auto editor = std::make_unique<Editor>();

    // MAIN LOOP
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
            ImGui_ImplSDL3_ProcessEvent(&e);
        }

        // update
        neu::GetEngine().Update();
        float dt = neu::GetEngine().GetTime().GetDeltaTime();
        if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;
       
        // scene
        scene->Update(dt);

        // editor
        editor->Begin();
        editor->UpdateGui(*scene);

        // draw
        neu::GetEngine().GetRenderer().Clear();
        scene->Draw(GetEngine().GetRenderer());
        // draw ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        neu::GetEngine().GetRenderer().Present();
    }

    neu::GetEngine().Shutdown();

    return 0;
}