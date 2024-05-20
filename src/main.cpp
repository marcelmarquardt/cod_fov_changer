#include <QApplication>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QLabel>
#include <QMainWindow>
#include <memory>
#include <thread>

#include "game/Game.h"
#include "games/games.h"

#define DEFAULT_FOV 90.f

std::unique_ptr<QLabel> label_status;
std::unique_ptr<QComboBox> game_selector;
std::unique_ptr<QCheckBox> enabled;
std::unique_ptr<QSpinBox> fov;

static float desired_fov = DEFAULT_FOV;
static bool checkbox_enabled = true;
static int selected_game = 0;

void game_thread()
{
    while (true)
    {
        // FOV value gets reset by the game whenever the player dies.
        // overriding the FOV 200ms after they respawn is more than enough
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        auto game = games::get_game(selected_game);
        if (!game || !game->initialize())
        {
            label_status->setText("Status: Waiting for Game to start");
            continue;
        }

        if (!game->is_running())
        {
            game->reset();
            continue;
        }

        label_status->setText("Status: Game found");

        if (!checkbox_enabled)
            continue;

        // if the player is outside of an active game session (in the lobby for example),
        // it's not possible to read or write to the FOV
        if (!game->read_fov().has_value())
            continue;

        if (game->read_fov().value() != desired_fov)
        {
            game->override_fov(desired_fov);
        }
    }
}

void init_games()
{
    // how to find offsets for other COD games
    // https://www.unknowncheats.me/forum/call-of-duty-modern-warfare-3-a/170427-dvar-offsets-write-internal.html

    games::add(new Game("mw2 mp", "iw4mp.exe", 0xAAC278, 0xC));
    games::add(new Game("mw3 sp", "iw5sp.exe", 0xA76130, 0xC));
    games::add(new Game("mw3 mp", "iw5mp.exe", 0xB0C738, 0xC));
}

int main(int argc, char* argv[])
{
    init_games();

    QApplication a(argc, argv);

    QWidget window;
    window.setWindowTitle("Call of Duty FOV Changer");
    window.setFixedSize(400, 200);
    window.show();

    label_status = std::make_unique<QLabel>(&window);
    label_status->show();
    label_status->setGeometry(QRect(10, 10, 250, 22));

    QLabel label_game_select(&window);
    label_game_select.show();
    label_game_select.setGeometry(QRect(10, 45, 250, 22));
    label_game_select.setText("Select Game:");

    game_selector = std::make_unique<QComboBox>(&window);
    game_selector->show();
    game_selector->setGeometry(QRect(110, 40, 155, 35));
    for (const std::string& game_name : games::get_game_names())
        game_selector->addItem(QString::fromStdString(game_name));

    enabled = std::make_unique<QCheckBox>(&window);
    enabled->show();
    enabled->setGeometry(QRect(10, 70, 155, 35));
    enabled->setText("Enabled");
    enabled->setCheckState(Qt::Checked);

    fov = std::make_unique<QSpinBox>(&window);
    fov->setMaximum(1000);
    fov->show();
    fov->setGeometry(QRect(110, 105, 155, 35));
    fov->setValue(DEFAULT_FOV);

    QLabel label_fov(&window);
    label_fov.show();
    label_fov.setGeometry(QRect(10, 110, 250, 22));
    label_fov.setText("Field of View:");

    QComboBox::connect(game_selector.get(), &QComboBox::currentTextChanged, [&]
    {
        selected_game = game_selector->currentIndex();
    });

    QCheckBox::connect(enabled.get(), &QCheckBox::stateChanged, [&]
    {
        checkbox_enabled = enabled->checkState() == Qt::Checked;
    });

    QSpinBox::connect(fov.get(), &QSpinBox::textChanged, [&]
    {
        desired_fov = fov->value();
    });

    std::thread game(game_thread);
    game.detach();

    return QApplication::exec();
}
