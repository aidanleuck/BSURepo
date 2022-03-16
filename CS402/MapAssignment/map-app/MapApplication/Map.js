
// import the necessary packages
import React, { useState, useEffect } from 'react';
import { Alert, Dimensions, VirtualizedList, TouchableOpacity, Button, FlatList, StyleSheet, Text, View } from 'react-native';
import MapView from 'react-native-maps';
import { Marker } from 'react-native-maps';
import { useWindowDimensions } from 'react-native';
import geolocation from './Helpers/geolocation'
import DialogInput from 'react-native-dialog-input';
import Geocoder from 'react-native-geocoding'

// create a style sheet for handling visual appearances, spacing, widths, and colors
const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#fff',
    alignItems: 'center',
    justifyContent: 'center',
  },
  rowblock: {
    height: 80,
    width: 300,
    padding: 5,
    borderWidth: 5,
  },

  map: { width: Dimensions.get('window').width, height: Dimensions.get('window').height / 2 },
  label: { flex: 0.2, fontSize: 22, padding: 5 }
});

// declare our Virtual List App object.
const MapList = (props) => {
  let mapref = React.createRef();
  // the state variables that keep our data available for the User Interface.
  const [myLocation, setLocation] = useState(null);

  /**
   * Handles getting location, waits for the list to be loaded before executing
   */
  useEffect(async () => {
    // Wait for list to load
    if (props.loaded) {
      if (!myLocation) {
        const currLocationInfo = await geolocation.getCurrentLocation();
        setLocation(currLocationInfo);
      }
      else {
        let currMarkers = [...props.markers];
        if (currMarkers.filter((item) => item.key === myLocation.key).length === 0) {
          currMarkers.push(myLocation);
          props.setMarkers(currMarkers);
        }
      }
    }
    else {
      // If location hasn't been grabbed then grab it
      if (!myLocation) {
        const currLocationInfo = await geolocation.getCurrentLocation();
        setLocation(currLocationInfo);
      }
    }
  }, [props.loaded, myLocation])

  // Wait for changes to the last selected property
  useEffect(() => {
    if(props.loaded){
      if(props.lastSelected){
        // Animate to the region once the change has occurred
        mapref.animateToRegion({
          latitude: props.lastSelected.data.latitude, 
          longitude: props.lastSelected.data.longitude, 
          latitudeDelta: 0.0922,
          longitudeDelta: 0.0421
         }, 2000)
      }
    }
  }, [props.lastSelected])

  /**
   * Renders a marker object
   * @param {*} marker - Marker to render
   * @returns marker
   */
  const renderMarker = (marker) => {
    return (
      <Marker
        coordinate={{ latitude: marker.data.latitude, longitude: marker.data.longitude }}
        title={marker.key}
      />
    );
  }

  // Screen width, screen height
  const SCREEN_WIDTH = useWindowDimensions().width;
  const SCREEN_HEIGHT = useWindowDimensions().height;
  var smaps = { width: SCREEN_WIDTH, height: SCREEN_HEIGHT / 2 }
  var mymap = <MapView ref={(mapView) => { mapref = mapView; }} style={smaps} >
    {props.markers.map((marker) => renderMarker(marker))}
  </MapView >

  return (mymap)
}



export default MapList;