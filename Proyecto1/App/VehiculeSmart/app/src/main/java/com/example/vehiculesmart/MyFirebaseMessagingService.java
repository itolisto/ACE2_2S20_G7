package com.example.vehiculesmart;

import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.widget.Toast;
import androidx.annotation.NonNull;
import com.google.firebase.messaging.FirebaseMessagingService;
import com.google.firebase.messaging.RemoteMessage;

/**
 * clase capas de manejar las notificaciones y manejarla a nuestro antojo
 * tomar en cuenta que gracias a esta clase podemos visualizar las notificaciones en PRIMER PLANO
 * ya que por defecto las notificaciones solo se muestran en segundo plano.
 */

public class MyFirebaseMessagingService extends FirebaseMessagingService {


    @Override
    public void onMessageReceived(@NonNull @org.jetbrains.annotations.NotNull final RemoteMessage remoteMessage) {

        if(remoteMessage == null){
            return;
        }
        Log.i("notificacion*****", remoteMessage.getData().toString()); //.getNotification().getBod()...
    }



}
