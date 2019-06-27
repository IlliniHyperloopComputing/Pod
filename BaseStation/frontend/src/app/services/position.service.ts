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
    return this.http.get<Position>("http://localhost:8000/api/data/position");
  }
}
