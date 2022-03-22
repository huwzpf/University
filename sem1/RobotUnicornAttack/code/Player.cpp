#include "Player.h"
#include "TextureManager.h"

Player::Player(const char* texture, int **cc, int height, int width)
{
 // load all textures for unicorn's move
        
      jump_sprites = TextureManager::load_texture("./jump.bmp", true);
      run_sprites = TextureManager::load_texture("./run.bmp", true);
      fall_sprites = TextureManager::load_texture("./fall.bmp", true);
      uni_explosion= TextureManager::load_texture("./uni_explosion.bmp", true);
      obj_texture = TextureManager::load_texture(texture,true);

 // initialize all the variables

    controls = false;
    velx = 0;
    vely = 0;
    collision_cords = cc;
    level_width = width;
    level_height = height;
    moves_count = 0;
    posx = 0;
    posy = 0;
}

Player::~Player() {
    SDL_DestroyTexture(jump_sprites);
    SDL_DestroyTexture(fall_sprites);
    SDL_DestroyTexture(run_sprites);
    SDL_DestroyTexture(uni_explosion);
    free(collision_cords);
}

void Player::handleEvent(SDL_Event& e)
{

    //switch controls mode 
    if (e.type == SDL_KEYDOWN )
    {
        if (e.key.keysym.sym == SDLK_d) {
            if (controls == true) {
                controls = false;
                vely = 0;
                velx = 0;
            }
            else {
                controls = true;
                vely = 0;
            }
        }
    }

    //controls with arrows
    if (controls == true) {
        if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
        {
            switch (e.key.keysym.sym) {
            case SDLK_UP: vely = -model_velocity; break;
            case SDLK_DOWN: vely += model_velocity; break;
            case SDLK_LEFT: velx -= model_velocity; break;
            case SDLK_RIGHT: velx += model_velocity; break;
            }
        }
        else if (e.type == SDL_KEYUP && e.key.repeat == 0)
        {
            switch (e.key.keysym.sym) {
            case SDLK_UP: vely += model_velocity; break;
            case SDLK_DOWN: vely -= model_velocity; break;
            case SDLK_LEFT: velx += model_velocity; break;
            case SDLK_RIGHT: velx -= model_velocity; break;
            }
        }
    }
    //default controls
    else if (controls == false) {
        if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
            switch (e.key.keysym.sym) {
                
            case SDLK_z:
                if (jumps_count < 2) {
                vely = -JUMP_SPEED;
                jumps_count++;
                }
                break;
                
            case SDLK_x: {
                dash = true;
                velx += DASH_SPEED;
                jumps_count--;
                break; }
            }
        }
        else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
            switch (e.key.keysym.sym) {
                
            case SDLK_z:
                if (vely < 0) {
                vely = 0;
                }break;
                
            }
        }
    }

}

void Player::move(int time_n)
{

    moves_count++;

    handle_move_special_conditions(time_n);

    posx += velx; //update unicorn's positions 
    posy += vely;

    int ray_casting_dis = model_velocity;

    handle_collision_with_borders(ray_casting_dis);


    cast_rays_upwards(ray_casting_dis);

    cast_rays_downwards(ray_casting_dis);

    cast_rays_backwards(ray_casting_dis);

    cast_rays_forwards(ray_casting_dis);

    if (velx != 0 && vely != 0) { //if unicorn is moving at some angle, not perpendicularly or horizontally

        cast_angled_rays_left_top(ray_casting_dis);

        cast_angled_rays_right_bottom(ray_casting_dis);

        cast_angled_rays_right_top(ray_casting_dis);

        cast_angled_rays_left_bottom(ray_casting_dis);
    }

    //set destination rectangle parameters to unicorn's size and its position 
        dest_rect.h = model_height;
        dest_rect.w = model_width;
        dest_rect.x = posx;
        dest_rect.y = posy;
}



void Player::render(int camx, int camy)
{
    SDL_Rect src;

    animate(src);

    dest_rect.x -= camx; //adjust x and y by camera cords
    dest_rect.y -= camy;

    choose_texture();



    TextureManager::draw(obj_texture, src, dest_rect); //draw selected texture to screen 
}

//"ray casting" functions "cast" a ray looking for the colosest colliding object and changing unicorn's position so it touches it but doesnt cross it's border
// every function begins with if statement, so it doesnt cast rays in direction where there is no way to collide with any object, ie. if upwards velocity is 0 or negative unicorn cant hit anything above
// angled ray casting function cast rays at 45deg angle in order to properly calculate collisions at corners of the hitbox 
//conditional expressions in tests make sure it doesnt try to read from negative place in array
//collision_cords array consists of cords of every pixel and information if it is part of terrain or not 1 means it is, so unicorn have to collide with it
//all the functions are pretty simillar but they cast rays in different directions and from different places
//functions subtract velocity so it looks from starting position before the move 

void Player::cast_angled_rays_left_bottom(int ray_casting_dis)
{
    if //left bottom
        ((collision_cords[posy - (posy > 0 ? 1 : 0) + model_height][posx + 1] == 1) && (velx <= 0 && vely >= 0))
    { // first if statement checks if hitbox angle collided with anything and statements after first && make sure it moves in correct direction

        for (int i = ray_casting_dis; i >= 0; i--) {
            if (collision_cords[posy + model_height - vely + i][posx - velx - i] == 0) { // when it meets first non-terrain pixel 
                posx = posx - velx - i; //revert move and move it just by i value
                posy = posy - vely + i;
                jumps_count = 0;
                break;
            }
        }

    }
}

void Player::cast_angled_rays_right_top(int ray_casting_dis)
{
    if //right top
        ((collision_cords[posy + 1][posx + model_width - (posx > 0 ? 1 : 0)] == 1) && (velx >= 0 && vely <= 0)) {

        for (int i = ray_casting_dis; i >= 0; i--) {
            if (collision_cords[posy - vely - i][posx + model_width - velx + i] == 0) {
                posx = posx - velx + i;
                posy = posy - vely - i;
                if (i != 0) {
                    //  did_crash = true;
                }
                break;
            }
        }
    }
}

void Player::cast_angled_rays_right_bottom(int ray_casting_dis)
{
    if //right bottom
        ((collision_cords[posy + model_height - (posy > 0 ? 1 : 0)][posx + model_width - (posx > 0 ? 1 : 0)] == 1) && (velx >= 0 && vely >= 0)) {


        for (int i = ray_casting_dis; i >= 0; i--) {
            if (collision_cords[posy + model_height - vely + i - 1][posx + model_width - velx + i - 1] == 0) {
                posx = posx - velx + i;
                posy = posy - vely + i;
                jumps_count = 0;
                break;
            }
        }

    }
}

void Player::cast_angled_rays_left_top(int ray_casting_dis)
{
    if //left top
        ((collision_cords[posy + 1][posx + 1] == 1) && (velx <= 0 && vely <= 0)) {


        for (int i = 0; i < ray_casting_dis; i++) {
            if (collision_cords[posy - vely + i][posx - velx + i] == 0) {
                posx = posx - velx + i;
                posy = posy - vely + i;
                if (i != 0) {
                    // did_crash = true;
                }
                break;
            }
        }
    }
}

void Player::cast_rays_forwards(int ray_casting_dis) //casts one ray from each forward-facing corner 
{
    if (velx > 0) { 
        for (int i = 0; i < ray_casting_dis; i++) {
            if (collision_cords[posy - vely][posx - (posx > velx ? velx : 0) + model_width + i - 1] == 1) {
                posx = posx - velx + i - 1;
                if (i != 1) {
                    did_crash = true;
                    explode = true;
                }
                break;
            }
            for (int i = 0; i < ray_casting_dis; i++) {
                if (collision_cords[posy - vely + model_height - 1][posx - (posx > velx ? velx : 0) + model_width + i - 1] == 1) {
                    posx = posx - velx + i - 1;
                    if (i != 1) {
                        did_crash = true;
                        explode = true;
                    }
                    break;
                }
            }
        }
    }
}

void Player::cast_rays_backwards(int ray_casting_dis)  //casts one ray from each backwards-facing corner 
{
    if (velx < 0) {

        for (int i = 0; i < ray_casting_dis; i++) {
            if (collision_cords[posy - vely][posx - velx - i] == 1) {
                posx = posx - velx - i + 1;
                break;
            }
            for (int i = 0; i < ray_casting_dis; i++) {
                if (collision_cords[posy - vely + model_height - 1][posx - velx - i] == 1) {
                    posx = posx - velx - i + 1;
                    break;
                }
            }
        }
    }
}

void Player::cast_rays_downwards(int ray_casting_dis) //casts one ray from each downward-facing corner and one forom the middle of botom border of hitbox 
{
    if (vely > 0 && posy<level_height - model_height - ray_casting_dis) {
        for (int i = 0; i < ray_casting_dis; i++) {
            if (collision_cords[posy + model_height - vely + i][posx + model_width - (posx > velx ? velx : 0) - 1] == 1) {
                posy = posy - vely + i;
                jumps_count = 0;
                vely = 16;
                break;
            }
            else  if (collision_cords[posy + model_height - vely + i][posx - (posx>velx ? velx : 0)] == 1) {
                posy = posy - vely + i;
                jumps_count = 0;
                vely = 16;
                break;
            }
            else  if (collision_cords[posy + model_height - vely + i][posx + model_width / 2 - (posx>velx ? velx : 0)] == 1) {
                posy = posy - vely + i;
                jumps_count = 0;
                vely = 16;
                break;
            }
        }

    }
}

void Player::cast_rays_upwards(int ray_casting_dis) //casts one ray from each upwards-facing corner and one forom the middle of top border of hitbox 
{
    if (vely < 0 && posy>0) {

        for (int i = 0; i < ray_casting_dis; i++) {
            if (collision_cords[posy - ((posy < vely - i) ? vely : 0) - ((posy < vely - i) ? i : 0)][posx + model_width - (posx>velx ? velx : 0) - 1] == 1) {
                posy = posy - vely - i + 1;
                if (i != 1) {
                    did_crash = true;
                    explode = true;
                }
                break;
            }
            else  if (collision_cords[posy - ((posy < vely - i) ? vely : 0) - ((posy < vely - i) ? i : 0)][posx - (posx > velx ? velx : 0)] == 1) {
                posy = posy - vely - i + 1;
                if (i != 1) {
                    did_crash = true;
                    explode = true;
                }
                break;
            }
            else  if (collision_cords[posy - ((posy < vely - i) ? vely : 0) - ((posy < vely - i) ? i : 0)][posx + model_width / 2 - (posx > velx ? velx : 0)] == 1) {
                posy = posy - vely - i + 1;
                if (i != 1) {
                    did_crash = true;
                    explode = true;
                }
                break;
            }
        }

    }
}

void Player::handle_collision_with_borders(int ray_casting_dis)
{
    if (posx + model_width >= level_width) // turns unicorn's position to 0 => loops the map
    {
        posx = 0;
        loop = true; //indicates that level just looped 
    }



    if ((posx < 0) || (posx + model_width > level_width)) // left and right border collision control 

    {
        posx -= velx;
    }



    if (posy + model_height > level_height || posy<0) // top and bottom border collision control

    {
        posy -= vely;
    }


    if (posy + model_height + ray_casting_dis >= level_height) { //if unicorn falls under the map 
        did_crash = true;
        explode = true;
    }
}

void Player::handle_move_special_conditions(int time_n)
{
    if (dash == true) {
        freeze = true;
        if (dash_start == -1) dash_start = time_n; //save time when dash was started
        if (time_n - dash_start > DASH_TIME) { // if DASH_TIME already passed stop dash
            dash_start = -1;
            dash = false;
            freeze = false;
            velx -= DASH_SPEED;
        }
    }

    //freezes when unicorn is dashing 
    if (freeze == true) {
        vely = 0;
        if (did_crash == true) { // or when it hit a wall 
            velx = 0;
        }
    }


    if (controls == false) { //default controls 
        if (vely < MAX_FALLING_SPEED && moves_count % FALLING_UPDATE_RATE == 0) {
            if (dash != true)vely += FALLING_SPEED_UPDATE_AMOUNT;
        }
        if (velx < MAX_FORWARD_SPEED && moves_count % FORWARD_UPDATE_RATE == 1) {
            velx += FORWARD_SPEED_UPDATE_AMOUNT;
        }
    }
}


//--------------------------------------------------------------------------------RENDER--------------------------------------------------------------------------

void Player::animate(SDL_Rect& src)
{
    int x_arr[9]; //arrays with positions of every next sprite to display
    int y_arr[9];
    for (int i = 0; i < 9; i++) {
        x_arr[i] = 160 * (i % 3);
        y_arr[i] = 80 * (i % 3);
    }
    src.w = 160; //source sprite w and h
    src.h = 80;
    src.x = x_arr[animation_frames];
    src.y = y_arr[animation_frames];
    if (moves_count % 3 == 0) //update animation frames every 3rd frame so it doesnt run too fast
        animation_frames++;
    if (animation_frames == 9) // cycle from 0 to 9 
        animation_frames = 0;
}

void Player::choose_texture()
{
    if (vely < 0) { // negative y velocity means unicorn is moving upwards
        obj_texture = jump_sprites;
    }
    else if (vely > 0 && vely != 16) { // positive y velocity means unicorn is moving downwards, y velocity equal to 16 means it touches the ground so it is not falling 
        obj_texture = fall_sprites;
    }
    else {
        obj_texture = run_sprites;
    }

    if (explode == true) { //change texture when unicorn is destroyed 
        obj_texture = uni_explosion;
    }
}
