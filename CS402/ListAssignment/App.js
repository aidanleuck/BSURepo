import React, { useState } from 'react';
import { FlatList, StyleSheet, Text, View, TouchableOpacity, Button, TextInput } from 'react-native';

// Styles 
const styles = StyleSheet.create({
  innerButton:{
    marginRight: 10,
    marginTop: 75,
    marginBottom: 0,
    width: 75,
  },
  buttonContainer: {
    flexDirection: 'row',
    backgroundColor: 'orange',
    alignItems: 'center',
    justifyContent: 'center',
  },
  textField:{
    height: 40,
    margin: 12,
    width: 310,
    color: 'white',
    borderWidth: 1,
    borderColor: 'white',
    padding: 10,  
  },
  textContainer:{
    marginTop: 0,
    backgroundColor: 'orange',
    alignItems: 'center',
    textAlign: 'center',
    justifyContent: 'center'
  },
  item: {
    padding: 10,
    fontSize: 22,
    height: 44,
    color: "black",
  },
});

// Initial data list
let dataList = [
  { key: 'Devin', selected: false },
  { key: 'Dan', selected: false },
  { key: 'Dominic', selected: false },
  { key: 'Jackson', selected: false },
  { key: 'James', selected: false },
  { key: 'Joel', selected: false },
  { key: 'John', selected: false },
  { key: 'Jillian', selected: false },
  { key: 'Jimmy', selected: false },
  { key: 'Julie', selected: false },
]

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
    <Text style={[textColor]}>{item.key}</Text>
  </TouchableOpacity>
);

/**
 * Builds up a FlatList
 * @returns A Flatlist
 */
const FlatListBasics = () => {

  // States
  const [list, setList] = useState(dataList);
  const [text, onChangeText] = useState("Add an item")


  // Toggles an item between selected and not selected
  // key : key in list to find
  function toggleItem(key) {
    const newList = list.map((listItem) => {
      if (listItem.key === key) {
        listItem.selected = !listItem.selected;
      }
      return listItem;
    })
    setList(newList);
  }

  /**
   * Describes how an item should be rendered
   * @param {*} item item object from array
   */
  const renderItem = ({ item }) => {

    // Toggles black/white background color depending on selection
    const [backgroundColor, color] = item.selected ? ['black', 'white'] : ['white', 'black'];
    return (
      <Item
        item={item}
        onPress={() => toggleItem(item.key)}
        backgroundColor={{ backgroundColor }}
        textColor={{ color }}>
      </Item>
    )
  }

  /**
   * Deletes selected items
   */
  function deleteItems() {
    setList(list.filter((listItem) => !listItem.selected))
  }

  /**
   * Adds an item to the lsit
   */
  function addItem() {
    let item = {
      key: text,
      selected: false
    }

    // Check for duplicate entries
    let count = list.filter((listItem) => listItem.key === text).length
    let newList = [...list];

    // If no duplicates
    if (!count) {

      // Find the first selected index
      let addIndex = list.findIndex((listItem) => listItem.selected)

      // If there is a selected index then keep same value otherwise just add to 0
      addIndex = addIndex >= 0 ? addIndex : 0;

      // Splice to add item into the array
      newList.splice(addIndex, 0, item);
      setList(newList);
    }
    else {
      alert(`Duplicate entry for item "${text}"`);
    }
  }

  /**
   * Splits list items separated by commas
   */
  function splitList() {
    let newList = [];
    for (const listItem of list) {

      // Finds selected items
      if (listItem.selected) {
        let newItems = listItem.key.split(",");

        // Builds up the object and deselects them
        newItems.map((item) => {
          let obj = { key: item.trim(), selected: false }
          newList.push(obj);
        })
      }
      else {
        newList.push(listItem);
      }
    }
    setList(newList);
  }

  /**
   * Joins list items separated by commas
   */
  function joinList() {
    let newList = [];
    let selectedItems = [];
    let firstSelectedIndex = null;

    for (let i = 0; i < list.length; i++) {
      const listItem = list[i];

      // Gets selected items and saves the first found index
      if (listItem.selected) {
        firstSelectedIndex = firstSelectedIndex ?? i;
        selectedItems.push(listItem.key);
      }
      else{
        newList.push(listItem);
      }
    }

    // Adds new item
    let addItem = {};
    addItem.key = selectedItems.join(", ")
    addItem.selected = false;

    // If there was a first selected index splice in
    if(firstSelectedIndex != null){
      newList.splice(firstSelectedIndex, 0, addItem);
      setList(newList)
    }
  }

  /**
   * Main flat list render method
   */
  const flatList = <View>
    <View style={styles.buttonContainer}>
      <View style={styles.innerButton}>
      <Button style title="+"
        onPress={addItem} />
      </View>
      <View style={styles.innerButton}>
      <Button title="-"
        onPress={deleteItems} />
      </View>
      <View style={styles.innerButton}>
      <Button title="Split"
        onPress={splitList} />
      </View>
      <View style={styles.innerButton}>
      <Button 
        title="Join"
        onPress={joinList} />
      </View>
    </View>
    <View style={styles.textContainer}>
    <TextInput style={styles.textField}
        onChangeText={onChangeText}
        value={text}
      />
    </View>
    <FlatList data={list}
      renderItem={renderItem}

    />
  </View>
  return (flatList)
}

export default FlatListBasics;