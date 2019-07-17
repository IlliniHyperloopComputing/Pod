import { Component, OnInit } from '@angular/core';

import { BatteryService } from '../../services/battery.service';
import { Battery } from '../../models/battery';

@Component({
  selector: 'app-battery',
  templateUrl: './battery.component.html',
  styleUrls: ['./battery.component.css']
})
export class BatteryComponent implements OnInit {
  battery:Battery = { "value": 0 };

  constructor(private batteryService: BatteryService) { }

  ngOnInit() {
    this.getValue();
  }

  getValue() {
    this.batteryService.getBatteryStatus().subscribe((data: Battery) => this.battery = { ...data })
  }

}
