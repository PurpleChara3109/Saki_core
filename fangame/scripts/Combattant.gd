class_name Combatant
extends RefCounted

var data: CreatureData
var level: int
var hp: int
var hp_max: int
var pool: AnimaPool

func _init(creature: CreatureData, lvl: int) -> void:
	data = creature
	level = lvl
	hp_max = data.base_hp + level
	hp = hp_max
	pool = AnimaPool.new(data.reserve_max(level))

func is_down() -> bool:
	return hp <= 0

# Renvoie un dictionnaire : "result" vaut "ok", "failed" ou "refused".
func cast(move: MoveData, overcharge: bool) -> Dictionary:
	var is_breath := move.palier == 0
	var cost := move.cost(pool.maximum)
	if overcharge:
		cost = ceili(cost * 1.5)
	elif cost > pool.debit():
		return {"result": "refused", "cost": 0}
	if is_breath and cost > pool.current:
		cost = 0
	elif cost > pool.current:
		return {"result": "refused", "cost": 0}
	var failed := not is_breath and randf() < pool.failure_chance(data.cohesion)
	pool.spend(cost)
	return {"result": "failed" if failed else "ok", "cost": cost}

func damage_against(target: Combatant, move: MoveData, boosted: bool) -> int:
	var power := float(move.power)
	if boosted:
		power *= 1.3
	var base := ((2.0 * level / 5.0 + 2.0) * power * data.base_attack / target.data.base_defense) / 50.0 + 2.0
	return maxi(int(base), 1)
