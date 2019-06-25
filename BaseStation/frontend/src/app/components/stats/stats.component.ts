import { Component, OnInit } from '@angular/core';

import { StatsService } from '../../services/stats.service';
import { Stat } from '../../models/stat';

@Component({
  selector: 'app-stats',
  templateUrl: './stats.component.html',
  styleUrls: ['./stats.component.css']
})
export class StatsComponent implements OnInit {
  stats:Stat[];

  constructor(private statsService: StatsService) { }

  ngOnInit() {
    this.getValues();
  }

  getValues() {
    this.statsService.getStatStatus().subscribe((data: Stat[]) => this.stats = data)
  }
}
