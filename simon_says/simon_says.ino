/*
 * Author: Denis Cosmin
 * Date: 19.09.2016
 * Name: Simon Says
 */

// Color definitions
// 0 - Yellow
// 1 - Green
// 2 - Red
// 3 - Blue
class Game {
    private:
      int debounce(int last, int buttonPin);
      void playNote(int note, int noteSpeed) const;
      void flashLed(int led, int flashSpeed) const;
    public:
      static const int RED_PIN;
      static const int BLUE_PIN;
      static const int GREEN_PIN;
      static const int YELLOW_PIN;
      static const int MICROPHONE_PIN;
      static const int RED_BUTTON_PIN;
      static const int BLUE_BUTTON_PIN;
      static const int GREEN_BUTTON_PIN;
      static const int YELLOW_BUTTON_PIN;
      static const int RED_TONE;
      static const int BLUE_TONE;
      static const int GREEN_TONE;
      static const int YELLOW_TONE;
      static const int GAMEOVER_TONE;
      int gameLevel[200];
      int gameSpeed;
      int lastButtonValue;
      int currentLevel;
      int gameIsOver;
      double gameDifficulty;
    public:
    Game();
    void playLevel();
    int userInput();
    int gameOver();
    int readButton(int buttonPin);
};

// Settings
/* Pin settings */
// Button pins must be in consecutive descending order! Eg: blue 12, red 11, green 10, yellow 9
static const int Game::MICROPHONE_PIN       = 13;
static const int Game::BLUE_PIN             = 12;
static const int Game::RED_PIN              = 11;
static const int Game::GREEN_PIN            = 10;
static const int Game::YELLOW_PIN           = 9;
static const int Game::BLUE_BUTTON_PIN      = 8;
static const int Game::RED_BUTTON_PIN       = 7;
static const int Game::GREEN_BUTTON_PIN     = 6;
static const int Game::YELLOW_BUTTON_PIN    = 5;
/* Tone frequencies */
static const int Game::RED_TONE             = 200;
static const int Game::BLUE_TONE            = 400;
static const int Game::YELLOW_TONE          = 600;
static const int Game::GREEN_TONE           = 800;
static const int Game::GAMEOVER_TONE        = 1000;

// Functions
Game::Game() : gameSpeed(1000), lastButtonValue(-1), currentLevel(0), gameDifficulty(5), gameIsOver(0) {
    Serial.println("Constructing game object");
    pinMode(Game::MICROPHONE_PIN, OUTPUT);
    pinMode(Game::BLUE_PIN, OUTPUT);
    pinMode(Game::RED_PIN, OUTPUT);
    pinMode(Game::GREEN_PIN, OUTPUT);
    pinMode(Game::YELLOW_PIN, OUTPUT);
}

int Game::debounce(int last, int buttonPin) {
      int current = digitalRead(buttonPin);
      if (last != current)
      {
        delay(5);
        current = digitalRead(buttonPin);
      }
      return current;
}

void Game::playNote(int note, int noteSpeed) const {
    Serial.print("playNote: Playing note: ");
    Serial.print(note);
    Serial.print(" with speed: ");
    Serial.println(noteSpeed);

    switch(note) {
      case 0:
          note = Game::YELLOW_TONE;
          break;
      case 1:
          note = Game::GREEN_TONE;
          break;
      case 2:
          note = Game::RED_TONE;
          break;
      case 3:
          note = Game::BLUE_TONE;
          break;
      case 4:
          note = Game::GAMEOVER_TONE;
          break;        
      default:
        Serial.println("playNote: Error! Invalid note!");
    }
    
    tone(Game::MICROPHONE_PIN, note, noteSpeed);  
}

void Game::flashLed(int led, int flashSpeed) const {
    Serial.print("flashLed: Flashing LED: ");
    Serial.print(led);
    Serial.print(" with speed: ");
    Serial.println(flashSpeed);

    led = led + Game::YELLOW_PIN;

    digitalWrite(led, HIGH);
    delay(flashSpeed);
    digitalWrite(led, LOW);
}

void Game::playLevel() {
  Serial.print("playLevel: Playing on level: ");
  Serial.println(currentLevel);
  Game::gameLevel[currentLevel] = random(0, 4); // Create a random move every time.
  ++currentLevel;
  Game::gameSpeed -= Game::gameDifficulty * Game::currentLevel; // decrease the speed;

  // Play all the moves
  for (int i = 0; i < currentLevel; ++i) {
      Game::playNote(Game::gameLevel[i], Game::gameSpeed);
      Game::flashLed(Game::gameLevel[i], Game::gameSpeed);
  }
}

int Game::readButton(int buttonPin) {
    int currentButtonValue = Game::debounce(Game::lastButtonValue, buttonPin);
    int return_value = -1;
    if (lastButtonValue == LOW && currentButtonValue > LOW) {
        return_value = buttonPin - Game::YELLOW_BUTTON_PIN;
    }
    Game::lastButtonValue = currentButtonValue;
    if (return_value >= 0) {
      Serial.print("readButton: Received signal from button number: ");
      Serial.println(return_value);
    }
    return return_value;
}

int Game::gameOver() {
    Serial.println("game_is_over: call");
    return Game::gameIsOver;
}

int Game::userInput() {
    for (int i = 0; i < Game::currentLevel; ++i) {
      Serial.print("userInput: User pressed: ");
      int buttonPressed = -1;
      while(true) {
          buttonPressed = readButton(Game::RED_BUTTON_PIN);
          if (buttonPressed != -1) { break; }
          buttonPressed = readButton(Game::GREEN_BUTTON_PIN);
          if (buttonPressed != -1) { break; }
          buttonPressed = readButton(Game::YELLOW_BUTTON_PIN);
          if (buttonPressed != -1) { break; }
          buttonPressed = readButton(Game::BLUE_BUTTON_PIN);
          if (buttonPressed != -1) { break; }
      }
      Serial.println(buttonPressed);
      if (buttonPressed != gameLevel[i]) {
          Game::playNote(4, 100); // game over note, and game over note speed.
          Game::flashLed(buttonPressed, 1000);
          return 0;
      }
      Game::playNote(buttonPressed, Game::gameSpeed);
      Game::flashLed(buttonPressed, Game::gameSpeed);
    }
    delay(500);
    return 1;
}

Game g;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  if ( !g.gameOver() ) {
    g.playLevel();
    if (g.userInput() == 0) {
        g.gameIsOver = 1;
    }
  }
}