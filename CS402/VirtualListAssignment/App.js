import React, { useEffect, useState } from 'react';
import { StyleSheet, Text, View, TouchableOpacity, Button, TextInput, VirtualizedList, SafeAreaView } from 'react-native';

// Styles 
const styles = StyleSheet.create({
  innerButton: {
    marginRight: 10,
    marginTop: 75,
    marginBottom: 0,
    width: 90,
  },
  utilButtons:{
    marginRight: 10,
    marginBottom: 0,
    width: 90,
    marginTop: 10
  },
  buttonContainer: {
    flexDirection: 'row',
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

let dataList = [];

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
 * Builds up a FlatList
 * @returns A Flatlist
 */
const VirtualList = () => {

  // States
  const [list, setList] = useState([]);
  useEffect(async () => {
    await loadButtonHandler();
  }, []);

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
  function addButtonHandler(){
    let listCopy = [...list];
    let prevLength = listCopy.length;

    let newList = addItem(listCopy, text);

    if(prevLength === newList.length){
      alert("Duplicate key entry!");
    }

    setList(newList);

    
  }

  /**
   * Adds an item to the lsitf
   */
  function addItem(list, text) {
    let item = {
      key: text,
      selected: false
    }

    // Check for duplicate entries
    let count = list.filter((listItem) => listItem.key === text).length

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

  // Gets the size of the list for internal use in the virtual list
  const getItemCount = (data) => list.length;

  // Gets the item to return to the virtual list so it can perform a render
  const getItem = (index, key) => list[key];

  /**
   * Handler for load button that sets server endpoint and proceeds to load data in the list
   */
  async function loadButtonHandler() {
    let baseUrl = "https://cs.boisestate.edu/~scutchin/cs402/codesnips/loadjson.php"
    let username = "aidanleuck"

    let url = new URL(baseUrl);
    url.searchParams.append("user", username)

    await loadRemoteList(url.toString());
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

      await saveList(url, json)
    }

  }

  /**
   * Loads from a remote url and updates the list with the specified values
   * @param {} url - The url to load data from
   */
  async function loadRemoteList(url) {
    try {

      let response = await fetch(url);
      let data = await response.json();

      let newList = [...list];
      for(const item of data){
        newList = addItem(newList, item.key);
        setList(newList);
      }
    }
    catch (err) {
      console.log(err);
    }

  }

  /**
   * Saves the list to database
   * @param {*} url - The url to save data to
   * @param {*} json - The json to send to the server
   */
  async function saveList(url, json) {
    const requestOptions = {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: json
    };

    try {
      await fetch(url, requestOptions);
    }
    catch (err) {
      console.log(err);
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
      else {
        newList.push(listItem);
      }
    }

    // Adds new item
    let addItem = {};
    addItem.key = selectedItems.join(", ")
    addItem.selected = false;

    // If there was a first selected index splice in
    if (firstSelectedIndex != null) {
      newList.splice(firstSelectedIndex, 0, addItem);
      setList(newList)
    }
  }

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
      <Button title="Split"
        onPress={splitList} />
    </View>
    <View style={styles.innerButton}>
      <Button
        title="Join"
        onPress={joinList} />
    </View>
  </View>
    <View style={styles.buttonContainer}>
      <View style={styles.utilButtons}>
        <Button title="Load List"
          onPress={loadButtonHandler} />
      </View>
      <View style={styles.utilButtons}>
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
      <View>
      {buttons}
      <VirtualizedList
        data={dataList}
        persistentScrollbar={true}
        initialNumToRender={12}
        getItemCount={getItemCount}
        getItem={getItem}
        keyExtractor={(item, index) => index}
        renderItem={renderItem}
      />
      </View>
  return (VirtualList)
}

export default VirtualList;