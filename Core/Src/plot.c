#include "stdio.h"
#include "plot.h"
#include "ssd1306.h"
#include <stdlib.h>
#include "math.h"
#include "usart.h"
#include "i2c.h"

#define GRAPH_WIDTH 80
#define GRAPH_HEIGHT 40
#define THRESHOLD_MULTIPLIER 1.2  // Moltiplicatore per la soglia di rilevazione dei picchi


// Inizializza la struttura PlotData
void init_graph(PlotData *graph) {
    graph->head = 0;
    graph->tail = 0;
    graph->num_points = 0;
    graph->min_value = 4095;
    graph->max_value = 0;  // Imposta il valore massimo al minimo valore float
}

void add_point(PlotData *graph, float x, float y) {


    float min_input = 1900.0;
    float max_input = 2500.0;

    float min_output = 5.0;
    float max_output = 40.0;
    char msg[30];
    if(y<min_input || y>max_input){
    	sprintf(msg, "%Sensore scollegato ");
    	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
    	ssd1306_SetCursor(10, 40);
		ssd1306_WriteString(msg, Font_7x10, White);
		ssd1306_UpdateScreen(&hi2c3);

    	return;
    }


    float scaled_y = 40 - ( min_output + ((y - min_input) / (max_input - min_input)) * (max_output - min_output));

    float sum=0.0;
    for (int i = graph->num_points-6; i<graph->num_points-1; i++){
    	sum+=graph->y[i];
    }
    scaled_y-= (sum+scaled_y)/6;


    if (x >= SSD1306_WIDTH) {
        x = SSD1306_WIDTH - 5;
    }

    ssd1306_SetCursor(x,scaled_y);
    	if(graph->num_points > 0  ){
	    	ssd1306_DrawLine(graph->x[graph->num_points-1], graph->y[graph->num_points-1],x,scaled_y,White);
    	}

		graph->x[graph->num_points] = x;
		graph->y[graph->num_points] = scaled_y;
		graph->num_points++;

		ssd1306_DrawPixel(x, scaled_y, White);

}


void scrollLeft(PlotData *graph) {
    for (int y = 0; y < SSD1306_HEIGHT - 14; y++) {
        for (int x = 1; x < SSD1306_WIDTH; x++) {
            uint8_t pixel = ssd1306_GetPixel(x, y);
            ssd1306_DrawPixel(x - 1, y, pixel);
        }
        ssd1306_DrawPixel(SSD1306_WIDTH - 1, y, Black);
    }

    if (graph->num_points > 80) {
        for (int i = 1; i < graph->num_points; i++) {
            graph->x[i - 1] = graph->x[i];
            graph->y[i - 1] = graph->y[i];
        }
        graph->num_points--;
    }
}


void detect_peaks(PlotData *graph, float *peaks_x, float *peaks_y, int *num_peaks) {
    *num_peaks = 0;

    for (int i = 1; i < graph->num_points - 1; i++) {
	if (graph->y[i] > graph->y[i - 1] && graph->y[i] > graph->y[i + 1] ) {
            peaks_x[*num_peaks] = graph->x[i];
            peaks_y[*num_peaks] = graph->y[i];
            (*num_peaks)++;
        }
    }
}


float calculate_heart_rate(float *peaks, int num_peaks) {
    if (num_peaks < 2) return 0;

    float total_intervals = 0.0;
    for (int i = 1; i < num_peaks; i++) {
        total_intervals += peaks[i] - peaks[i - 1];
    }
    float average_interval = total_intervals / (num_peaks - 1);
    float heart_rate = 6000 / average_interval;
    return heart_rate;
}

float calculate_hrv(float *peaks, int num_peaks) {
    if (num_peaks < 2) return 0;

    float intervals[num_peaks - 1];
    for (int i = 1; i < num_peaks; i++) {
        intervals[i - 1] = peaks[i] - peaks[i - 1];
    }

    // Calculate standard deviation of intervals
    float sum = 0;
    for (int i = 0; i < num_peaks - 1; i++) {
        sum += intervals[i];
    }
    float mean = sum / (num_peaks - 1);

    float variance = 0;
    for (int i = 0; i < num_peaks - 1; i++) {
        variance += (intervals[i] - mean) * (intervals[i] - mean);
    }
    variance /= (num_peaks - 1);

    float hrv = sqrt(variance);
    return hrv;
}
