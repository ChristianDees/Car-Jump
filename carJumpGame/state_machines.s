.arch msp430g2553
    .data
    .align 2
    .global current_state
    current_state:  .word 0; current_state = INTRO
    .p2align 1,0
    .text

    ; global functions created
    .global state_intro
    .global state_waiting
    .global state_change_time
    .global state_control_page_one
    .global state_control_page_two
    .global state_game
    .global state_pause
    .global state_game_over
    .global transition
    
    ; global variables/functions needed
    .extern intro_buzz
    .extern display_intro
    .extern display_clock
    .extern blink_change_clock
    .extern display_controls
    .extern display_controls_two
    .extern move_clouds
    .extern move_enemy
    .extern jump_flag
    .extern character_jump
    .extern jump_buzz
    .extern display_score
    .extern display_pause
    .extern reset_pause
    .extern draw_game_over
    .extern buzz_flag
    .extern buzz_game_over
    .extern update_vars

state_intro:
    call #intro_buzz        ; intro_buzz()
    call #display_intro     ; display_intro()
    pop r0

state_waiting:
    call #display_clock     ; display_clock()
    pop r0

state_change_time:
    call #blink_change_clock; blink_change_clock()
    pop r0

state_control_page_one:
    call #display_controls  ; display_controls()
    pop r0

state_control_page_two:
    call #display_controls_two ; display_controls_two()
    pop r0

state_game:
    call #move_clouds       ; move_clouds()
    call #move_enemy        ; move_enemy()
    cmp.b #0, &jump_flag    ; jump_flag - 0
    jz state_game_end       ; if jump_flag == 0, end
    call #character_jump    ; character_jump()
    call #jump_buzz         ; jump_buzz()

state_game_end:
    call #display_score     ; display_score()
    pop r0

state_pause:
    call #display_pause     ; display_pause()
    call #reset_pause       ; reset_pause()
    pop r0

state_game_over:
    call #draw_game_over    ; draw_game_over()
    cmp.b #0, &buzz_flag    ; buzz_flag - 0
    jz state_game_over_end  ; if buzz_flag == 0, end
    call #buzz_game_over    ; buzz_game_over()

state_game_over_end:
    pop r0

transition:
    mov r12, &current_state ; current_state = next_state
    call #update_vars       ; update_vars()
    pop r0

    
