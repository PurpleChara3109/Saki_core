extends Node

func _ready() -> void:
    var t := Trainer.new()
    assert(t.mark_threshold() == 9)
    t.threshold_bonus = -2
    assert(t.mark_threshold() == 7)

    var r := Trainer.new()          # Rémy : 1 Marque tous les 3 Poussées
    r.pushes_per_mark = 3
    r.gauge = 9
    for i in 3:
        r.register_push()
    assert(r.marks == 1)

    var d := Trainer.new()          # Don : 2 Marques par Poussée, seuil 7
    d.threshold_bonus = -2
    d.marks_per_push = 2
    d.gauge = 9
    for i in 4:
        d.register_push()
    assert(d.marks == 8)
    assert(d.mark_state() == Trainer.MarkState.SATURATED)
    assert(not d.can_push())
    print("Trainer: tous les tests passent")