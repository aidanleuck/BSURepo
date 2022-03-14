import Geocoder from 'react-native-geocoding'
Geocoder.init("AIzaSyDqW8jK0xxnIRKTKXACxIK-q3UerQTiCsA");
let geoLocation = {
  getLocation: async function(location){
        let foundLocation = null;
        try{
            const response = await Geocoder.from(location);
            foundLocation = response.results[0].geometry.location
        }
        catch (err){
            console.log(err);
        }
        return foundLocation;
    }
}

module.exports = geoLocation;