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
  memset(mem, 0, sizeof(mem));
  memset(regs, 0, sizeof(regs));
  int halt_solution = 0;
  load(buffer);
  

  while (1){
    uint16_t instruction = mem[pc];

    uint16_t opcode_interval = 0xF000;

    uint8_t opcode = (instruction & opcode_interval) >> 12;
    uint8_t Rd = (instruction >> 8) & 0x3;
    uint8_t Rs = (instruction >> 6) & 0x3;

    uint8_t imm = instruction & 0xFF;
    int16_t extended_imm = sext(imm);

    if(instruction == IMM){
      halt_solution++;
    }else{
      halt_solution =0;
    }

    if(halt_solution >= 2){
      break;
    }

    switch (opcode){
      case IMM : 
        printf("\n===========IMM===========\n");
        printf("IMM : R%d = 0x%04X,  (%d)\n", Rd, (uint16_t)extended_imm, extended_imm);
        printf("Avant : R%d = 0x%d (%d)\n", Rd, regs[Rd], (int16_t)regs[Rd]);
        regs[Rd] = extended_imm; 
        printf("Après : R%d = 0x%04X (%d)\n", Rd, regs[Rd], (int16_t)regs[Rd]);
        pc++;
        break;

      case ADD : 
        printf("\n===========ADD===========\n"); 
        printf("ADD : R%d = R%d + R%d\n", Rd, Rd, Rs);
        printf("Avant : R%d = 0x%04X (%d), R%d = 0x%04X (%d)\n", Rd, regs[Rd], (int16_t)regs[Rd], Rs, regs[Rs], (int16_t)regs[Rs]);
        regs[Rd] = regs[Rd] + regs[Rs];
        printf("Après : R%d = 0x%04X (%d)\n", Rd, regs[Rd], (int16_t)regs[Rd]);
        pc++;
        break;

      case ADD_EXT :
        printf("\n===========ADD_EXT===========\n");
        printf("ADD_EXT : R%d = R%d + 0x%04X (%d)\n", Rd, Rs, (uint16_t)extended_imm, (int16_t)extended_imm);
        printf("Avant : R%d = 0x%04X (%d)\n", Rs, regs[Rs], (int16_t)regs[Rs]);
        regs[Rd] = regs[Rs] + extended_imm;
        printf("Après : R%d = 0x%04X (%d)\n", Rd, regs[Rd], (int16_t)regs[Rd]);
        pc++;
        break;

      case SUB :
        printf("===========SUB===========\n");
        printf("SUB : R%d = R%d - R%d\n", Rd, Rd, Rs);
        printf("Avant : R%d = 0x%04X (%d), R%d = 0x%04X (%d)\n", Rd, regs[Rd], (int16_t)regs[Rd], Rs, regs[Rs], (int16_t)regs[Rs]);
        regs[Rd] = regs[Rd] - regs[Rs];
        printf("Après : R%d = 0x%04X (%d)\n", Rd, regs[Rd], (int16_t)regs[Rd]);
        pc++;
        break;

      case XOR : 
        printf("\n===========XOR===========\n");
        printf("XOR : R%d = R%d ^ R%d\n", Rd, Rd, Rs);
        printf("Avant : R%d = 0x%04X (%d), R%d = 0x%04X (%d)\n", Rd, regs[Rd], (int16_t)regs[Rd], Rs, regs[Rs], (int16_t)regs[Rs]);
        regs[Rd] = regs[Rd] ^ regs[Rs];
        printf("Après : R%d = 0x%04X (%d)\n", Rd, regs[Rd], (int16_t)regs[Rd]);
        pc += 1;
        break;

      case OR : 
        printf("\n===========OR===========\n");
        printf("OR : R%d = R%d | R%d\n", Rd, Rd, Rs);
        printf("Avant : R%d = 0x%04X (%d), R%d = 0x%04X (%d)\n", Rd, regs[Rd], (int16_t)regs[Rd], Rs, regs[Rs], (int16_t)regs[Rs]);
        regs[Rd] = regs[Rd] | regs[Rs];
        printf("Après : R%d = 0x%04X (%d)\n", Rd, regs[Rd], (int16_t)regs[Rd]);
        pc++;
        break;

      case AND:
        printf("\n===========AND===========\n");
        printf("AND : R%d = R%d & R%d\n", Rd, Rd, Rs);
        printf("Avant : R%d = 0x%04X (%d), R%d = 0x%04X (%d)\n", Rd, regs[Rd], (int16_t)regs[Rd], Rs, regs[Rs], (int16_t)regs[Rs]);
        regs[Rd] = regs[Rd] & regs[Rs];
        printf("Après : R%d = 0x%04X (%d)\n", Rd, regs[Rd], (int16_t)regs[Rd]);
        pc++;
        break;

      case SHIFT_R:
        printf("\n===========SHIFT_R===========\n");
        printf("SHIFT_R : R%d = R%d >> 1\n", Rd, Rd);
        printf("Avant : R%d = 0x%04X (%d)\n", Rd, regs[Rd], (int16_t)regs[Rd]);
        regs[Rd] = (regs[Rd] & 0x8000) | (regs[Rd] >> 1);
        printf("Après : R%d = 0x%04X (%d)\n", Rd, regs[Rd], (int16_t)regs[Rd]);
        pc++;
        break;

      case OR_EXT:
        printf("\n===========OR_EXT===========\n");
        printf("OR_EXT : R%d = R%d | 0x%02X (%d)\n", Rd, Rd, imm, imm);
        printf("Avant : R%d = 0x%04X (%d)\n", Rd, regs[Rd], (int16_t)regs[Rd]);
        regs[Rd] = regs[Rd] | imm;
        printf("Après : R%d = 0x%04X (%d)\n", Rd, regs[Rd], (int16_t)regs[Rd]);
        pc++;
        break;

      case OR_SHIFT_L:
        printf("\n===========OR_SHIFT_L===========\n");
        printf("OR_SHIFT_L : R%d = R%d | (0x%02X << 8)\n", Rd, Rd, imm);
        printf("Avant : R%d = 0x%04X (%d)\n", Rd, regs[Rd], (int16_t)regs[Rd]);
        regs[Rd] = regs[Rd] | (imm << 8);
        printf("Après : R%d = 0x%04X (%d)\n", Rd, regs[Rd], (int16_t)regs[Rd]);
        pc++;
        break;

      case LOAD:
        printf("\n===========LOAD===========\n");
        printf("LOAD : R%d = mem[R%d] (mem[0x%04X])\n", Rd, Rs, regs[Rs]);
        printf("Avant : R%d = 0x%04X (%d)\n", Rd, regs[Rd], (int16_t)regs[Rd]);
        regs[Rd] = mem[regs[Rs]];
        printf("Après : R%d = 0x%04X (%d)\n", Rd, regs[Rd], (int16_t)regs[Rd]);
        pc++;
        break;

      case STORE:
        printf("\n===========STORE===========\n");
        printf("STORE : mem[R%d] = R%d (mem[0x%04X] = 0x%04X)\n", Rs, Rd, regs[Rs], regs[Rd]);
        printf("Avant : mem[0x%04X] = 0x%04X\n", regs[Rs], mem[regs[Rs]]);
        mem[regs[Rs]] = regs[Rd];
        printf("Après : mem[0x%04X] = 0x%04X\n", regs[Rs], mem[regs[Rs]]);
        pc++;
        break;

      case INF:
        printf("===========INF===========\n");
        printf("INF : R%d < R%d ?\n", Rd, Rs);
        printf("R%d = 0x%04X (%d), R%d = 0x%04X (%d)\n", Rd, regs[Rd], (int16_t)regs[Rd], Rs, regs[Rs], (int16_t)regs[Rs]);
        if (regs[Rd] < regs[Rs]) {
            pc += extended_imm;
        } else {
            pc++;
        }
        break;

      case EQUAL:
        printf("===========EQUAL===========\n");
        printf("EQUAL : R%d == R%d ?\n", Rd, Rs);
        printf("R%d = 0x%04X (%d), R%d = 0x%04X (%d)\n", Rd, regs[Rd], (int16_t)regs[Rd], Rs, regs[Rs], (int16_t)regs[Rs]);
        if (regs[Rd] == regs[Rs]) {
            pc += extended_imm;
        } else {
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
