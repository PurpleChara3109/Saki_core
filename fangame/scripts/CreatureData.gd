class_name CreatureData
extends Resource

@export var species_name: String
@export var signatures: Array[StringName] = []
@export var base_hp: int = 40
@export var base_attack: int = 40
@export var base_defense: int = 40
@export var base_speed: int = 40
@export_range(0.0, 1.0) var cohesion: float = 0.2
@export var reserve_base: int = 20
@export var moves: Array[MoveData] = []

func reserve_max(level: int) -> int:
    return reserve_base + 3 * level