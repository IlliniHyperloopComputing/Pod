import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'app-battery',
  templateUrl: './battery.component.html',
  styleUrls: ['./battery.component.css']
})
export class BatteryComponent implements OnInit {
  value:string = "10";
  name:string = "Battery";
  constructor() { }

  ngOnInit() {
  }

}
