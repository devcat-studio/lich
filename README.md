# lich : Lua-C++ Interoperation Helper

## �Ұ�
��� C API�� ���� ����ؼ� ���ε� �ڵ带 ¥�� �Ǽ��ϱ� ���� �б� ��ƽ��ϴ�.
lich�� ���� C++���� ��� �Լ��� ȣ���ϰų�, �����̳ʸ� ��ƿ� �ְ�ްų�,
��� ���� ���� ������ �����ϴ� ���� ���� �����ϰ� �����ϰ� �� �� �ְ� �˴ϴ�.

lich�� ��� 5.1 �� ���� �������ϰ� �׽�Ʈ�߽��ϴ�. 5.2�� 5.3�� ���غý��ϴ�.



## ��� ���

### �غ�
- https://www.lua.org/ftp/lua-5.1.5.tar.gz �� �޾Ƽ� `src` ���丮 ���� ������ lich �������͸��� `lua-5.1.5` ���� �ȿ� �������ּ���.
- `lich/all.h`�� ��Ŭ����ϼ���.
- `luaL_newstate()` ȣ�� ���Ŀ� `lich::enable_ref(L)` �� �ҷ��ּ���.

### load_program
```
lich::ref fn;
pair<bool, string> result = lich::load_program(L, "return 42", "", fn);
```
- ��� ���α׷��� �������Ͽ� ��� �Լ��� �ٲ� ��, �������� �ʰ� ������ ���� �������� `load_program`�� ����ϼ���.
- �������� �����ϸ� `result.first`�� `true`�� ����, ��� �Լ��� `fn`�� ���ϴ�.
- �������� �����ϸ� `result.first`�� `false`��, `result.second`���� ���� �޽����� ���ϴ�.

### run_program
```
tuple<int> rv;
pair<bool, string> result = lich::run_program(L, "return 42", "", rv);
assert(get<0>(rv) == 42);
```
- ��� ���α׷��� �������Ͽ� ��� �Լ��� �ٲ� ��, �ٷ� �����Ϸ��� `run_program`�� ����ϼ���.
- �����ϰ� ������ �����ϸ� `result.first`�� `true`�� ����, ���� ����� ������ ���ڿ� ���ϴ�.
- �������� �����ϰų� ���� �� ������ �߻��ϸ� `result.first`�� `false`��, `result.second`���� ���� �޽����� ���ϴ�.
- ���ϰ��� �����̾ �޾ƿ� �� �ֽ��ϴ�. `tuple<int, int, string>` �� ���� �ϸ� �˴ϴ�.
- Ư�� C++ Ÿ���� �������� �ʰ� ��� �� ���¸� ������ ä�� ������ ������ ���� �ֽ��ϴ�. `int` ��� `lich::ref`�� ����� ������.

### pcall(ref, ARG, RET)
```
lich::ref ref;
lich::load_program(L, "return 42", "", ref);

tuple<int> rv;
lich::pcall(ref, tuple<>(), rv);
assert(get<0>(rv) == 42);
```
- ��� �Լ��� �����Ϸ��� `pcall`�� ����ϼ���.
- ���ϰ��� �ǹ̴� `run_program`�� �����ϴ�.
- ���ڿ� ���ϰ� ��� `tuple`�� �ְ�޽��ϴ�.

### pcall(L, ���� �Լ� �̸�, ARG, RET)
```
// type(1)
tuple<string> rv;
lich::pcall(L, "type", tuple<int>(1), rv);
assert(get<0>(rv) == "number");
```

### pcall(L, (���� �Լ� �̸�), ARG, RET)
```
// string.rep('��', 5)
tuple<string> rv;
lich::pcall(L, make_tuple("string", "rep"), make_tuple("��", 5), rv);
assert(get<0>(rv), "�ƾƾƾƾ�");
```

### pcall(L, (��ü�� �޼��� �̸�), ARG, RET)
```
// s = "justive rains from above"
lich::push(L, "justive rains from above");
lich::ref s;
lich::to(L, -1, s);
lua_pop(L, 1);

// s:upper() == s.upper(s)
tuple<string> rv;
lich::pcall(L, tie(s, "upper"), tie(s), rv);
assert(get<0>(rv), "JUSTIVE RAINS FROM ABOVE");
```

### C++ ������ ��ȯ
- ���ϴ� C++ Ÿ�Կ� ���� `push`�� `to` ���� �Լ��� �����ε��ϸ�
`run_program`, `pcall`, `ref::to` ���� �� Ÿ���� ���� �ٷ� ����ϰ� �� �� �ֽ��ϴ�.
����� ��� �⺻ Ÿ�԰� `std::string`, `std::vector`, `std::map`�� ���ؼ��� ������ �ξ����ϴ�.

### ref
- ��� ���� ���� ������ C++���� �����ϰ� ������ `ref`�� ����ϼ���.
- `ref` ��ü�κ��� Ư�� C++ Ÿ������ ��ȯ�ϰ� ������ `ref::to` �� ����ϸ� �˴ϴ�.
- �ݵ�� `lua_close`�� �ϱ� ���� ��� `ref` ��ü�� �Ҹ���Ѿ� �մϴ�!
  �׷��� ������ `ref` ��ü�� �Ҹ��ڰ� �Ҹ��� ������ ũ���ð� ���ϴ�. �����ϼ���.





## ����/�׽�Ʈ ���

### Visual Studio
- Visual Studio 2015���� �����ϰ� �׽�Ʈ�߽��ϴ�.
- `vs2015/lich-test.sln` �� ���� ������ ������.
- �ֿܼ� `OK`�� �߸� �����Դϴ�.

### g++
- g++ 4.8.2���� �����ϰ� �׽�Ʈ�߽��ϴ�.
- `gcc` �������� `build_and_test` �� ��ũ��Ʈ�� �����ϼ���(cmake�� �ʿ��մϴ�).
- �ֿܼ� `OK`�� �߸� �����Դϴ�.




## ���� ��
- C++ ��ü�� `shared_ptr`�� ���� ��ƿ� �����ϱ�
