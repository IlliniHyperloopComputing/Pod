import { Component, OnInit } from '@angular/core';

import { ButtonsService } from '../../services/buttons.service';

@Component({
  selector: 'app-buttons',
  templateUrl: './buttons.component.html',
  styleUrls: ['./buttons.component.css']
})
export class ButtonsComponent implements OnInit {
  leftButtonName:string = "Ready";
  rightButtonName:string = "E-Stop";
  constructor(private bs: ButtonsService) { }

  ngOnInit() {
    this.setInitialName();
  }

  setInitialName() {
    this.bs.getState().subscribe((data:number) => this.leftButtonName = this.stateToName(data))
    
  }

  clickLeft() {
    this.bs.clickLeft().subscribe((data:number) => this.leftButtonName = this.stateToName(data))
  }

  clickRight() {
    this.bs.clickRight().subscribe()
  }

  stateToName(state:number):string {
    if (state == 0) {
      return "Test"
    }
    else if (state == 1) {
      return "Loading"
    }
    else if (state == 2) {
      return "Launch"
    }
    else {
      return "NO USE"
    }
  }
}
