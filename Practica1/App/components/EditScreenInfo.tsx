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
import { Text, View } from "./Themed";
import axios from "axios";

export default function EditScreenInfo({ path }: { path: string }) {
  const [isLoading, setLoading] = useState(true);
  const [data, setData] = useState([]);
  const [movies, setMovies] = useState([]);

  useEffect(() => {
    fetch("http://167.99.237.132:3600/bitacora")
      .then((response) => response.json())
      .then((json) => {
        setData(json.existe);
        if (json.existe == 1) {
          Alert.alert(
            "Hey bro!",
            "There's an object in  your smart mailbox :)"
          );
        }
      })
      .catch((error) => console.error(error))
      .finally(() => setLoading(false));

    fetch("http://167.99.237.132:3600/bitacora")
      .then((response) => response.json())
      .then((json) => setMovies(json.peso))
      .catch((error) => console.error(error))
      .finally(() => setLoading(false));
  }, []);

  return (
    <View>
      <View style={styles.getStartedContainer}>
        <Text
          style={styles.getStartedText}
          lightColor="rgba(0,0,0,0.8)"
          darkColor="rgba(255,255,255,0.8)"
        >
          Is there an object?
        </Text>

        <Text
          style={styles.getStartedText}
          lightColor="rgba(0,0,0,0.8)"
          darkColor="rgba(255,255,255,0.8)"
        >
          yes = 1
        </Text>
        <Text
          style={styles.getStartedText}
          lightColor="rgba(0,0,0,0.8)"
          darkColor="rgba(255,255,255,0.8)"
        >
          no = 0
        </Text>
      </View>

      <View style={{ flex: 1, padding: 24 }}>
        {isLoading ? (
          <ActivityIndicator />
        ) : (
          <Text style={styles.flipResultText}>{data}</Text>
        )}
      </View>

      <View style={styles.getStartedContainer}>
        <Text
          style={styles.getStartedText}
          lightColor="rgba(0,0,0,0.8)"
          darkColor="rgba(255,255,255,0.8)"
        >
          Object Weight:
        </Text>
      </View>

      <View style={{ flex: 1, padding: 24 }}>
        {isLoading ? (
          <ActivityIndicator />
        ) : (
          <Text style={styles.flipResultText}>{movies}</Text>
        )}
      </View>
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
};
*/

/*


      <View style={{ flex: 1, padding: 24 }}>


<FlatList
          data={data}
          renderItem={({ item }) => (
            <Text>
              {item.existe}, {item.peso}
            </Text>
          )}
          //keyExtractor={(item, index) => index}
        />

      </View>



*/

/*



      


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
});
