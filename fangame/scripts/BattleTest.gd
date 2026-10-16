extends Node

func _ready() -> void:
    var battle := BattleManager.new()
    add_child(battle)
    battle.player_pool = AnimaPool.new(50)
    battle.enemy_pool = AnimaPool.new(50)
    battle.state_changed.connect(func(s): print("État : ", s))
    var move := MoveData.new()
    move.palier = 1
    battle.choose_move(move)
    battle.end_turn()
    print("Réserve joueur : ", battle.player_pool.current)