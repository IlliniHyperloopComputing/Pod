import { Injectable } from '@angular/core';

import { HttpClient, HttpHeaders } from '@angular/common/http';

@Injectable({
  providedIn: 'root'
})
export class ButtonsService {
  httpOptions = {
    headers: new HttpHeaders({
      'Content-Type': 'application/json'
    })
  }

  constructor(private http: HttpClient) { }

  clickLeft() {
    const toSend = {
      "button":"ready"
    }
    console.log(toSend)
    return this.http.post("http://localhost:8000/api/commands/button", toSend, this.httpOptions);
  }

  clickRight() {
    const toSend = {
      "button":"e-stop"
    }
    return this.http.post("http://localhost:8000/api/commands/button", toSend, this.httpOptions);
  }

  getState() {
    return this.http.get<number>("http://localhost:8000/api/data/state");
  }
}
