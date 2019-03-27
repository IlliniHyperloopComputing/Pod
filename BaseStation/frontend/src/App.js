import React, { Component } from 'react';
import midwest from './midwest.png';
import { Line, Circle } from 'rc-progress';
import axios from 'axios';
import './App.css';

class App extends Component {
  constructor() {
    super();
    this.state = {
      speedPercent: "0",
      speedColor: '#3FC7FA',
      posPercent: "0",
      posColor: '#3FC7FA',
      accelPercent: "0",
      accelColor: '#3FC7FA',
      connectionText: "",
    };
    this.getColorFor = this.getColorFor.bind(this);
    this.stopPressed = this.stopPressed.bind(this);
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
            else if (name === "acceleration") {
              let col = _this.getColorFor(element.value);
              _this.setState({
                accelPercent: element.value,
                accelColor: col,
              });
            }
            _this.setState({
              connectionText: "",
            });
          });
        }).catch(function (error) {
          _this.setState({
            connectionText: "BACKEND CONNECTION LOST",
          });
          console.log("caught")
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

  stopPressed() {
    axios.post(`http://localhost:8000/podconnect/commands/stop`,)
      .then(res => {
        console.log(res);
        console.log(res.data);
      }).catch(function (error) {
        console.log("Stop failed!!")
      })
  }

  readyPressed() {
    axios.post(`http://localhost:8000/podconnect/commands/ready`,)
      .then(res => {
        console.log(res);
        console.log(res.data);
      }).catch(function (error) {
        console.log("Ready failed!!")
      })
  }

  render() {
    const { speedPercent, speedColor, posPercent, posColor, accelColor, accelPercent, connectionText } = this.state;
    return (
      <div className="App">
        <header className="App-header">
          <img src={midwest} className="App-logo" alt="logo" />
          <Circle percent={speedPercent} className="Speed-Circle" strokeWidth="4" strokeColor={speedColor}></Circle>
          <Circle percent={accelPercent} className="Accel-Circle" strokeWidth="4" strokeColor={accelColor}></Circle>
          <Line percent={posPercent} strokeColor={posColor} className="Pos-Line"></Line>
          <p className="Pos-Text">Position</p>
          <p className="Speed-Text">Velocity</p>
          <p className="Accel-Text">Acceleration</p>
          <p className="Connection-Text">{ connectionText }</p>
          <button className="Stop-Button"  onClick={() => this.stopPressed()}>STOP</button>
          <button className="Ready-Button" onClick={() => this.readyPressed()}>Ready</button>
        </header>
      </div>
    );
  }
}

export default App;
