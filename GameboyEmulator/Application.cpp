#include "Application.h"

Application::Application(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
    UIHandler::Initialize(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

    UIHandler::AddWindow(L"Gameboy Emulator", L"SFML App", [this](HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam) { return MainWindowEvent(Handle, Message, WParam, LParam); });
    debugger = new Debugger(&gameboy);

    std::clock_t start;

    double duration;

    int framerate = 60;

    start = std::clock();
    MSG Message;
    Message.message = ~WM_QUIT;
    DWORD Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS;
    HWND  View1 = CreateWindow(L"STATIC", NULL, Style, 0, 0, 500, 500, UIHandler::GetWindow(L"Gameboy Emulator")->Window, NULL, hInstance, NULL);

    sf::RenderWindow SFMLView(View1);
    while (Message.message != WM_QUIT)
    {
        if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
        {
            // If a message was waiting in the message queue, process it
            TranslateMessage(&Message);
            DispatchMessage(&Message);
            
        }
        else 
        {
            if ((std::clock() - start) / (double)CLOCKS_PER_SEC >= (double)1.0 / framerate) {
                debugger->Update();
                start = std::clock();
            }
            SFMLView.clear();
            LPRECT rect = new RECT();
            GetWindowRect(UIHandler::GetWindow(L"Gameboy Emulator")->Window, rect);
            SFMLView.setSize(sf::Vector2u(rect->right - rect->left, rect->bottom - rect->top));
        

            sf::Texture texture;
            texture.create(LCDWIDTH, LCDHEIGHT);
            texture.update((uint8_t*)gameboy.bus->CurrentIOstate.Buffers[!gameboy.bus->CurrentIOstate.BufferSelect], LCDWIDTH, LCDHEIGHT, 0, 0);
            sf::Sprite sprite;
            
            gameboy.bus->CurrentIOstate.ControllerInput.A = sf::Keyboard::isKeyPressed(sf::Keyboard::C);
            gameboy.bus->CurrentIOstate.ControllerInput.B = sf::Keyboard::isKeyPressed(sf::Keyboard::X);
            gameboy.bus->CurrentIOstate.ControllerInput.Start = sf::Keyboard::isKeyPressed(sf::Keyboard::Enter);
            gameboy.bus->CurrentIOstate.ControllerInput.Select = sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
            gameboy.bus->CurrentIOstate.ControllerInput.Left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
            gameboy.bus->CurrentIOstate.ControllerInput.Right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
            gameboy.bus->CurrentIOstate.ControllerInput.Up = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
            gameboy.bus->CurrentIOstate.ControllerInput.Down = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);

            sprite.setScale(sf::Vector2f(400 / 160.0f, 400 / 144.0f));
            sprite.setTexture(texture);
            SFMLView.draw(sprite);
            free(rect);


            SFMLView.display();
        }
    }

}

LRESULT Application::MainWindowEvent(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam)
{
    switch (Message)
    {
        // Quit when we close the main window


    case WM_COMMAND:

        OnCommand(WParam);

        break;
    case WM_CREATE:
        MenuHandler(Handle);
        break;
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(Handle, Message, WParam, LParam);
}

void Application::OnCommand(WPARAM WParam)
{
    switch (WParam) {
    case OPEN_DEBUGGER:
        debugger->StartWindow();
        break;
    case EMULATION_RESET:
        gameboy.Reset();
        break;
    }
}

void Application::MenuHandler(HWND Handle)
{
    hMenu = CreateMenu();
    hDebug = CreateMenu();
    hFileMenu = CreateMenu();
    hEmulation = CreateMenu();

    AppendMenu(hFileMenu, MF_STRING, NULL, L"Open");
    AppendMenu(hDebug, MF_STRING, OPEN_DEBUGGER, L"Debugger");
    AppendMenu(hEmulation, MF_STRING, EMULATION_RESET, L"Reset");

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hEmulation, L"Emulation");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hDebug, L"Debug");

    SetMenu(Handle, hMenu);

}
