package com.example.vehiculesmart.Model;

public class VehiculeResponse {

    private String ubicacion;
    private String estado;
    private int total_contador;
    private int total_obstaculos;
    private int peso_promedio;
    private int tiempo_promedio_ida;
    private int tiempo_promedio_regreso;

    public String getUbicacion() {
        return ubicacion;
    }

    public void setUbicacion(String ubicacion) {
        this.ubicacion = ubicacion;
    }

    public String getEstado() {
        return estado;
    }

    public void setEstado(String estado) {
        this.estado = estado;
    }

    public int getTotal_contador() {
        return total_contador;
    }

    public void setTotal_contador(int total_contador) {
        this.total_contador = total_contador;
    }

    public int getTotal_obstaculos() {
        return total_obstaculos;
    }

    public void setTotal_obstaculos(int total_obstaculos) {
        this.total_obstaculos = total_obstaculos;
    }

    public int getPeso_promedio() {
        return peso_promedio;
    }

    public void setPeso_promedio(int peso_promedio) {
        this.peso_promedio = peso_promedio;
    }

    public int getTiempo_promedio_ida() {
        return tiempo_promedio_ida;
    }

    public void setTiempo_promedio_ida(int tiempo_promedio_ida) {
        this.tiempo_promedio_ida = tiempo_promedio_ida;
    }

    public int getTiempo_promedio_regreso() {
        return tiempo_promedio_regreso;
    }

    public void setTiempo_promedio_regreso(int tiempo_promedio_regreso) {
        this.tiempo_promedio_regreso = tiempo_promedio_regreso;
    }
}
