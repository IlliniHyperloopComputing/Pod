import { Component, OnInit } from '@angular/core';
import { interval } from 'rxjs';

import { StatsService } from '../../services/stats.service';
import { Stat } from '../../models/stat';

@Component({
  selector: 'app-stats',
  templateUrl: './stats.component.html',
  styleUrls: ['./stats.component.css']
})
export class StatsComponent implements OnInit {
  stats:Stat[];

  constructor(private statsService: StatsService) { 
    interval(500).subscribe(x => {
      this.getValues();
    })
  }

  ngOnInit() {
    this.getValues();
  }

  getValues() {
    this.statsService.getStatStatus().subscribe((data: Stat[]) => this.stats = data)
  }
}
