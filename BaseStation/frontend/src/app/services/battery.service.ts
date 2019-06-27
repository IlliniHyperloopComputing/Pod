import { Injectable } from '@angular/core';
import { Observable, of } from 'rxjs';
import { HttpClient } from '@angular/common/http';

import { Battery } from '../models/battery';

@Injectable({
  providedIn: 'root'
})
export class BatteryService {

  constructor(private http: HttpClient) { }

  getBatteryStatus(): Observable<Battery> {
    return this.http.get<Battery>("http://localhost:8000/api/data/battery");
  }
}
