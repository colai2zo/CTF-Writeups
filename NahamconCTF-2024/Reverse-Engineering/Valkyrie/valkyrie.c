/**
Adapted from decompiled Ghidra code of valkyrie
With some comments and deobfuscation added manually
*/
undefined8 o___76(long param_1)

{
  byte bVar1;
  byte bVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  void **ppvVar6;
  long lVar7;
  undefined8 uVar8;
  char **__endptr;
  ulong uVar9;
  long in_FS_OFFSET;
  uint local_10d8;
  uint local_1078;
  uint local_1008;
  uint local_fa8;
  byte local_f48;
  byte local_f18;
  byte local_ee8;
  byte local_eb8;
  byte local_e88;
  byte local_e58;
  byte local_e28;
  byte local_df8;
  byte local_dc8;
  byte local_d98;
  byte local_d68;
  byte local_d38;
  byte local_d08;
  byte local_cd8;
  byte local_ca8;
  byte local_c78;
  byte local_c48;
  byte local_c18;
  byte local_be8;
  byte local_bb8;
  byte local_b88;
  byte local_b18;
  byte local_a98;
  byte local_a28;
  byte local_9b8;
  byte local_988;
  byte local_958;
  byte local_928;
  byte local_8f8;
  byte local_8c8;
  byte local_898;
  byte local_868;
  byte local_838;
  byte local_7c8;
  byte local_748;
  byte local_6d8;
  byte local_668;
  byte local_5f8;
  byte local_578;
  byte local_508;
  byte local_498;
  byte local_428;
  byte local_3a8;
  byte local_338;
  byte local_2e8;
  byte local_2a8;
  byte local_268;
  byte local_228;
  uint local_1e8;
  uint local_1a8;
  uint local_168;
  uint local_128;
  int local_d4;
  int comp_counter;
  int local_cc;
  ulong local_b8;
  char local_8d;
  char local_8c;
  undefined local_8b;
  char local_8a;
  char local_89;
  byte local_88 [32];
  byte local_68 [4];
  undefined local_64;
  char local_63;
  char local_62;
  char local_61;
  char local_60;
  char local_5f;
  char local_5e;
  char local_5d;
  undefined local_5c;
  char local_5b;
  char local_5a;
  undefined local_59;
  undefined local_58;
  char local_57;
  char local_56;
  undefined local_55;
  char local_54;
  char local_53;
  char local_52;
  char local_51;
  undefined local_50;
  char local_4f;
  long local_40;
  
  local_40 = *(long *)(in_FS_OFFSET + 0x28);
  o___75 = malloc(0x18);
  *(void **)((long)o___75 + 0x10) = o___75;
  *(void **)o___75 = o___75;
  for (local_cc = 0; local_cc < 3; local_cc = local_cc + 1) {
    ppvVar6 = (void **)malloc(0x18);
    *(uint *)(ppvVar6 + 1) = ((uint)o___56 + (~(uint)o___56 | 0xfffffff7) + 9) * local_cc;
    ppvVar6[2] = *(void **)((long)o___75 + 0x10);
    *ppvVar6 = o___75;
    **(long ***)((long)o___75 + 0x10) = (long *)ppvVar6;
    *(void ***)((long)o___75 + 0x10) = ppvVar6;
  }
  o___63 = *(undefined8 *)((long)o___75 + 0x10);
  o___70 = o___63;
// Compare rip+0x6671, rip+0x6682 (might be one of the malloc'd values?)
  if (o___71 != o___77) {
    o___76(param_1);
  }
// Compare these same values in memory
  if (o___71 == o___77) { 
    local_8d = 'L';
  }
  else {
    local_8d = '\x04';
  }
  iVar4 = (int)o___56;
  // local_8c = 'o'
  // local_8d = 'L
  local_8c = (char)((uint)(&o___64)[(((int)(o___56 + 9 >> 0x3f) + iVar4 + 9U ^
                                     (uint)(o___56 + 9 >> 0x3f)) % 10) * 3] % DAT_0010a0b4) + 'l';
  
  // Still comparing the same ones
  // local_8b = 'v'
  if (o___71 == o___77) {
    local_8b = 0x76;  // this is what it takes the first time
  }
  else {
    local_8b = 0x7a;
  }
  bVar2 = (byte)o___56;
  bVar1 = (byte)o___66;
  
  // local_8a = 'e';
  local_8a = (bVar1 & (bVar2 ^ 0xf9)) * ((bVar2 ^ 6) & ~bVar1) +
             (bVar1 & (bVar2 ^ 6)) * (bVar1 | bVar2 ^ 6) + 'e';

  // local_89 = '\x00'
  local_89 = (char)((uint)(&o___64)[(((iVar4 - ((iVar4 + -10) * 2 & (uint)(o___56 + -10 >> 0x3f))) -
                                     10) % 10) * 3 + 1] % DAT_0010a0cc) + -5;
  if (o___71 == o___77) {
    iVar4 = 10;
    __endptr = (char **)0x0;
  }
  else {
    iVar4 = 9;
    __endptr = (char **)0x2;
  }
  lVar7 = strtol(&local_8d,__endptr,iVar4);
  // lVar7 = strtol("Love",(char **)0x0,10);
  srand((uint)lVar7);
  
  // for (int i = 0; i < 0x1a; i++)
  for (local_d4 = 0; local_d4 < 0x1a; local_d4 = local_d4 + 1) {
    iVar4 = rand();
    local_88[local_d4] = (byte)iVar4 ^ *(byte *)(param_1 + local_d4);
  }
  
  local_b8 = 0x19;
  if (o___71 == o___77) goto LAB_00104289;

  do {
    iVar4 = rand();
    uVar9 = (ulong)(long)iVar4 % ((ulong)(o___71 != o___77) + (ulong)(o___71 == o___77) + local_b8);
    ran
    bVar1 = local_88[local_b8];
    local_88[local_b8] = local_88[uVar9];
    local_88[uVar9] = bVar1;
    local_b8 = local_b8 + ((ulong)(o___71 != o___77) - (ulong)(o___71 == o___77));

LAB_00104289:
    uVar3 = (uint)o___56;
  } while ((ulong)((uint)(&o___64)[(((uVar3 - ((uVar3 - 5) * 2 & (uint)(o___56 + -5 >> 0x3f))) - 5)
                                   % 10) * 3 + 1] % DAT_0010a0cc) - 5 < local_b8);


  bVar2 = (byte)o___56;
  bVar1 = (byte)o___66;

// Assign the contents of local_68
  local_68[0] = 0xa7;
  local_68[1] = 0x86;
  local_68[2] = 0x8e;
  local_68[3] = 0x26;


  if (o___71 == o___77) {
    local_64 = 0x92;
  }
  else {
    local_64 = 3;
  }
  
  local_63 = 'L'
  local_62 = 'T';
  local_61 = 'o';


  local_338 = bVar1 & (bVar2 & 0xf6) * (~bVar2 & 9) + (bVar2 | 9) * (bVar2 & 9);
  local_3a8 = bVar1 | (bVar2 & 0xf6) * (~bVar2 & 9) + (bVar2 | 9) * (bVar2 & 9);
  local_428 = bVar1 & ~((bVar2 & 0xf6) * (~bVar2 & 9) + (bVar2 | 9) * (bVar2 & 9));
  local_498 = ~bVar1 & (bVar2 & 0xf6) * (~bVar2 & 9) + (bVar2 | 9) * (bVar2 & 9);
  local_508 = bVar1 & (bVar2 & 0xf6) * (~bVar2 & 9) + (bVar2 | 9) * (bVar2 & 9);
  local_578 = bVar1 | (bVar2 & 0xf6) * (~bVar2 & 9) + (bVar2 | 9) * (bVar2 & 9);
  local_5f8 = bVar1 & ~((bVar2 & 0xf6) * (~bVar2 & 9) + (bVar2 | 9) * (bVar2 & 9));
  local_668 = ~bVar1 & (bVar2 & 0xf6) * (~bVar2 & 9) + (bVar2 | 9) * (bVar2 & 9);
  local_60 = ((local_428 * local_498 + local_338 * local_3a8) * '\x02' -
             (local_508 * local_578 + local_5f8 * local_668)) + '\x1d';
  local_6d8 = bVar1 & (bVar2 & 0xf7) * (~bVar2 & 8) + (bVar2 | 8) * (bVar2 & 8);
  local_748 = bVar1 | (bVar2 & 0xf7) * (~bVar2 & 8) + (bVar2 | 8) * (bVar2 & 8);
  local_7c8 = bVar1 & ~((bVar2 & 0xf7) * (~bVar2 & 8) + (bVar2 | 8) * (bVar2 & 8));
  local_838 = ~bVar1 & (bVar2 & 0xf7) * (~bVar2 & 8) + (bVar2 | 8) * (bVar2 & 8);
  local_5f = local_7c8 * local_838 + local_6d8 * local_748 + -0x6a;
  local_5e = (bVar1 & ~((bVar2 * '\x02' & 2) + (bVar2 ^ 1))) *
             ((bVar2 * '\x02' & 2) + (bVar2 ^ 1) & ~bVar1) +
             (bVar1 & (bVar2 ^ 1) + (bVar2 * '\x02' & 2)) *
             (bVar1 | (bVar2 ^ 1) + (bVar2 * '\x02' & 2)) + -0x2c;
  local_5d = (((bVar1 & ~((bVar2 * '\x02' & 10) + (bVar2 ^ 5))) *
               ((bVar2 * '\x02' & 10) + (bVar2 ^ 5) & ~bVar1) +
              (bVar1 & (bVar2 ^ 5) + (bVar2 * '\x02' & 10)) *
              (bVar1 | (bVar2 ^ 5) + (bVar2 * '\x02' & 10))) * '\x02' -
             ((bVar1 & (bVar2 ^ 5) + (bVar2 * '\x02' & 10)) *
              (bVar1 | (bVar2 ^ 5) + (bVar2 * '\x02' & 10)) +
             (bVar1 & ~((bVar2 * '\x02' & 10) + (bVar2 ^ 5))) *
             ((bVar2 * '\x02' & 10) + (bVar2 ^ 5) & ~bVar1))) + -0x6d;
  local_5c = 0x8b;
  local_868 = bVar1 & bVar2 + 8;
  local_898 = bVar1 | bVar2 + 8;
  local_8c8 = bVar1 & -bVar2 - 9;
  local_8f8 = ~bVar1 & bVar2 + 8;
  local_928 = bVar1 & bVar2 + 8;
  local_958 = bVar1 | bVar2 + 8;
  local_988 = bVar1 & -bVar2 - 9;
  local_9b8 = ~bVar1 & bVar2 + 8;
  local_5b = ((local_8c8 * local_8f8 + local_868 * local_898) * '\x02' -
             (local_928 * local_958 + local_988 * local_9b8)) + -0x58;
  local_a28 = bVar1 & (bVar2 & 0xfc) * (~bVar2 & 3) + (bVar2 | 3) * (bVar2 & 3);
  local_a98 = bVar1 | (bVar2 & 0xfc) * (~bVar2 & 3) + (bVar2 | 3) * (bVar2 & 3);
  local_b18 = bVar1 & ~((bVar2 & 0xfc) * (~bVar2 & 3) + (bVar2 | 3) * (bVar2 & 3));
  local_b88 = ~bVar1 & (bVar2 & 0xfc) * (~bVar2 & 3) + (bVar2 | 3) * (bVar2 & 3);
  local_5a = local_b18 * local_b88 + local_a28 * local_a98 + '(';
  if (o___71 == o___77) {
    local_59 = 0xa9;
  }
  else {
    local_59 = 0;
  }
  if (o___71 == o___77) {
    local_58 = 0x18;
  }
  else {
    local_58 = 0x17;
  }
  local_bb8 = bVar1 & bVar2 - 3;
  local_be8 = bVar1 | bVar2 - 3;
  local_c18 = bVar1 & 2 - bVar2;
  local_c48 = ~bVar1 & bVar2 - 3;
  local_c78 = bVar1 & bVar2 - 3;
  local_ca8 = bVar1 | bVar2 - 3;
  local_cd8 = bVar1 & 2 - bVar2;
  local_d08 = ~bVar1 & bVar2 - 3;
  local_57 = ((local_c18 * local_c48 + local_bb8 * local_be8) * '\x02' -
             (local_c78 * local_ca8 + local_cd8 * local_d08)) + -0x66;
  local_56 = (char)((uint)(&o___64)[(((uVar3 ^ 10) - (uVar5 & (uVar3 * 2 ^ 0x14))) % 10) * 3] %
                   DAT_0010a0b4) + 'g';
  if (o___71 == o___77) {
    local_55 = 0x5a;
  }
  else {
    local_55 = 99;
  }
  local_54 = (char)((uint)(&o___64)[(((uVar5 ^ uVar3 ^ 4) - uVar5) % 10) * 3 + 1] % DAT_0010a0cc) +
             '9';
  local_d38 = bVar1 & bVar2 - 5;
  local_d68 = bVar1 | bVar2 - 5;
  local_d98 = bVar1 & 4 - bVar2;
  local_dc8 = ~bVar1 & bVar2 - 5;
  local_53 = local_d98 * local_dc8 + local_d38 * local_d68 + -0x66;
  local_52 = ((o___71 == o___77) - (o___71 != o___77)) * '\'';
  local_df8 = bVar1 & bVar2 + 4;
  local_e28 = bVar1 | bVar2 + 4;
  local_e58 = bVar1 & -bVar2 - 5;
  local_e88 = ~bVar1 & bVar2 + 4;
  local_51 = local_e58 * local_e88 + local_df8 * local_e28 + -0x75;
  if (o___71 == o___77) {
    local_50 = 0xee;
  }
  else {
    local_50 = 0xed;
  }
  local_eb8 = bVar1 & bVar2 - 5;
  local_ee8 = bVar1 | bVar2 - 5;
  local_f18 = bVar1 & 4 - bVar2;
  local_f48 = ~bVar1 & bVar2 - 5;
  local_4f = local_f18 * local_f48 + local_eb8 * local_ee8 + '\x1c';
  
  
  // This is where the magic happens
  comp_counter = 0;
  do {
    uVar5 = (uint)o___66;

    if (comp_counter > 0x19) {
      uVar8 = 1;
LAB_00107508:
      if (local_40 == *(long *)(in_FS_OFFSET + 0x28)) {
        return uVar8;
      }

    }
    if (((uint)(&o___64)[(((uVar3 - ((uVar3 + 6) * 2 & (uint)(o___56 + 6 >> 0x3f))) + 6) % 10) * 3]
         % DAT_0010a0b4 == DAT_0010a0a8) && (local_88[comp_counter] != local_68[comp_counter])) {
      if (o___71 == o___77) {
        uVar8 = 0;
      }
      else {
        uVar8 = 1;
      }
      goto LAB_00107508;
    }
    comp_counter = comp_counter +
                   (uint)(&o___64)[(((uVar3 * 2 ^ (uint)(o___56 * 2 >> 0x3f)) -
                                    (int)(o___56 * 2 >> 0x3f)) % 10) * 3 + 1] % DAT_0010a0cc + -4;
  } while( true );
}

