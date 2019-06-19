import { Component, OnInit } from '@angular/core';

import { StatsService } from '../../services/stats.service';
import { stat } from '../../models/stat';

@Component({
  selector: 'app-stats',
  templateUrl: './stats.component.html',
  styleUrls: ['./stats.component.css']
})
export class StatsComponent implements OnInit {
  stats:stat[];

  constructor(private statsService: StatsService) { }

  ngOnInit() {
    this.getValues();
  }

  getValues() {
    this.statsService.getStatStatus().subscribe(value => this.stats = value)
  }

}
