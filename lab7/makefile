chatbot: srv_eliza.c cnt_eliza.c
	gcc srv_eliza.c -o bin/srv_eliza
	gcc cnt_eliza.c -o bin/cnt_eliza

fifo: srvfifo.c cntfifo.c
	gcc srvfifo.c -o bin/srvfifo ; gcc cntfifo.c -o bin/cntfifo

clean:
	rm -f *~