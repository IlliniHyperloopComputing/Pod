import { Component, OnInit } from '@angular/core';

import { BatteryService } from '../../services/battery.service';

@Component({
  selector: 'app-battery',
  templateUrl: './battery.component.html',
  styleUrls: ['./battery.component.css']
})
export class BatteryComponent implements OnInit {
  value:string;
  name:string = "Battery %";

  constructor(private batteryService: BatteryService) { }

  ngOnInit() {
    this.getValue();
  }

  getValue() {
    this.batteryService.getBatteryStatus().subscribe(value => this.value = value);
  }

}
