import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'app-diagnostics',
  templateUrl: './diagnostics.component.html',
  styleUrls: ['./diagnostics.component.css']
})
export class DiagnosticsComponent implements OnInit {
  commands:string[] = ["test", "test2"];
  currValue:number;
  currCommand:string = this.commands[0];

  constructor() { }

  ngOnInit() {
    console.log(this.commands[0]);
  }

  onSelect(currCommand:string) {
    this.currCommand = currCommand;
  }

  onEvent(currValue:number) {
    this.currValue = currValue;
  }

  onSubmit() {
    console.log(this.currCommand + " " + this.currValue)
  }
}
