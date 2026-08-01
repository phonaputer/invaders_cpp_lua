#include "framework/event_broker.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <ostream>
#include <vector>

struct TestMessage {
    int id;
    bool operator==(const TestMessage &) const = default;
    friend void PrintTo(const TestMessage &tm, std::ostream *os) {
      *os << "TestMessage(" << tm.id << ")";
    }
};

struct TestMessageTwo {
    int id_two;
    bool operator==(const TestMessageTwo &) const = default;
    friend void PrintTo(const TestMessageTwo &tm, std::ostream *os) {
      *os << "TestMessageTwo(" << tm.id_two << ")";
    }
};

TEST(EventBroker, GetAllWhenNoMessagesExistShouldReturnEmptyVec) {
  auto mb = std::make_unique<framework::EventBroker>();

  auto result = mb->get_all<TestMessage>();

  EXPECT_TRUE(result.empty());
}

TEST(EventBroker, GetAllWhenSeveralMessagesExistShouldReturnAllInOrder) {
  auto mb = std::make_unique<framework::EventBroker>();
  auto m1 = TestMessage{.id = 1};
  auto m2 = TestMessage{.id = 2};
  auto m3 = TestMessage{.id = 3};
  mb->push_back(m1);
  mb->push_back(m2);
  mb->push_back(m3);

  auto result = mb->get_all<TestMessage>();

  const std::vector<TestMessage> expected = {m1, m2, m3};
  EXPECT_THAT(expected, testing::ElementsAreArray(result));
}

TEST(EventBroker, SetWithMessagesOfMultipleTypesShouldStoreDifferentTypesSeparately) {
  auto mb = std::make_unique<framework::EventBroker>();
  auto m1 = TestMessage{.id = 1};
  mb->push_back(m1);
  auto m2 = TestMessageTwo{.id_two = 2};
  mb->push_back(m2);

  auto result = mb->get_all<TestMessage>();

  const std::vector<TestMessage> expected_tm = {m1};
  EXPECT_THAT(expected_tm, testing::ElementsAreArray(mb->get_all<TestMessage>()));
  const std::vector<TestMessageTwo> expected_tm2 = {m2};
  EXPECT_THAT(expected_tm2, testing::ElementsAreArray(mb->get_all<TestMessageTwo>()));
}

TEST(EventBroker, GetSingletonAfterSettingShouldReturnTheSetValue) {
  auto mb = std::make_unique<framework::EventBroker>();
  auto message = TestMessage{
      .id = 111,
  };
  mb->set_singleton(message);

  auto result = mb->get_singleton<TestMessage>();

  EXPECT_TRUE(result.has_value());
  EXPECT_EQ(message, result.value()); // NOLINT(bugprone-unchecked-optional-access)
}

TEST(EventBroker, DoubleSetSingletonShouldLeaveOnlyTheLastValuePresent) {
  auto mb = std::make_unique<framework::EventBroker>();
  auto message = TestMessage{
      .id = 111,
  };
  mb->set_singleton(message);
  auto messageTwo = TestMessage{
      .id = 222,
  };
  mb->set_singleton(messageTwo);

  auto result = mb->get_singleton<TestMessage>();

  EXPECT_TRUE(result.has_value());
  EXPECT_EQ(messageTwo, result.value()); // NOLINT(bugprone-unchecked-optional-access)
}

TEST(EventBroker, SetDifferentTypeSingletonsShouldNotInterfereWithEachOther) {
  auto mb = std::make_unique<framework::EventBroker>();
  auto message = TestMessage{
      .id = 111,
  };
  mb->set_singleton(message);
  auto messageTwo = TestMessageTwo{
      .id_two = 222,
  };
  mb->set_singleton(messageTwo);

  auto result = mb->get_singleton<TestMessage>();
  EXPECT_TRUE(result.has_value());
  EXPECT_EQ(message, result.value()); // NOLINT(bugprone-unchecked-optional-access)

  auto resultTwo = mb->get_singleton<TestMessageTwo>();
  EXPECT_TRUE(resultTwo.has_value());
  EXPECT_EQ(messageTwo, resultTwo.value()); // NOLINT(bugprone-unchecked-optional-access)
}

TEST(EventBroker, GetSingletonWhenNoneExistsShouldReturnEmptyOptional) {
  auto mb = std::make_unique<framework::EventBroker>();

  auto result = mb->get_singleton<TestMessage>();

  EXPECT_FALSE(result.has_value());
}

TEST(EventBroker, ClearAllShouldClearAllEvents) {
  auto mb = std::make_unique<framework::EventBroker>();
  auto m1 = TestMessage{.id = 1};
  mb->push_back(m1);
  auto m2 = TestMessageTwo{.id_two = 2};
  mb->push_back(m2);
  auto s1 = TestMessage{
      .id = 111,
  };
  mb->set_singleton(s1);
  auto s2 = TestMessageTwo{
      .id_two = 222,
  };
  mb->set_singleton(s2);

  mb->clear_all();

  EXPECT_EQ(0, mb->get_all<TestMessage>().size());
  EXPECT_EQ(0, mb->get_all<TestMessageTwo>().size());
  EXPECT_FALSE(mb->get_singleton<TestMessage>().has_value());
  EXPECT_FALSE(mb->get_singleton<TestMessageTwo>().has_value());
}

TEST(EventBroker, ClearShouldNotRemoveDrawPhaseEvents) {
  auto mb = std::make_unique<framework::EventBroker>();
  auto m1 = TestMessage{.id = 1};
  mb->push_back(m1);
  auto m2 = TestMessageTwo{.id_two = 2};
  mb->push_back_draw(m2);

  mb->clear_all();

  EXPECT_TRUE(mb->get_all<TestMessage>().empty());
  const std::vector<TestMessageTwo> expected_tm2 = {m2};
  EXPECT_THAT(expected_tm2, testing::ElementsAreArray(mb->get_all_draw<TestMessageTwo>()));
}

TEST(EventBroker, GetAllDrawWhenNoMessagesExistShouldReturnEmptyVec) {
  auto mb = std::make_unique<framework::EventBroker>();

  auto result = mb->get_all_draw<TestMessage>();

  EXPECT_TRUE(result.empty());
}

TEST(EventBroker, GetAllDrawWhenSeveralMessagesExistShouldReturnAllInOrder) {
  auto mb = std::make_unique<framework::EventBroker>();
  auto m1 = TestMessage{.id = 1};
  auto m2 = TestMessage{.id = 2};
  auto m3 = TestMessage{.id = 3};
  mb->push_back_draw(m1);
  mb->push_back_draw(m2);
  mb->push_back_draw(m3);

  auto result = mb->get_all_draw<TestMessage>();

  const std::vector<TestMessage> expected = {m1, m2, m3};
  EXPECT_THAT(expected, testing::ElementsAreArray(result));
}

TEST(EventBroker, SetDrawWithMessagesOfMultipleTypesShouldStoreDifferentTypesSeparately) {
  auto mb = std::make_unique<framework::EventBroker>();
  auto m1 = TestMessage{.id = 1};
  mb->push_back_draw(m1);
  auto m2 = TestMessageTwo{.id_two = 2};
  mb->push_back_draw(m2);

  const std::vector<TestMessage> expected_tm = {m1};
  EXPECT_THAT(expected_tm, testing::ElementsAreArray(mb->get_all_draw<TestMessage>()));
  const std::vector<TestMessageTwo> expected_tm2 = {m2};
  EXPECT_THAT(expected_tm2, testing::ElementsAreArray(mb->get_all_draw<TestMessageTwo>()));
}

TEST(EventBroker, ClearDrawShouldRemoveAllMessagesOfAllTypes) {
  auto mb = std::make_unique<framework::EventBroker>();
  auto m1 = TestMessage{.id = 1};
  mb->push_back_draw(m1);
  auto m2 = TestMessageTwo{.id_two = 2};
  mb->push_back_draw(m2);

  mb->clear_all_draw();

  EXPECT_TRUE(mb->get_all_draw<TestMessage>().empty());
  EXPECT_TRUE(mb->get_all_draw<TestMessageTwo>().empty());
}