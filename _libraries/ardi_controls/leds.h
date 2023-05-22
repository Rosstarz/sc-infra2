void enableOneLed(int);
void enableMultipleLeds(uint8_t);
void enableAllLeds();

void lightUpOneLed(int); 
void lightUpMultipleLeds(uint8_t);
void lightUpAllLeds();

void lightDownOneLed(int); 
void lightDownMultipleLeds(uint8_t);
void lightDownAllLeds();

void lightToggleOneLed(int);
void enableAllLedsDown();

bool isLedOn(int);

void dimLed(int lednumber, int percentage, int duration);
void fadeInLed(int lednumber, int duration);
void fadeOutLed(int lednumber, int duration);

void blinkLed(int lednumber, int duration);
void blinkAllLeds(int duration);