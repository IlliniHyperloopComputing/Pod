import React, { Component } from 'react';
import './App.css';

//https://medium.freecodecamp.org/force-refreshing-a-react-child-component-the-easy-way-6cdbb9e6d99c

class App extends Component {
  state = {
    "body": "",
  };

  componentDidMount() {
    this.intervalID = setInterval(
      () => this.tick(),
      20
    );
  }
  componentWillUnmount() {
    clearInterval(this.intervalID);
  }
  tick() {
    this.callApi()
      .then(res => this.setState({ body: res }))
      .catch(err => console.log(err));
  }

  callApi = async () => {
    const response = await fetch('/api/complete');
    const body = await response.json();
    if (response.status !== 200) throw Error(body.message);
    return JSON.stringify(body);
  };

render() {
    return (
      <div className="App">
        <h1>{ this.state.body }</h1>
      </div>
    );
  }
}
export default App;