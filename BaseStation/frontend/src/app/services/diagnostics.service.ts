import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Diagnostics } from '../models/diagnostics';

@Injectable({
  providedIn: 'root'
})
export class DiagnosticsService {
  httpOptions = {
    headers: new HttpHeaders({
      'Content-Type': 'application/json'
    })}

  constructor(private http:HttpClient) { }

  sendStatus(diagnostic:Diagnostics) {
    const toSend = {
      "command":diagnostic.currCommand,
      "value":diagnostic.currValue
    }
    console.log(toSend)
    this.http.post("http://localhost:8000/api/commands/dev", toSend, this.httpOptions).subscribe();
  }
}
