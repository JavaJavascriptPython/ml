
<header class="topbar">
  <!-- Logo and title -->
  <div class="brand">
    <div class="brand-mark">
      <mat-icon>sync_alt</mat-icon>
    </div>

    <div class="brand-copy">
      <span class="brand-title">CDC Monitor</span>
      <span class="brand-subtitle">Transfer Operations</span>
    </div>
  </div>

  <!-- Right-side controls -->
  <div class="header-controls">

    <!-- Bank selector -->
    <div class="bank-picker">
      <mat-icon class="bank-icon">account_balance</mat-icon>

      <mat-form-field appearance="outline" class="bank-select">
        <mat-select
          [value]="selectedBank"
          aria-label="Select bank"
          (selectionChange)="selectBank($event.value)">

          <mat-option value="SBI">SBI</mat-option>
          <mat-option value="HDFC">HDFC Bank</mat-option>
          <mat-option value="ICICI">ICICI Bank</mat-option>
          <mat-option value="AXIS">Axis Bank</mat-option>

        </mat-select>
      </mat-form-field>
    </div>

    <!-- Date and time -->
    <div class="datetime">
      <span class="date">{{ currentTime | date:'EEE, dd MMM' }}</span>
      <span class="time">{{ currentTime | date:'hh:mm:ss a' }}</span>
    </div>

    <!-- Notifications -->
    <button
      mat-icon-button
      class="header-icon-button"
      aria-label="Notifications"
      [matMenuTriggerFor]="notificationMenu">

      <mat-icon
        [matBadge]="unreadCount"
        matBadgeColor="warn"
        matBadgeSize="small"
        [matBadgeHidden]="unreadCount === 0">
        notifications_none
      </mat-icon>
    </button>

    <mat-menu #notificationMenu="matMenu" xPosition="before">
      <div class="notification-heading">Notifications</div>
      <button mat-menu-item>
        <mat-icon>info_outline</mat-icon>
        <span>View recent CDC events</span>
      </button>
      <button mat-menu-item>
        <mat-icon>error_outline</mat-icon>
        <span>Check failed transfers</span>
      </button>
    </mat-menu>

    <!-- Day / night mode -->
    <button
      mat-icon-button
      class="theme-toggle"
      [attr.aria-label]="isDarkMode ? 'Switch to light mode' : 'Switch to dark mode'"
      [matTooltip]="isDarkMode ? 'Light mode' : 'Dark mode'"
      (click)="toggleTheme()">

      <mat-icon>{{ isDarkMode ? 'light_mode' : 'dark_mode' }}</mat-icon>
    </button>

  </div>
</header>








/* Main header */
.topbar {
  width: 100%;
  min-height: 72px;
  padding: 10px 24px;
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 20px;

  background: #ffffff;
  border-bottom: 1px solid #e8ecef;
  box-shadow: 0 2px 8px rgba(15, 23, 42, 0.035);

  font-family: Arial, Helvetica, sans-serif;
  box-sizing: border-box;
  position: relative;
  z-index: 10;
}

/* Logo and brand */
.brand {
  display: flex;
  align-items: center;
  gap: 11px;
  flex-shrink: 0;
}

.brand-mark {
  width: 42px;
  height: 42px;
  display: flex;
  align-items: center;
  justify-content: center;
  border-radius: 11px;

  background: #e5f3ef;
  color: #137a65;
}

.brand-mark mat-icon {
  font-size: 25px;
  width: 25px;
  height: 25px;
}

.brand-copy {
  display: flex;
  flex-direction: column;
  gap: 3px;
}

.brand-title {
  font-size: 17px;
  line-height: 20px;
  font-weight: 700;
  color: #172b36;
  letter-spacing: -0.3px;
}

.brand-subtitle {
  font-size: 11px;
  line-height: 14px;
  color: #7b8791;
}

/* All header controls stay in one row */
.header-controls {
  display: flex;
  align-items: center;
  justify-content: flex-end;
  gap: 14px;
  min-width: 0;
}

/* Bank dropdown */
.bank-picker {
  display: flex;
  align-items: center;
  gap: 8px;
  padding-left: 12px;
  border-left: 1px solid #e8ecef;
}

.bank-icon {
  color: #657581;
  font-size: 20px;
  width: 20px;
  height: 20px;
}

.bank-select {
  width: 145px;
  font-size: 13px;
}

/* Remove extra space under Material form field */
.bank-select ::ng-deep .mat-form-field-wrapper {
  padding-bottom: 0;
}

.bank-select ::ng-deep .mat-form-field-infix {
  width: auto;
  min-width: 75px;
  padding: 8px 0;
  border-top: 0;
}

.bank-select ::ng-deep .mat-form-field-flex {
  align-items: center;
}

.bank-select ::ng-deep .mat-form-field-outline {
  color: #dce3e7;
}

.bank-select ::ng-deep .mat-form-field-outline-start,
.bank-select ::ng-deep .mat-form-field-outline-end,
.bank-select ::ng-deep .mat-form-field-outline-gap {
  border-width: 1px;
}

.bank-select ::ng-deep .mat-form-field-underline {
  display: none;
}

/* Date and time */
.datetime {
  display: flex;
  flex-direction: column;
  align-items: flex-end;
  gap: 3px;
  padding-left: 14px;
  border-left: 1px solid #e8ecef;
  white-space: nowrap;
}

.date {
  font-size: 12px;
  font-weight: 600;
  color: #344653;
}

.time {
  font-size: 11px;
  color: #7b8791;
  font-variant-numeric: tabular-nums;
}

/* Notification and theme buttons */
.header-icon-button,
.theme-toggle {
  width: 38px;
  height: 38px;
  line-height: 38px;
  display: flex;
  align-items: center;
  justify-content: center;
  flex-shrink: 0;

  color: #647581;
  border-radius: 10px;
  transition: background-color 0.2s ease,
              color 0.2s ease;
}

.header-icon-button:hover,
.theme-toggle:hover {
  background: #edf5f3;
  color: #137a65;
}

.theme-toggle {
  background: #f3f6f7;
  color: #596c79;
}

.notification-heading {
  padding: 12px 16px 8px;
  font-size: 13px;
  font-weight: 700;
  color: #263b47;
}

/* Tablet and mobile */
@media (max-width: 700px) {
  .topbar {
    min-height: 64px;
    padding: 8px 12px;
    gap: 8px;
  }

  .brand {
    gap: 8px;
  }

  .brand-mark {
    width: 36px;
    height: 36px;
  }

  .brand-title {
    font-size: 14px;
  }

  .brand-subtitle {
    display: none;
  }

  .header-controls {
    gap: 5px;
  }

  .bank-picker {
    gap: 4px;
    padding-left: 7px;
  }

  .bank-icon {
    display: none;
  }

  .bank-select {
    width: 105px;
    font-size: 12px;
  }

  .datetime {
    display: none;
  }

  .header-icon-button,
  .theme-toggle {
    width: 34px;
    height: 34px;
    line-height: 34px;
  }
}

/* Very narrow screens */
@media (max-width: 380px) {
  .topbar {
    padding: 8px;
  }

  .brand-mark {
    display: none;
  }

  .brand-title {
    font-size: 13px;
  }

  .bank-select {
    width: 88px;
  }

  .header-controls {
    gap: 2px;
  }
}





import { Component, Output, EventEmitter } from '@angular/core';

export class HeaderComponent {
  selectedBank = 'SBI';

  // Keep updating this value every second in your existing clock logic.
  currentTime: Date = new Date();

  unreadCount = 0;
  isDarkMode = false;

  @Output() bankChange = new EventEmitter<string>();
  @Output() themeChange = new EventEmitter<boolean>();

  selectBank(bankCode: string): void {
    this.selectedBank = bankCode;
    this.bankChange.emit(bankCode);
  }

  toggleTheme(): void {
    this.isDarkMode = !this.isDarkMode;
    this.themeChange.emit(this.isDarkMode);
  }
}
