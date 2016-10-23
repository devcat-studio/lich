# lich : Lua-C++ Interoperation Helper

## 소개
루아 C API는 단순하고 배우기도 쉬운 편이지만, 읽기는 어렵고, 실수하기는 쉽습니다.
lich는 루아 함수를 호출하거나, 컨테이너를 루아와 주고받거나,
루아 값에 대한 참조를 유지하는 등의 일을 C++에서 간편하게 하게 해 줍니다.

lich는 루아 5.1 에 대해 컴파일하고 테스트했습니다. 5.2나 5.3은 안해봤습니다.



## 사용 방법

### 준비
- https://www.lua.org/ftp/lua-5.1.5.tar.gz 를 받아서 `src` 디렉토리 안의 내용을 lich 리파지터리의 `lua-5.1.5` 폴더 안에 복사해주세요.
- `lich/all.h`를 인클루드하세요.
- `luaL_newstate()` 호출 직후에 `lich::enable_ref(L)` 을 불러주세요.

### load_program
```
lich::ref fn;
pair<bool, string> result = lich::load_program(L, "return 42", "", fn);
```
- 루아 프로그램을 컴파일하여 루아 함수로 바꾼 뒤, 실행하지 않고 참조를 갖고 있으려면 `load_program`을 사용하세요.
- 컴파일이 성공하면 `result.first`에 `true`가 담기고, 루아 함수가 `fn`에 담깁니다.
- 컴파일이 실패하면 `result.first`에 `false`가, `result.second`에는 에러 메시지가 담깁니다.

### run_program
```
tuple<int> rv;
pair<bool, string> result = lich::run_program(L, "return 42", "", rv);
assert(get<0>(rv) == 42);
```
- 루아 프로그램을 컴파일하여 루아 함수로 바꾼 뒤, 바로 실행하려면 `run_program`을 사용하세요.
- 컴파일과 실행이 성공하면 `result.first`에 `true`가 담기고, 실행 결과가 마지막 인자에 담깁니다.
- 컴파일이 실패하거나 실행 중 에러가 발생하면 `result.first`에 `false`가, `result.second`에는 에러 메시지가 담깁니다.
- 리턴값이 여럿이어도 받아올 수 있습니다. `tuple<int, int, string>` 과 같이 하면 됩니다.
- 특정 C++ 타입을 가정하지 않고 루아 값 형태를 유지한 채로 참조만 가져올 수도 있습니다. `int` 대신 `lich::ref`를 사용해 보세요.

### pcall
```
lich::ref ref;
lich::load_program(L, "return 42", "", ref);

tuple<int> rv;
lich::pcall(ref, tuple<>(), rv);
assert(get<0>(rv) == 42);
```
- 루아 함수를 실행하려면 `pcall`을 사용하세요.
- 리턴값의 의미는 `run_program`과 같습니다.
- 인자와 리턴값 모두 `tuple`로 주고받습니다.

### C++ 값으로 변환
- 원하는 C++ 타입에 대해 `push`와 `to` 전역 함수를 오버로드하면
`run_program`, `pcall`, `ref::to` 에서 그 타입의 값을 바로 사용하게 할 수 있습니다.
현재는 몇몇 기본 타입과 `std::string`, `std::vector`, `std::map`에 대해서만 구현해 두었습니다.

### ref
- 루아 값에 대한 참조를 C++에서 유지하고 싶으면 `ref`를 사용하세요.
- `ref` 객체로부터 특정 C++ 타입으로 변환하고 싶으면 `ref::to` 를 사용하면 됩니다.
- 반드시 `lua_close`를 하기 전에 모든 `ref` 객체를 소멸시켜야 합니다!
  그러지 않으면 `ref` 객체의 소멸자가 불리는 시점에 크래시가 납니다. 주의하세요.





## 빌드/테스트 방법

### Visual Studio
- Visual Studio 2015에서 빌드하고 테스트했습니다.
- `vs2015/lich-test.sln` 을 열고 실행해 보세요.
- 콘솔에 `OK`가 뜨면 정상입니다.

### g++
- g++ 4.8.2에서 빌드하고 테스트했습니다.
- `gcc` 폴더에서 `build_and_test` 쉘 스크립트를 실행하세요(cmake가 필요합니다).
- 콘솔에 `OK`가 뜨면 정상입니다.




## 남은 일
- `pcall`에서 전역 함수를 호출할 수 있게 하기
- C++ 객체를 `shared_ptr`을 통해 루아에 노출하기
