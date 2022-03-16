import React, { useEffect, useState } from 'react';
import { StyleSheet, Text, View, TouchableOpacity, Button, TextInput, VirtualizedList, StatusBar, SafeAreaView } from 'react-native';
import remoteList from './Helpers/remoteList'
import geoLocator from './Helpers/geolocation'

// List styles
const styles = StyleSheet.create({
  innerButton: {
    marginRight: 10,
    marginTop: 10,
    marginBottom: 0,
    width: 90,
  },
  container:{
    flex:1,
    marginTop: StatusBar.currentHeight
  },
  utilButtons:{
    marginRight: 10,
    marginBottom: 0,
    width: 90,
    marginTop: 0
  },
  buttonContainer: {
    flexDirection: 'row',
    width:"100%",
    backgroundColor: 'orange',
    alignItems: 'center',
    justifyContent: 'center',
  },
  textField: {
    height: 40,
    margin: 12,
    width: 310,
    color: 'white',
    borderWidth: 1,
    borderColor: 'white',
    padding: 10,
  },
  textContainer: {
    marginTop: 0,
    backgroundColor: 'orange',
    alignItems: 'center',
    textAlign: 'center',
    justifyContent: 'center'
  },
  item: {
    padding: 5,
    height: 44,
    color: "black",
  },
  text: {
    fontSize: 22
  }
});

/**
 * Describes an item component
 * @param {*} item - item object containing key and selected
 * @param {*} onPress - Function to execute when list item is pressed
 * @param {*} backgroundColor - Background color to change to on press
 * @param {*} textColor - color of the text
 * @returns Built item functional component
 */
const Item = ({ item, onPress, backgroundColor, textColor }) => (
  <TouchableOpacity onPress={onPress} style={[styles.item, backgroundColor]}>
    <Text style={[textColor, styles.text]}>{item.key}</Text>
  </TouchableOpacity>
);

/**
 * Builds up a Virutal List
 * @returns A Virtual list
 */
const VirtualList = (props) => {
  useEffect(async () => {
    await loadButtonHandler();
    props.setLoaded(true);
  }, []);

  // Lets reassign the set list and list to the prop sent in from parent
  const [list, setList] = [props.markers, props.setMarkers];
  const [text, onChangeText] = useState("Search a Location!")


  // Toggles an item between selected and not selected
  // key : key in list to find
  function toggleItem(index) {
    const newList = [...list];
    let listItem = newList[index];
    if (!listItem.selected) {
      props.setLastSelected(listItem);
    }
    listItem.selected = !listItem.selected;
    setList(newList);
  }



  /**
   * Describes how an item should be rendered
   * @param {*} item item object from array
   */
  const renderItem = ({ item, index }) => {

    // Toggles black/white background color depending on selection
    const [backgroundColor, color] = item.selected ? ['black', 'white'] : ['white', 'black'];
    return (
      <Item
        key={index}
        item={item}
        onPress={() => toggleItem(index)}
        backgroundColor={{ backgroundColor }}
        textColor={{ color }}>
      </Item>
    )
  }


  /**
   * Deletes selected items
   */
  function deleteItems() {
    const newList = list.filter((listItem) => !listItem.selected);
    setList(newList);
  }
  async function addButtonHandler() {
    let copyList = [...list];
    let prevLength = copyList.length;
    const locationData = await geoLocator.getLocation(text)

    if (locationData != null) {
      let newList = addLocation(text, locationData);

      if (prevLength === newList.length) {
        alert("Duplicate Location")
      }
    }
    else {
      alert("Failed to find location!");
    }
  }

  /**
 * Adds an item to the lsitf
 */
  function addItem(list, addItem) {
    let item = {
      key: addItem.key,
      selected: false,
      data: addItem.data
    }
    // Check for duplicate entries
    let count = list.filter((listItem) => listItem.key === addItem.key).length

    // If no duplicates
    if (!count) {

      // Find the first selected index
      let addIndex = list.findIndex((listItem) => listItem.selected)

      // If there is a selected index then keep same value otherwise just add to 0
      addIndex = addIndex >= 0 ? addIndex : 0;

      // Splice to add item into the array
      list.splice(addIndex, 0, item);
    }
    return list;
  }

  /**
   * Adds a location
   * @param {*} locationString 
   * @param {*} locationData 
   * @returns 
   */
  function addLocation(locationString, locationData) {
    let locationList = [...list];

    const createdLocationData = { key: locationString, data: { longitude: locationData.lng, latitude: locationData.lat } }
    locationList = addItem(locationList, createdLocationData);
    setList(locationList);
    props.setMarkers(locationList);

    return locationList;
  }

  // Gets the size of the list for internal use in the virtual list
  const getItemCount = (data) => list.length;

  // Gets the item to return to the virtual list so it can perform a render
  const getItem = (data, index) => list[index];

  /**
   * Handler for load button that sets server endpoint and proceeds to load data in the list
   */
  async function loadButtonHandler() {
    let baseUrl = "https://cs.boisestate.edu/~scutchin/cs402/codesnips/loadjson.php"
    let username = "aidanleuck"

    let url = new URL(baseUrl);
    url.searchParams.append("user", username)

    let loadedList = await remoteList.loadRemoteList(url.toString());
    let newList = [];
    for (const item of loadedList) {
      newList.push(item);
    }
    setList(newList);
  }

  /**
   * Handles save button action, checks if any item is selected and only saves that item
   */
  async function saveButtonHandler() {
    if (list.length > 0) {
      let selected = list.filter((listItem) => listItem.selected);
      let json = "";
      if (selected.length > 0) {
        json = JSON.stringify(selected);
      }
      else {
        json = JSON.stringify(list);
      }
      let baseUrl = "https://cs.boisestate.edu/~scutchin/cs402/codesnips/savejson.php"
      let username = "aidanleuck"

      let url = new URL(baseUrl);
      url.searchParams.append("user", username)
      await remoteList.saveList(url, json)
    }

  }

  /**
   * List of buttons
   */
  const buttons = <View><View style={styles.buttonContainer}>
    <View style={styles.innerButton}>
      <Button style title="+"
        onPress={addButtonHandler} />
    </View>
    <View style={styles.innerButton}>
      <Button title="-"
        onPress={deleteItems} />
    </View>
    <View style={styles.innerButton}>
      <Button title="Load List"
        onPress={loadButtonHandler} />
    </View>
    <View style={styles.innerButton}>
      <Button title="Save List"
        onPress={saveButtonHandler} />
    </View>
  </View>

    <View style={styles.textContainer}>
      <TextInput style={styles.textField}
        onChangeText={onChangeText}
        value={text}
      />
    </View>
  </View>

  /**
   * Main virtual list render method
   */
  const VirtualList =
      <SafeAreaView style={styles.container}>
      {buttons}
      <VirtualizedList
        data={list}
        initialNumToRender={4}
        getItemCount={getItemCount}
        getItem={getItem}
        keyExtractor={(item, index) => index}
        renderItem={renderItem}
      />
      </SafeAreaView>
    



  return (VirtualList)
}

export default VirtualList;