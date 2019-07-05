import { Component, OnInit } from '@angular/core';

import { Diagnostics, Command } from '../../models/diagnostics';
import { DiagnosticsService } from '../../services/diagnostics.service';

@Component({
  selector: 'app-diagnostics',
  templateUrl: './diagnostics.component.html',
  styleUrls: ['./diagnostics.component.css']
})
export class DiagnosticsComponent implements OnInit {
  data:Diagnostics;
  sentText:string = "";

  constructor(private ds: DiagnosticsService) { }

  ngOnInit() {
    this.data = {
      "commands": [{
        "name":"TRANS_SAFE_MODE",
        "value":0
      }, 
      {
        "name":"TRANS_FUNCTIONAL_TEST",
        "value":1
      },
      {
        "name":"TRANS_LOADING",
        "value":2
      },
      {
        "name":"TRANS_LAUNCH_READY",
        "value":3
      },
      {
        "name":"LAUNCH",
        "value":4
      },
      {
        "name":"EMERGENCY_BRAKE",
        "value":5
      },
      {
        "name":"ENABLE_MOTOR",
        "value":6
      },
      {
        "name":"DISABLE_MOTOR",
        "value":7
      },
      {
        "name":"SET_MOTOR_SPEED",
        "value":8
      },
      {
        "name":"ENABLE_BRAKE",
        "value":9
      },
      {
        "name":"DISABLE_BRAKE",
        "value":10
      },
      {
        "name":"TRANS_FLIGHT_COAST",
        "value":11
      },
      {
        "name":"TRANS_FLIGHT_BRAKE",
        "value":12
      },
      {
        "name":"TRANS_ERROR_STATE",
        "value":13
      },
      {
        "name":"SET_ADC_ERROR",
        "value":14
      },
      {
        "name":"SET_CAN_ERROR",
        "value":15
      },
      {
        "name":"TSET_I2C_ERROR",
        "value":16
      },
      {
        "name":"SET_PRU_ERROR",
        "value":17
      },
      {
        "name":"SET_NETWORK_ERROR",
        "value":18
      },
      {
        "name":"SET_OTHER_ERROR",
        "value":19
      },
      {
        "name":"CLR_ADC_ERROR",
        "value":20
      },
      {
        "name":"CLR_CAN_ERROR",
        "value":21
      },
      {
        "name":"CLR_I2C_ERROR",
        "value":22
      },
      {
        "name":"CLR_PRU_ERROR",
        "value":23
      },
      {
        "name":"CLR_NETWORK_ERROR",
        "value":24
      },
      {
        "name":"CLR_OTHER_ERROR",
        "value":25
      },
      {
        "name":"SET_HV_RELAY_HV_POLE",
        "value":26
      },
      {
        "name":"SET_HV_RELAY_LV_POLE",
        "value":27
      },
      {
        "name":"SET_HV_RELAY_PRE_CHARGE",
        "value":28
      }],
      "currValue": 0,
      "currCommand": 1
    }
  }

  onSelect(value:number) {
    this.data.currCommand = value;
  }

  onEvent(currValue:number) {
    this.data.currValue = currValue;
  }

  onSubmit() {
    this.ds.sendStatus(this.data);
    this.sentText = "Sent: " + this.data.currCommand + ": " + this.data.currValue;
  }
}
