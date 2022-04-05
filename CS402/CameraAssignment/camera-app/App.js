
// import the necessary packages
import React, {useState,useEffect} from 'react';
import { View } from 'react-native';
import Camera from './CameraApplication/Camera'
import CameraList from './CameraApplication/List'


function render(){
  const [list, setList] = useState([])
  return (
    <View style={{ flex: 1 }}>
      <Camera list={list} setList={setList}></Camera>
      <CameraList list={list} setList={setList}></CameraList>
    </View>
  );
}

export default render;