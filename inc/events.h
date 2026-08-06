#ifndef EVENTS_H
#define EVENTS_H

#include "defines.h"
#include <hardware/gpio.h>
#include <stdint.h>

/**
 * @brief Types of events that can occur.
 *
 * This enum defines the different types of events that can be captured.
 *
 */
enum EventType {
  EVENT_BUTTON_PRESS,
  EVENT_BUTTON_RELEASE,
  EVENT_ENCODER_ROTATE,
  EVENT_BARCODE_READ,
};

/**
 * @brief Structure representing an event.
 *
 * This structure holds information about an event, including its type and
 * associated data (e.g., button ID for button events, delta and position for
 * encoder events).
 *
 * It uses a union to store event-specific data efficiently.
 *
 */
typedef struct {
  enum EventType type;
  union {
    struct {
      uint8_t button_id;
    } button;
    struct {
      int32_t delta;
      int32_t position;
    } encoder;
    char barcode[BARCODE_BUFFER_SIZE];
  };
} Event;

/**
 * @brief Buffer for event storage.
 *
 * Circular buffer for holding events such as button presses and encoder
 * rotations. Tasks reading from the event buffer should maintain their own
 * `EventReader` to track their current read position. This allows multiple
 * tasks to read events independently without interfering with each other.
 *
 */
typedef struct {
  Event events[EVENT_BUFFER_SIZE];
  size_t head;
} EventBuffer;

/**
 * @brief Reader for event buffer.
 *
 * Each task that reads from the event buffer should have its own instance of
 * `EventReader`. This allows multiple tasks to read events independently
 * without interfering with each other's.
 *
 */
typedef struct {
  size_t next_index;
} EventReader;

/**
 * @brief Push an event into the buffer.
 *
 * This function adds an event to the circular buffer and updates the head
 * index.
 *
 * @param event The button event to be added.
 * @param event_buffer Pointer to the EventBuffer where the event will be
 * stored.
 *
 */
static void event_push(Event event, EventBuffer *event_buffer) {
  uint8_t next_head = (event_buffer->head + 1) % EVENT_BUFFER_SIZE;
  event_buffer->events[event_buffer->head] = event;
  event_buffer->head = next_head;
}

static bool event_peek(EventReader *reader, EventBuffer *buffer,
                       Event *out_event) {
  if (reader->next_index == buffer->head) {
    out_event = NULL;
    return false;
  }
  *out_event = buffer->events[reader->next_index];
  return true;
}

static bool event_read(EventReader *reader, EventBuffer *buffer,
                       Event *out_event) {
  bool has_event = event_peek(reader, buffer, out_event);
  if (!has_event)
    return false;
  reader->next_index = (reader->next_index + 1) % EVENT_BUFFER_SIZE;
  return true;
}

#endif // EVENTS_H
