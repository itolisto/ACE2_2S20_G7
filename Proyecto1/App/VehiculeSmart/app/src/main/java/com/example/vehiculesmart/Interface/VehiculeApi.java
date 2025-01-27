package com.example.vehiculesmart.Interface;

import com.example.vehiculesmart.Model.Global;
import com.example.vehiculesmart.Model.VehiculeResponse;
import com.example.vehiculesmart.Model.VehiculoModePost;
import retrofit2.Call;
import retrofit2.http.Body;
import retrofit2.http.GET;
import retrofit2.http.Headers;
import retrofit2.http.POST;

public interface VehiculeApi {

    @GET("dashboard")
    Call<VehiculeResponse> getVehicule();

    @GET("safebox/dashboard")
    Call<Global> getGlobal();


    @GET("safebox/dashboarddia")
    Call<Global> getDiario();

    @Headers({"Content-type: application/json"})
    @POST("vehiculo/modo")
    Call<VehiculoModePost> sendPosts(@Body VehiculoModePost posts);
}
