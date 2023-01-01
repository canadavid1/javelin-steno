//---------------------------------------------------------------------------

#include "all_up.h"
#include "../console.h"

//---------------------------------------------------------------------------

void StenoAllUp::Process(const StenoKeyState &value, StenoAction action) {
  nextProcessor.Process(value, action);

  switch (action) {
  case StenoAction::PRESS:
    pressedKeyState |= value;
    break;

  case StenoAction::RELEASE:
    if (value.IsEmpty()) {
      nextProcessor.Process(pressedKeyState, StenoAction::TRIGGER);
      pressedKeyState.Reset();
    }
    break;

  case StenoAction::CANCEL:
    pressedKeyState.Reset();
    break;

  case StenoAction::TRIGGER:
    break;
  }
}

void StenoAllUp::PrintInfo() const {
  Console::Printf("  AllUp\n");
  nextProcessor.PrintInfo();
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

#include "../unit_test.h"
#include "fake_processor.h"

TEST_BEGIN("AllUp tests") {
  FakeStenoProcessor fakeProcessor;
  StenoAllUp allUp(fakeProcessor);
  StenoProcessor processor(allUp);

  processor.Process(StenoKey::KL, true);
  processor.Process(StenoKey::A, true);
  processor.Process(StenoKey::TR, true);
  const StenoKeyState katKeyState = processor.GetCurrentKeyState();
  assert(fakeProcessor.triggers.size() == 0);

  processor.Process(StenoKey::A, false);
  assert(fakeProcessor.triggers.size() == 0);

  processor.Process(StenoKey::A, true);
  assert(fakeProcessor.triggers.size() == 0);

  processor.Process(StenoKey::KL, false);
  assert(fakeProcessor.triggers.size() == 0);

  processor.Process(StenoKey::A, false);
  processor.Process(StenoKey::TR, false);
  assert(fakeProcessor.triggers.size() == 1);
  assert(fakeProcessor.triggers[0] == katKeyState);
}
TEST_END

//---------------------------------------------------------------------------
