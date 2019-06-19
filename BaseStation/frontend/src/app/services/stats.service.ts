import { Injectable } from '@angular/core';
import { Observable, of } from 'rxjs';

import { stat } from '../models/stat';

@Injectable({
  providedIn: 'root'
})
export class StatsService {

  constructor() {

   }

   getStatStatus(): Observable<stat[]> {
      const stats:stat[] = [{
        name: "Acceleration",
        value: 5
      },
    {
      name: "Velocity",
      value: 10
    }]

    return of(stats);
   }
}
