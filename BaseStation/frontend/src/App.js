import React, { Component } from 'react';
import './App.css';
class App extends Component {
  state = {
    "body": "",
  };

  componentDidMount() {
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