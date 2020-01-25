#include "calc_unit.hpp"

using namespace std;

void get_multi_plan_track(const vector<Plan> &plans, vector<BID> &tracks) {
  for (int i = 0; i < plans.size(); ++i) {
    const vector<const Block *> &track = plans[i].track();
    for (int j = 0; j < track.size(); ++j) {
      tracks.push_back(track[j]->id());
    }
  }
}

Calc_unit::Calc_unit(const vector<Plan> &root_plans) {
  _old_tracks = &_tracks1;
  _new_tracks = &_tracks2;
  _old_tracks->push_back(root_plans[0].track().size());
  get_multi_plan_track(root_plans, *_old_tracks);
}

void* Calc_unit::thread_run(void *p) {
  Calc_unit &unit = *((Calc_unit *) p);
  vector<BID> *tmp_ptr = NULL;

  unit._statis.reset();
  const Plan *curr_plan;
  vector<Plan> further_plans;
  vector<BID> curr_track;
  int track_BID_num = (*unit._old_tracks)[0];
  unit._new_tracks->push_back(track_BID_num + 1);
  for (int i = 1; i < unit._old_tracks->size(); ++i) {
    BID id = (*unit._old_tracks)[i];
    curr_track.push_back(id);
    if (curr_track.size() == track_BID_num) {
      curr_plan = unit._replayer.replay(curr_track);
      curr_track.clear();
      curr_plan->next_step(further_plans, unit._best_plan, unit._statis);
      get_multi_plan_track(further_plans, *unit._new_tracks);
      further_plans.clear();
    }
  }

  unit._old_tracks->clear();
  tmp_ptr = unit._old_tracks;
  unit._old_tracks = unit._new_tracks;
  unit._new_tracks = tmp_ptr;

  return NULL;
}

void Calc_unit::start_round() {
  if (pthread_create(&_tid, NULL, Calc_unit::thread_run, this) != 0) {
    throw "Failed to create thread";
  }
}

void Calc_unit::join_round() {
  if (pthread_join(_tid, NULL) != 0) {
    throw "Failed to join thread";
  }
}
