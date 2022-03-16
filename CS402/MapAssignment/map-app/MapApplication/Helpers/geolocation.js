import Geocoder from 'react-native-geocoding'
import * as Location from 'expo-location';
import { Marker } from 'react-native-maps';

Geocoder.init("AIzaSyDqW8jK0xxnIRKTKXACxIK-q3UerQTiCsA");
let geoLocation = {

    /**
     * Retrieves a location specified by the given string
     * @param {*} location - string to find location of
     * @returns - Location of place
     */
    getLocation: async function (location) {
        let foundLocation = null;
        try {
            // Runs name through geocoder
            const response = await Geocoder.from(location);
            
            // Returns the first response
            foundLocation = response.results[0].geometry.location
        }
        catch (err) {
            console.log(err);
        }
        return foundLocation;
    },

    /**
     * Retrieves the current location of the user.
     * @returns - Current location
     */
    getCurrentLocation: async function () {
        // Asks user for permission
        let { status } = await Location.requestForegroundPermissionsAsync();
        let locationResult = null;

        if (status !== 'granted') {
            console.log('Permission to access location was denied');

        } else {
            locationResult = {};
            try {
                // Retrieves position if granted
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