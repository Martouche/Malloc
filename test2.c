/*
 *     klc-research
 *                  [http://www.klc-research.com]
 *
 *     RedKod.HQ
 *               [http://www.redkod.com]
 *
 *          NostroBO
 *                   [http://nostrobo.redkod.com]
 *                   [nostrobo@klc-research.com]
 *
 */

/*
 * includes *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * defines *
 */

/*
 * SECURE_MODE_ENABLE
 *
 * This option will secure your memory allocations.
 * This option does not assure security.
 * With this option, smalloc() will be slower.
 */

#define SECURE_MODE_ENABLE

/*
 * SECURE_CONTROL_ENABLE
 *
 * This option is used to control actively memory modifications.
 * With this option, smalloc() will be slower than with
 * SECURE_MODE_ENABLE option.
 */

#define SECURE_CONTROL_ENABLE

#define CHECKSUM_LIMIT 65536
#define ABS(x) (x >= 0 ? x : x/-1)
#define ERROR -1
#define OK 0

typedef struct s_secureblock
{
  unsigned int size;
  short checksum;
  short control;
} t_secureblock;

typedef struct s_block
{
  unsigned int size;
} t_block;

typedef struct s_securefreeblock
{
  int size;
  struct s_securefreeblock *next;
  short checksum;
  short control;
} t_securefreeblock;

typedef struct s_freeblock
{
  int size;
  struct s_freeblock *next;
} t_freeblock;

/*
 * prototypes *
 */

int calc_pow_mem(int);
void del_free_entry(void *);
void init_smalloc();
void show_alloc_mem();
void show_free_mem();
int check_block(char *);
int check_mem();
void *smalloc(unsigned int);
int sfree(char *);

/*
 * globals and statics *
 */

void *freelist = NULL;
void *blocklist = NULL;

static short init = 0;

static short checksum;
static short control;

/*
 * source *
 */

int calc_pow_mem(int mem_size)
{
  int mem;
  int pow;

  for (pow = 0, mem = 1; mem < mem_size; pow++, mem *= 2)
    ;

  return (mem);
}

void del_free_entry(void *addr)
{
  t_freeblock *previous;
  t_freeblock *tmp;

  previous = NULL;
  for (tmp = freelist; (tmp != NULL) && ((unsigned int) tmp != (unsigned int) addr); previous = tmp, tmp = tmp->next)
    ;

  if (tmp == NULL)
    return ;                /* free junk pointer */

  if (previous == NULL)
    {
      t_freeblock *next;

      next = tmp->next;
      freelist = next;

      return ;
    }
  previous->next = tmp->next;
}

void init_smalloc()
{
  srand(time(0));

  blocklist = sbrk(0);

  checksum = 0;
  control = rand() % CHECKSUM_LIMIT;

#ifdef SECURE_CONTROL_ENABLE
#ifndef SECURE_MODE_ENABLE
  fprintf(stderr, "***[ SECURE_CONTROL_ENABLE activated but SECURE_MODE_ENABLE not : error ]\n");
  exit(ERROR);
#endif
#endif

  init = 1;
}

void show_alloc_mem()
{
  unsigned int limit;
  int *addr;

  printf(".::[ Show Alloc Mem Function ]::.\n");
  (void *) limit = sbrk(0);
  printf("break : 0x%x\n", limit);

  for (addr = blocklist; (unsigned int) addr < limit;
       (unsigned int) addr = (unsigned int) addr + (int) ABS(*addr))
    if (*addr > 0)
      printf("0x%x - 0x%x : %u bytes\n", addr, (unsigned int) addr + (unsigned int) *addr, *addr);
}

void show_free_mem()
{
#ifdef SECURE_MODE_ENABLE
  t_securefreeblock *tmp;
#else
  t_freeblock *tmp;
#endif

  printf(".::[ Show Free Mem Function ]::.\n");
  printf("break : 0x%x\n", sbrk(0));

  for (tmp = freelist; tmp != NULL; tmp = tmp->next)
    printf("0x%x - 0x%x : %u bytes\n", tmp, (unsigned int) tmp + (unsigned int) ABS(tmp->size), ABS(tmp->size));
}

int check_block(char *addr)
{
  t_securefreeblock *sfb;
  t_secureblock *sb;
  t_secureblock *block;
  unsigned int limit;
  int *search;

#ifndef SECURE_MODE_ENABLE
  return (ERROR);
#endif

  (unsigned int) addr -= sizeof(*block);
  (unsigned int) block = (unsigned int) addr;

  (void *) limit = sbrk(0);

  for (search = blocklist; (unsigned int) search < limit;
       (unsigned int) search = (unsigned int) search + (int) ABS(*search))
    if (((unsigned int) search + (unsigned int) ABS(*search)) == (unsigned int) block)
      break;

  (unsigned int) sb = (unsigned int) search;
  (unsigned int) sfb = (unsigned int) search;

  if (*addr > 0)
    {
      if (sb->control == block->checksum)
	return (OK);
      else
	return (ERROR);
    }

  if (*addr < 0)
    {
      if (sfb->control == block->checksum)
	return (OK);
      else
	return (ERROR);
    }
}

int check_mem()
{
  t_securefreeblock *nsfb;
  t_securefreeblock *sfb;
  t_secureblock *nsb;
  t_secureblock *sb;
  unsigned int limit;
  int *search;

#ifndef SECURE_MODE_ENABLE
  return (ERROR);
#endif

  (void *) limit = sbrk(0);

  for ((unsigned int) search = blocklist;
       (unsigned int) search < limit;
       (unsigned int) search = (unsigned int) search + (unsigned int) ABS(*search))
    {
      int *next;

      (unsigned int) next = (unsigned int) search + (unsigned int) ABS(*search);
      (unsigned int) sb = (unsigned int) search;
      (unsigned int) sfb = (unsigned int) search;
      (unsigned int) nsb = (unsigned int) next;
      (unsigned int) nsfb = (unsigned int) next;

      if ((unsigned int) next >= limit)
	break;

      if (*search > 0 && *next > 0)
	if (sb->control != nsb->checksum)
	  return (ERROR);

      if (*search > 0 && *next < 0)
	if (sb->control != nsfb->checksum)
	  return (ERROR);

      if (*search < 0 && *next > 0)
	if (sfb->control != nsb->checksum)
	  return (ERROR);

      if (*search < 0 && *next < 0)
	if (sfb->control != nsfb->checksum)
	  return (ERROR);
    }
  return (OK);
}

void *smalloc(unsigned int size)
{
#ifdef SECURE_MODE_ENABLE
  t_securefreeblock *tmp;
  t_secureblock block;
#else
  t_freeblock *tmp;
  t_block block;
#endif
  int new_size;
  void *alloc;
  int find;

  if (!init)
    init_smalloc();

#ifdef SECURE_CONTROL_ENABLE
  if (check_mem() == ERROR)
    return (void *) (ERROR);
#endif

#ifdef SECURE_MODE_ENABLE
  block.checksum = checksum;
  block.control = control;
#endif

  new_size = size + sizeof(block);
  new_size = calc_pow_mem(new_size);

  block.size = new_size;

  find = 0;

  for (tmp = freelist; tmp != NULL && !find; tmp = tmp->next)
    if (ABS(tmp->size) == new_size)
      {
	alloc = tmp;
	find = 1;
      }

  if (find == 1)
    {
#ifdef SECURE_MODE_ENABLE
      (unsigned int) tmp = (unsigned int) alloc;
      block.checksum = tmp->checksum;
      block.control = tmp->control;
#endif
      del_free_entry(alloc);
      memcpy(alloc, &block, sizeof(block));
    }
  else
    {
      (char *) alloc = sbrk(new_size);
      memcpy(alloc, &block, sizeof(block));
    }

#ifdef SECURE_MODE_ENABLE
  checksum = control;
  control = rand() % CHECKSUM_LIMIT;
#endif

  return (void *) ((unsigned int) alloc + sizeof(block));
}

int sfree(char *addr)
{
#ifdef SECURE_MODE_ENABLE
  t_securefreeblock freeblock;
  t_secureblock *block;
#else
  t_freeblock freeblock;
  t_block *block;
#endif

#ifdef SECURE_CONTROL_ENABLE
  if (check_mem() == ERROR)
    return (ERROR);
#endif

  (unsigned int) addr -= sizeof(*block);
  (unsigned int) block = (unsigned int) addr;

  freeblock.size = - block->size;

#ifdef SECURE_MODE_ENABLE
  freeblock.checksum = block->checksum;
  freeblock.control = block->control;
#endif

  if (freelist == NULL)
    {
      freeblock.next = NULL;
      freelist = addr;
    }
  else
    {
      freeblock.next = freelist;
      freelist = addr;
    }

  memcpy(addr, &freeblock, sizeof(freeblock));

  return (OK);
}
