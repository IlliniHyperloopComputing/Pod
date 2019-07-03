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
  }

  clickLeft() {
    this.bs.clickLeft()
  }

  clickRight() {
    this.bs.clickRight()
  }

}
