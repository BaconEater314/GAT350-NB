using namespace neu;

int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();

    // initialize scene


    SDL_Event e;
    bool quit = false;

    std::vector<vec3> points{ { -0.5f, -0.5f, 0 }, { 0, 0.5f, 0 }, { 0.5f, -0.5f, 0 } }; 
    std::vector<vec3> colors{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };
    std::vector<vec2> textcoord{ {0, 0 } , {0.5f, 1.0f },{ 1, 1 } };

    GLuint vbo[3];
    glGenBuffers(1, vbo);

    //vertext buffer (position)
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * points.size(), points.data(), GL_STATIC_DRAW);

    //vertext buffer (color)
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * colors.size(), colors.data(), GL_STATIC_DRAW);

    //vertext buffer (textcoord)
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * textcoord.size(), textcoord.data(), GL_STATIC_DRAW);

    //vertex array
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //position
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    //color
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    //textcoord
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    //vertex shader
    std::string vs_source;
    file::ReadTextFile("shaders/basic.vert", vs_source);
    const char* vs_cstr = vs_source.c_str();

    GLuint vs;
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_cstr, NULL);
    glCompileShader(vs);

    int success;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::string infoLog(512, '\0');  // pre-allocate space
        GLsizei length;
        glGetShaderInfoLog(vs, (GLsizei)infoLog.size(), &length, &infoLog[0]);
        infoLog.resize(length);

        LOG_WARNING("Shader compilation failed: {}", infoLog);
    }

    //fragment shader
    std::string fs_source;
    file::ReadTextFile("shaders/basic.frag", fs_source);
    const char* fs_cstr = fs_source.c_str();

    GLuint fs;
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_cstr, NULL);
    glCompileShader(fs);

    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::string infoLog(512, '\0');  // pre-allocate space
        GLsizei length;
        glGetShaderInfoLog(fs, (GLsizei)infoLog.size(), &length, &infoLog[0]);
        infoLog.resize(length);

        LOG_WARNING("Shader compilation failed: {}", infoLog);
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);


    glUseProgram(shaderProgram);

    //texture
    res_t<Texture> texture = Resources().Get<Texture>("textures/beast.png");

    //uniform
    GLint uniform = glGetUniformLocation(shaderProgram, "u_time");
    ASSERT(uniform != -1);
    ASSERT_MSG(uniform != -1, "Could not find uniform u_time.");

    GLint tex_uniform = glGetUniformLocation(shaderProgram, "u_texture");
    glUniform1i(tex_uniform, texture->m_texture);

    // MAIN LOOP
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        // update
        neu::GetEngine().Update();
        glUniform1f(uniform, neu::GetEngine().GetTime().GetTime());

        if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;
        glUniform1f(uniform, GetEngine().GetTime().GetTime());

        // draw
        neu::GetEngine().GetRenderer().Clear();

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)points.size());

        neu::GetEngine().GetRenderer().Present();
    }

    neu::GetEngine().Shutdown();

    return 0;
}
