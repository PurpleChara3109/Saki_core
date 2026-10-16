class_name BattleManager
extends Node

enum State { INTENTION, MISE_EN_FORME, EMISSION, RESOLUTION, FIN_DE_TOUR }

signal state_changed(new_state: State)

var state: State = State.INTENTION
var player_pool: AnimaPool
var enemy_pool: AnimaPool
var player_cohesion := 0.2
var chosen_move: MoveData
var overcharge := false

func choose_move(move: MoveData, use_overcharge: bool = false) -> void:
    if state != State.INTENTION:
        return
    chosen_move = move
    overcharge = use_overcharge
    _set_state(State.MISE_EN_FORME)
    _shape_move()

func _shape_move() -> void:
    var is_breath := chosen_move.palier == 0
    var cost := chosen_move.cost(player_pool.maximum)
    if overcharge:
        cost = ceili(cost * 1.5)
    elif cost > player_pool.debit():
        _set_state(State.INTENTION)  # coup refusé : le dresseur donne un autre ordre
        return
    if is_breath and cost > player_pool.current:
        cost = 0  # le Souffle devient gratuit quand la réserve est vide
    elif cost > player_pool.current:
        _set_state(State.INTENTION)
        return
    var failed := not is_breath and randf() < player_pool.failure_chance(player_cohesion)
    player_pool.spend(cost)
    _set_state(State.FIN_DE_TOUR if failed else State.EMISSION)

func end_turn() -> void:
    player_pool.reflux(5.0)
    enemy_pool.reflux(5.0)
    _set_state(State.INTENTION)

func _set_state(new_state: State) -> void:
    state = new_state
    state_changed.emit(new_state)