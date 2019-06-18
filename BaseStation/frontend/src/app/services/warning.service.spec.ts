import { TestBed } from '@angular/core/testing';

import { WarningService } from './warning.service';

describe('WarningService', () => {
  beforeEach(() => TestBed.configureTestingModule({}));

  it('should be created', () => {
    const service: WarningService = TestBed.get(WarningService);
    expect(service).toBeTruthy();
  });
});
