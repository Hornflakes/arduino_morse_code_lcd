const char MORSE_BINARY_INDEXED_TREE[] = "_ETIANMSURWDKGOHUF_L_PJBXCYZQ__54_3___2__+____16=/_____7___8_90_____________________.________'____-_______________,____:";

int buildMorseTreeNodeIndex(int prevIndex, int dashOrDotAdd) {
	return 2*prevIndex + dashOrDotAdd;
}
