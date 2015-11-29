#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer,*s_date_layer;
static TextLayer *s_food_layer;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;
static char date_buffer[16];



char* lunchToday(char* dateInput){
    char *lunch;
    switch (dateInput[0]){
      case 'M':
      lunch= "DC Burgers";
      break;
      case 'T':
      lunch= "SLC Pizza";
      break;
      case 'W':
      lunch= "DC Chopsticks";
      break;
      case 'F':
      lunch = "V1 Cafe: Aunties Kitchen";
        break;
     default:
      lunch = "V1 Cafe: ALL DAY BABY";
    }
  
  
  
  return lunch;
}


static void update_time() {

  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M", tick_time);
  
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
  strftime(date_buffer, sizeof(date_buffer), "%a %d %b", tick_time);
// Show the date
text_layer_set_text(s_date_layer, date_buffer);
  //show whats for lunch
  text_layer_set_text(s_food_layer, lunchToday(date_buffer));
}
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
update_time();
}
static void main_window_load(Window *window) {
  // Create GBitmap
s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SEXX_BACK);

// Create BitmapLayer to display the GBitmap
 s_background_layer = bitmap_layer_create(GRect(0, 0, 144 , 168));

 // Set the bitmap onto the layer and add to the window
 bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);

  
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(
      GRect(0, 70, bounds.size.w, 50));
  s_food_layer = text_layer_create( GRect(0,136, bounds.size.w,26));
  
  //(move in x domain, ,  ,  changes height that displays)
  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  //stuff around the time 
  text_layer_set_background_color(s_food_layer, GColorClear);
  text_layer_set_text_color(s_food_layer, GColorWhite);
  text_layer_set_font(s_food_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_food_layer, GTextAlignmentCenter);
  
  // Create date TextLayer
s_date_layer = text_layer_create(GRect(0, 120, 154, 30));
text_layer_set_text_color(s_date_layer, GColorWhite);
text_layer_set_background_color(s_date_layer, GColorClear);
text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  
  // Add it as a child layer to the Window's root layer
  
   layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
 layer_add_child(window_layer, text_layer_get_layer(s_food_layer));
}








static void main_window_unload(Window *window) {
  text_layer_destroy(s_date_layer);
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  // Destroy GBitmap
  text_layer_destroy(s_food_layer);
gbitmap_destroy(s_background_bitmap);

// Destroy BitmapLayer
bitmap_layer_destroy(s_background_layer);
}
static void init() {
  
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  s_main_window = window_create();
  // Show the Window on the watch, with animated=true
  

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
 
  if(s_main_window != NULL) {
    // Allocation was successful!
    window_stack_push(s_main_window, true);
  } else {
    // The Window could not be allocated! 
    // Tell the user that the operation could not be completed
    
  }
  // Make sure the time is displayed from the start
  update_time();
  
  
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}