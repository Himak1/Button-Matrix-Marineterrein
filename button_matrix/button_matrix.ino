/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   button-matrix-final.ino                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jhille <jhille@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/08 13:17:44 by jhille        #+#    #+#                 */
/*   Updated: 2022/08/08 13:42:00 by jhille        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <ArduinoQueue.h>                 // Import the arduino Queue library.
#include "game.ino"

/*
	 IO pin definitions for the interface boards.
*/
const uint8_t SER_LED_IN = 18;            // The LED bit-stream
const uint8_t SER_CLK = 14;               // The serial shift cloc (LED and switches (SHCP))
const uint8_t LATCH = 15;                 // The parallel latch (LED and switches (STCP))
const uint8_t SW_LOAD = 16;               // Switches load register (~PL)
const uint8_t SER_SW_OUT = 17;            // The Switches bit stream
/*
	 System configuration parameters.
*/
const uint8_t N_IO_BOARDS = 4;            // Number of IO boards. Eqach board supports 8 panels.
const uint8_t N_PANELS = N_IO_BOARDS * 8; // The total number of panels.
const uint8_t OUT_MAP[] = {31, 30, 23, 22, 15, 14, 7, 6,
													 29, 28, 21, 20, 13, 12, 5, 4,
													 27, 26, 19, 18, 11, 10, 3, 2,
													 25, 24, 17, 16, 9,  8,  1, 0};

const uint8_t IN_MAP[] = {25, 24, 17, 16,  9,  8, 1, 0,
													27, 26, 19, 18, 11, 10, 3, 2,
													29, 28, 21, 20, 13, 12, 5, 4,
													31, 30, 23, 22, 15, 14, 7, 6};

#define INPUT_QUEUE_LENGTH 10             // Length of pressed switch panel input queue.
#define DEBOUNCE_TIME  (200 / 2)           // Debounce time in 2 ms units.

/*
	 Global storage for the system.
*/
bool led_on[N_PANELS];                    // When true the LEDs on one panel are on. First panel has index 0.
uint8_t switch_time[N_PANELS];            // ms clock mod 256 of switch pressed.
ArduinoQueue<uint8_t> sw_pushed(INPUT_QUEUE_LENGTH);  // create queue for pressed switches.

// Forward declaration for the game routines.
void gameSetup();
int game(int state, bool hasInput);

/*
	 Initialize the system
*/
void setup() {
	// Start set-up serial connection via USB.
	Serial.begin(115200);

	// Define the IO pins we use and give the output pins their initial value
	pinMode(SER_LED_IN , OUTPUT);
	pinMode(SER_CLK , OUTPUT);
	pinMode(LATCH , OUTPUT);
	pinMode(SW_LOAD , OUTPUT);
	pinMode(SER_SW_OUT , INPUT);
	digitalWrite(SER_LED_IN , LOW);
	digitalWrite(SER_CLK , LOW);
	digitalWrite(LATCH , LOW);
	digitalWrite(SW_LOAD , HIGH);

	// Wait for Serial connection to complete set-up.
	while (!Serial) {
		// Wait for serial ready
	};

	Serial.println("\nSystem is starting up!");
	// Set up initial LED and switch on time.
	for ( int i = 0; i < N_PANELS; i++ ) {
		led_on[i] = false;
		switch_time[i] = 0;
	}
	gameSetup();      // System ready, so run the initialize of the game.
}

/*
	 Shift all LED settings to the panel and poll all switches.
*/
void _poll_shift_registers() {
	bool pressed;
	uint32_t ms = (millis() / 2) % 256; // 2 ms clock time MOD 256 for this round.

	if (ms == 0) ms = 1;              // 0 has special meaning, so we lie one ms to avoid 0.

	digitalWrite(SW_LOAD , LOW);      // Enable input latch.
	digitalWrite(LATCH, HIGH);        // Sample and hold input latches and push out previous LED settings.
	digitalWrite(LATCH, LOW);         // Be ready for next round.
	digitalWrite(SW_LOAD , HIGH);     // Isolate input latch.

	for ( int i = 0; i < N_PANELS; i++ ) {
		// shift out the led bit and shif-in the switch bit.
		digitalWrite(SER_LED_IN, led_on[i] ? HIGH : LOW);
		pressed = digitalRead(SER_SW_OUT) == LOW;
		if ( pressed ) {
			// Press detected
			if ( switch_time[i] == 0 ) {
				// New press detected.
				sw_pushed.enqueue(IN_MAP[i]);         // Notify the game.
				switch_time[i] = ms;               // start debounce.
			}
		} else {
			// No press detected.
			if (int st = switch_time[i])  {
				// Switch is on in debounce. Time to release?
				if ((ms - st) % 256 > DEBOUNCE_TIME) {
					switch_time[i] = 0;
				}
			}
		}
		digitalWrite(SER_CLK , HIGH);   // Toggle serial shift clock to get netx bit in and out.
		digitalWrite(SER_CLK , LOW);
	}
}

int gameState = 0;
void loop() {
	_poll_shift_registers();
	gameState = game(gameState, hasInput());
}

/*
	 True if input switch number is in the queue
*/
inline bool hasInput() {
	return sw_pushed.itemCount() > 0;
}

/*
	 Get switch input number and remove from queue
*/
inline uint8_t getInput() {
	return sw_pushed.dequeue();
}

/*
	 Get switch input number. Do NOT remove from queue
*/
inline uint8_t peekInput() {
	return sw_pushed.getHead();
}

/*
	 Empty the queue.
*/
void emptyQueue() {
	while (sw_pushed.itemCount())
		sw_pushed.dequeue();
}

inline void led_on_off(uint8_t lednum, bool on_off)
{
	led_on[OUT_MAP[lednum]] = on_off;
}

/*
		returns the state of LED number (lednum)
*/
inline bool led_state(uint8_t lednum)
{
	return (led_on[OUT_MAP[lednum]]);
}
