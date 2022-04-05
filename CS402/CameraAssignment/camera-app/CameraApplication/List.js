import React, { useEffect, useState } from 'react';
import { StyleSheet, Text, View, TouchableOpacity, Button, TextInput, VirtualizedList, StatusBar, Image, SafeAreaView } from 'react-native';
import { AntDesign } from '@expo/vector-icons';
import * as MediaLibrary from 'expo-media-library';
import * as Sharing from 'expo-sharing'; 
// List styles
const styles = StyleSheet.create({
  innerButton: {
    marginRight: 10,
    marginBottom: 0,
    width: 90,
  },
  image:{
    flex: 2,
    height: 100,
    justifyContent: 'center',
    alignItems: 'center',
  },
  container:{
    flex: 1,
    justifyContent: 'center',
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
  check:{
    position:"relative", 
    left: 5, 
    top:30, 
    zIndex: 1, 
    padding:0,
    marginTop: -20
  },

  text: {
    fontSize: 22
  },
});


/**
 * Describes an item component
 * @param {*} item - item object containing key and selected
 * @param {*} onPress - Function to execute when list item is pressed
 * @param {*} backgroundColor - Background color to change to on press
 * @param {*} textColor - color of the text
 * @returns Built item functional component
 */
const Item = ({ item, check, onPress}) => (
  <TouchableOpacity onPress={onPress} style={[styles.item]}>
  <View style={{paddingLeft:25, paddingRight: 25}}>
    {check}
    <Image
      style={styles.image}
      source={{ uri: item.photo }}
    ></Image>
  </View>
  </TouchableOpacity>
);

/**
 * Builds up a Virutal List
 * @returns A Virtual list
 */
const VirtualList = (props) => {
  // Ask camera permissions after every redraw.
  useEffect(async () => {
    if (mediaPerm !== "granted") {
      const { status } = await MediaLibrary.requestPermissionsAsync();
      setMediaPerm(status);
    }
  });
     
  // Lets reassign the set list and list to the prop sent in from parent
  const [list, setList] = [props.list, props.setList]
  const [mediaPerm, setMediaPerm] = useState(null);

  // Toggles an item between selected and not selected
  // key : key in list to find
  function toggleItem(index) {
    const newList = [...list];
    let listItem = newList[index];
    listItem.selected = !listItem.selected;
    setList(newList);
  }

  /**
   * Describes how an item should be rendered
   * @param {*} item item object from array
   */
  const renderItem = ({ item, index }) => {

    // Toggles black/white background color depending on selection
    const check = item.selected ? <AntDesign name="checkcircle" size={24} color="orange" style={styles.check} /> : <View></View> 
    return (
      <View
        style={{
          flex: 4,
          flexDirection: "column",
          margin: 3,
        }}
      >
        <Item
          key={index}
          item={item}
          check = {check}
          onPress={() => toggleItem(index)}
        ></Item>
      </View>
    );
  }


  /**
   * Deletes selected items
   */
  function deleteItems() {
    const newList = list.filter((listItem) => !listItem.selected);
    setList(newList);
  }
  
  // Gets the size of the list for internal use in the virtual list
  const getItemCount = (data) => list.length;

  // Gets the item to return to the virtual list so it can perform a render
  const getItem = (data, index) => list[index];

  /**
   * Handler for share button
   */
  async function shareButtonHandler() {
    let selectedList = list.filter((item) => item.selected);
    for(let item of selectedList){
      await Sharing.shareAsync(item.photo);
    }
  }

  /**
   * Handles save button action, checks if any item is selected and only saves that item
   */
  async function saveGalleryButtonHandler() {
    let selectedList = list.filter((item) => item.selected);
    for(let selectedItem of selectedList){
      await MediaLibrary.createAssetAsync(selectedItem.photo);
    }
  }

  /**
   * List of buttons
   */
  const buttons = <View><View style={styles.buttonContainer}>
    <View style={styles.innerButton}>
      <Button title="-"
        onPress={deleteItems} />
    </View>
    <View style={styles.innerButton}>
      <Button title="Save Gallery"
        onPress={saveGalleryButtonHandler} />
    </View>
    <View style={styles.innerButton}>
      <Button title="Share"
        onPress={shareButtonHandler} />
    </View>
  </View>
  </View>

  /**
   * Main virtual list render method
   */
  const VirtualList =
      <View style={styles.container}>
      {buttons}
      <SafeAreaView style={styles.container}>
      <VirtualizedList
        data={list}
        initialNumToRender={4}
        getItemCount={getItemCount}
        getItem={getItem}
        keyExtractor={(item, index) => index}
        renderItem={renderItem}
      />
      </SafeAreaView>
      </View>
  
  return (VirtualList)
}

export default VirtualList;