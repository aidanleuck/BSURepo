
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

  // the state variables that keep our data available for the User Interface.
  const [list, setlist] = useState([]);
  const [mylocation, setLocation] = useState();
  const [mypos, setPosition] = useState();
  useEffect(async () => {
   let locationInfo = await geolocation.getCurrentLocation(setLocation);
 }, [])

  const renderMarker = (marker) => {
    return(
    <Marker
      coordinate={{ latitude: marker.data.latitude, longitude: marker.data.longitude }}
      title={marker.key}
    />
    );
  }

  const mapref = React.createRef();
  const SCREEN_WIDTH = useWindowDimensions().width;
  const SCREEN_HEIGHT = useWindowDimensions().height;
  var smaps = { width: SCREEN_WIDTH, height: SCREEN_HEIGHT / 2 }
  var mymap = <MapView ref={mapref} style={smaps} >
    {mylocation}
    {props.markers.map((marker) => renderMarker(marker))}
  </MapView >


  return (mymap)

}



export default MapList;