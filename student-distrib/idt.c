/*idt.c used to init interrupt description table*/

/*Version 1 ML 2021/3/20 18:53*/
/*Version 2 LYC 2021/3/20 18:45*/
/*Version 3 ZLH 2021/3/22 19:00*/
/*Version 4 ML 2021/3/22 19:53 add function header and comments*/

/*Version 1 ML*/
#include "x86_desc.h"
#include "lib.h"
#include "i8259.h"
#include "idt.h"
#include "rtc.h"
#include "keyboard.h"
#include "assembly.h"

/*Version 4 ML*/

/* construct_ex_handler
 * 
 * This function is used to build the exception handler
 * Inputs: function_name: the name of the exception
 *         string: the string we want to print as out "bule screen"
 * Outputs: None
 * Side Effects: None
 */
#define constuct_ex_handler(function_name, string)   \
void function_name(){                                \
    printf("%s\n",string);                           \
    while(1){}                                       \
}                                                    \

constuct_ex_handler(ex_divide_error,"ex_divide_error");
constuct_ex_handler(ex_reserved,"ex_reserved");
constuct_ex_handler(ex_nmi_interrupt,"ex_nmi_interrupt");
constuct_ex_handler(ex_breakpoint,"ex_breakpoint");
constuct_ex_handler(ex_overflow,"ex_overflow");
constuct_ex_handler(ex_bound_range_exceeded,"ex_bound_range_exceeded");
constuct_ex_handler(ex_invalid_opcode,"ex_invalid_opcode");
constuct_ex_handler(ex_device_not_available,"ex_device_not_available");
constuct_ex_handler(ex_double_fault,"ex_double_fault");
constuct_ex_handler(ex_coprocessor_segment,"ex_coprocessor_segment");
constuct_ex_handler(ex_invalid_tss,"ex_invalid_tss");
constuct_ex_handler(ex_segment_not_present,"ex_segment_not_present");
constuct_ex_handler(ex_stack_segment_fault,"ex_stack_segment_fault");
constuct_ex_handler(ex_general_protection,"ex_general_protection");
constuct_ex_handler(ex_page_fault,"ex_page_fault");
constuct_ex_handler(ex_math_fault,"ex_math_fault");
constuct_ex_handler(ex_alignment_check,"ex_alignment_check");
constuct_ex_handler(ex_machine_check,"ex_machine_check");
constuct_ex_handler(ex_simd_floating_point_exception,"ex_simd_floating_point_exception");


//This is only for checkpoint 1
/*Version 3 ZLH*/
constuct_ex_handler(system_call_handler,"system_call_handler");
/*Version 3 ZLH*/


/* init_idt()
 * 
 * This function is used to init the idt
 * Inputs: None
 * Outputs: None
 * Side Effects: set the exception, interrupt, system call handler function pointers to out idt
 */
void init_idt(){
    cli();
    int i; //loop index

// /* An interrupt descriptor entry (goes into the IDT) */
// typedef union idt_desc_t {
//     uint32_t val[2];
//     struct {
//         uint16_t offset_15_00;
//         uint16_t seg_selector;
//         uint8_t  reserved4;
//         uint32_t reserved3 : 1;
//         uint32_t reserved2 : 1;
//         uint32_t reserved1 : 1;
//         uint32_t size      : 1;
//         uint32_t reserved0 : 1;
//         uint32_t dpl       : 2;
//         uint32_t present   : 1;
//         uint16_t offset_31_16;
//     } __attribute__ ((packed));
// } idt_desc_t;

    for (i = 0; i<NUM_VEC; i++){
        // do not touch offset!

        // init
        idt[i].seg_selector = KERNEL_CS;
        idt[i].reserved4 = 0x0;
        idt[i].reserved3 = 0x1; // it should be set to 0, when interrupt handlers
        idt[i].reserved2 = 0x1;
        idt[i].reserved1 = 0x1;
        idt[i].size = 0x1; // size<-1 means that 32-bit space for each interrupt handler 
        idt[i].reserved0 = 0x0;
        idt[i].dpl = 0; // user level (system call) : 3, kernel level : 0
        idt[i].present = 0; // present should be set to 1 to use
    }

    // call the SET_IDT_ENTRY function for each exception
    SET_IDT_ENTRY(idt[0], ex_divide_error);
    SET_IDT_ENTRY(idt[1], ex_reserved);
    SET_IDT_ENTRY(idt[2], ex_nmi_interrupt);
    SET_IDT_ENTRY(idt[3], ex_breakpoint);
    SET_IDT_ENTRY(idt[4], ex_overflow);
    SET_IDT_ENTRY(idt[5], ex_bound_range_exceeded);
    SET_IDT_ENTRY(idt[6], ex_invalid_opcode);
    SET_IDT_ENTRY(idt[7], ex_device_not_available);
    SET_IDT_ENTRY(idt[8], ex_double_fault);
    SET_IDT_ENTRY(idt[9], ex_coprocessor_segment);
    SET_IDT_ENTRY(idt[10], ex_invalid_tss);
    SET_IDT_ENTRY(idt[11], ex_segment_not_present);
    SET_IDT_ENTRY(idt[12], ex_stack_segment_fault);
    SET_IDT_ENTRY(idt[13], ex_general_protection);
    SET_IDT_ENTRY(idt[14], ex_page_fault);
    //SET_IDT_ENTRY[idt[15], ]; DO NOT USE!
    SET_IDT_ENTRY(idt[16], ex_math_fault);
    SET_IDT_ENTRY(idt[17], ex_alignment_check);
    SET_IDT_ENTRY(idt[18], ex_machine_check);
    SET_IDT_ENTRY(idt[19], ex_simd_floating_point_exception);

    for (i=0; i<15; i++){
        idt[i].present = 1;
    }
    for (i=16; i<20; i++){
        idt[i].present = 1;
    }

    /*Version 2 LYC*/
    SET_IDT_ENTRY(idt[RTC_IDT_INDEX],rtc_interrupt_handler);
    idt[RTC_IDT_INDEX].present=1;
    SET_IDT_ENTRY(idt[KEYBOARD_IDT_INDEX],keyboard_interrupt_handler);
    idt[KEYBOARD_IDT_INDEX].present=1;
    /*Version 2 LYC*/
    
    /*Version 3 ZLH*/
    SET_IDT_ENTRY(idt[SYSTEM_IDT_INDEX],system_call_handler);
    idt[SYSTEM_IDT_INDEX].present = 1;
    /*Version 3 ZLH*/    

    // set the special case
    lidt(idt_desc_ptr);
    sti();
    return;
}
/*Version 1 ML*/

