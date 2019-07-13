import { Component, OnInit } from '@angular/core';

import { StateService } from '../../services/state.service';
import { interval } from 'rxjs';

@Component({
  selector: 'app-state',
  templateUrl: './state.component.html',
  styleUrls: ['./state.component.css']
})
export class StateComponent implements OnInit {
  stateName:string = "Disconnected";

  constructor(private stateService: StateService) { 
    interval(100).subscribe(x => {
      this.getValues();
    })
  }

  ngOnInit() {
  }

  getValues() {
    this.stateService.getState().subscribe(
      (data:string) => this.stateName = this.convertNumToName(data),
      error => this.stateName = "Disconnected"
    )
  }

  convertNumToName(stateNum:string):string {
    var toRet = "";
    if (stateNum == "0") {
      toRet = "SAFE_MODE"
    }
    else if (stateNum == "1") {
      toRet = "FUNCTIONAL_TEST_OUTSIDE"
    }
    else if (stateNum == "2") {
      toRet = "LOADING"
    }
    else if (stateNum == "3") {
      toRet = "FUNCTIONAL_TEST_INSIDE"
    }
    else if (stateNum == "4") {
      toRet = "LAUNCH_READY"
    }
    else if (stateNum == "5") {
      toRet = "FLIGHT_ACCEL"
    }
    else if (stateNum == "6") {
      toRet = "FLIGHT_COAST"
    }
    else if (stateNum == "7") {
      toRet = "FLIGHT_BRAKE"
    }
    else if (stateNum == "8") {
      toRet = "FLIGHT_ABORT"
    }
    else {
      toRet = "STATE OVERFLOW"
    }
    return toRet
  }
}
