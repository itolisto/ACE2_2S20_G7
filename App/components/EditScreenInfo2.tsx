import * as WebBrowser from "expo-web-browser";
import React, { useState, useEffect } from "react";
import {
  StyleSheet,
  TouchableOpacity,
  Button,
  Alert,
  ActivityIndicator,
  FlatList,
} from "react-native";

import Colors from "../constants/Colors";
import { MonoText } from "./StyledText";
//FIXME: View
import { Text, View } from "./Themed";
import axios from "axios";
import { SafeAreaView } from "react-native-safe-area-context";

export default function EditScreenInfo2({ path }: { path: string }) {
  const [isLoading, setLoading] = useState(true);
  const [data, setData] = useState([]);

  const [isBajo, setBajo] = useState(true);
  const [isMedio, setMedio] = useState(true);
  const [isAlto, setAlto] = useState(true);

  useEffect(() => {
    fetch("http://167.99.237.132:3600/liquido")
      .then((response) => response.json())
      .then((json) => {
        setData(json.nivel);
        if (json.nivel == "bajo") {
          setBajo(false);
          Alert.alert("Hey bro!", "There's not enough liquid only 10% or less");
        } else if (json.nivel == "medio") {
          setMedio(false);
        } else {
          //alto
          setAlto(false);
        }
      })
      .catch((error) => console.error(error))
      .finally(() => {
        setLoading(false);
      });
  }, []);

  return (
    <View>
      <View style={styles.getStartedContainer}>
        <Text
          style={styles.getStartedText}
          lightColor="rgba(0,0,0,0.8)"
          darkColor="rgba(255,255,255,0.8)"
        ></Text>

        <View style={styles.getStartedContainer}>
          <Text
            style={styles.getStartedText}
            lightColor="rgba(0,0,0,0.8)"
            darkColor="rgba(255,255,255,0.8)"
          >
            alto [100%-51%]
          </Text>
          <Text
            style={styles.getStartedText}
            lightColor="rgba(0,0,0,0.8)"
            darkColor="rgba(255,255,255,0.8)"
          >
            medio[50%-11%]
          </Text>
          <Text
            style={styles.getStartedText}
            lightColor="rgba(0,0,0,0.8)"
            darkColor="rgba(255,255,255,0.8)"
          >
            bajo[10% or less]
          </Text>
        </View>
      </View>

      {isLoading ? (
        <ActivityIndicator />
      ) : (
        <View style={{ flex: 1, padding: 24 }}>
          <Text
            style={styles.getStartedText}
            lightColor="rgba(0,0,0,0.8)"
            darkColor="rgba(255,255,255,0.8)"
          >
            Status of your sprayer:
          </Text>

          {/*isLoading ? <ActivityIndicator/> : (
                  <Text style={styles.flipResultText}>{data}</Text>
            )*/}
          {isAlto ? null : (
            <Text style={styles.flipResultTextAlto}>{data}</Text>
          )}
          {isMedio ? null : (
            <Text style={styles.flipResultTextMedio}>{data}</Text>
          )}
          {isBajo ? null : (
            <Text style={styles.flipResultTextBajo}>{data}</Text>
          )}
        </View>
      )}
    </View>
  );
}

/*
      <View style={styles.buttonContainer}>
        <Button
          title="Get Data :D"
          onPress={() => fetchData()}
          color="black"
          accessibilityLabel="Fetch heads or tails"
        />
      </View>
      
const fetchData = () => {
  console.log('flipped')
  fetch("http://167.99.237.132:3600/bitacora")
  .then(response => response.json())
  .then((responseJson) => {
      console.log('getting data from fetch', responseJson) 
  })
  .catch(error => console.log(error))
};


*/

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: "#fff",
  },
  developmentModeText: {
    marginBottom: 20,
    fontSize: 14,
    lineHeight: 19,
    textAlign: "center",
  },
  contentContainer: {
    paddingTop: 30,
  },
  welcomeContainer: {
    alignItems: "center",
    marginTop: 10,
    marginBottom: 20,
  },
  welcomeImage: {
    width: 100,
    height: 80,
    resizeMode: "contain",
    marginTop: 3,
    marginLeft: -10,
  },
  getStartedContainer: {
    alignItems: "center",
    marginHorizontal: 50,
  },
  homeScreenFilename: {
    marginVertical: 7,
  },
  codeHighlightText: {
    color: "rgba(96,100,109, 0.8)",
  },
  codeHighlightContainer: {
    borderRadius: 3,
    paddingHorizontal: 4,
  },
  getStartedText: {
    fontSize: 17,
    lineHeight: 24,
    textAlign: "center",
  },
  helpContainer: {
    marginTop: 15,
    marginHorizontal: 20,
    alignItems: "center",
  },
  helpLink: {
    paddingVertical: 15,
  },
  helpLinkText: {
    textAlign: "center",
  },
  buttonContainer: {
    backgroundColor: "#1a1aff",
    marginTop: 16,
    marginBottom: 16,
    borderRadius: 5,
    paddingVertical: 4,
    paddingHorizontal: 5,
  },
  flipResultText: {
    fontSize: 35,
    color: "#FF0000",
    textAlign: "center",
    fontWeight: "700",
  },
  prueba: {
    padding: 24,
  },
  flipResultTextBajo: {
    fontSize: 35,
    color: "#FF0000",
    textAlign: "center",
    fontWeight: "700",
  },
  flipResultTextMedio: {
    fontSize: 35,
    color: "#FFFF00",
    textAlign: "center",
    fontWeight: "700",
  },
  flipResultTextAlto: {
    fontSize: 35,
    color: "#00FF00",
    textAlign: "center",
    fontWeight: "700",
  },
});
