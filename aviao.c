#include "aviao.h"

/**
 * aviao.c
 * Implementação das funções do aviao.h
 * Descrições em aviao.h
 **/

aviao_t * criar_aviao(size_t id, size_t combustivel) {
	aviao_t *aviao = (aviao_t *) malloc(sizeof (aviao_t));

	aviao->combustivel = combustivel;
	aviao->id = id;

	return aviao;
}

void abastecerAviao(aviao_t *aviao) {
	aviao->combustivel = 100;
}

void desaloca_aviao(aviao_t* aviao) {
	free(aviao);
}
