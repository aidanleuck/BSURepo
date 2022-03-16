import React, { useEffect, useState } from 'react';
import { StyleSheet, Text, View} from 'react-native';
import Map from './Map'
import MapList from './List';

const MapWrapper = () =>{
    const [markers, setMarkers] = useState([]);
    const [loaded, setLoaded] = useState(false);
    const [lastSelected, setLastSelected] = useState({})

    return(
        <View style={{flex:1}}>
            <Map markers = {markers} setMarkers={setMarkers} loaded={loaded} lastSelected={lastSelected}></Map>
            <MapList markers = {markers} setMarkers= {setMarkers} setLoaded={setLoaded} setLastSelected={setLastSelected}></MapList>
        </View>
    )
}
export default MapWrapper;