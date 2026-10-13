
/* Global theme variables */
:root {
  color-scheme: light;

  --page-bg: #f5f7f9;
  --surface: #ffffff;
  --surface-hover: #f0f5f4;

  --text-primary: #172b36;
  --text-secondary: #667783;
  --text-muted: #8996a0;

  --border-color: #e3e9ed;
  --accent: #137a65;
  --accent-soft: #e5f3ef;

  --icon-color: #647581;

  --danger: #dc2626;
  --danger-soft: rgba(239, 68, 68, 0.10);
  --danger-strong: rgba(239, 68, 68, 0.18);

  --shadow: 0 2px 8px rgba(15, 23, 42, 0.04);
}

/* Dark theme overrides */
.app-shell.dark-theme {
  color-scheme: dark;

  --page-bg: #101820;
  --surface: #19242e;
  --surface-hover: #25343e;

  --text-primary: #edf3f6;
  --text-secondary: #b0bec7;
  --text-muted: #8b9ba7;

  --border-color: #30404b;
  --accent: #69cbb0;
  --accent-soft: #203e3a;

  --icon-color: #b0bec7;

  --danger: #ff727b;
  --danger-soft: rgba(255, 75, 85, 0.10);
  --danger-strong: rgba(255, 75, 85, 0.19);

  --shadow: 0 2px 10px rgba(0, 0, 0, 0.16);
}

/* Application background */
html,
body {
  margin: 0;
  min-height: 100%;
}

body {
  background: var(--page-bg);
}

app-root {
  display: block;
  min-height: 100vh;
}

.app-shell {
  min-height: 100vh;
  background: var(--page-bg);
  color: var(--text-primary);
  transition: background-color 0.2s ease, color 0.2s ease;
}

* {
  box-sizing: border-box;
}

/* Override existing Tailwind text and background utilities in dark mode */
.app-shell.dark-theme .bg-white {
  background-color: var(--surface) !important;
}

.app-shell.dark-theme .text-gray-500 {
  color: var(--text-secondary) !important;
}

.app-shell.dark-theme .text-gray-400 {
  color: var(--text-muted) !important;
}

.app-shell.dark-theme .border-gray-200 {
  border-color: var(--border-color) !important;
}







:host {
  display: block;
}

.topbar {
  min-height: 68px;
  padding: 10px 24px;
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 20px;

  background: var(--surface);
  color: var(--text-primary);
  border-bottom: 1px solid var(--border-color);
  box-shadow: var(--shadow);

  font-family: Arial, Helvetica, sans-serif;
  box-sizing: border-box;
  position: relative;
  z-index: 10;
}

/* Brand */
.brand {
  display: flex;
  align-items: center;
  gap: 11px;
  flex-shrink: 0;
}

.brand-mark {
  width: 40px;
  height: 40px;
  display: flex;
  align-items: center;
  justify-content: center;
  border-radius: 11px;
  background: var(--accent-soft);
  color: var(--accent);
}

.brand-copy {
  display: flex;
  flex-direction: column;
  gap: 3px;
}

.brand-title {
  color: var(--text-primary);
  font-size: 17px;
  line-height: 20px;
  font-weight: 700;
  letter-spacing: -0.3px;
}

.brand-subtitle {
  color: var(--text-muted);
  font-size: 11px;
  line-height: 14px;
}

/* Right-side controls */
.header-controls {
  display: flex;
  align-items: center;
  justify-content: flex-end;
  gap: 12px;
  min-width: 0;
}

.bank-picker {
  display: flex;
  align-items: center;
  gap: 8px;
  padding-left: 12px;
  border-left: 1px solid var(--border-color);
}

.bank-icon {
  color: var(--icon-color);
}

.bank-select {
  width: 145px;
  font-size: 13px;
}

/* Compact Angular Material select */
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
  color: var(--border-color);
}

.bank-select ::ng-deep .mat-select-value,
.bank-select ::ng-deep .mat-select-arrow {
  color: var(--text-primary);
}

.bank-select ::ng-deep .mat-form-field-underline {
  display: none;
}

.datetime {
  display: flex;
  flex-direction: column;
  align-items: flex-end;
  gap: 3px;
  padding-left: 14px;
  border-left: 1px solid var(--border-color);
  white-space: nowrap;
}

.date {
  color: var(--text-primary);
  font-size: 12px;
  font-weight: 600;
}

.time {
  color: var(--text-muted);
  font-size: 11px;
  font-variant-numeric: tabular-nums;
}

/* Notification and theme buttons */
.header-icon-button,
.theme-toggle {
  width: 38px;
  height: 38px;
  flex-shrink: 0;
  border-radius: 10px;
  color: var(--icon-color);
  transition: background-color 0.2s ease, color 0.2s ease;
}

.header-icon-button:hover,
.theme-toggle:hover {
  background: var(--surface-hover);
  color: var(--accent);
}

.theme-toggle {
  background: var(--surface-hover);
}

.notification-heading {
  padding: 12px 16px 8px;
  font-size: 13px;
  font-weight: 700;
  color: var(--text-primary);
}

/* Mobile */
@media (max-width: 700px) {
  .topbar {
    min-height: 60px;
    padding: 8px 12px;
    gap: 8px;
  }

  .brand {
    gap: 8px;
  }

  .brand-mark {
    width: 34px;
    height: 34px;
  }

  .brand-title {
    font-size: 14px;
  }

  .brand-subtitle,
  .datetime,
  .bank-icon {
    display: none;
  }

  .header-controls {
    gap: 4px;
  }

  .bank-picker {
    padding-left: 6px;
  }

  .bank-select {
    width: 105px;
    font-size: 12px;
  }

  .header-icon-button,
  .theme-toggle {
    width: 34px;
    height: 34px;
  }
}

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







:host {
  display: block;
}

/* Dashboard layout */
main {
  color: var(--text-primary);
}

/* Date badge */
.today-label {
  display: flex;
  align-items: center;
  gap: 8px;
  padding: 9px 12px;
  border: 1px solid var(--border-color);
  border-radius: 9px;
  background: var(--surface);
  color: var(--text-secondary);
  font-size: 12px;
  white-space: nowrap;
}

.today-label mat-icon {
  width: 18px;
  height: 18px;
  font-size: 18px;
  color: var(--accent);
}

/* Metric cards */
.metric-card {
  position: relative;
  isolation: isolate;
  overflow: hidden;
  min-width: 0;
  padding: 20px;
  border: 1px solid var(--border-color);
  border-radius: 13px;
  background: var(--surface);
  color: var(--text-primary);
  box-shadow: var(--shadow);
  transition:
    border-color 0.2s ease,
    background-color 0.2s ease,
    box-shadow 0.2s ease;
}

/* Keep card contents above the alert overlay */
.metric-card > * {
  position: relative;
  z-index: 1;
}

.metric-card:not(.alert-card):hover {
  border-color: var(--accent);
  box-shadow: 0 5px 16px rgba(15, 23, 42, 0.07);
}

/* Card title and icon */
.metric-heading {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 10px;
  color: var(--text-secondary);
  font-size: 13px;
  font-weight: 500;
}

.metric-heading mat-icon {
  width: 21px;
  height: 21px;
  font-size: 21px;
  color: var(--icon-color);
  flex-shrink: 0;
}

/* Metric value */
.metric-value {
  margin-top: 17px;
  color: var(--text-primary);
  font-size: 30px;
  font-weight: 700;
  line-height: 1.2;
  letter-spacing: -0.7px;
  overflow-wrap: anywhere;
  font-variant-numeric: tabular-nums;
}

.status-value {
  display: flex;
  align-items: center;
  gap: 9px;
  font-size: 23px;
  letter-spacing: -0.3px;
}

/* Agent status indicator */
.status-dot {
  width: 9px;
  height: 9px;
  display: inline-block;
  flex-shrink: 0;
  border-radius: 50%;
  background: #16a34a;
  box-shadow: 0 0 0 3px rgba(22, 163, 74, 0.12);
}

/* Status text colors */
.status-value:has(+ .metric-description) {
  color: var(--text-primary);
}

/* Card description */
.metric-description {
  margin: 10px 0 0;
  color: var(--text-muted);
  font-size: 12px;
  line-height: 1.5;
}

/* Transfer logs panel */
.logs-panel {
  border: 1px solid var(--border-color);
  background: var(--surface);
  color: var(--text-primary);
  box-shadow: var(--shadow);
}

.logs-panel h3 {
  color: var(--text-primary);
}

.logs-panel p {
  color: var(--text-secondary);
}

.placeholder-icon {
  color: var(--accent);
  font-size: 24px;
  width: 24px;
  height: 24px;
}

/* Failed transfer alert */
.metric-card.alert-card {
  border-color: var(--danger);
  animation: alert-border-pulse 1.5s ease-in-out infinite;
}

.metric-card.alert-card::before {
  content: "";
  position: absolute;
  inset: 0;
  z-index: 0;
  pointer-events: none;
  background: var(--danger-soft);
  animation: alert-overlay-pulse 1.5s ease-in-out infinite;
}

/* Highlight the failed count and icon */
.metric-card.alert-card .metric-value,
.metric-card.alert-card .metric-heading mat-icon {
  color: var(--danger);
}

@keyframes alert-border-pulse {
  0%, 100% {
    box-shadow: 0 0 0 0 transparent;
    border-color: var(--danger);
  }

  50% {
    box-shadow: 0 0 0 4px var(--danger-soft);
  }
}

@keyframes alert-overlay-pulse {
  0%, 100% {
    opacity: 0.35;
  }

  50% {
    opacity: 1;
  }
}

/* Accessibility: retain the warning without motion */
@media (prefers-reduced-motion: reduce) {
  .metric-card.alert-card,
  .metric-card.alert-card::before {
    animation: none;
  }

  .metric-card.alert-card {
    background: var(--danger-soft);
    border: 2px solid var(--danger);
  }
}




