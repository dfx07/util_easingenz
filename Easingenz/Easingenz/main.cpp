#include <GLWindow.h>
#include <GLEasing.h>
#include <GLSp.h>

glm::mat4   matView;
glm::mat4   matProject;
float zoom = 1.0f;

float startpox = -300.f;
float endpox   =  300.f;


EasingBack    animationBack;
EasingBounce  animationBounce;
EasingElastic animationElastic;
EasingQuart   animationQuart;
EasingQuint   animationQuint;

EasingBase* animation;

Combobox* cbbType;
Combobox* cbbMode;


bool bCheck = false;
glm::vec3 pos = { startpox, 0.f, 100.f };

MenuContext* menucontext;

void btnclick(Window* win, Button* mn)
{
    EaseType type = static_cast<EaseType>(*(int*)cbbType->GetSelectData());
    EaseMode mode = static_cast<EaseMode>(*(int*)cbbMode->GetSelectData());

    cout << "Type :" << cbbType->GetSelectText() << " + " << cbbMode->GetSelectText() << endl;

    switch (type)
    {
    case EaseType::Back:
        animation = &animationBack;
        break;
    case EaseType::Quint:
        animation = &animationQuint;
        break;
    case EaseType::Elastic:
        animation = &animationElastic;
        break;
    case EaseType::Quart:
        animation = &animationQuart;
        break;
    case EaseType::Bounce:
        animation = &animationBounce;
        break;
    default:
        break;
    }
    animation->SetMode(mode);
    animation->Start();
}


void Create(Window* win)
{
    matView = glm::lookAt(glm::vec3(0, 0, 8), glm::vec3(0, 0, 8) + glm::vec3(0, 0, -1), glm::vec3(0.f, 1.f, 0.f));

    int width = win->GetWidth();
    int height = win->GetHeight();

    GLdouble left = -GLdouble(width / 2) / zoom;
    GLdouble right = GLdouble(width / 2) / zoom;
    GLdouble top = -GLdouble(height / 2) / zoom;
    GLdouble bottom = GLdouble(height / 2) / zoom;
    glViewport(0, 0, width, height);
    matProject = glm::ortho(left, right, top, bottom, double(0.1), double(-1000));

    Button* btn = new Button();
    btn->SetPosition(250, 0);
    btn->SetEvent(btnclick);
    btn->SetSize(50, 25);
    btn->SetLabel("Run");


    cbbType = new Combobox(0, 0, 120, 300);
    cbbType->TextEdit(false);

    cbbType->AddItem("Back"   , (void*) new int((int)EaseType::Back));
    cbbType->AddItem("Bounce" , (void*) new int((int)EaseType::Bounce));

    cbbType->AddItem("Elastic", (void*) new int((int)EaseType::Elastic));
    cbbType->AddItem("Quart"  , (void*) new int((int)EaseType::Quart));
    cbbType->AddItem("Quint"  , (void*) new int((int)EaseType::Quint));
    cbbType->SetSelect(0);


    cbbMode = new Combobox(125, 0, 120, 300);
    cbbMode->TextEdit(false);
    cbbMode->AddItem("In"   , (void*) new int((int)EaseMode::In));
    cbbMode->AddItem("Out"  , (void*) new int((int)EaseMode::Out));
    cbbMode->AddItem("InOut", (void*) new int((int)EaseMode::InOut));

    cbbMode->SetSelect(0);

    animationBack.Setup(EaseMode::Out, startpox, endpox, 2.f);
    animationBounce.Setup(EaseMode::Out, startpox, endpox, 2.f);
    animationElastic.Setup(EaseMode::Out, startpox, endpox, 2.f);
    animationQuart.Setup(EaseMode::Out, startpox, endpox, 2.f);
    animationQuint.Setup(EaseMode::Out, startpox, endpox, 2.f);

    animation = &animationBack;

    win->AddControl(btn);
    win->AddControl(cbbType);
    win->AddControl(cbbMode);
}

void MouseButton(Window* win)
{
    if (win->GetMouseButtonStatus(GLMouse::LeftButton))
    {
        zoom += 0.2f;

        matView = glm::lookAt(glm::vec3(0, 0, 8), glm::vec3(0, 0, 8) + glm::vec3(0, 0, -1), glm::vec3(0.f, 1.f, 0.f));

        int width  = win->GetWidth();
        int height = win->GetHeight();

        GLdouble left = -GLdouble(width / 2) / zoom;
        GLdouble right = GLdouble(width / 2) / zoom;
        GLdouble top  = -GLdouble(height / 2) / zoom;
        GLdouble bottom = GLdouble(height / 2) / zoom;
        glViewport(0, 0, width, height);
        matProject = glm::ortho(left, right, top, bottom, double(0.1), double(-1000));
    }
}

void Keyboard(Window* win)
{
    if (win->GetKeyboardStatus(GLKeyboard::Escapex))
    {
        win->ExitFullScreen();
    }

    else if (win->GetKeyboardStatus(GLKeyboard::KeyA))
    {
        menucontext->Show(POINT{ 0 ,0 });
    }
}

void Draw(Window* win)
{
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(matProject));
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(matView));

    glsp::glDrawPoint(pos, GL_COLOR_RED, 100.f);
}

void Resize(Window* win)
{
    int width  = win->GetWidth();
    int height = win->GetHeight();
    GLdouble left   = -GLdouble(width / 2) / zoom;
    GLdouble right  =  GLdouble(width / 2) / zoom;
    GLdouble top    = -GLdouble(height / 2) / zoom;
    GLdouble bottom =  GLdouble(height / 2) / zoom;
    matProject = glm::ortho(left, right, top, bottom, double(0.1), double(-1000));
}

void Process(Window * pWin)
{
    float t = pWin->GetTimeElapsed();

    if (animation->IsActive())
    {
        pos.x = animation->Excute(t);
    }
}

int main()
{
    GLWindow window;

    Window *win1 = new Window("Easingenz", 100, 100, 800, 640);
    win1->SetOnDrawfunc(Draw);
    win1->SetFont("Arial", 15);
    win1->SetOnCreatedfunc(Create);
    win1->SetOnMouseButtonfunc(MouseButton);
    win1->SetProcessfunc(Process);
    win1->SetOnKeyboardfunc(Keyboard);
    win1->SetOnResizefunc(Resize);

    window.AddWindow(win1);

    while (window.Closed())
    {
        window.Process();

        window.Draw();
        window.PollEvent();
    }
}
