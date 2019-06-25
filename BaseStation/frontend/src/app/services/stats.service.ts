import { Injectable } from '@angular/core';
import { Observable } from 'rxjs';
import { HttpClient } from '@angular/common/http';

import { Stat } from '../models/stat';

@Injectable({
  providedIn: 'root'
})
export class StatsService {

  constructor(private http: HttpClient) {}

   getStatStatus(): Observable<Stat[]> {
     return this.http.get<Stat[]>("http://localhost:8000/api/commands/test");
   }
}
