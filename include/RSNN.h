/*
* Copyright (c) 2024 ColleagueRiley ColleagueRiley@gmail.com
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following r estrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.



** #define RSNN_IMPLEMENTATION ** - include function defines
*/

#if !defined(u8)
    #include <stdint.h>

   	typedef uint8_t     u8;
	typedef int8_t      i8;
	typedef uint16_t   u16;
	typedef int16_t    i16;
	typedef uint32_t     u32;
	typedef int32_t    i32;
	typedef uint64_t   u64;
	typedef int64_t    i64;
   	typedef u8 b8;
#endif

#ifndef RSNN_H
#define RSNN_H

#ifndef RSNNDEF
#ifndef RSGLDEF
#ifdef __APPLE__
#define RSNNDEF extern inline
#else
#define RSNNDEF inline
#endif
#else
#define RSNNDEF RSGLDEF
#endif
#endif

#ifndef RSNN_CALLOC
#define RSNN_CALLOC calloc
#define RSNN_FREE free
#endif 

typedef struct RSNN_net {
    float* weights_hidden;
    float* biases_hidden;
    float* weights_output;
    float* biases_output;
    float* hidden;
    float* output;

    size_t inputs_len;
    size_t hidden_len;
	size_t hidden_layers;
    size_t out_len;
} RSNN_net;

RSNNDEF RSNN_net* RSNN_netInit(size_t inputs_len, size_t hidden_layers, size_t hidden_len, size_t out_len);
RSNNDEF void RSNN_netEval(RSNN_net* net, float* inputs);
RSNNDEF void RSNN_netFree(RSNN_net* net);
#endif /* ndef RSNN_H */

#ifdef RSNN_IMPLEMENTATION

RSNN_net* RSNN_netInit(size_t inputs_len, size_t hidden_layers, size_t hidden_len, size_t out_len) {
	RSNN_net* net = (RSNN_net*)malloc(sizeof(RSNN_net));
    net->inputs_len = inputs_len;
    net->hidden_len = hidden_len;
	net->hidden_layers = hidden_layers;
    net->out_len = out_len;

    net->weights_hidden = (float*)RSNN_CALLOC(hidden_len * hidden_layers, sizeof(*net->weights_hidden));
    net->biases_hidden = (float*)RSNN_CALLOC(hidden_len * hidden_layers, sizeof(*net->biases_hidden));
    net->weights_output = (float*)RSNN_CALLOC(hidden_len * out_len, sizeof(*net->weights_output));
    net->biases_output = (float*)RSNN_CALLOC(out_len, sizeof(*net->biases_output));
    net->hidden = (float*)RSNN_CALLOC(hidden_len * hidden_layers, sizeof(*net->hidden));
    net->output = (float*)RSNN_CALLOC(out_len, sizeof(*net->output));

    return net;
}

void RSNN_netEval(RSNN_net* net, float* inputs) {
	size_t i;
	size_t y;

	for (y = 0; y < net->hidden_layers; y++) {
		for (i = 0; i < net->hidden_len; i++) {
			float weight = 0;
			size_t j = 0;
			for (j = 0; j < net->hidden_len; j++) {
				weight += net->weights_hidden[(y * net->hidden_layers) + j];
			}

			float prevValue = 0; 
			if (y == 0) {
				prevValue = inputs[i];
			}
			else {				
				for (j = 0; j < net->hidden_len; j++) {
					prevValue += net->hidden[((y - 1) * (net->hidden_layers)) + j];	
				}
			}

			float value = net->biases_hidden[(y * (net->hidden_layers + 1)) + i] + prevValue;
			net->hidden[(y * (net->hidden_layers + 1)) + i] = value * (weight);
		}
	}

	for (i = 0; i < net->out_len; i++) {
		float weight = 1;
		size_t j;
		for (j = 0; j < net->out_len; j++) {
			weight += net->weights_output[(y * net->hidden_layers) + i];
		}

		float prevValue = net->hidden[((net->hidden_layers - 1) * net->hidden_layers) + i + 1];
		float value = net->biases_output[i] + prevValue;
		net->output[i] = value * weight;
	}
}

void RSNN_netFree(RSNN_net* net) {
    RSNN_FREE(net->weights_hidden);
    RSNN_FREE(net->biases_hidden);
    RSNN_FREE(net->weights_output);
    RSNN_FREE(net->biases_output);
    RSNN_FREE(net->hidden);
    RSNN_FREE(net->output);
	RSNN_FREE(net);
}

#endif /* RSNN_IMPLEMENTATION */