extends Node

func _ready() -> void:
    var pool := AnimaPool.new(50)
    var move := MoveData.new()
    move.palier = 2
    print("Coût d'un coup Puissant : ", move.cost(pool.maximum))
    pool.spend(move.cost(pool.maximum))
    print("Réserve : ", pool.current, " (", pool.percent(), " %)")
    pool.reflux(5.0)
    print("Après Reflux : ", pool.current)
    pool.current = 5
    print("Risque d'échec à 10 % : ", pool.failure_chance(0.2))