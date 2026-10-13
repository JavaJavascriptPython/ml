
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <title>Pulse | CDC Monitor</title>
  <base href="/">

  <meta name="viewport"
        content="width=device-width, initial-scale=1">

  <link rel="preconnect"
        href="https://fonts.googleapis.com">

  <link rel="preconnect"
        href="https://fonts.gstatic.com"
        crossorigin>

  <link href="https://fonts.googleapis.com/css2?family=Inter:wght@400;500;600;700&display=swap"
        rel="stylesheet">

  <link href="https://fonts.googleapis.com/icon?family=Material+Icons"
        rel="stylesheet">

  <!-- Development only: Tailwind CSS v4 browser CDN -->
  <script src="https://cdn.jsdelivr.net/npm/@tailwindcss/browser@4"></script>
</head>
<body>
  <app-root></app-root>
</body>
</html>






@import '~@angular/material/prebuilt-themes/indigo-pink.css';

html,
body {
  margin: 0;
  min-height: 100%;
  font-family: 'Inter', Arial, sans-serif;
  background: #f7f9f8;
  color: #182b25;
}

* {
  box-sizing: border-box;
}

body {
  min-width: 320px;
}

.mat-select-panel {
  max-height: 320px !important;
}




import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';

import { MatButtonModule } from '@angular/material/button';
import { MatIconModule } from '@angular/material/icon';
import { MatMenuModule } from '@angular/material/menu';
import { MatSelectModule } from '@angular/material/select';
import { MatBadgeModule } from '@angular/material/badge';

import { AppComponent } from './app.component';

@NgModule({
  declarations: [
    AppComponent
  ],
  imports: [
    BrowserModule,
    BrowserAnimationsModule,
    MatButtonModule,
    MatIconModule,
    MatMenuModule,
    MatSelectModule,
    MatBadgeModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }




import { Component, OnInit, OnDestroy } from '@angular/core';
import { interval, Subscription } from 'rxjs';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent implements OnInit, OnDestroy {

  currentTime: Date = new Date();
  selectedBank = 'SBI';

  private clockSubscription: Subscription;

  banks = [
    { code: 'SBI', name: 'State Bank of India' },
    { code: 'HDFC', name: 'HDFC Bank' },
    { code: 'ICICI', name: 'ICICI Bank' },
    { code: 'AXIS', name: 'Axis Bank' }
  ];

  notifications = [
    {
      title: 'CDC monitoring initialized',
      message: 'Dashboard is ready to monitor activity.',
      time: 'Just now',
      unread: true
    },
    {
      title: 'Sample transfer warning',
      message: 'Example notification for demonstration.',
      time: '10 min ago',
      unread: true
    }
  ];

  bankMetrics: any = {
    SBI: {
      successful: 1248,
      failed: 3,
      lastTransfer: '09:42:18',
      status: 'Monitoring'
    },
    HDFC: {
      successful: 986,
      failed: 1,
      lastTransfer: '09:38:42',
      status: 'Monitoring'
    },
    ICICI: {
      successful: 762,
      failed: 5,
      lastTransfer: '09:35:11',
      status: 'Attention'
    },
    AXIS: {
      successful: 1104,
      failed: 0,
      lastTransfer: '09:41:06',
      status: 'Monitoring'
    }
  };

  constructor() {}

  ngOnInit(): void {
    this.clockSubscription = interval(1000)
      .subscribe(() => {
        this.currentTime = new Date();
      });
  }

  ngOnDestroy(): void {
    if (this.clockSubscription) {
      this.clockSubscription.unsubscribe();
    }
  }

  get selectedBankName(): string {
    const bank = this.banks.find(
      item => item.code === this.selectedBank
    );

    return bank ? bank.name : '';
  }

  get metrics(): any {
    return this.bankMetrics[this.selectedBank];
  }

  get unreadCount(): number {
    return this.notifications.filter(
      item => item.unread
    ).length;
  }

  selectBank(bankCode: string): void {
    this.selectedBank = bankCode;
  }

  markNotificationsRead(): void {
    this.notifications.forEach(item => {
      item.unread = false;
    });
  }

}






<div class="min-h-screen bg-gray-50 text-gray-800">

  <!-- Main header -->
  <header class="bg-white border-b border-gray-200">
    <div class="max-w-screen-2xl mx-auto px-4 sm:px-6">

      <div class="flex items-center justify-between gap-4 py-4">

        <!-- Logo and product name -->
        <div class="flex items-center gap-3 min-w-0">
          <div class="brand-mark flex items-center justify-center rounded-xl">
            <mat-icon>monitor_heart</mat-icon>
          </div>

          <div class="min-w-0">
            <h1 class="text-xl font-semibold tracking-tight">
              Pulse
            </h1>
            <p class="text-xs tracking-widest text-gray-500 uppercase">
              CDC Monitor
            </p>
          </div>
        </div>

        <!-- Date and live clock -->
        <div class="hidden md:block text-right">
          <p class="text-sm font-semibold text-gray-800">
            {{ currentTime | date:'hh:mm:ss a' }}
          </p>
          <p class="text-xs text-gray-500 mt-1">
            {{ currentTime | date:'EEE, dd MMM yyyy' }}
          </p>
        </div>

        <!-- Notifications -->
        <button
          mat-icon-button
          [matMenuTriggerFor]="notificationMenu"
          aria-label="Notifications"
          [matBadge]="unreadCount"
          [matBadgeHidden]="unreadCount === 0"
          matBadgeColor="warn"
          matBadgeSize="small"
          (menuOpened)="markNotificationsRead()">
          <mat-icon>notifications_none</mat-icon>
        </button>

        <mat-menu #notificationMenu="matMenu" xPosition="before">
          <div class="notification-menu-heading">
            <span>Notifications</span>
            <span class="notification-count">
              {{ unreadCount }} new
            </span>
          </div>

          <div
            class="notification-item"
            *ngFor="let notification of notifications">

            <div class="notification-dot"
                 [class.dot-muted]="!notification.unread">
            </div>

            <div class="min-w-0">
              <p class="text-sm font-medium">
                {{ notification.title }}
              </p>
              <p class="text-xs text-gray-500 mt-1">
                {{ notification.message }}
              </p>
              <p class="text-xs text-gray-400 mt-2">
                {{ notification.time }}
              </p>
            </div>
          </div>

          <div class="notification-empty"
               *ngIf="notifications.length === 0">
            You're all caught up.
          </div>
        </mat-menu>

      </div>

      <!-- Bank selection row -->
      <div class="flex flex-wrap items-center justify-between gap-4 py-4 border-t border-gray-100">

        <div class="flex items-center gap-3">
          <div class="bank-mark flex items-center justify-center rounded-lg">
            <mat-icon>account_balance</mat-icon>
          </div>

          <div>
            <p class="text-xs font-medium text-gray-500 uppercase tracking-wider">
              Monitoring environment
            </p>
            <p class="text-sm font-semibold mt-1">
              {{ selectedBankName }}
            </p>
          </div>
        </div>

        <div class="bank-selector">
          <mat-form-field appearance="outline" floatLabel="never">
            <mat-label>Select bank</mat-label>

            <mat-select
              [value]="selectedBank"
              (selectionChange)="selectBank($event.value)"
              aria-label="Select bank">

              <mat-option
                *ngFor="let bank of banks"
                [value]="bank.code">

                {{ bank.name }}
              </mat-option>

            </mat-select>
          </mat-form-field>
        </div>

      </div>
    </div>
  </header>

  <!-- Dashboard content -->
  <main class="max-w-screen-2xl mx-auto px-4 sm:px-6 py-8">

    <div class="flex flex-wrap items-start justify-between gap-3 mb-6">
      <div>
        <h2 class="text-2xl font-semibold tracking-tight">
          Overview
        </h2>
        <p class="text-sm text-gray-500 mt-1">
          Today's CDC activity and transfer summary
        </p>
      </div>

      <div class="today-label">
        <mat-icon>calendar_today</mat-icon>
        {{ currentTime | date:'dd MMM yyyy' }}
      </div>
    </div>

    <!-- Summary cards -->
    <div class="grid grid-cols-1 sm:grid-cols-2 xl:grid-cols-4 gap-4">

      <div class="metric-card">
        <div class="metric-heading">
          <span>Agent status</span>
          <mat-icon>monitor_heart</mat-icon>
        </div>
        <div class="metric-value status-value">
          <span class="status-dot"></span>
          {{ metrics.status }}
        </div>
        <p class="metric-description">
          {{ selectedBank }} monitoring environment
        </p>
      </div>

      <div class="metric-card">
        <div class="metric-heading">
          <span>Successful transfers</span>
          <mat-icon>check_circle_outline</mat-icon>
        </div>
        <div class="metric-value">
          {{ metrics.successful | number }}
        </div>
        <p class="metric-description">
          Successful files recorded today
        </p>
      </div>

      <div class="metric-card">
        <div class="metric-heading">
          <span>Failed transfers</span>
          <mat-icon>error_outline</mat-icon>
        </div>
        <div class="metric-value">
          {{ metrics.failed | number }}
        </div>
        <p class="metric-description">
          Failed events recorded today
        </p>
      </div>

      <div class="metric-card">
        <div class="metric-heading">
          <span>Last successful transfer</span>
          <mat-icon>schedule</mat-icon>
        </div>
        <div class="metric-value">
          {{ metrics.lastTransfer }}
        </div>
        <p class="metric-description">
          Example event timestamp
        </p>
      </div>

    </div>

    <!-- Placeholder for upcoming dashboard sections -->
    <div class="mt-6 bg-white border border-gray-200 rounded-xl p-6">
      <div class="flex items-center gap-3">
        <mat-icon class="placeholder-icon">description</mat-icon>
        <div>
          <h3 class="text-base font-semibold">
            Transfer logs
          </h3>
          <p class="text-sm text-gray-500 mt-1">
            The latest transfer events and errors will appear here.
          </p>
        </div>
      </div>
    </div>

    <p class="text-xs text-gray-400 mt-5">
      Demo mode · All transfer counts and notifications are sample data.
    </p>

  </main>
</div>






:host {
  display: block;
  min-height: 100vh;
}

.brand-mark {
  width: 44px;
  height: 44px;
  background: #173d35;
  color: #ffffff;
}

.brand-mark mat-icon {
  font-size: 25px;
  width: 25px;
  height: 25px;
}

.bank-mark {
  width: 42px;
  height: 42px;
  color: #176b52;
  background: #e8f4ee;
}

.bank-selector {
  width: 280px;
  max-width: 100%;
}

.bank-selector mat-form-field {
  width: 100%;
  font-size: 14px;
}

.metric-card {
  background: #ffffff;
  border: 1px solid #e5e9e7;
  border-radius: 12px;
  padding: 22px;
  min-width: 0;
  transition: border-color 160ms ease,
              box-shadow 160ms ease;
}

.metric-card:hover {
  border-color: #cbd9d2;
  box-shadow: 0 5px 18px rgba(24, 43, 37, 0.04);
}

.metric-heading {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 12px;
  color: #6b7771;
  font-size: 13px;
  font-weight: 500;
}

.metric-heading mat-icon {
  color: #789087;
  font-size: 21px;
  width: 21px;
  height: 21px;
}

.metric-value {
  display: flex;
  align-items: center;
  gap: 9px;
  margin-top: 22px;
  color: #1d3028;
  font-size: 28px;
  line-height: 1.2;
  font-weight: 600;
  letter-spacing: -0.7px;
}

.status-value {
  font-size: 22px;
  letter-spacing: -0.4px;
}

.status-dot,
.notification-dot {
  display: inline-block;
  flex-shrink: 0;
  width: 8px;
  height: 8px;
  border-radius: 50%;
  background: #16a36a;
}

.metric-description {
  margin-top: 12px;
  font-size: 12px;
  color: #89938e;
  line-height: 1.5;
}

.today-label {
  display: flex;
  align-items: center;
  gap: 8px;
  padding: 9px 12px;
  border: 1px solid #e5e9e7;
  background: #ffffff;
  border-radius: 8px;
  color: #52635a;
  font-size: 13px;
}

.today-label mat-icon {
  font-size: 17px;
  width: 17px;
  height: 17px;
}

.placeholder-icon {
  color: #527b68;
}

.notification-menu-heading {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 20px;
  padding: 14px 16px;
  font-size: 14px;
  font-weight: 600;
  border-bottom: 1px solid #edf0ee;
}

.notification-count {
  color: #527b68;
  font-size: 12px;
  font-weight: 500;
}

.notification-item {
  display: flex;
  align-items: flex-start;
  gap: 11px;
  width: 310px;
  max-width: 85vw;
  padding: 14px 16px;
  white-space: normal;
  border-bottom: 1px solid #f0f2f1;
}

.notification-item:last-child {
  border-bottom: 0;
}

.notification-dot {
  margin-top: 5px;
}

.notification-dot.dot-muted {
  background: #cbd5cf;
}

.notification-empty {
  padding: 20px;
  color: #758179;
  font-size: 13px;
}

@media (max-width: 599px) {
  .bank-selector {
    width: 100%;
  }

  .metric-card {
    padding: 18px;
  }

  .metric-value {
    margin-top: 16px;
  }
}




import { MatFormFieldModule } from '@angular/material/form-field';