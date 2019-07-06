import { Injectable } from '@angular/core';
import { Observable } from 'rxjs';
import { HttpClient } from '@angular/common/http';

import { Row } from '../models/stat';

@Injectable({
  providedIn: 'root'
})
export class StatsService {

  constructor(private http: HttpClient) {}

   getStatStatus(): Observable<Row[]> {
     return this.http.get<Row[]>("http://localhost:8000/api/data/stats");
   }
}
