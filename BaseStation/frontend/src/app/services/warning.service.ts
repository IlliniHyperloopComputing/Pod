import { Injectable } from '@angular/core';
import { Observable, of } from 'rxjs';

import { warningInfo } from '../models/warning';

@Injectable({
  providedIn: 'root'
})
export class WarningService {

  constructor() { }

  getWarningStatus(): Observable<warningInfo> {
    const warn:warningInfo = {
      warnings: ["Something", "Not so bad!"],
      topWarning: "Not so bad!",
      warningColor: "LimeGreen",
      textColor: "black"
    }

    return of(warn);
  }
}
