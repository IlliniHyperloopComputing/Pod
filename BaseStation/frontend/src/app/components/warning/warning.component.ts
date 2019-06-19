import { Component, OnInit } from '@angular/core';

import { warningInfo } from '../../models/warning';
import { WarningService } from '../../services/warning.service';

@Component({
  selector: 'app-warning',
  templateUrl: './warning.component.html',
  styleUrls: ['./warning.component.css']
})
export class WarningComponent implements OnInit {
  value:warningInfo;
  constructor(private warningService: WarningService) { }

  ngOnInit() {
    this.getValue();
  }

  getValue() {
    this.warningService.getWarningStatus().subscribe(value => this.value = value);
  }

  setMyStyles() {
    let styles = {
      'background-color': this.value.warningColor,
      'color': this.value.textColor,
      'cursor': 'pointer'
    }

    return styles;
  }

  onClick() {
    alert(this.value.warnings);
  }

}
