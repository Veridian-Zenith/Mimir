# Mimir Theme Specification

## AMOLED BlackxRed Theme

Optimized for OLED displays with true black background and vibrant red accents.

## Color Palette

### Base Colors

| Token | Hex | Usage |
|-------|-----|-------|
| `--bg-primary` | `#000000` | Main window background (true black for OLED) |
| `--bg-surface` | `#0D0D0D` | Cards, panels, elevated surfaces |
| `--bg-surface-hover` | `#1A1A1A` | Hover states on surfaces |
| `--text-primary` | `#FFFFFF` | Primary text, headings |
| `--text-secondary` | `#B3B3B3` | Secondary text, labels |
| `--text-disabled` | `#666666` | Disabled text |

### Accent Colors

| Token | Hex | Usage |
|-------|-----|-------|
| `--accent-primary` | `#FF1A1A` | Primary actions, links, active states |
| `--accent-secondary` | `#CC0000` | Secondary accent, pressed states |
| `--accent-glow` | `#FF1A1A33` | Glow effects (20% opacity) |

### Semantic Colors

| Token | Hex | Usage |
|-------|-----|-------|
| `--success` | `#1AFF1A` | Clean scan, success states |
| `--warning` | `#FFB31A` | Warnings, signature age |
| `--error` | `#FF0033` | Threats detected, errors |
| `--info` | `#1A99FF` | Informational states |

### Borders & Dividers

| Token | Hex | Usage |
|-------|-----|-------|
| `--border` | `#262626` | Subtle borders |
| `--border-active` | `#FF1A1A` | Active/focused borders |

## Typography

| Element | Size | Weight |
|---------|------|--------|
| Heading 1 | 24px | 600 |
| Heading 2 | 20px | 600 |
| Body | 14px | 400 |
| Caption | 12px | 400 |
| Button | 14px | 500 |

Font family: System default (sans-serif on both GTK and Qt).

## Spacing

Base unit: 8px.

| Token | Value |
|-------|-------|
| `--space-xs` | 4px |
| `--space-sm` | 8px |
| `--space-md` | 16px |
| `--space-lg` | 24px |
| `--space-xl` | 32px |

## Component Styles

### Buttons

**Primary:**
- Background: `--accent-primary`
- Text: `#FFFFFF`
- Border-radius: 8px
- Hover: `--accent-secondary`
- Pressed: `#990000`

**Secondary:**
- Background: `--bg-surface`
- Text: `--accent-primary`
- Border: 1px `--border`
- Hover: `--bg-surface-hover`

### Cards

- Background: `--bg-surface`
- Border-radius: 12px
- Padding: `--space-md`
- Border: 1px `--border`

### Status Indicators

**Clean:**
- Color: `--success`
- Style: Filled circle

**Infected:**
- Color: `--error`
- Style: Filled circle with pulse animation

**Scanning:**
- Color: `--accent-primary`
- Style: Spinning indicator

## GTK Implementation

```css
/* mimir-gtk.css */
@define-color bg_primary #000000;
@define-color bg_surface #0D0D0D;
@define-color accent_primary #FF1A1A;
/* ... etc */
```

## Qt Implementation

```qss
/* mimir-qt.qss */
QMainWindow {
  background-color: #000000;
}
/* ... etc */
```

## Theme Application Flow

1. libmimir defines `theme.hh` with color tokens
2. Each frontend loads theme at startup
3. GTK uses CSS variables via `mimir-gtk.css`
4. Qt uses stylesheet via `mimir-qt.qss`
