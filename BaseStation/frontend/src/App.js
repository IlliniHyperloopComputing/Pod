import React, { Component } from 'react';
import midwest from './midwest.png';
import { Line, Circle } from 'rc-progress';
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
    this.changeState = this.changeState.bind(this);
  }

  changeState() {
    const colorMap = ['#3FC7FA', '#85D262', '#FE8C6A'];
    const speedValue = parseInt(Math.random() * 100, 10);
    const posValue = parseInt(Math.random() * 100, 10);
    this.setState({
      speedPercent: speedValue,
      speedColor: colorMap[parseInt(Math.random() * 3, 10)],
      posPercent: posValue,
    });
  }

  render() {
    const { speedPercent, speedColor, posPercent, posColor } = this.state;
    return (
      <div className="App">
        <header className="App-header">
          <img src={midwest} className="App-logo" alt="logo" />
          <Circle percent={speedPercent} className="Speed-Circle" strokeWidth="4" strokeColor={speedColor}></Circle>
          <Line percent={posPercent} strokeColor={posColor} className="Pos-Line"></Line>
          <button onClick={this.changeState}></button>
        </header>
      </div>
    );
  }
}

export default App;
