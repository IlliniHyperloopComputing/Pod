import { Injectable } from '@angular/core';

import { HttpClient } from '@angular/common/http';
@Injectable({
  providedIn: 'root'
})
export class StateService {

  constructor(private http: HttpClient) { }

  getState() {
    return this.http.get("http://localhost:8000/api/data/state")
  }
}
