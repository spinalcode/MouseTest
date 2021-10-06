#include "Pokitto.h"
#include "Smile.h"
#include "ps2/PS2Mouse.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;

PS2Mouse ps2ms(P0_19, P1_11); // CLK, DAT

int MouseX, MouseY;

int main(){
    using PC=Pokitto::Core;
    using PS=Pokitto::Sound;
    using PD=Pokitto::Display;
    using PB=Pokitto::Buttons;


    PC::begin();
    PD::invisiblecolor = 0;
    PD::persistence = true;
    PD::adjustCharStep = 0;
    PD::adjustLineStep = 0;

    PS2Mouse::mouse_event_t evt_ms;
    while( PC::isRunning() ){
        
        if( !PC::update() ) 
            continue;

        if (ps2ms.processing(&evt_ms)) {
            printf("%c%c%c:%4d,%4d,%2d\n", evt_ms.left ? 'L' : '.', evt_ms.center ? 'C' : '.', evt_ms.right ? 'R' : '.', evt_ms.x, evt_ms.y, evt_ms.z);
            MouseX += evt_ms.x;
            MouseY += evt_ms.y;
        }
        PD::drawBitmap(MouseX, MouseY, Smile);
    }

    return(1);
}

