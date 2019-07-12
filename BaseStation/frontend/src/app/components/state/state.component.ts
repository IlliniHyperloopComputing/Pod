import { Component, OnInit } from '@angular/core';

import { StateService } from '../../services/state.service';
import { interval } from 'rxjs';

@Component({
  selector: 'app-state',
  templateUrl: './state.component.html',
  styleUrls: ['./state.component.css']
})
export class StateComponent implements OnInit {
  stateName:string = "Disconnected"

  constructor(private stateService: StateService) { 
    interval(500).subscribe(x => {
      this.getValues();
    })
  }

  ngOnInit() {
  }

  getValues() {
    this.stateService.getState().subscribe(
      (data:string) => this.stateName = data,
      error => this.stateName = "Disconnected"
    )
  }
}
