package com.example.vehiculesmart;

import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import com.example.vehiculesmart.Interface.VehiculeApi;
import com.example.vehiculesmart.Model.VehiculeResponse;
import com.example.vehiculesmart.Model.VehiculoModePost;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.messaging.FirebaseMessaging;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.moshi.MoshiConverterFactory;

import static android.util.Log.println;

public class MainActivity extends AppCompatActivity {

    private TextView mJsonTxtView;
    private Button btnActivar, btnDesactivar, btnActualizar;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mJsonTxtView = findViewById(R.id.jsonText);
        getVehicule();

        btnActualizar = (Button) findViewById(R.id.buttonActualizar);
        btnActivar = (Button) findViewById(R.id.buttonActivar);
        btnDesactivar = (Button) findViewById(R.id.buttonDesactivar);

        btnActualizar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                getVehicule();
            }
        });

        btnActivar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                postMode("activo");
            }
        });

        btnDesactivar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                postMode("inactivo");
            }
        });

        suscribirUsuario();

        //para que yo recupere la info y genere la push notification
//        String accion = getIntent().getStringExtra("accion");
//        if(accion != null){
//
//        }

        //que el envie la push  notification de una

        //                  "                           con data y la recupero

        //                  "                           con esa cadena de string,,,, separados por [,]

    }


    private void suscribirUsuario(){
        //suscribir este usuario a un tema FIREBASE
        FirebaseMessaging.getInstance().subscribeToTopic("grupoVehiculo")
                .addOnCompleteListener(new OnCompleteListener<Void>() {
                    @Override
                    public void onComplete(@NonNull Task<Void> task) {
                        String msg = "****** Suscrito ******";
                        if (!task.isSuccessful()) {
                            msg = "****** Suscripccion fallida ******";
                        }
                        Toast.makeText(MainActivity.this, msg, Toast.LENGTH_SHORT).show();
                    }
                });
    }

    private void getVehicule(){
        Retrofit retrofit = new Retrofit.Builder()
                .baseUrl("http://167.99.237.132:3600/")
                .addConverterFactory(MoshiConverterFactory.create())
                .build();


        VehiculeApi vehiculeApi = retrofit.create(VehiculeApi.class);
        Call<VehiculeResponse>  call = vehiculeApi.getVehicule();

        call.enqueue(new Callback<VehiculeResponse>() {
            @Override
            public void onResponse(Call<VehiculeResponse> call, Response<VehiculeResponse> response) {
                if(!response.isSuccessful()){
                    mJsonTxtView.setText("Codigo: "+response.code());
                    return;
                }

                mJsonTxtView.setText("");

                VehiculeResponse vehicule = response.body();
                mJsonTxtView.append("Estado: " + vehicule.getEstado() + "\n");
                mJsonTxtView.append("Ubicacion: " + vehicule.getUbicacion() + "\n");
                mJsonTxtView.append("Tiempo Promedio: " + vehicule.getPeso_promedio() + "\n");
                mJsonTxtView.append("Tiempo Promedio Ida: " + vehicule.getTiempo_promedio_ida() + "\n");
                mJsonTxtView.append("Tiempo Promedio Regreso: " + vehicule.getTiempo_promedio_regreso() + "\n");
                mJsonTxtView.append("Total Contador: " + vehicule.getTotal_contador() + "\n");
                mJsonTxtView.append("Total Obstaculos: " + vehicule.getTotal_obstaculos() + "\n");
            }

            @Override
            public void onFailure(Call<VehiculeResponse> call, Throwable t) {
                mJsonTxtView.setText("Mensaje: "+t.getMessage());
            }
        });
    }

    private void postMode(String modo){
        Retrofit retrofit = new Retrofit.Builder()
                .baseUrl("http://167.99.237.132:3600/")
                .addConverterFactory(MoshiConverterFactory.create())
                .build();

        VehiculeApi vehiculeApi = retrofit.create(VehiculeApi.class);

        VehiculoModePost vehiculomode = new VehiculoModePost();
        vehiculomode.setModo(modo);

        Call<VehiculoModePost> call = vehiculeApi.sendPosts(vehiculomode);

        call.enqueue(new Callback<VehiculoModePost>() {
            @Override
            public void onResponse(Call<VehiculoModePost> call, Response<VehiculoModePost> response) {
                //respuesta del post, pero en este caso este post no tiene respuesta
                //lblresponse.setText(response.body().toString());
                //o mostrar un mensaje de "solicitud enviada correctamente"
            }

            @Override
            public void onFailure(Call<VehiculoModePost> call, Throwable t) {
                Toast.makeText(getApplicationContext(), t.toString(), Toast.LENGTH_LONG).show();
            }
        });
    }


}