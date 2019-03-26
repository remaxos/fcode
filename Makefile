compile: main.c fcode_object.c fcode_parser.c
	gcc fcode_list.c fcode_object.c fcode_parser.c fcode_draw.c fcode_xml.c main.c -o fcode `pkg-config --cflags --libs gtk+-3.0` `pkg-config --cflags libxml-2.0` -lxml2
run: fcode
	./fcode /home/john/u-boot
clean:
	rm fcode 
