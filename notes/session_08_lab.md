# Session 8 Lab - Music Mode and Full Integration Sign-Off

Hardware target: ATmega644P with active buzzer on PD3 and LEDs on PB0 to PB4.
Code target: full nine-mode integration.

## Lab Goal

Validate audio generation, LED synchronisation and full system behaviour before sign-off.

## Software Setup

1. Select environment `06_state_machine`.
2. Build and upload.
3. Step to mode 8 for melody test.

## Part A - Melody Playback Check

1. Enter mode 8.
2. Listen to full sequence once.
3. Confirm rests are audible as pauses.
4. Confirm no sustained stuck tone after playback.

Pass criteria:

- Melody sequence completes cleanly.
- Buzzer returns silent after notes.

## Part B - LED and Audio Sync

1. Observe LED state changes through the melody.
2. Confirm LED step advances with note index.
3. Confirm brief gap between notes is visible.

Pass criteria:

- LED transitions are aligned with note events.

## Part C - Full Regression Sweep

1. Cycle through modes 0 to 8.
2. Spend at least 15 seconds in each mode.
3. Verify button transitions remain reliable.
4. Return to mode 8 and retest melody.

Pass criteria:

- No regression after visiting all modes.

## Part D - Upload and Recovery Drill

1. Reflash same firmware again.
2. Confirm upload works first try.
3. If upload fails, check COM4 and `-B 10`.
4. Retry after reconnecting programmer.

Pass criteria:

- Reliable development loop is confirmed.

## Sign-Off Checklist

- [ ] Startup animation behaves correctly
- [ ] All 9 modes reachable
- [ ] Mode wraparound works
- [ ] Melody plays fully
- [ ] Button transitions stable
- [ ] No unexplained reset observed

## Lab Record

- Date:
- Build hash or commit:
- Regression summary:
- Remaining risks:
- Next planned improvement:
