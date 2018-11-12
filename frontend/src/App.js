import React, { Component } from 'react';
import './App.css';
import { Circle } from 'rc-progress';

class App extends Component {
  render() {
    const circleContainerStyle = {
      width: '250px',
      height: '250px',
      display: 'inline-block',
    };
    return (
      <div className="App">
        <header className="App-header"> 
        <div style={circleContainerStyle}>
          <Circle percent={30} strokeWidth={"6"} strokeLineCap={"round"} strokeColor={"#3FC7FA"}></Circle>
        </div>
        </header>
      </div>
    );
  }
}

export default App;
