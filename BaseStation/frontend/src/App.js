import React, { Component } from 'react';
import midwest from './midwest.png';
import { Line, Circle } from 'rc-progress';
import axios from 'axios';
import './App.css';

class App extends Component {
  constructor() {
    super();
    this.state = {
      speedPercent: 30,
      speedColor: '#3FC7FA',
      posPercent: 30,
      posColor: '#3FC7FA',
    };
    this.getColorFor = this.getColorFor.bind(this);
  }

  componentDidMount() {
    this.interval = setInterval(() => this.getInfo(), 500); //Update every half second
  }
  
  getInfo() { //Updating code
    var _this = this;
    this.serverRequest = 
      axios
        .get("http://localhost:8000/podconnect/data/latest")
        .then(function(result) {    
          var XMLParser = require('react-xml-parser');
          var xml = new XMLParser().parseFromString(result.data);
          xml.children[0].children.forEach(element => {
            var name = element.attributes.name;
            if (name === "position") {
              let col = _this.getColorFor(element.value);
              _this.setState({
                posPercent: element.value,
                posColor: col,
              });
            }
            else if (name === "velocity") {
              let col = _this.getColorFor(element.value);
              _this.setState({
                speedPercent: element.value,
                speedColor: col,
              });
            }
          });
        })
  }

  componentWillUnmount() {
    clearInterval(this.interval);
  }

  getColorFor(test) {
    const colorMap = ['#3FC7FA', '#85D262', '#FE8C6A'];
    var temp = parseInt(test,10);
    var col;
    if (temp < 10) {
      col = colorMap[1];
    }
    else if (temp <= 50 && temp > 10) {
      col = colorMap[3];
    }
    else {
      col = colorMap[2];
    }
    return col;
  }

  render() {
    console.log("here");
    const { speedPercent, speedColor, posPercent, posColor } = this.state;
    var speedPercentStr = speedPercent + "";
    var posPercentStr = posPercent + "";
    return (
      <div className="App">
        <header className="App-header">
          <img src={midwest} className="App-logo" alt="logo" />
          <Circle percent={speedPercentStr} className="Speed-Circle" strokeWidth="4" strokeColor={speedColor}></Circle>
          <Line percent={posPercentStr} strokeColor={posColor} className="Pos-Line"></Line>
          <p className="Pos-Text">Position</p>
          <p className="Speed-Text">Velocity</p>
        </header>
      </div>
    );
  }
}

export default App;
