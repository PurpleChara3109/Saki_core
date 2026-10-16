extends Node2D

const TILE_SIZE := 16
const MOVE_TIME := 0.15

var moving := false

@onready var ray: RayCast2D = $RayCast2D

func _process(_delta: float) -> void:
    if moving:
        return
    var dir := Vector2.ZERO
    if Input.is_action_pressed("ui_right"):
        dir = Vector2.RIGHT
    elif Input.is_action_pressed("ui_left"):
        dir = Vector2.LEFT
    elif Input.is_action_pressed("ui_down"):
        dir = Vector2.DOWN
    elif Input.is_action_pressed("ui_up"):
        dir = Vector2.UP
    if dir != Vector2.ZERO:
        _try_move(dir)

func _try_move(dir: Vector2) -> void:
    ray.target_position = dir * TILE_SIZE
    ray.force_raycast_update()
    if ray.is_colliding():
        return
    moving = true
    var tween := create_tween()
    tween.tween_property(self, "position", position + dir * TILE_SIZE, MOVE_TIME)
    await tween.finished
    moving = false