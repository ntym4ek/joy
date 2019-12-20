<?php


function commerce_helper_commerce_customer_profile_insert($profile)
{
    global $user;
}

function commerce_helper_commerce_customer_profile_update($profile)
{
    if (!empty($_COOKIE['user_region']) || !empty($_COOKIE['user_boxberry'])) {
        $data = json_decode($profile->field_data['und'][0]['value'], TRUE);

        if (empty($_COOKIE['user_region'])) {
            unset($data['region']);
        } else {
            $cookie = json_decode($_COOKIE['user_region'], TRUE);
            $data['region'] = $cookie;
        }
        if (empty($_COOKIE['user_boxberry'])) {
            unset($data['boxberry']);
        } else {
            $cookie = json_decode($_COOKIE['user_boxberry'], TRUE);
            $data['boxberry'] = $cookie;
        }
        $profile->field_data['und'][0]['value'] = json_encode($data);
    }
}

