int potpin = 5;
int buttonpin = 7;
int buttonstate = 0;

int old_state = 0; // last pot reading
int new_state = 0; // newest pot reading

int num_leds = 6;
int wait = 0;
int i = 0;
int led = 0;

int divisor = 4; // divide the input for smaller loops

int ledcount[6] = {0,0,0,0,0,0};  // stats counter
float blinks = 0; // stat counter

void setup() {
    pinMode(buttonpin, INPUT);
    pinMode(potpin, INPUT);

    /* assume the LEDs start at pin 8 and assign them as outputs */
    for (i=0;i<num_leds;i++)
    {
        pinMode(i+8, OUTPUT);
    }

    Serial.begin(9600);
    new_state = analogRead(potpin)/divisor;
    old_state = new_state;
    Serial.print("Loops dialed in: ");
    Serial.println(new_state);
}

void lightLed(int led, int length)
{
    digitalWrite(led, HIGH);
    delay(length);
    digitalWrite(led, LOW);
}

void loop()
{
    new_state = analogRead(potpin)/divisor; // pot values 0-1023, make the value smaller
    delay(100); // let the pot settle down a bit so we don't spam the console
    if (new_state != old_state) 
    {
        old_state = new_state;
        Serial.println(new_state);
    }

    buttonstate = digitalRead(buttonpin);

    if (buttonstate == HIGH) // user pressed the button, run the loop
    {
        Serial.print("Running ");
        Serial.print(new_state);
        Serial.println(" loops...");
        for (i=0;i<new_state;i++)
        {
            wait = random(100,500); // LEDs are lit for random lengths
            led = random(1,num_leds+1); // pick a random LED to light up
            Serial.print("...[");
            Serial.print(i);
            Serial.print("]: #");
            Serial.print(led);
            Serial.print(" for ");
            Serial.print(wait);
            Serial.println("ms");
            lightLed(led+7, wait); // blink the led
            ledcount[led-1]++; // increment LED counter
            blinks++; // increment total blink counter
        }
        Serial.println("...done!");

        /* display some statistics */
        Serial.print("Counts (");
        Serial.print(blinks);
        Serial.print(" blinks): ");
        for (i=0; i<num_leds; i++)
        {
            Serial.print("#");
            Serial.print(i+1);
            Serial.print(" : ");
            Serial.print(ledcount[i]);
            Serial.print(" (");
            Serial.print(ledcount[i]/blinks * 100);
            Serial.print("%)");
            Serial.print(" | ");
        }
        Serial.println(); 
        blinks = 0;             // reset the counters
        memset(ledcount, 0, sizeof(ledcount));;   // reset counters
    }
}
