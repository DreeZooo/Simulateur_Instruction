#include "iss.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define IMM 0
#define ADD 1
#define ADD_EXT 2
#define SUB 3
#define XOR 4
#define OR 5
#define AND 6
#define SHIFT_R 7
#define OR_EXT 8
#define OR_SHIFT_L 9
#define LOAD 10
#define STORE 11
#define INF 12
#define EQUAL 13




uint16_t mem[(1<<16)-1];
uint16_t regs[4];



//Fonction d'extension de signe
uint16_t sext(uint8_t imm){
  if(imm & 0x80){
    return 0xFF00 | imm;
  }else{
    return imm;
  }
}

void load(char *buffer)
{
  char *token = strtok(buffer, "\n ");
  token = strtok(NULL, "\n ");
  token = strtok(NULL, "\n ");
  uint16_t addr = 0;
  while (token != NULL) {
    printf("token: %s\n", token);
    int n;
    int res = sscanf(token, "%x", &n);
    if (res != 1) {
      fprintf(stderr, "Error parsing input file.\n");
      exit(1);
    }
    mem[addr++] = (uint16_t)n;
    token = strtok(NULL, "\n ");
  }
  printf("\n");
}

int cpu(char *buffer){
  int pc = 0;
  int halt_solution = 0;
  memset(mem, 0, sizeof(mem));
  memset(regs, 0, sizeof(regs));
  load(buffer);

  while (1){
    uint16_t instruction = mem[pc];

    uint16_t opcode_interval = 0xF000;

    uint8_t opcode = (instruction & opcode_interval) >> 12;
    uint8_t Rd = (instruction >> 8) & 0x3;
    uint8_t Rs = (instruction >> 6) & 0x3;

    uint8_t imm = instruction & 0xFF;
    uint16_t extended_imm = sext(imm);

    if(instruction == IMM){
      halt_solution++;
    }else{
      halt_solution =0;
    }

    if(halt_solution >= 1000){
      break;
    }

    switch (opcode){
      case IMM : 
        regs[Rd] = extended_imm; 
        pc++;
        break;
      case ADD : 
        regs[Rd] = regs[Rd] + regs[Rs];
        pc++;
        break;
      case ADD_EXT :
        regs[Rd] = regs[Rs] + extended_imm;
        pc++;
        break;
      case SUB :
        regs[Rd] = regs[Rd] - regs[Rs];
        pc++;
        break;
      case XOR : 
        regs[Rd] = regs[Rd] ^ regs[Rs];
        pc += 1;
        break;
      case OR : 
        regs[Rd] = regs[Rd] | regs[Rs];
        pc++;
        break;
      case AND :
        regs[Rd] = regs[Rd] & regs[Rs];
        pc++;
        break; 
      case SHIFT_R : 
        regs[Rd] = regs[Rd] >> 1;
        pc++;
        break;
      case OR_EXT :
        regs[Rd] = regs[Rd] | imm;
        pc++;
        break;
      case OR_SHIFT_L :
        regs[Rd] = regs[Rd] | (imm << 8);
        pc++;
        break;
      case LOAD : 
        regs[Rd] = mem[regs[Rs]];
        pc++;
        break;
      case STORE :
        mem[regs[Rs]] = regs[Rd];
        pc++;
        break;
      case INF :
        if(regs[Rd] < regs[Rs]){
          pc += extended_imm;
        }else{
          pc++;
        }
        break;
      case EQUAL :
        if(regs[Rd] == regs[Rs]){
          pc += extended_imm; 
        }else{
          pc++;
        }
        break;
      default : 
        printf("Opcode inconnu\n");
        return -1;
    }
  }
  return 0;
}
