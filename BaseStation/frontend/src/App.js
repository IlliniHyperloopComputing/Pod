import React, { Component } from 'react';
import { BrowserRouter, Route } from 'react-router-dom';
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
      command: "",
      value: 0,
      command_response: "",
      runSetup: 0,
      dev_data: ""
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
        .get("http://localhost:8000/api/data/latest")
        .then(function(result) {    
          _this.setState({
            dev_data: result.data
          })
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
    axios.post(`http://localhost:8000/api/commands/stop`,)
      .then(res => {
        console.log(res);
        console.log(res.data);
      }).catch(function (error) {
        console.log("Stop failed!!")
      })
  }

  readyPressed() {
    axios.post(`http://localhost:8000/api/commands/ready`,)
      .then(res => {
        console.log(res);
        console.log(res.data);
      }).catch(function (error) {
        console.log("Ready failed!!")
      })
  }

  handleCommandChange = (event) => {
    this.setState({
      command: event.target.value
    });
  }

  handleValueChange = (event) => {
    this.setState({
      value: event.target.value
    });
  }

  handleCommandSubmit = (event) => {
    axios.post(`http://localhost:8000/api/commands/dev`, {
      command: this.state.command,
      value: this.state.value
    })
      .then(res => {
        this.setState({
          command_response: res.data
        })
      }).catch(function (error) {
        console.log("Command send failed!!")
      })
  }

  render() {
    const { dev_data, speedPercent, speedColor, posPercent, posColor, accelColor, accelPercent, connectionText, command_response } = this.state;
    return (
      <div className="App">
        <header className="App-header">
          <BrowserRouter>
            <div className="Main-Content">
              <Route path="/dev" render={(props) => {
                return(
                  <div>
                    <p>{ dev_data }</p>
                    <p>{ command_response }</p>
                    <form id="Command-Form">
                      <select form="Command-Form" onChange={this.handleCommandChange}>
                        { /* Adjust this to add commands to send */ }
                        <option value={-1}>Select Command</option>
                        <option value={0}>TRANS_SAFE_MODE</option>
                        <option value={1}>TRANS_FUNCTIONAL_TEST</option>
                        <option value={6}>ENABLE_MOTOR</option>
                        <option value={7}>DISABLE_MOTOR</option>
                        <option value={8}>SET_MOTOR_SPEED</option>
                        <option value={26}>SET_HV_RELAY_HV_POLE</option>
                        <option value={27}>SET_HV_RELAY_LV_POLE</option>
                        <option value={28}>SET_HV_RELAY_PRE_CHARGE</option>
                      </select>
                      Value: <input type="number" name="value" onChange={this.handleValueChange}></input><br></br>
                      <input type="button" value="Submit" onClick={this.handleCommandSubmit}></input>
                    </form>
                  </div>
                );
              }}></Route>
              <Route exact path="/" render={(props) => {
                return(
                  <div>
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
                  </div>
                );
              }}></Route>
            </div>
          </BrowserRouter>
        </header>
      </div>
    );
  }
}

export default App;
