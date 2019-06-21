import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'app-buttons',
  templateUrl: './buttons.component.html',
  styleUrls: ['./buttons.component.css']
})
export class ButtonsComponent implements OnInit {
  leftButtonName:string = "Ready";
  rightButtonName:string = "E-Stop";
  constructor() { }

  ngOnInit() {
  }

}
