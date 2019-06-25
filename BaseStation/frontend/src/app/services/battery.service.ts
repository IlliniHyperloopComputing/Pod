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
    return this.http.get<Battery>("http://www.mocky.io/v2/5d118051310000231908cccc");
  }
}
