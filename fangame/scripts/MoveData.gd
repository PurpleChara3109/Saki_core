class_name MoveData
extends Resource

const COST_PERCENT := [5, 10, 20, 40]

@export var move_name: String
@export var signature: StringName
@export_enum("Impact", "Jet", "Lien") var forme: int
@export_enum("Souffle", "Standard", "Puissant", "Ultime") var palier: int = 1
@export var power: int = 40
@export var accuracy: int = 100

func cost(reserve_max: int) -> int:
    return ceili(reserve_max * COST_PERCENT[palier] / 100.0)