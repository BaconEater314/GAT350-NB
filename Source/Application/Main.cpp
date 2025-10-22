using namespace neu;

int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();

    
    SDL_Event e;
    bool quit = false;

    struct Vertex {
        vec3 position;
        vec3 color;
        vec2 texcoord;
    };

    std::vector<Vertex> vertices{
        { { -0.5f, -0.5f, 0 }, { 1, 0, 0 }, {0, 0 } },
        { { -0.5, 0.5f, 0 }, { 0, 1, 0 }, { 0, 1 } },
        { { 1, 1, 0 }, { 0, 0, 1 }, { 1, 1 } },
        { { 0.5, -0.5f, 0 }, { 0, 1, 1 }, { 1, 0 } }
    };

    std::vector<GLushort> indices{ 0, 1, 2, 2, 3, 0 };

    //vertex buffer
    auto model3d = std::make_shared<Model>();
    model3d->Load("Models/spot.obj");

    //vertex and fragment shaders
    auto vs = neu::Resources().Get<neu::Shader>("shaders/basic_lit.vert", GL_VERTEX_SHADER);
    auto fs = neu::Resources().Get<neu::Shader>("shaders/basic_lit.frag", GL_FRAGMENT_SHADER);

    auto program = std::make_shared<neu::Program>();
    program->AttachShader(vs);
    program->AttachShader(fs);
    program->Link();
    program->Use();

    //texture
    res_t<Texture> texture = Resources().Get<Texture>("Textures/spot_diffuse.png");
    program->SetUniform("u_texture", 0);

    //lights
    program->SetUniform("u_ambient_light", glm::vec3{ 0.5f });
    Transform light{ {2, 4, 3} };

    //transform
    float rotation = 0;
    glm::vec3 eye{ 0, 0, 1 };

    Transform transform{ {1, 0, 0} };
    Transform camera{ {0, 0, 3} };

    //program


    // MAIN LOOP
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        // update
        neu::GetEngine().Update();
        if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;

        rotation += GetEngine().GetTime().GetDeltaTime() * 90;

        //model matrix
        //glm::mat4 model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        //model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        //model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        //transform.rotation.y += rotation;
        program->SetUniform("u_model", transform.GetMatrix());

        //view matrix
        float dt = neu::GetEngine().GetTime().GetDeltaTime();
        transform.rotation.y += 90 * dt;
       
        float speed = 10.0f;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_A)) camera.position.x -= speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_D)) camera.position.x += speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_W)) camera.position.z -= speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_S)) camera.position.z += speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_LSHIFT)) camera.position.y -= speed * dt;
        //if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_Q)) camera.position.y -= speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_SPACE)) camera.position.y += speed * dt;

        glm::mat4 view = glm::lookAt(camera.position, camera.position + glm::vec3{ 0, 0, -1 }, glm::vec3{0, 1, 0});
        program->SetUniform("u_view", view);

        program->SetUniform("u_light.color", glm::vec3{1, 1, 1});
        program->SetUniform("u_light.position", (glm::vec3)(view * glm::vec4(light.position, 1)));

        //projection matrix
        float aspect = (float)GetEngine().GetRenderer().GetWidth() / GetEngine().GetRenderer().GetHeight();
        glm::mat4 projection = glm::perspective(glm::radians(90.0f), aspect, 0.01f, 100.0f);
        program->SetUniform("u_projection", projection);

        // draw
        neu::GetEngine().GetRenderer().Clear();

        model3d->Draw(GL_TRIANGLES);

        neu::GetEngine().GetRenderer().Present();
    }

    neu::GetEngine().Shutdown();

    return 0;
}
