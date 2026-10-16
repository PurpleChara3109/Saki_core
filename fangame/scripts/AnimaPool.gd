class_name AnimaPool
extends RefCounted

var current: int
var maximum: int

func _init(max_value: int) -> void:
    maximum = max_value
    current = max_value

func debit() -> int:
    return int(maximum / 4.0)

func percent() -> float:
    return 100.0 * current / maximum

func failure_chance(cohesion: float) -> float:
    var missing := 25.0 - percent()
    if missing <= 0.0:
        return 0.0
    return clampf(missing * 2.0, 0.0, 50.0) * (1.0 - cohesion)

func spend(amount: int) -> void:
    current = maxi(current - amount, 0)

func reflux(percent_of_max: float) -> void:
    current = mini(current + int(maximum * percent_of_max / 100.0), maximum)