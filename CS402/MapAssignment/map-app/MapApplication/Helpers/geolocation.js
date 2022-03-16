import Geocoder from 'react-native-geocoding'
import * as Location from 'expo-location';
import { Marker } from 'react-native-maps';

Geocoder.init("AIzaSyDqW8jK0xxnIRKTKXACxIK-q3UerQTiCsA");
let geoLocation = {
    getLocation: async function (location) {
        let foundLocation = null;
        try {
            const response = await Geocoder.from(location);
            foundLocation = response.results[0].geometry.location
        }
        catch (err) {
            console.log(err);
        }
        return foundLocation;
    },
    getCurrentLocation: async function (setLocation) {
        let { status } = await Location.requestForegroundPermissionsAsync();
        let locationResult = null;

        if (status !== 'granted') {
            console.log('Permission to access location was denied');

        } else {
            locationResult = {};
            try {
                let foundLocation = await Location.getCurrentPositionAsync();
                const [latitude, longitude] = [foundLocation.coords.latitude, foundLocation.coords.longitude];
                let locationInfo = await Geocoder.from(latitude, longitude);
                let locationName = locationInfo.results[0].formatted_address;

                locationResult = {key: locationName, data:{latitude: latitude, longitude: longitude}};
            }
            catch(err){
                console.log("Error occured reading location!" + err);
            }
        }
        return locationResult;
    }
}

module.exports = geoLocation;