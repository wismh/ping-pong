# Ping-Pong Game

Цей проєкт використовує CMake, SDL3, SDL3_mixer, OpenGL та кілька залежностей, підключених через git submodules.

Нижче наведена покрокова інструкція зі збірки проєкту з нуля.

## Вимоги

Перед початком переконайся, що у тебе встановлено:

- Git
- CMake ≥ 3.15
- C++ compiler з підтримкою C++23
  - Windows: MSVC (Visual Studio 2022)
  - macOS: clang (Xcode Command Line Tools)
  - Linux: gcc / clang
- OpenGL

## Як зібрати проект 

1. Clone repo 
    ```bash
    git clone https://github.com/wismh/ping-pong.git
    cd ping-pong 
    ```
2. Ініціалізація submodules
    ```bash
    git submodule update --init --recursive
    ```
3. Конфігурація CMake
    ```bash
    cmake -G "Visual Studio 17 2022" -S . -B ./build
    ```
4. Білд
    ```bash
   cmake --build build --target game
   ```
5. Запуск 
    ```bash
   cd ./build/bin/
   ./game.exe
    ```

## Ключові моменти:
- dependency injection підхід
- universal render api (implemented opengl backend)
- ui auto-layout (implemented properties: direction, align, cross-align, gap)
- enity-component-system
- event bus
- input system
- assetsdb
- audio system
- auto difficulty balance
  
## Dependency Injection

Проєкт використовує dependency injection (DI) для зменшення звʼязності між підсистемами.

- Core системи (window, renderer, input, audio, assets) не створюють залежності напряму
- Залежності передаються через конструктори
- Легко міняти реалізації (наприклад, рендер або аудіо бекенд)

Переваги:
- Чистіший код
- Легше тестувати
- Менше глобального стану

## Universal Render API

Рендеринг побудований навколо універсального render API, незалежного від графічного бекенду.

- IRenderBackend, ICanvas, IMesh - абстракції
- OpenGL бекенд - поточна реалізація
- Логіка гри не знає, який бекенд використовується

Мета:
- можливість додати DirectX / Metal без переписування гри

## UI System (Auto-layout)

UI побудований як ієрархія вузлів (UI tree) з автоматичним лейаутом.

Підтримувані властивості:

- direction - горизонтальний / вертикальний лейаут
- align - вирівнювання по основній осі
- crossAlign - вирівнювання по другій осі
- gap - відступи між елементами

## Entity Component System (ECS)

Гра використовує Entity Component System підхід:

- Entity — ідентифікатор
- Component — дані (Transform, RigidBody, Collider, Renderable)
- System — логіка, яка працює з наборами компонентів

Переваги:

- Дані відокремлені від логіки
- Легко додавати нові системи
- Краще масштабується ніж класичний OOP

## Event Bus

Внутрішня комунікація побудована через event bus.

- Системи не викликають одна одну напряму
- Події типу:
    - input
    - collisions
    - state changes
- Асинхронна та слабко звʼязана архітектура

## Input System

Input система:

- Абстрагує SDL input
- Працює через actions, а не кнопки
- Дозволяє легко міняти керування

## AssetsDB

AssetsDb відповідає за:

- Завантаження ресурсів (textures, audio, UI images)
- Кешування
- Контроль життєвого циклу

Ресурси не завантажуються напряму з гри —
вся робота йде через централізовану базу.

## Auto Difficulty Balancing

Гра містить систему автоматичного балансування складності:
- Чим довше триває раунд тим більше AI помиляється 
- Чим більше у гравця рахунок тим краше грає AI

## Architectural Goals
- Мінімальна звʼязність
- Розширюваність
- Чітке розділення відповідальностей
- Орієнтація на real-time applications
