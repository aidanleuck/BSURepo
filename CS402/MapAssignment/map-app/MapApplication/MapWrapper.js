import React, { useEffect, useState } from 'react';
import { StyleSheet, Text, View} from 'react-native';
import Map from './Map'
import MapList from './List';

const MapWrapper = () =>{
    const [markers, setMarkers] = useState([]);

    return(
        <View>
        <Map markers = {markers}></Map>
        <MapList markers = {markers} setMarkers= {setMarkers}></MapList>
        </View>
    )
}
export default MapWrapper;