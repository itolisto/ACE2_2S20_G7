package com.example.vehiculesmart.Model;

public class VehiculoModePost {

    String modo;

    public String getModo() {
        return modo;
    }

    public void setModo(String modo) {
        this.modo = modo;
    }

    @Override
    public String toString() {
        return "{" +
                "modo='" + modo + '\'' +
                '}';
    }
}
