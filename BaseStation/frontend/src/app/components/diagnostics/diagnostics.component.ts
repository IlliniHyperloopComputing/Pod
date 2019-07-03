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

  constructor(private ds: DiagnosticsService) { }

  ngOnInit() {
    //console.log(this.data.commands[0]);
    this.data = {
      "commands": [{
        "name":"test",
        "value":1
      }, 
      {
        "name":"test2",
        "value":2
      }],
      "currValue": 0,
      "currCommand": 1
    }
  }

  onSelect(currCommand:number) {
    this.data.currCommand = currCommand;
    console.log(currCommand)
  }

  onEvent(currValue:number) {
    this.data.currValue = currValue;
  }

  onSubmit() {
    this.ds.sendStatus(this.data);
    console.log("test")
  }
}
