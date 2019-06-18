import { Injectable } from '@angular/core';
import { Observable, of } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class BatteryService {

  constructor() { }

  getBatteryStatus(): Observable<string> {
    const value:string = "80";

    return of(value);
  }
}
