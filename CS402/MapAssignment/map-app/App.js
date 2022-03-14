
// import the necessary packages
import React, {useState,useEffect} from 'react';
import {Alert, Dimensions, VirtualizedList, TouchableOpacity, Button, FlatList, StyleSheet, Text, View } from 'react-native';
import MapView from 'react-native-maps';
import {Marker} from 'react-native-maps';
import { useWindowDimensions } from 'react-native';
import DialogInput from 'react-native-dialog-input';
import Geocoder from 'react-native-geocoding'
import MapWrapper from './MapApplication/MapWrapper'

function render(){
  return(
    <MapWrapper></MapWrapper>
  );
}

export default render;