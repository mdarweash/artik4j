/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.qoudra.artik4j;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.security.AuthProvider;
import java.util.Properties;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.imageio.IIOException;

/**
 *
 * @author mdarweash
 */
public class ArtikGPIO {

    private static final String BASE_PIN_PATH = "/sys/class/gpio/gpio";
    private static final String BASE_PIN_EXPORT_PATH = "/sys/class/gpio/export";
    private static final String BASE_PIN_UNEXPORT_PATH = "/sys/class/gpio/unexport";

    private static final String BASE_PWM_PATH = "/sys/class/pwm/pwmchip0/";
    private static final String BASE_PWM_EXPORT_PATH = "/sys/class/pwm/pwmchip0/export";
    private static final String BASE_PWM_UNEXPORT_PATH = "/sys/class/pwm/pwmchip0/unexport";


//    private static final String GPIO="gpio";
    Properties pinsConfiguration;
    ARTIK_MODELS currentModel;

    public enum PWM_PIN {

        PWM0, PWM1
    }

    public enum PIN_STATE {

        HIGH, LOW
    }

    public enum PIN_DIRECTION {

        IN, OUT
    };

    public enum ARTIK_MODELS {

        ARTIK5, ARTIK10
    };

    public enum ARTIK_PINS {

        //J26pin0, J26pin1,
        J26pin2, J26pin3, J26pin4,
        J26pin5, J26pin6,
        J26pin7, J27pin10, J27pin11, J27pin12, J27pin13, J27pin8, J27pin9, J27pinSCL, J27pinSDA
    };

    public enum ANALOG_PIN {

        A0, A1
    };
    static File analog0, analog1;

    static {
        analog0 = new File("/sys/devices/126c0000.adc/iio:device0/in_voltage0_raw");
        analog1 = new File("/sys/devices/126c0000.adc/iio:device0/in_voltage1_raw");
    }

    public ArtikGPIO(ARTIK_MODELS currentModel) {
        this.currentModel = currentModel;

        pinsConfiguration = new Properties();
        try {
            InputStream resourceAsStream = ArtikGPIO.class.getResourceAsStream("/com/qoudra/artik4j/conf/ARTIK_PINOUTs.properties");
//            System.out.println(resourceAsStream);
            pinsConfiguration.load(resourceAsStream);
        } catch (IOException ex) {
            Logger.getLogger(ArtikGPIO.class.getName()).log(Level.SEVERE, null, ex);
        }
        System.out.println("Loaded " + pinsConfiguration.size() + " pins");
    }

    public int getPinNumber(ARTIK_PINS pin) {
        String res[] = pinsConfiguration.getProperty(pin.toString()).split(";");
        if (currentModel == ARTIK_MODELS.ARTIK5) {
            return Integer.parseInt(res[1]);
        } else if (currentModel == ARTIK_MODELS.ARTIK10) {
            return Integer.parseInt(res[0]);
        } else {
            throw new Error("unknown model :" + currentModel);
        }
    }
    //    public static final String 

    public void releasePin(ARTIK_PINS pin) throws IOException {
        assertPinExported(pin);
        echoToPath(BASE_PIN_UNEXPORT_PATH, getPinNumber(pin));
    }

    public void initPin(PIN_DIRECTION direction, ARTIK_PINS pin) throws IOException {
        //export by writing to /sys/class/gpio/export/PIN_NMBER
        System.out.println("init pin:" + pin);
        assertPinNotExported(pin);
        echoToPath(BASE_PIN_EXPORT_PATH, getPinNumber(pin));
        changePinDirection(pin, direction);
        //setting direction by writing to /sys/class/gpio/PIN_NUMBER/direction {in,out}

    }

    private String getPinPath(ARTIK_PINS pin) {
        return BASE_PIN_PATH + getPinNumber(pin);
    }

    private void assertPinExported(ARTIK_PINS pin) throws IOException {
        System.out.println("assertPinExported : " + pin);
        if (!new File(getPinPath(pin)).exists()) {
            throw new IOException("the pin :" + pin + " is not exported, please call initPin() | " + new File(getPinPath(pin)).getAbsolutePath() + " not found");

        }
    }

    private void assertPinNotExported(ARTIK_PINS pin) throws IOException {
        System.out.println("assertNoExported:" + pin);
        if (new File(getPinPath(pin)).exists()) {
            throw new IOException("the pin :" + pin + " is already exported, please call initPin()");

        }
    }

    public void changePinDirection(ARTIK_PINS pin, PIN_DIRECTION direction) throws IOException {
        assertPinExported(pin);
        echoToPath(getPinPath(pin) + "/direction", direction.toString().toLowerCase());
    }

    public void setPinState(ARTIK_PINS pin, PIN_STATE state) throws IOException {
        assertPinExported(pin);
        echoToPath(getPinPath(pin) + "/value", state == PIN_STATE.HIGH ? "1" : "0");
    }

    private void echoToPath(String path, String txt) throws IOException {
        System.out.println("echo " + txt + " >> " + path);
        try (FileWriter fw = new FileWriter(path)) {
            fw.write(txt);
        }
    }

    private void echoToPath(String path, long val) throws IOException {
        System.out.println("echo " + val + " >> " + path);
        try (FileWriter fw = new FileWriter(path)) {
            fw.write(String.valueOf(val));
        }
    }

    public PIN_STATE readPinStatus(ARTIK_PINS pin) throws IOException {
        BufferedReader br = new BufferedReader(new FileReader(getPinPath(pin) + "/value"));
        int i = Integer.parseInt(br.readLine());
        return (i == 0)
                ? PIN_STATE.LOW
                : PIN_STATE.HIGH;
    }

    public int readAnalogueValue(ANALOG_PIN pin) throws IOException {
        BufferedReader br = null;
        switch (pin) {
            case A0:
                br = new BufferedReader(new FileReader(analog0));
                break;
            case A1:
                br = new BufferedReader(new FileReader(analog1));
                break;
            default:
                throw new IIOException("Unknown analog pin | this is for ARTIK5 for now");
        }
        return Integer.parseInt(br.readLine());
    }

    public void initPWM(PWM_PIN pin, long periodNanos, long dutyCycleNanos) throws IOException {
        echoToPath(BASE_PWM_EXPORT_PATH, pin == PWM_PIN.PWM0 ? 0 : 1);
        
        echoToPath(getPwmPath(pin)+"/period", periodNanos);
        echoToPath(getPwmPath(pin)+"/duty_cycle", dutyCycleNanos);
    }

    public void enablePwm(PWM_PIN pin,boolean enable) throws IOException {
        echoToPath(getPwmPath(pin) + "/enable", enable?1:0);
    }


    public void releasePwm(PWM_PIN pin) throws IOException {
        echoToPath(BASE_PWM_UNEXPORT_PATH, pin == PWM_PIN.PWM0 ? 0 : 1);
    }

    private String getPwmPath(PWM_PIN pin) {
        return BASE_PWM_PATH + pin.toString().toLowerCase();
    }

    public static void main(String[] args) throws IOException, InterruptedException {
        ArtikGPIO artikGPIO = new ArtikGPIO(ARTIK_MODELS.ARTIK5);

//        artikGPIO.initPin(ArtikGPIO.PIN_DIRECTION.OUT, ArtikGPIO.ARTIK_PINS.J26pin5);

//        artikGPIO.initPin(ArtikGPIO.PIN_DIRECTION.IN, ArtikGPIO.ARTIK_PINS.J26pin6);
        
        artikGPIO.initPWM(PWM_PIN.PWM0, 1000000, 500000);
        artikGPIO.enablePwm(PWM_PIN.PWM0,true);
        Thread.sleep(2000);
        artikGPIO.enablePwm(PWM_PIN.PWM0,false);
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

}
