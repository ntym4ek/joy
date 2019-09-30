<?php
/**
 * @file
 * Hooks provided by the Commerce RussianPost module.
 */


/**
 * Allows modules to alter the shipping rate request before it is submitted.
 *
 * @param array &$rate_request
 *   An array of SimpleXML elements containing a shipping rate request (order
 *   data that has been formatted for submission to RussianPost web services).
 *   One element for each package.
 * @param object $order
 *   The order object the shipping rate request is being generated for.
 *
 * @see commerce_russianpost_build_rate_request()
 */
function hook_commerce_russianpost_build_rate_request_alter(&$rate_request, $order) {
  // EXAMPLE: Add 24 hours to the turnaround time if today is Friday.
  if (date('l') == 'Friday') {
    // Get turnaround time (saved in hours).
    $turnaround_time = variable_get('commerce_russianpost_turnaround', 0);

    // Add an additional 24 hours.
    $turnaround_time += 24;

    // Update the rate request.
    foreach ($rate_request as &$rate_request_element) {
      $rate_request_element->{'expected-mailing-date'} = date('Y-m-d', strtotime('+' . $turnaround_time . ' hours'));
    }
  }
}
