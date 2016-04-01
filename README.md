# artik4j

here's a sample on how to use it

public static void main(String[] args) throws IOException, InterruptedException {
        ArtikGPIO artikGPIO = new ArtikGPIO(ARTIK_MODELS.ARTIK5);

        artikGPIO.initPin(ArtikGPIO.PIN_DIRECTION.OUT, ArtikGPIO.ARTIK_PINS.J26pin5);
        artikGPIO.initPin(ArtikGPIO.PIN_DIRECTION.IN, ArtikGPIO.ARTIK_PINS.J26pin6);
        try {
            artikGPIO.initPWM(PWM_PIN.PWM0);
        } catch (IOException iOException) {
            System.out.println("Already Exported...");
        }
        artikGPIO.enablePwm(PWM_PIN.PWM0, true);
        artikGPIO.adjustPwm(PWM_PIN.PWM0, 1000000000, 500);
        Thread.sleep(2000);
        artikGPIO.enablePwm(PWM_PIN.PWM0, false);
        artikGPIO.releasePwm(PWM_PIN.PWM0);
        System.exit(0);
        int x = 0;
        for (int i = 0; i < 600; i++) {
            artikGPIO.setPinState(ArtikGPIO.ARTIK_PINS.J26pin5, x++ % 2 == 0 ? ArtikGPIO.PIN_STATE.HIGH : ArtikGPIO.PIN_STATE.LOW);
            PIN_STATE readPinStatus = artikGPIO.readPinStatus(ArtikGPIO.ARTIK_PINS.J26pin6);
            System.out.println("pinstatus :" + readPinStatus);
            int value = artikGPIO.readAnalogueValue(ArtikGPIO.ANALOG_PIN.A0);
            System.out.println("analog :" + value);
            Thread.sleep(1000);
        }
        artikGPIO.releasePin(ArtikGPIO.ARTIK_PINS.J26pin5);
        artikGPIO.releasePin(ArtikGPIO.ARTIK_PINS.J26pin6);
    }
