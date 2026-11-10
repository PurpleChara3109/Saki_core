class_name Trainer
extends RefCounted

enum MarkState { DISCREET, VISIBLE, SATURATED }

var rank: int = 3
var gauge: int = 3
var marks: int = 0
var threshold_bonus: int = 0   # -2 pour un porteur de Don
var marks_per_push: int = 1    # 2 pour un porteur de Don
var pushes_per_mark: int = 1   # 2 pour un Techno, 3 pour Rémy
var push_count: int = 0

func mark_threshold() -> int:
    return 6 + rank + threshold_bonus

func mark_state() -> MarkState:
    if marks >= mark_threshold():
        return MarkState.SATURATED
    if marks * 2 >= mark_threshold():
        return MarkState.VISIBLE
    return MarkState.DISCREET

func can_push() -> bool:
    return gauge > 0 and mark_state() != MarkState.SATURATED

# Appeler après une Poussée réussie ou ratée.
func register_push() -> void:
    gauge -= 1
    push_count += 1
    if push_count % pushes_per_mark == 0:
        marks += marks_per_push