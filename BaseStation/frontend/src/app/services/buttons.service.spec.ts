import { TestBed } from '@angular/core/testing';

import { ButtonsService } from './buttons.service';

describe('ButtonsService', () => {
  beforeEach(() => TestBed.configureTestingModule({}));

  it('should be created', () => {
    const service: ButtonsService = TestBed.get(ButtonsService);
    expect(service).toBeTruthy();
  });
});
