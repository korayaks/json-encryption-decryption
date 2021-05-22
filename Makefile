INC="./inc"
LIB="./lib"
TARGET=Project
FINAL=kripto
all: $(FINAL)

kripto: ./lib/libfdr.a $(TARGET).c
		gcc -I$(INC) $(TARGET).c -o $@ -L$(LIB) -lfdr
clean:
		rm -rf $(FINAL)
run: clean $(FINAL) islem

islem: 
		./kripto -e ornek_metin encrypted
		./kripto -d encrypted decrypted

cleanall: 
		rm -rf encrypted decrypted ornek_metin *txt .kilit
