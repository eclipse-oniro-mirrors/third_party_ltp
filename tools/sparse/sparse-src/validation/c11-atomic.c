void f00(int _Atomic  dst);
void f01(int _Atomic *dst);
void f02(int _Atomic *dst);
void f03(int _Atomic *dst);

int _Atomic qo;
int         uo;

void f00(int dst)	  { }	/* check-should-fail */
void f01(typeof(&qo) dst) { }	/* check-should-pass */
void f02(int *dst)	  { }	/* check-should-fail */
void f03(typeof(&uo) dst) { }	/* check-should-fail */

void foo(void)
{
	qo = uo;		/* check-should-pass */
	uo = qo;		/* check-should-pass */
}

void ref(void)
{
	const int qo;
	int uo;
	const int *pqo;
	      int *puo;

	pqo = &qo;		/* check-should-pass */
	pqo = &uo;		/* check-should-pass */
	pqo = puo;		/* check-should-pass */

	puo = &uo;		/* check-should-pass */

	puo = &qo;		/* check-should-fail */
	puo = pqo;		/* check-should-fail */
}

void bar(void)
{
	int _Atomic *pqo;
	int         *puo;

	pqo = &qo;		/* check-should-pass */
	pqo = &uo;		/* check-should-fail */
	pqo = puo;		/* check-should-fail */

	puo = &uo;		/* check-should-pass */

	puo = &qo;		/* check-should-fail */
	puo = pqo;		/* check-should-fail */
}

void baz(void)
{
	typeof(&qo) pqo;
	typeof(&uo) puo;

	pqo = &qo;		/* check-should-pass */
	pqo = &uo;		/* check-should-fail*/
	pqo = puo;		/* check-should-fail */

	puo = &uo;		/* check-should-pass */

	puo = &qo;		/* check-should-fail */
	puo = pqo;		/* check-should-fail */
}

/*
 * check-name: C11 _Atomic type qualifier
 * check-command: sparse -Wno-decl $file
 *
 * check-error-start
c11-atomic.c:9:6: error: symbol 'f00' redeclared with different type (incompatible argument 1 (different modifiers)):
c11-atomic.c:9:6:    void extern [addressable] [toplevel] f00( ... )
c11-atomic.c:1:6: note: previously declared as:
c11-atomic.c:1:6:    void extern [addressable] [toplevel] f00( ... )
c11-atomic.c:11:6: error: symbol 'f02' redeclared with different type (incompatible argument 1 (different modifiers)):
c11-atomic.c:11:6:    void extern [addressable] [toplevel] f02( ... )
c11-atomic.c:3:6: note: previously declared as:
c11-atomic.c:3:6:    void extern [addressable] [toplevel] f02( ... )
c11-atomic.c:12:6: error: symbol 'f03' redeclared with different type (incompatible argument 1 (different modifiers)):
c11-atomic.c:12:6:    void extern [addressable] [toplevel] f03( ... )
c11-atomic.c:4:6: note: previously declared as:
c11-atomic.c:4:6:    void extern [addressable] [toplevel] f03( ... )
c11-atomic.c:33:13: warning: incorrect type in assignment (different modifiers)
c11-atomic.c:33:13:    expected int *[assigned] puo
c11-atomic.c:33:13:    got int const *
c11-atomic.c:34:13: warning: incorrect type in assignment (different modifiers)
c11-atomic.c:34:13:    expected int *[assigned] puo
c11-atomic.c:34:13:    got int const *[assigned] pqo
c11-atomic.c:43:13: warning: incorrect type in assignment (different modifiers)
c11-atomic.c:43:13:    expected int [atomic] *[assigned] pqo
c11-atomic.c:43:13:    got int *
c11-atomic.c:44:13: warning: incorrect type in assignment (different modifiers)
c11-atomic.c:44:13:    expected int [atomic] *[assigned] pqo
c11-atomic.c:44:13:    got int *puo
c11-atomic.c:48:13: warning: incorrect type in assignment (different modifiers)
c11-atomic.c:48:13:    expected int *[assigned] puo
c11-atomic.c:48:13:    got int [atomic] *
c11-atomic.c:49:13: warning: incorrect type in assignment (different modifiers)
c11-atomic.c:49:13:    expected int *[assigned] puo
c11-atomic.c:49:13:    got int [atomic] *[assigned] pqo
c11-atomic.c:58:13: warning: incorrect type in assignment (different modifiers)
c11-atomic.c:58:13:    expected int [atomic] *[assigned] pqo
c11-atomic.c:58:13:    got int *
c11-atomic.c:59:13: warning: incorrect type in assignment (different modifiers)
c11-atomic.c:59:13:    expected int [atomic] *[assigned] pqo
c11-atomic.c:59:13:    got int *puo
c11-atomic.c:63:13: warning: incorrect type in assignment (different modifiers)
c11-atomic.c:63:13:    expected int *[assigned] puo
c11-atomic.c:63:13:    got int [atomic] *
c11-atomic.c:64:13: warning: incorrect type in assignment (different modifiers)
c11-atomic.c:64:13:    expected int *[assigned] puo
c11-atomic.c:64:13:    got int [atomic] *[assigned] pqo
 * check-error-end
 */
