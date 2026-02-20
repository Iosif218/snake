#include <SFML/Graphics.hpp>
#include <ctime>
using namespace sf;

const int windowWidth = 360;
const int windowHeight = 760;

class Food {
private: int x, y, r; CircleShape circleSmall, circleBig;
public:
    Food() {
        x = (rand() % (windowWidth /20))*20; y = (rand() % (windowHeight /20))*20; r = 6;
        circleSmall.setFillColor(Color::White);
        circleSmall.setRadius(r);
        circleBig.setFillColor(Color::Black);
        circleBig.setRadius(r + 3);
    }
    void draw(RenderWindow& window) {
        circleBig.setPosition(Vector2f(x+1, y+1));
        window.draw(circleBig);
        circleSmall.setPosition(Vector2f(x+4, y+4));
        window.draw(circleSmall);
    }
    void update(Texture& backgroundTexture) {
        /*
        Image im = backgroundTexture.copyToImage();
        Color pixelColor = im.getPixel(Vector2u(x, y));
        Color invertedColor = Color(
            255 - pixelColor.r,
            255 - pixelColor.g,
            255 - pixelColor.b
        );
        */
        x = (rand() % (windowWidth / 20)) * 20; y = (rand() % (windowHeight / 20)) * 20;
    }
    int getX() const {
        return x;
    }
    int getY() const {
        return y;
    }
    int getR() const{
        return r;
    }
};

class Snake {
private: 
    int  size = 20, length = 4, dir = 3, x = 320, y = 240, grow=0;
    bool life = true;
    int body[500][2];
    RectangleShape rectangle;
public:
    Snake() {
        body[0][0] = x; body[0][1] = y; length = 4;
        rectangle.setFillColor(Color::Color(52, 47, 40));
        rectangle.setSize(Vector2f(size-2, size-2));
        for (int i = 1; i < length; i++) {
            body[i][0] = body[i - 1][0] + size;
            body[i][1] = y;
        }
    }
    void draw(RenderWindow& window) {
        for (int i = 0; i < length; i++) {
            rectangle.setPosition(Vector2f(body[i][0],body[i][1]));
            window.draw(rectangle);
        }
    }
    void changeDir(int d, bool& c) {
        if (c) {
            if ((dir - d) % 2 != 0) { dir = d; c = !c; }
        }
    }
    void update(Food& f, Texture& t){
        if (dir == 0) body[0][1] -= size;
        if (dir == 1) body[0][0] += size;
        if (dir == 2) body[0][1] += size;
        if (dir == 3) body[0][0] -= size;

        for (int i = 1; i < length; i++) {
            if (body[0][0] == body[i][0] && body[0][1] == body[i][1]) life = false;
        }
        
        if (body[0][0] <= -size) body[0][0] = windowWidth;
        else if (body[0][0] >= windowWidth) body[0][0] = 0;
        if (body[0][1] <= -size) body[0][1] = windowHeight;
        else if (body[0][1] >= windowHeight) body[0][1] = 0;


        int x = f.getX(), y = f.getY(), r = f.getR();

        if (body[0][0] == x && body[0][1] == y) {
            f.update(t); length++;
        }

        for (int i = length - 1; i > 0; i--) {
            body[i][0] = body[i - 1][0]; body[i][1] = body[i - 1][1];
        }
    
    }
    bool getLife()const { return life; }
    void changeLife() { life = !life; }
    void reset() {
        changeLife();
        for (int i = 0; i < length; i++) {
            body[i][0] = 0; body[i][1] = 0;
        }
        body[0][0] = x; body[0][1] = y; length = 4; dir = 3;
        for (int i = 1; i < length; i++) {
            body[i][0] = body[i - 1][0] + size;
            body[i][1] = y;
        }
    }
};

int main()
{
    srand(time(0));

    RenderWindow window(VideoMode({ windowWidth, windowHeight }), "Snake");

    Texture backTexture("back3.jpeg");
    Sprite background(backTexture);
    Vector2f textureSize = static_cast<Vector2f>(backTexture.getSize());
    float scale = std::max(
        windowWidth/textureSize.x,
        windowHeight/textureSize.y
    );
    background.setScale({ scale, scale });

    Snake s;
    Food f;

    Clock clock;
    Time timePerUpdate = seconds(0.05f);
    bool dirChanged = true;

    while (window.isOpen())
    {
        Time elapsedTime = clock.getElapsedTime();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
        }
        if (s.getLife()) {
            if (elapsedTime < timePerUpdate) {
                if (Keyboard::isKeyPressed(Keyboard::Key::Up)) s.changeDir(0, dirChanged);
                else if (Keyboard::isKeyPressed(Keyboard::Key::Right)) s.changeDir(1, dirChanged);
                else if (Keyboard::isKeyPressed(Keyboard::Key::Down)) s.changeDir(2, dirChanged);
                else if (Keyboard::isKeyPressed(Keyboard::Key::Left)) s.changeDir(3, dirChanged);
            }
            else if (elapsedTime >= timePerUpdate) { s.update(f, backTexture); dirChanged = !dirChanged; clock.restart(); }
        } 
        else if (Keyboard::isKeyPressed(Keyboard::Key::R)) s.reset();

        window.clear(Color::Color(163,188,220));
        window.draw(background);
        s.draw(window);
        f.draw(window);
        window.display();
    }
}