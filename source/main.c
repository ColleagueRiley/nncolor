#define RSGL_IMPLEMENTATION
#include "RSGL/RSGL.h"
#include <stdio.h>

#define RSNN_IMPLEMENTATION
#include "RSNN.h"

uint32_t P = 2147483647;
uint32_t A = 16807;
uint32_t current = 1;

double Rand() {
    current = current * A % P;
    double result = (double)current / P;
    return result;
}

const int ITERS = 4000;

void drawCircleLine(RSGL_point p) {
    size_t y, i;
    for (y = 0; y < 3; y++) {
        for (i = 0; i < 3; i++) {
            RSGL_drawLine(RSGL_POINT(p.x + (50 / 4), p.y + (50 / 2)), RSGL_POINT(185 + (50 / 4), 100 + (50 / 4) + (y * 55)), 3, RSGL_RGB(255, 255, 255));
        }
    }
}

int main(void) {
    RSNN_net* net = RSNN_netInit(3, 2, 3, 3);

    RSGL_window* win = RSGL_createWindow("nncolor", (RSGL_rect){0, 0, 700, 300}, RGFW_ALLOW_DND | RGFW_CENTER);

    RSGL_setFont(RSGL_loadFont("Super Easy.ttf"));

    RSGL_button redCir = RSGL_initButton();
    RSGL_button bluCir = RSGL_initButton();
    RSGL_button greenCir = RSGL_initButton();

    RSGL_button_setPolygon(&redCir, RSGL_RECT(20, 100, 50, 50), 50);
    RSGL_button_setPolygon(&bluCir, RSGL_RECT(20, 155, 50, 50), 50);
    RSGL_button_setPolygon(&greenCir, RSGL_RECT(20, 210, 50, 50), 50);

    while (RGFW_window_shouldClose(win) == false) {
        float inputs[3] = {redCir.toggle, bluCir.toggle, greenCir.toggle};

        size_t i, y;
        while (RSGL_window_checkEvent(win)) {
            if (win->event.type == RGFW_quit)
                break;
            
            switch (win->event.type) {
                case RGFW_keyReleased: {
                    size_t start = 0;
                    if (win->event.keyCode == RGFW_Left || win->event.keyCode == RGFW_Right)
                        start = 3;
                    
                    for (i = start; i < start + 3; i++)
                        net->weights_hidden[i] += 0.005 * ((win->event.keyCode == RGFW_Down || win->event.keyCode == RGFW_Left) ? -1 : 1);
                    
                    break;   
                }
                default:
                    break;
            }

            RSGL_button_update(&redCir, win->event);
            RSGL_button_update(&bluCir, win->event);
            RSGL_button_update(&greenCir, win->event);
        }

        if (win->event.type == RGFW_quit)
            break;

        for (y = 0; y < 3; y++)
            drawCircleLine(RSGL_POINT(20, 100 + (55 * y)));  

        for (i = 0; i < net->hidden_layers; i++) {
           for (y = 0; y < 3; y++) {
                drawCircleLine(RSGL_POINT(185 + (i * 180), 100 + (55 * y)));
            
                RSGL_drawLine(RSGL_POINT(200 + (i * 180), 125 + (55 * y)), RSGL_POINT(200 + (net->hidden_layers * 180), 180), 3, RSGL_RGB(255, 255, 255));
            }
        }

        for (i = 0; i < net->hidden_layers; i++) {
            RSGL_drawCircle(RSGL_CIRCLE(185 + (i * 180), 100, 50), RSGL_RGB((net->hidden[(i * net->hidden_len) + 0] * 255), 0, 0));
            RSGL_drawCircle(RSGL_CIRCLE(185 + (i * 180), 155, 50), RSGL_RGB(0, net->hidden[(i * net->hidden_len) + 1] * 255, 0));
            RSGL_drawCircle(RSGL_CIRCLE(185 + (i * 180), 210, 50), RSGL_RGB(0, 0, net->hidden[(i * net->hidden_len) + 2] * 255));
        }

        RSGL_button_setColor(&redCir, RSGL_RGB(inputs[0] * 255, 0, 0));
        RSGL_button_setColor(&bluCir, RSGL_RGB(0, inputs[1] * 255, 0));
        RSGL_button_setColor(&greenCir, RSGL_RGB(0, 0, inputs[2] * 255));

        RSGL_drawButton(redCir);
        RSGL_drawButton(bluCir);
        RSGL_drawButton(greenCir);
        RSGL_drawCircle(RSGL_CIRCLE(185 + (net->hidden_layers * 180), 155, 50), RSGL_RGB(net->output[0] * 255, net->output[1] * 255, net->output[2] * 255));

        RSNN_netEval(net, inputs);

        RSGL_drawText("Inputs", RSGL_CIRCLE(20, 60, 30), RSGL_RGB(0, 0, 0));
        RSGL_drawText("Hidden 1", RSGL_CIRCLE(185, 60, 30), RSGL_RGB(0, 0, 0));
        RSGL_drawText("Hidden 2", RSGL_CIRCLE(185 + 180, 60, 30), RSGL_RGB(0, 0, 0));
        RSGL_drawText("Output", RSGL_CIRCLE(185 + (net->hidden_layers * 180), 60, 30), RSGL_RGB(0, 0, 0));

        RSGL_drawText(RSGL_strFmt("%f", net->weights_hidden[0]), RSGL_CIRCLE(105, 80, 20), RSGL_RGB(40, 20, 40));
        RSGL_drawText(RSGL_strFmt("%f", net->weights_hidden[3]), RSGL_CIRCLE(265, 80, 20), RSGL_RGB(40, 20, 40));

        RSGL_window_clear(win, RSGL_RGB(100, 100, 100));
    }

    RSGL_window_close(win);

    RSNN_netFree(net);

    return 0;
}