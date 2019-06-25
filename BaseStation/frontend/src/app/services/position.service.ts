import { Injectable } from '@angular/core';
import { Observable } from 'rxjs';
import { HttpClient } from '@angular/common/http';

import { Position } from '../models/position';

@Injectable({
  providedIn: 'root'
})
export class PositionService {

  constructor(private http: HttpClient) {}

  getPositionStatus(): Observable<Position> {
    return this.http.get<Position>("http://www.mocky.io/v2/5d1184413100000a1e08ccdc");
  }
}
