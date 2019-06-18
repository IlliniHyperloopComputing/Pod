import { Component, OnInit } from '@angular/core';

import { WarningService } from '../../services/warning.service';
import { warningInfo } from '../../models/warning';

@Component({
  selector: 'app-warning',
  templateUrl: './warning.component.html',
  styleUrls: ['./warning.component.css']
})
export class WarningComponent implements OnInit {
  info:warningInfo;
  //warningColor:string = "LawnGreen";
  //textColor:string = "black";
  //text:string = "All Clear";

  constructor(private warningService: WarningService) { }

  ngOnInit() {
    this.getInfo()
  }

  getInfo() {
    this.warningService.getWarningStatus().subscribe(info => this.info = info)
  }

  applyStyles() {
    const styles = {'background-color': this.info.warningColor, 'color': this.info.textColor};
    return styles;
  }

}
