#include "idt.h"
#include "isr.h"
#include "gdt.h"

void __attribute((cdecl)) i686_isr_0();
void __attribute((cdecl)) i686_isr_1();
void __attribute((cdecl)) i686_isr_2();
void __attribute((cdecl)) i686_isr_3();
void __attribute((cdecl)) i686_isr_4();
void __attribute((cdecl)) i686_isr_5();
void __attribute((cdecl)) i686_isr_6();
void __attribute((cdecl)) i686_isr_7();
void __attribute((cdecl)) i686_isr_8();
void __attribute((cdecl)) i686_isr_9();
void __attribute((cdecl)) i686_isr_10();
void __attribute((cdecl)) i686_isr_11();
void __attribute((cdecl)) i686_isr_12();
void __attribute((cdecl)) i686_isr_13();
void __attribute((cdecl)) i686_isr_14();
void __attribute((cdecl)) i686_isr_15();
void __attribute((cdecl)) i686_isr_16();
void __attribute((cdecl)) i686_isr_17();
void __attribute((cdecl)) i686_isr_18();
void __attribute((cdecl)) i686_isr_19();
void __attribute((cdecl)) i686_isr_20();
void __attribute((cdecl)) i686_isr_21();
void __attribute((cdecl)) i686_isr_22();
void __attribute((cdecl)) i686_isr_23();
void __attribute((cdecl)) i686_isr_24();
void __attribute((cdecl)) i686_isr_25();
void __attribute((cdecl)) i686_isr_26();
void __attribute((cdecl)) i686_isr_27();
void __attribute((cdecl)) i686_isr_28();
void __attribute((cdecl)) i686_isr_29();
void __attribute((cdecl)) i686_isr_30();
void __attribute((cdecl)) i686_isr_31();
void __attribute((cdecl)) i686_isr_32();
void __attribute((cdecl)) i686_isr_33();
void __attribute((cdecl)) i686_isr_34();
void __attribute((cdecl)) i686_isr_35();
void __attribute((cdecl)) i686_isr_36();
void __attribute((cdecl)) i686_isr_37();
void __attribute((cdecl)) i686_isr_38();
void __attribute((cdecl)) i686_isr_39();
void __attribute((cdecl)) i686_isr_40();
void __attribute((cdecl)) i686_isr_41();
void __attribute((cdecl)) i686_isr_42();
void __attribute((cdecl)) i686_isr_43();
void __attribute((cdecl)) i686_isr_44();
void __attribute((cdecl)) i686_isr_45();
void __attribute((cdecl)) i686_isr_46();
void __attribute((cdecl)) i686_isr_47();
void __attribute((cdecl)) i686_isr_48();
void __attribute((cdecl)) i686_isr_49();
void __attribute((cdecl)) i686_isr_50();
void __attribute((cdecl)) i686_isr_51();
void __attribute((cdecl)) i686_isr_52();
void __attribute((cdecl)) i686_isr_53();
void __attribute((cdecl)) i686_isr_54();
void __attribute((cdecl)) i686_isr_55();
void __attribute((cdecl)) i686_isr_56();
void __attribute((cdecl)) i686_isr_57();
void __attribute((cdecl)) i686_isr_58();
void __attribute((cdecl)) i686_isr_59();
void __attribute((cdecl)) i686_isr_60();
void __attribute((cdecl)) i686_isr_61();
void __attribute((cdecl)) i686_isr_62();
void __attribute((cdecl)) i686_isr_63();
void __attribute((cdecl)) i686_isr_64();
void __attribute((cdecl)) i686_isr_65();
void __attribute((cdecl)) i686_isr_66();
void __attribute((cdecl)) i686_isr_67();
void __attribute((cdecl)) i686_isr_68();
void __attribute((cdecl)) i686_isr_69();
void __attribute((cdecl)) i686_isr_70();
void __attribute((cdecl)) i686_isr_71();
void __attribute((cdecl)) i686_isr_72();
void __attribute((cdecl)) i686_isr_73();
void __attribute((cdecl)) i686_isr_74();
void __attribute((cdecl)) i686_isr_75();
void __attribute((cdecl)) i686_isr_76();
void __attribute((cdecl)) i686_isr_77();
void __attribute((cdecl)) i686_isr_78();
void __attribute((cdecl)) i686_isr_79();
void __attribute((cdecl)) i686_isr_80();
void __attribute((cdecl)) i686_isr_81();
void __attribute((cdecl)) i686_isr_82();
void __attribute((cdecl)) i686_isr_83();
void __attribute((cdecl)) i686_isr_84();
void __attribute((cdecl)) i686_isr_85();
void __attribute((cdecl)) i686_isr_86();
void __attribute((cdecl)) i686_isr_87();
void __attribute((cdecl)) i686_isr_88();
void __attribute((cdecl)) i686_isr_89();
void __attribute((cdecl)) i686_isr_90();
void __attribute((cdecl)) i686_isr_91();
void __attribute((cdecl)) i686_isr_92();
void __attribute((cdecl)) i686_isr_93();
void __attribute((cdecl)) i686_isr_94();
void __attribute((cdecl)) i686_isr_95();
void __attribute((cdecl)) i686_isr_96();
void __attribute((cdecl)) i686_isr_97();
void __attribute((cdecl)) i686_isr_98();
void __attribute((cdecl)) i686_isr_99();
void __attribute((cdecl)) i686_isr_100();
void __attribute((cdecl)) i686_isr_101();
void __attribute((cdecl)) i686_isr_102();
void __attribute((cdecl)) i686_isr_103();
void __attribute((cdecl)) i686_isr_104();
void __attribute((cdecl)) i686_isr_105();
void __attribute((cdecl)) i686_isr_106();
void __attribute((cdecl)) i686_isr_107();
void __attribute((cdecl)) i686_isr_108();
void __attribute((cdecl)) i686_isr_109();
void __attribute((cdecl)) i686_isr_110();
void __attribute((cdecl)) i686_isr_111();
void __attribute((cdecl)) i686_isr_112();
void __attribute((cdecl)) i686_isr_113();
void __attribute((cdecl)) i686_isr_114();
void __attribute((cdecl)) i686_isr_115();
void __attribute((cdecl)) i686_isr_116();
void __attribute((cdecl)) i686_isr_117();
void __attribute((cdecl)) i686_isr_118();
void __attribute((cdecl)) i686_isr_119();
void __attribute((cdecl)) i686_isr_120();
void __attribute((cdecl)) i686_isr_121();
void __attribute((cdecl)) i686_isr_122();
void __attribute((cdecl)) i686_isr_123();
void __attribute((cdecl)) i686_isr_124();
void __attribute((cdecl)) i686_isr_125();
void __attribute((cdecl)) i686_isr_126();
void __attribute((cdecl)) i686_isr_127();
void __attribute((cdecl)) i686_isr_128();
void __attribute((cdecl)) i686_isr_129();
void __attribute((cdecl)) i686_isr_130();
void __attribute((cdecl)) i686_isr_131();
void __attribute((cdecl)) i686_isr_132();
void __attribute((cdecl)) i686_isr_133();
void __attribute((cdecl)) i686_isr_134();
void __attribute((cdecl)) i686_isr_135();
void __attribute((cdecl)) i686_isr_136();
void __attribute((cdecl)) i686_isr_137();
void __attribute((cdecl)) i686_isr_138();
void __attribute((cdecl)) i686_isr_139();
void __attribute((cdecl)) i686_isr_140();
void __attribute((cdecl)) i686_isr_141();
void __attribute((cdecl)) i686_isr_142();
void __attribute((cdecl)) i686_isr_143();
void __attribute((cdecl)) i686_isr_144();
void __attribute((cdecl)) i686_isr_145();
void __attribute((cdecl)) i686_isr_146();
void __attribute((cdecl)) i686_isr_147();
void __attribute((cdecl)) i686_isr_148();
void __attribute((cdecl)) i686_isr_149();
void __attribute((cdecl)) i686_isr_150();
void __attribute((cdecl)) i686_isr_151();
void __attribute((cdecl)) i686_isr_152();
void __attribute((cdecl)) i686_isr_153();
void __attribute((cdecl)) i686_isr_154();
void __attribute((cdecl)) i686_isr_155();
void __attribute((cdecl)) i686_isr_156();
void __attribute((cdecl)) i686_isr_157();
void __attribute((cdecl)) i686_isr_158();
void __attribute((cdecl)) i686_isr_159();
void __attribute((cdecl)) i686_isr_160();
void __attribute((cdecl)) i686_isr_161();
void __attribute((cdecl)) i686_isr_162();
void __attribute((cdecl)) i686_isr_163();
void __attribute((cdecl)) i686_isr_164();
void __attribute((cdecl)) i686_isr_165();
void __attribute((cdecl)) i686_isr_166();
void __attribute((cdecl)) i686_isr_167();
void __attribute((cdecl)) i686_isr_168();
void __attribute((cdecl)) i686_isr_169();
void __attribute((cdecl)) i686_isr_170();
void __attribute((cdecl)) i686_isr_171();
void __attribute((cdecl)) i686_isr_172();
void __attribute((cdecl)) i686_isr_173();
void __attribute((cdecl)) i686_isr_174();
void __attribute((cdecl)) i686_isr_175();
void __attribute((cdecl)) i686_isr_176();
void __attribute((cdecl)) i686_isr_177();
void __attribute((cdecl)) i686_isr_178();
void __attribute((cdecl)) i686_isr_179();
void __attribute((cdecl)) i686_isr_180();
void __attribute((cdecl)) i686_isr_181();
void __attribute((cdecl)) i686_isr_182();
void __attribute((cdecl)) i686_isr_183();
void __attribute((cdecl)) i686_isr_184();
void __attribute((cdecl)) i686_isr_185();
void __attribute((cdecl)) i686_isr_186();
void __attribute((cdecl)) i686_isr_187();
void __attribute((cdecl)) i686_isr_188();
void __attribute((cdecl)) i686_isr_189();
void __attribute((cdecl)) i686_isr_190();
void __attribute((cdecl)) i686_isr_191();
void __attribute((cdecl)) i686_isr_192();
void __attribute((cdecl)) i686_isr_193();
void __attribute((cdecl)) i686_isr_194();
void __attribute((cdecl)) i686_isr_195();
void __attribute((cdecl)) i686_isr_196();
void __attribute((cdecl)) i686_isr_197();
void __attribute((cdecl)) i686_isr_198();
void __attribute((cdecl)) i686_isr_199();
void __attribute((cdecl)) i686_isr_200();
void __attribute((cdecl)) i686_isr_201();
void __attribute((cdecl)) i686_isr_202();
void __attribute((cdecl)) i686_isr_203();
void __attribute((cdecl)) i686_isr_204();
void __attribute((cdecl)) i686_isr_205();
void __attribute((cdecl)) i686_isr_206();
void __attribute((cdecl)) i686_isr_207();
void __attribute((cdecl)) i686_isr_208();
void __attribute((cdecl)) i686_isr_209();
void __attribute((cdecl)) i686_isr_210();
void __attribute((cdecl)) i686_isr_211();
void __attribute((cdecl)) i686_isr_212();
void __attribute((cdecl)) i686_isr_213();
void __attribute((cdecl)) i686_isr_214();
void __attribute((cdecl)) i686_isr_215();
void __attribute((cdecl)) i686_isr_216();
void __attribute((cdecl)) i686_isr_217();
void __attribute((cdecl)) i686_isr_218();
void __attribute((cdecl)) i686_isr_219();
void __attribute((cdecl)) i686_isr_220();
void __attribute((cdecl)) i686_isr_221();
void __attribute((cdecl)) i686_isr_222();
void __attribute((cdecl)) i686_isr_223();
void __attribute((cdecl)) i686_isr_224();
void __attribute((cdecl)) i686_isr_225();
void __attribute((cdecl)) i686_isr_226();
void __attribute((cdecl)) i686_isr_227();
void __attribute((cdecl)) i686_isr_228();
void __attribute((cdecl)) i686_isr_229();
void __attribute((cdecl)) i686_isr_230();
void __attribute((cdecl)) i686_isr_231();
void __attribute((cdecl)) i686_isr_232();
void __attribute((cdecl)) i686_isr_233();
void __attribute((cdecl)) i686_isr_234();
void __attribute((cdecl)) i686_isr_235();
void __attribute((cdecl)) i686_isr_236();
void __attribute((cdecl)) i686_isr_237();
void __attribute((cdecl)) i686_isr_238();
void __attribute((cdecl)) i686_isr_239();
void __attribute((cdecl)) i686_isr_240();
void __attribute((cdecl)) i686_isr_241();
void __attribute((cdecl)) i686_isr_242();
void __attribute((cdecl)) i686_isr_243();
void __attribute((cdecl)) i686_isr_244();
void __attribute((cdecl)) i686_isr_245();
void __attribute((cdecl)) i686_isr_246();
void __attribute((cdecl)) i686_isr_247();
void __attribute((cdecl)) i686_isr_248();
void __attribute((cdecl)) i686_isr_249();
void __attribute((cdecl)) i686_isr_250();
void __attribute((cdecl)) i686_isr_251();
void __attribute((cdecl)) i686_isr_252();
void __attribute((cdecl)) i686_isr_253();
void __attribute((cdecl)) i686_isr_254();
void __attribute((cdecl)) i686_isr_255();

void i686_isr_init_gates()
{
    i686_idt_descriptor(0, i686_isr_0, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(1, i686_isr_1, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(2, i686_isr_2, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(3, i686_isr_3, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(4, i686_isr_4, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(5, i686_isr_5, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(6, i686_isr_6, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(7, i686_isr_7, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(8, i686_isr_8, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(9, i686_isr_9, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(10, i686_isr_10, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(11, i686_isr_11, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(12, i686_isr_12, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(13, i686_isr_13, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(14, i686_isr_14, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(15, i686_isr_15, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(16, i686_isr_16, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(17, i686_isr_17, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(18, i686_isr_18, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(19, i686_isr_19, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(20, i686_isr_20, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(21, i686_isr_21, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(22, i686_isr_22, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(23, i686_isr_23, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(24, i686_isr_24, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(25, i686_isr_25, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(26, i686_isr_26, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(27, i686_isr_27, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(28, i686_isr_28, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(29, i686_isr_29, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(30, i686_isr_30, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(31, i686_isr_31, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(32, i686_isr_32, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(33, i686_isr_33, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(34, i686_isr_34, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(35, i686_isr_35, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(36, i686_isr_36, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(37, i686_isr_37, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(38, i686_isr_38, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(39, i686_isr_39, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(40, i686_isr_40, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(41, i686_isr_41, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(42, i686_isr_42, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(43, i686_isr_43, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(44, i686_isr_44, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(45, i686_isr_45, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(46, i686_isr_46, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(47, i686_isr_47, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(48, i686_isr_48, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(49, i686_isr_49, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(50, i686_isr_50, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(51, i686_isr_51, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(52, i686_isr_52, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(53, i686_isr_53, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(54, i686_isr_54, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(55, i686_isr_55, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(56, i686_isr_56, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(57, i686_isr_57, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(58, i686_isr_58, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(59, i686_isr_59, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(60, i686_isr_60, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(61, i686_isr_61, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(62, i686_isr_62, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(63, i686_isr_63, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(64, i686_isr_64, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(65, i686_isr_65, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(66, i686_isr_66, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(67, i686_isr_67, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(68, i686_isr_68, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(69, i686_isr_69, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(70, i686_isr_70, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(71, i686_isr_71, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(72, i686_isr_72, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(73, i686_isr_73, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(74, i686_isr_74, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(75, i686_isr_75, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(76, i686_isr_76, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(77, i686_isr_77, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(78, i686_isr_78, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(79, i686_isr_79, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(80, i686_isr_80, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(81, i686_isr_81, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(82, i686_isr_82, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(83, i686_isr_83, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(84, i686_isr_84, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(85, i686_isr_85, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(86, i686_isr_86, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(87, i686_isr_87, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(88, i686_isr_88, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(89, i686_isr_89, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(90, i686_isr_90, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(91, i686_isr_91, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(92, i686_isr_92, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(93, i686_isr_93, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(94, i686_isr_94, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(95, i686_isr_95, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(96, i686_isr_96, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(97, i686_isr_97, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(98, i686_isr_98, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(99, i686_isr_99, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(100, i686_isr_100, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(101, i686_isr_101, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(102, i686_isr_102, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(103, i686_isr_103, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(104, i686_isr_104, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(105, i686_isr_105, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(106, i686_isr_106, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(107, i686_isr_107, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(108, i686_isr_108, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(109, i686_isr_109, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(110, i686_isr_110, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(111, i686_isr_111, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(112, i686_isr_112, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(113, i686_isr_113, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(114, i686_isr_114, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(115, i686_isr_115, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(116, i686_isr_116, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(117, i686_isr_117, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(118, i686_isr_118, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(119, i686_isr_119, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(120, i686_isr_120, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(121, i686_isr_121, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(122, i686_isr_122, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(123, i686_isr_123, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(124, i686_isr_124, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(125, i686_isr_125, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(126, i686_isr_126, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(127, i686_isr_127, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(128, i686_isr_128, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(129, i686_isr_129, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(130, i686_isr_130, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(131, i686_isr_131, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(132, i686_isr_132, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(133, i686_isr_133, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(134, i686_isr_134, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(135, i686_isr_135, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(136, i686_isr_136, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(137, i686_isr_137, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(138, i686_isr_138, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(139, i686_isr_139, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(140, i686_isr_140, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(141, i686_isr_141, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(142, i686_isr_142, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(143, i686_isr_143, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(144, i686_isr_144, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(145, i686_isr_145, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(146, i686_isr_146, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(147, i686_isr_147, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(148, i686_isr_148, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(149, i686_isr_149, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(150, i686_isr_150, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(151, i686_isr_151, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(152, i686_isr_152, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(153, i686_isr_153, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(154, i686_isr_154, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(155, i686_isr_155, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(156, i686_isr_156, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(157, i686_isr_157, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(158, i686_isr_158, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(159, i686_isr_159, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(160, i686_isr_160, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(161, i686_isr_161, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(162, i686_isr_162, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(163, i686_isr_163, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(164, i686_isr_164, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(165, i686_isr_165, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(166, i686_isr_166, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(167, i686_isr_167, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(168, i686_isr_168, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(169, i686_isr_169, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(170, i686_isr_170, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(171, i686_isr_171, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(172, i686_isr_172, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(173, i686_isr_173, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(174, i686_isr_174, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(175, i686_isr_175, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(176, i686_isr_176, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(177, i686_isr_177, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(178, i686_isr_178, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(179, i686_isr_179, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(180, i686_isr_180, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(181, i686_isr_181, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(182, i686_isr_182, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(183, i686_isr_183, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(184, i686_isr_184, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(185, i686_isr_185, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(186, i686_isr_186, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(187, i686_isr_187, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(188, i686_isr_188, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(189, i686_isr_189, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(190, i686_isr_190, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(191, i686_isr_191, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(192, i686_isr_192, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(193, i686_isr_193, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(194, i686_isr_194, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(195, i686_isr_195, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(196, i686_isr_196, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(197, i686_isr_197, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(198, i686_isr_198, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(199, i686_isr_199, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(200, i686_isr_200, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(201, i686_isr_201, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(202, i686_isr_202, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(203, i686_isr_203, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(204, i686_isr_204, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(205, i686_isr_205, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(206, i686_isr_206, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(207, i686_isr_207, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(208, i686_isr_208, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(209, i686_isr_209, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(210, i686_isr_210, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(211, i686_isr_211, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(212, i686_isr_212, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(213, i686_isr_213, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(214, i686_isr_214, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(215, i686_isr_215, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(216, i686_isr_216, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(217, i686_isr_217, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(218, i686_isr_218, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(219, i686_isr_219, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(220, i686_isr_220, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(221, i686_isr_221, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(222, i686_isr_222, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(223, i686_isr_223, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(224, i686_isr_224, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(225, i686_isr_225, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(226, i686_isr_226, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(227, i686_isr_227, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(228, i686_isr_228, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(229, i686_isr_229, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(230, i686_isr_230, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(231, i686_isr_231, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(232, i686_isr_232, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(233, i686_isr_233, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(234, i686_isr_234, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(235, i686_isr_235, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(236, i686_isr_236, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(237, i686_isr_237, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(238, i686_isr_238, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(239, i686_isr_239, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(240, i686_isr_240, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(241, i686_isr_241, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(242, i686_isr_242, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(243, i686_isr_243, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(244, i686_isr_244, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(245, i686_isr_245, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(246, i686_isr_246, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(247, i686_isr_247, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(248, i686_isr_248, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(249, i686_isr_249, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(250, i686_isr_250, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(251, i686_isr_251, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(252, i686_isr_252, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(253, i686_isr_253, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(254, i686_isr_254, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
    i686_idt_descriptor(255, i686_isr_255, i686_GDT_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32B_INT);
}