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

    //std::vector<vec3> points = { {0.5,0.5,0}, {0.5,-0.5,0},{-0.5,0.5,0} };
    //std::vector<vec3> colors = { {1,1,0},{0,1,1},{1,0,1}};
    std::vector<vec3> points{ { -0.5f, -0.5f, 0 }, { 0, 0.5f, 0 }, { 0.5f, -0.5f, 0 } }; 
    std::vector<vec3> colors{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };

    GLuint vbo;
    glGenBuffers(1, &vbo);

    //vertext buffer (position)
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * points.size(), points.data(), GL_STATIC_DRAW);

    GLuint vao;
    glGenVertexArrays(1, &vao);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    //vertex shader
    std::string vs_source;
    file::ReadTextFile("shaders/basic.vert", vs_source);
    const char* vs_cstr = vs_source.c_str();

    GLuint vs;
    vs = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vs, 1, &vs_cstr, NULL);
    glCompileShader(vs);

    //fragment shader


    //float angle = GetEngine().GetTime().GetTime()*90.0f;
    //float scale = math::Remap(-1.0f, 1.0f, 0.3f, 1.5f, math::sin(GetEngine().GetTime().GetTime()));

    //vec2 mouse = GetEngine().G

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

        // draw
        neu::vec3 color{ 0, 0, 0 };
        neu::GetEngine().GetRenderer().SetColor(color.r, color.g, color.b);
        neu::GetEngine().GetRenderer().Clear();

        glLoadIdentity();
        glPushMatrix();
        //glTranslate(position.x, position.y);

        /*glBegin(GL_TRIANGLES);

        glColor3f(1,0,0);
        glVertex3f(0.5,0.5,0);
        glColor3f(0,1,0);
        glVertex3f(0.5,-0.5,0);
        glColor3f(0,0,1);
        glVertex3f(-0.5,0.5,0);*/

        glRotatef(angle,0,0,1);
        //glScalef(scale, scale, 0);

        glBegin(GL_TRIANGLES);
        for (int i = 0; i < 3; i++) {
            glColor3f(colors[i].r, colors[i].g, colors[i].b);
            glVertex3f(points[i].x, points[i].y, points[i].z);
        }

        glEnd();


        neu::GetEngine().GetRenderer().Present();
    }

    neu::GetEngine().Shutdown();

    return 0;
}
