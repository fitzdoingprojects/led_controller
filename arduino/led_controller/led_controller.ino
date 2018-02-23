#include <IRremote.h>
#include<FastLED.h>

#define	IR_BPlus  0xFF3AC5	// 
#define	IR_BMinus 0xFFBA45	// 
#define	IR_ON 	  0xFF827D	// 
#define	IR_OFF 	  0xFF02FD	// 
#define	IR_R 	  0xFF1AE5	// 
#define	IR_G 	  0xFF9A65	// 
#define	IR_B  	  0xFFA25D	// 
#define	IR_W 	  0xFF22DD	// 
#define	IR_B1	  0xFF2AD5	// 
#define	IR_B2	  0xFFAA55	// 
#define	IR_B3	  0xFF926D	// 
#define	IR_B4	  0xFF12ED	// 
#define	IR_B5	  0xFF0AF5	// 
#define	IR_B6	  0xFF8A75	// 
#define	IR_B7	  0xFFB24D	// 
#define	IR_B8	  0xFF32CD	// 
#define	IR_B9	  0xFF38C7	// 
#define	IR_B10	  0xFFB847	// 
#define	IR_B11	  0xFF7887	// 
#define	IR_B12	  0xFFF807	// 
#define	IR_B13	  0xFF18E7	// 
#define	IR_B14	  0xFF9867	// 
#define	IR_B15	  0xFF58A7	// 
#define	IR_B16	  0xFFD827	// 
#define	IR_UPR 	  0xFF28D7	// 
#define	IR_UPG 	  0xFFA857	// 
#define	IR_UPB 	  0xFF6897	// 
#define	IR_QUICK  0xFFE817	// 
#define	IR_DOWNR  0xFF08F7	// 
#define	IR_DOWNG  0xFF8877	// 
#define	IR_DOWNB  0xFF48B7	// 
#define	IR_SLOW   0xFFC837	// 
#define	IR_DIY1   0xFF30CF	// 
#define	IR_DIY2   0xFFB04F	// 
#define	IR_DIY3   0xFF708F	// 
#define	IR_AUTO   0xFFF00F	// 
#define	IR_DIY4   0xFF10EF	// 
#define	IR_DIY5   0xFF906F	// 
#define	IR_DIY6   0xFF50AF	// 
#define	IR_FLASH  0xFFD02F	// 
#define	IR_JUMP3  0xFF20DF	// 
#define	IR_JUMP7  0xFFA05F	// 
#define	IR_FADE3  0xFF609F	// 
#define	IR_FADE7  0xFFE01F	// 

#define redPin 9
#define greenPin 6
#define bluePin 10

#define FADE_MODE 0
#define CONST_MODE 1
#define JUMP_MODE 2
#define PULSE_MODE 3
#define STROBE_MODE 4
#define SATURATION_MODE 5

#define MIN_PERIOD 4 //4ms
#define DEFAULT_PERIOD 200
#define MAX_PERIOD 4687 //takes 20minutes to go through all 256 colors

#define JUMP_AMOUNT 45



uint8_t toggle_on = 1;
uint8_t mode = FADE_MODE;
uint16_t period = DEFAULT_PERIOD;
uint16_t brightness = 7;
uint8_t hue = 0;
uint8_t saturation = 0;
uint8_t value = 0;
uint16_t red, green, blue;
elapsedMillis timer;
uint8_t old_hsv = false;
uint8_t strobe_on = 0;
uint8_t first_time = true;
int8_t pulse_dir = 1;
uint8_t last_results;

decode_results results;
int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);

/* HSV to RGB conversion function with only integer
 * web.mit.edu/storborg/Public/hsvtorgb.c */
void set_hsv_fast(unsigned char h, unsigned char s, unsigned char v) {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char region, fpart, p, q, t;
	
    if(s == 0) {
        /* color is grayscale */
        r = g = b = v;
        return;
    }
    
    /* make hue into sectors 0-5 */
    region = h / 43;
    /* find remainder part, make it from 0-255 */
    fpart = (h - (region * 43)) * 6;
    
    /* calculate temp vars, doing integer multiplication */
    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * fpart) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - fpart)) >> 8))) >> 8;
        
    /* assign temp vars based on color cone region */
    switch(region) {
        case 0:
            r = v; g = t; b = p; break;
        case 1:
            r = q; g = v; b = p; break;
        case 2:
            r = p; g = v; b = t; break;
        case 3:
            r = p; g = q; b = v; break;
        case 4:
            r = t; g = p; b = v; break;
        default:
            r = v; g = p; b = q; break;
    }
	red = (((uint16_t) r) << brightness);
	green = (((uint16_t) g) << brightness);
	blue = (((uint16_t) b) << brightness);
	analogWrite(redPin, red);
	analogWrite(greenPin, green);
	analogWrite(bluePin, blue);
}

void set_hsv(unsigned char h, unsigned char s, unsigned char v) {
	CHSV hsv( h, s, v); // pure blue in HSV Rainbow space
	CRGB rgb;
	hsv2rgb_rainbow( hsv, rgb);

	red = (((uint16_t) rgb.red) << brightness);
	green = (((uint16_t) rgb.green) << brightness);
	blue = (((uint16_t) rgb.blue) << brightness);
	analogWrite(redPin, red);
	analogWrite(greenPin, green);
	analogWrite(bluePin, blue);
}

//make sure to reset timer when switching modes

void setup() {
  analogWriteResolution(16); /*default frequency of 488Hz */  
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  Serial.println("starting");
  Serial.begin(115200);
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
}

void loop() {

	
	/* codes from http://woodsgood.ca/projects/2015/02/13/rgb-led-strip-controllers-ir-codes/ */
	if (irrecv.decode(&results)) {
		if((results.value < 0xFFFFFF) && (results.value > 0xFF0000)) {
			set_hsv(0, 0, 0);
			delay(100);
			last_results = results.value;
		}
		Serial.println(results.value, HEX);
		switch(results.value) {
		case IR_BPlus:
			Serial.println("+");
			
			if(brightness < 7) {
				brightness++;
			}
			if(mode == SATURATION_MODE) {
				set_hsv(hue, 255, saturation);
			}
			break;
		case IR_BMinus:
			Serial.println("-");
			if(brightness > 0) {
				brightness--; 
			}
			if(mode == SATURATION_MODE) {
				set_hsv(hue, 255, saturation);
			}
			break;
		case IR_ON:
		case IR_OFF:
			if(toggle_on) {
				set_hsv(0, 0, 0);
				toggle_on = 0;
			} else {
				toggle_on = 1;
			} 
			break;

		case IR_R:
			Serial.println("red");
			mode = CONST_MODE;
			hue = 0;
			saturation = 255;
			break;
		case IR_B1:
			mode = CONST_MODE;
			hue = 19;
			saturation = 255;
			break;
		case IR_B5:
			mode = CONST_MODE;
			hue = 38;
			saturation = 255;
			break;
		case IR_B9:
			mode = CONST_MODE;
			hue = 57;
			saturation = 255;
			break;
		case IR_B13:
			mode = CONST_MODE;
			hue = 77;
			saturation = 255;
			break;

		case IR_G:
			mode = CONST_MODE;
			hue = 96;
			saturation = 255;
			break;
		case IR_B2:
			mode = CONST_MODE;
			hue = 109;
			saturation = 255;
			break;
		case IR_B6:
			mode = CONST_MODE;
			hue = 122;
			saturation = 255;
			break;
		case IR_B10:
			mode = CONST_MODE;
			hue = 135;
			saturation = 255;
			break;
		case IR_B14:
			mode = CONST_MODE;
			hue = 148;
			saturation = 255;
			break;


		case IR_B:
			mode = CONST_MODE;
			hue = 160;
			saturation = 255;
			break;
		case IR_B3:
			mode = CONST_MODE;
			hue = 179;
			saturation = 255;
			break;
		case IR_B7:
			mode = CONST_MODE;
			hue = 198;
			saturation = 255;
			break;
		case IR_B11:
			mode = CONST_MODE;
			hue = 217;
			saturation = 255;
			break;
		case IR_B15:
			mode = CONST_MODE;
			hue = 236;
			saturation = 255;
			break;


		case IR_W:
			mode = SATURATION_MODE;
			saturation = 0;
			break;
		case IR_B4:
			mode = SATURATION_MODE;
			hue = 0;
			saturation = 10;
			break;		
		case IR_B8:
			mode = SATURATION_MODE;
			hue = 96;
			saturation = 10;
			break;		
		case IR_B12:
			mode = SATURATION_MODE;
			hue = 160;
			saturation = 10;
			break;		
		case IR_B16:
			mode = SATURATION_MODE;
			saturation = 20;
			break;


		case IR_UPR:
			break; 
		case IR_UPG:
			break; 
		case IR_UPB:
			break; 
		case IR_DOWNR:
			break; 
		case IR_DOWNG:
			break; 
		case IR_DOWNB:
			break; 


		case IR_QUICK:
			period /= 1.5;
			if(period < MIN_PERIOD) {
				period = MIN_PERIOD;
				timer = period;
			}
			break; 
		case IR_SLOW:
			period *= 1.5;
			if(period > MAX_PERIOD) {
				period = MAX_PERIOD;
				timer = period;
			}
			break; 

		case IR_DIY1:
			break; 
		case IR_DIY2:
			break; 
		case IR_DIY3:
			break; 
		case IR_DIY4:
			break; 
		case IR_DIY5:
			break; 
		case IR_DIY6:
			break; 
			
		case IR_AUTO:
			timer = 0;
			mode = FADE_MODE;
			break; 
		case IR_FLASH:
			timer = 0;	
			mode = JUMP_MODE;
			break; 
		case IR_JUMP3:
			break; 
		case IR_JUMP7:
			break; 
		case IR_FADE3:
			timer = 0;
			mode = PULSE_MODE;
			break; 
		case IR_FADE7:
			Serial.println("strobe mode");
			timer = 0;
			mode = STROBE_MODE;
			break; 
		}
		irrecv.resume(); // Receive the next value
	}
	if(toggle_on) {
		switch(mode) {
		case FADE_MODE:
			if(timer > period) {
				timer -= period;
				hue++;
				set_hsv(hue, 255, 255);
			}
			break;
		case CONST_MODE:
			if(first_time) {
				first_time = false;
			}
			set_hsv(hue, 255, 255);
			break;
		case JUMP_MODE:
			if(timer > period) {
				timer -= period;
				hue+= JUMP_AMOUNT;
			}
			set_hsv(hue, 255, 255);
			break;
		case PULSE_MODE: 
			if(timer > (period >> 4)) {
				timer -= (period >> 4);
				if(value + pulse_dir == 255) {
					pulse_dir = -1;
				} else if (value + pulse_dir == 0) {
					pulse_dir = 1;
				} else {
					if(pulse_dir > 0)
						value++;
					else
						value--;
				}
				set_hsv(hue, saturation, value);
			}
			break;
		case STROBE_MODE:
			if(timer > period) {
				timer -= period;
				if(strobe_on == 1) {
					set_hsv(255, 0, 255);
					strobe_on = 0;
				} else {
					set_hsv(0, 0, 0);
					strobe_on = 1;
				}
			}
			break;
		case SATURATION_MODE:
			set_hsv(hue, saturation, 255);

			break;
		}
	}

}

