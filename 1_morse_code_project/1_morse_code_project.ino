void setup() {
  // put your setup code here, to run once:

}

void loop() {
  #include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD I2C Address (Common ones: 0x27 or 0x3F, adjust if needed)
LiquidCrystal_I2C mylcd(0x27, 16, 2);  

const int buttonPin = 6;  // Button for dots and dashes
const int spaceButton = 7;  // Button for space
const int resetButton = 8;  // Button for reset
const int ledPin = 13;  // LED indicator

bool buttonPressed = false;
unsigned long pressTime;

String morseInput = "";
String translatedText = "";

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(spaceButton, INPUT_PULLUP);
  pinMode(resetButton, INPUT_PULLUP);

  mylcd.init();  
  mylcd.backlight();  

  mylcd.setCursor(0, 0);
  mylcd.print("Morse Translator");
  delay(2000);
  mylcd.clear();
  mylcd.setCursor(0, 0);
  mylcd.print("Morse: ");
  mylcd.setCursor(0, 1);
  mylcd.print("Text: ");
}

void loop() {
  bool updateScreen = false;

  // Morse Input Button (Dot & Dash)
  if (digitalRead(buttonPin) == LOW) {
    if (!buttonPressed) {
      buttonPressed = true;
      pressTime = millis();
    }
  } else if (buttonPressed) {
    unsigned long duration = millis() - pressTime;
    buttonPressed = false;

    if (duration < 300) {
      morseInput += ".";  // Short press = dot
      blinkLED(1);  // Short blink
    } else {
      morseInput += "-";  // Long press = dash
      blinkLED(3);  // Long blink
    }

    updateScreen = true;
  }

  // Space Button (New Word)
  if (digitalRead(spaceButton) == LOW) {
    delay(50);  
    while (digitalRead(spaceButton) == LOW);  
    morseInput += " ";
    updateScreen = true;
  }

  // Reset Button (Clear Input)
  if (digitalRead(resetButton) == LOW) {
    delay(50);  
    while (digitalRead(resetButton) == LOW);  
    morseInput = "";
    translatedText = "";
    mylcd.clear();
    mylcd.setCursor(0, 0);
    mylcd.print("Morse: ");
    mylcd.setCursor(0, 1);
    mylcd.print("Text: ");
  }

  // Update LCD Display Only When Needed
  if (updateScreen) {
    mylcd.clear();  
    mylcd.setCursor(0, 0);
    mylcd.print("Morse: " + morseInput);

    // Convert Morse to text and display it
    translatedText = morseToText(morseInput);
    mylcd.setCursor(0, 1);
    mylcd.print("Text: " + translatedText);
  }
}

// Blink LED for Morse Code Effect
void blinkLED(int timeUnits) {
  digitalWrite(ledPin, HIGH);
  delay(200 * timeUnits);  
  digitalWrite(ledPin, LOW);
  delay(200);
}

// Convert Morse to Text
String morseToText(String morse) {
  // Define Morse code dictionary
  struct MorseMapping {
    String code;
    char letter;
  };

  MorseMapping morseDictionary[] = {
    {".-", 'A'}, {"-...", 'B'}, {"-.-.", 'C'}, {"-..", 'D'}, {".", 'E'},
    {"..-.", 'F'}, {"--.", 'G'}, {"....", 'H'}, {"..", 'I'}, {".---", 'J'},
    {"-.-", 'K'}, {".-..", 'L'}, {"--", 'M'}, {"-.", 'N'}, {"---", 'O'},
    {".--.", 'P'}, {"--.-", 'Q'}, {".-.", 'R'}, {"...", 'S'}, {"-", 'T'},
    {"..-", 'U'}, {"...-", 'V'}, {".--", 'W'}, {"-..-", 'X'}, {"-.--", 'Y'},
    {"--..", 'Z'}, {"-----", '0'}, {".----", '1'}, {"..---", '2'}, {"...--", '3'},
    {"....-", '4'}, {".....", '5'}, {"-....", '6'}, {"--...", '7'}, {"---..", '8'},
    {"----.", '9'}
  };

  String translated = "";
  String currentCode = "";

  for (int i = 0; i < morse.length(); i++) {
    if (morse[i] == ' ') {
      // Convert the stored Morse sequence
      for (MorseMapping entry : morseDictionary) {
        if (entry.code == currentCode) {
          translated += entry.letter;
          break;
        }
      }
      currentCode = "";  // Reset for next letter
    } else {
      currentCode += morse[i];
    }
  }

  // Convert last Morse sequence if not empty
  if (currentCode.length() > 0) {
    for (MorseMapping entry : morseDictionary) {
      if (entry.code == currentCode) {
        translated += entry.letter;
        break;
      }
    }
  }

  return translated;
}
}