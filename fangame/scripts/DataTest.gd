extends Node

func _ready() -> void:
	var choc: MoveData = load("res://data/moves/test/bloom.tres")
	var caillou: CreatureData = load("res://data/creatures/tests/rika.tres")
	print(choc.move_name, " coûte ", choc.cost(caillou.reserve_max(10)))
	print(caillou.species_name, " : Réserve max ", caillou.reserve_max(10))
