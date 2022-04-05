// import the necessary components
import React, { useState, useEffect } from "react";
import {
  Image,
  SafeAreaView,
  StyleSheet,
  Text,
  View,
  TouchableOpacity,
  Platform,
  PermissionsAndroid,
} from "react-native";
import { Camera } from "expo-camera";
import { useWindowDimensions } from "react-native";
import { MaterialIcons } from "@expo/vector-icons";
import { backgroundColor } from "react-native/Libraries/Components/View/ReactNativeStyleAttributes";
import { clickProps } from "react-native-web/dist/cjs/modules/forwardedProps";

// declare a basic Application
export default function CameraApp(props) {
  // define variables that save information between UI redraws.
  const [hasPermission, setHasPermission] = useState(null);
  const [lastPhoto, setLastPhoto] = useState(null);
  const [type, setType] = useState(Camera.Constants.Type.back);
  const [viewmode, setVMode] = useState(false);

  let SnapCamera = null;
  const SCREEN_WIDTH = useWindowDimensions().width;
  const SCREEN_HEIGHT = useWindowDimensions().height;

  const styles = StyleSheet.create({
    camera: {
      flex: 1,
    },
    container: {
      flex: 1,
      width: "100%",
      height: "100%",
      backgroundColor:"black"
      
    },
    cameraRow: {
      flex: 6,
      flexDirection: "row",
      alignItems: "flex-end",
    },
    cameraContainer: {
      flex: 6,
    },
    rotateView: {
      flex: 6,
    },
    snapView: {
      flex: 6,
      justifyContent: "center",
    },
    smallPreview: {
      backgroundColor: "black",
      marginBottom: 5,
      marginRight: 5,
      width: 40,
      height: 40,
    },
  });

  // Ask camera permissions after every redraw.
  useEffect(async () => {
    if (hasPermission !== "granted") {
      const { status } = await Camera.requestCameraPermissionsAsync();
      setHasPermission(status);
    }
  });

  // if do not have permission to access camera show notice
  if (hasPermission === null || hasPermission === false) {
    var notice = (
      <View style={{ justifyContent: "center" }}>
        <Text>No Access To Camera</Text>
      </View>
    );
    return notice;
  }

  if (SCREEN_WIDTH > SCREEN_HEIGHT) {
  }

  // Sets preview image in right hand corner of camera component.
  const previewImage = lastPhoto ? <Image source={{uri: lastPhoto}} style={{width: 40, height: 40}}></Image> : <Image></Image>;
  let camui = <Text> Major Failure</Text>;

  // Displays camera view
  camui = (
    <View style={styles.container}>
      <SafeAreaView style={styles.container}>
      <Camera
        style={styles.camera}
        type={type}
        ref={(ref) => (SnapCamera = ref)}
      >
        <View style={styles.cameraContainer}>
          <View style={styles.cameraRow}>
            <View style={styles.rotateView}>
              <TouchableOpacity
                onPress={() => {
                  setType(
                    type === Camera.Constants.Type.back
                      ? Camera.Constants.Type.front
                      : Camera.Constants.Type.back
                  );
                }}
              >
                <MaterialIcons
                  style={styles.flipIcon}
                  name="flip-camera-ios"
                  size={32}
                  color="white"
                />
              </TouchableOpacity>
            </View>
            <View style={styles.snapView}>
              <TouchableOpacity
                onPress={async () => {
                  await snap();
                }}
              >
                <MaterialIcons name="camera" size={50} color="white" />
              </TouchableOpacity>
            </View>

            <View style={styles.smallPreview}>
              {previewImage}
            </View>
          </View>
        </View>
      </Camera>
      </SafeAreaView>
    </View>
  );

  // function to take camera picture
  var snap = async () => {
    if (hasPermission) {
      let newList = [...props.list];
      const options = { quality: 0.5, base64: true };
      let photo = await SnapCamera.takePictureAsync(options);
      
      // Sets last taken photo
      setLastPhoto(photo.uri);

      // Pushes picture to list
      newList.push({key: photo.uri, photo:photo.uri, selected: false});
      props.setList(newList);
    }
  };

  return camui;
}
