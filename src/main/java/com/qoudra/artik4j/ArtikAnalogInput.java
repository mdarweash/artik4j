/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.qoudra.artik4j;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import javax.imageio.IIOException;

/**
 *
 * @author mdarweash
 */
public class ArtikAnalogInput {

    public enum ANALOG_PIN {

        A0, A1
    };
    static File analog0, analog1;

    static {
        analog0 = new File("/sys/devices/126c0000.adc/iio:device0/in_voltage0_raw");
        analog1 = new File("/sys/devices/126c0000.adc/iio:device0/in_voltage1_raw");
    }

    static public int getAnalogRead(ANALOG_PIN pin) throws IOException {
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
}
