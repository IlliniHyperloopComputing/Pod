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
     return this.http.get<Stat[]>("http://www.mocky.io/v2/5d117e513100000a1e08ccc3");
   }
}
