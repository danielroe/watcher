#ifndef INOTIFY_H
#define INOTIFY_H

#include <unordered_map>
#include <sys/inotify.h>
#include "../shared/BruteForceBackend.hh"
#include "../DirTree.hh"
#include "../Signal.hh"

struct InotifySubscription {
  DirEntry *entry;
  Watcher *watcher;
};

class InotifyBackend : public BruteForceBackend {
public:
  void start() override;
  ~InotifyBackend();
  void subscribe(Watcher &watcher) override;
  void unsubscribe(Watcher &watcher) override;
private:
  int mPipe[2];
  int mInotify;
  std::unordered_multimap<int, InotifySubscription *> mSubscriptions;
  Signal mEndedSignal;

  void watchDir(Watcher &watcher, DirEntry *entry);
  void handleEvents();
  void handleEvent(struct inotify_event *event, std::unordered_set<Watcher *> &watchers);
  bool handleSubscription(struct inotify_event *event, InotifySubscription *sub);
};

#endif
