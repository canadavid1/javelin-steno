//---------------------------------------------------------------------------

#include "console.h"
#include "str.h"
#include "unicode.h"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

//---------------------------------------------------------------------------

ConsoleWriter::ClassData ConsoleWriter::classData = {
    {nullptr, nullptr, nullptr, nullptr},
    0,
    &ConsoleWriter::instance,
};

//---------------------------------------------------------------------------

ConsoleWriter ConsoleWriter::instance;

#ifdef RUN_TESTS

std::vector<char> Console::history;

__attribute__((weak)) void ConsoleWriter::Write(const char *data,
                                                size_t length) {
  std::copy(data, data + length, std::back_inserter(Console::history));
}

#endif

void ConsoleWriter::Push(IWriter *writer) {
  classData.data[classData.count++] = classData.active;
  classData.active = writer;
}

void ConsoleWriter::Pop() {
  if (classData.count == 0) {
    return;
  }
  classData.active = classData.data[--classData.count];
}

__attribute__((weak)) void
ConsoleWriter::SetConnection(ConnectionId connectionId,
                             uint16_t connectionHandle) {}

//---------------------------------------------------------------------------

Console Console::instance;

static const ConsoleCommand HELP_COMMAND = {
    .command = "help",
    .description = "Provides a list of commands",
    .handler = &Console::HelpCommand,
    .context = nullptr,
};

static const ConsoleCommand HELLO_COMMAND = {
    .command = "hello",
    .description = "Used to initiate a stable communication channel",
    .handler = &Console::HelloCommand,
    .context = nullptr,
};

static const size_t MAX_COMMAND_COUNT = 64;
static size_t commandCount = 2;
static ConsoleCommand commands[MAX_COMMAND_COUNT] = {
    HELLO_COMMAND,
    HELP_COMMAND,
};

//---------------------------------------------------------------------------

void Console::RegisterCommand(const ConsoleCommand &command) {
  assert(commandCount < MAX_COMMAND_COUNT);
  commands[commandCount++] = command;
}

void Console::RegisterCommand(const char *command, const char *description,
                              void (*handler)(void *context, const char *line),
                              void *context) {
  assert(commandCount < MAX_COMMAND_COUNT);
  commands[commandCount].command = command;
  commands[commandCount].description = description;
  commands[commandCount].handler = handler;
  commands[commandCount].context = context;
  commandCount++;
}

//---------------------------------------------------------------------------

#ifdef NDEBUG
void Console::PrintfInternal(const char *format, ...) {
#else
void Console::Printf(const char *format, ...) {
#endif
  va_list args;
  va_start(args, format);

  ConsoleWriter::VprintfToActive(format, args);
  va_end(args);
}

void Console::WriteAsJson(const char *data, char *buffer) {
  char *p = Str::WriteJson(buffer, data);
  Write(buffer, p - buffer);
}

void Console::WriteAsJson(const char *data) {
  size_t length = strlen(data);
  char *buffer = (char *)malloc(2 * length);
  WriteAsJson(data, buffer);
  free(buffer);
}

void Console::WriteButtonScriptEvent(const char *text) {
  Console::Printf("EV {\"event\":\"button_script_event\",\"text\":\"");
  Console::WriteAsJson(text);
  Console::Printf("\"}\n\n");
}

__attribute__((weak)) void Console::Flush() {}

//---------------------------------------------------------------------------

void Console::HandleInput(const char *data, size_t length) {
  for (size_t i = 0; i < length; ++i) {
    if (data[i] == 0) {
      continue;
    } else if (data[i] == '\n') {
      ProcessLineBuffer();
      continue;
    } else if (lineBufferCount == sizeof(lineBuffer) - 1) {
      ProcessLineBuffer();
    }
    lineBuffer[lineBufferCount++] = data[i];
  }
}

void Console::ProcessLineBuffer() {
  if (lineBufferCount == 0) {
    return;
  }

  lineBuffer[lineBufferCount] = '\0';
  lineBufferCount = 0;

  size_t commandOffset = 0;
  while (commandOffset < 12 &&
         Unicode::IsAsciiDigit(lineBuffer[commandOffset])) {
    ++commandOffset;
  }
  if (lineBuffer[commandOffset] == ' ') {
    commandOffset++;
    Write(lineBuffer, commandOffset);
  } else {
    commandOffset = 0;
  }

  char *lineBufferWithoutId = lineBuffer + commandOffset;

  const ConsoleCommand *command = GetCommand(lineBufferWithoutId);
  if (command) {
    (*command->handler)(command->context, lineBufferWithoutId);
  } else {
    Printf("ERR Invalid command. Use \"help\" for a list of commands\n\n");
  }
}

const ConsoleCommand *Console::GetCommand(const char *buffer) {
  for (size_t i = 0; i < commandCount; ++i) {
    if (Str::HasPrefix(buffer, commands[i].command) &&
        Unicode::IsWhitespace(buffer[strlen(commands[i].command)])) {
      return &commands[i];
    }
  }
  return nullptr;
}

bool Console::RunCommand(const char *command, IWriter &writer) {
  const ConsoleCommand *consoleCommand = GetCommand(command);
  if (!consoleCommand) {
    return false;
  }

  ConsoleWriter::Push(&writer);
  (*consoleCommand->handler)(consoleCommand->context, command);
  ConsoleWriter::Pop();

  return true;
}

void Console::SendOk() { Write("OK\n\n", 4); }

// Used to flush the output buffer and ensure a stable connection.
void Console::HelloCommand(void *context, const char *line) {
  char buffer[256];
  memset(buffer, 0, 256);
  Write(buffer, 256);
  Write("ID Hello\n\n", 10);
}

void Console::HelpCommand(void *context, const char *line) {
  for (size_t i = 0; i < commandCount; ++i) {
    Printf("%s: %s\n", commands[i].command, commands[i].description);
  }
  Write("\n", 1);
}

//---------------------------------------------------------------------------

#if RUN_TESTS

#include "unit_test.h"

TEST_BEGIN("Console should handle invalid commands") {
  Console console;
  console.HandleInput("asdf\n", 5);

  Console::history.push_back(0);
  assert(
      Str::Eq(&Console::history.front(),
              "ERR Invalid command. Use \"help\" for a list of commands\n\n"));

  Console::history.clear();
}
TEST_END

#endif

//---------------------------------------------------------------------------
