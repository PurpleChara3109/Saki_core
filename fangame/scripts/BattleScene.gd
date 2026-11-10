extends Control

const LEVEL := 10
const START_GAUGE := 3

@onready var enemy_name: Label = %FoeName
@onready var enemy_hp_bar: ProgressBar = %HPFoe
@onready var enemy_reserve_bar: ProgressBar = %FoeReserve
@onready var player_name: Label = %PlayerName
@onready var player_hp_bar: ProgressBar = %HPPlayer
@onready var player_reserve_bar: ProgressBar = %PlayerReserve
@onready var player_state: Label = %PlayerStatus
@onready var journal: Label = %Journal
@onready var actions: HBoxContainer = %Actions
@onready var overcharge_box: CheckButton = %Surge

var player: Combatant
var enemy: Combatant
var trainer:= Trainer.new()
var busy := false
var lines: Array[String] = []

func _ready() -> void:
	var caillou: CreatureData = load("res://data/creatures/tests/rika.tres")
	var etincelle: CreatureData = load("res://data/creatures/tests/pikachu.tres")
	player = Combatant.new(caillou, LEVEL)
	enemy = Combatant.new(etincelle, LEVEL)
	player_hp_bar.modulate = Color(0.5, 1.0, 0.5)
	enemy_hp_bar.modulate = Color(0.5, 1.0, 0.5)
	player_reserve_bar.modulate = Color(0.4, 0.7, 1.0)
	enemy_reserve_bar.modulate = Color(0.4, 0.7, 1.0)
	for move in player.data.moves:
		var button := Button.new()
		button.text = "%s (coût %d)" % [move.move_name, move.cost(player.pool.maximum)]
		button.pressed.connect(_on_move_pressed.bind(move))
		actions.add_child(button)
	_log("Le combat commence : %s contre %s." % [player.data.species_name, enemy.data.species_name])
	_refresh()

func _on_move_pressed(move: MoveData) -> void:
	if busy:
		return
	if overcharge_box.button_pressed and !trainer.can_push():
		_log("Jauge vide : pas de Surge.")
	var boosted := overcharge_box.button_pressed and trainer.can_push()
	if not _play_turn(player, enemy, move, boosted):
		return
	if boosted:
		trainer.register_push()
	overcharge_box.button_pressed = false
	busy = true
	_refresh()
	await get_tree().create_timer(0.6).timeout
	if not enemy.is_down():
		_play_turn(enemy, player, _enemy_choose(), false)
		_refresh()
		await get_tree().create_timer(0.6).timeout
	_end_round()
	busy = false

func _play_turn(attacker: Combatant, target: Combatant, move: MoveData, boosted: bool) -> bool:
	var who := attacker.data.species_name
	var result := attacker.cast(move, boosted)
	match result["result"]:
		"refused":
			_log("%s ne peut pas lancer %s : coût trop élevé." % [who, move.move_name])
			return false
		"failed":
			_log("%s rate %s : réserve instable, %d perdus." % [who, move.move_name, result["cost"]])
		_:
			var damage := attacker.damage_against(target, move, boosted)
			target.hp = maxi(target.hp - damage, 0)
			var suffix := " en Surge" if boosted else ""
			_log("%s lance %s%s : %d dégâts." % [who, move.move_name, suffix, damage])
	return true

func _enemy_choose() -> MoveData:
	var options: Array[MoveData] = []
	for m in enemy.data.moves:
		if m.cost(enemy.pool.maximum) <= mini(enemy.pool.debit(), enemy.pool.current):
			options.append(m)
	if options.is_empty():
		return enemy.data.moves[0]
	return options.pick_random() as MoveData

func _end_round() -> void:
	player.pool.reflux(5.0)
	enemy.pool.reflux(5.0)
	_refresh()
	if enemy.is_down():
		_finish("Victoire !")
	elif player.is_down():
		_finish("Défaite...")

func _finish(message: String) -> void:
	_log(message)
	for child in actions.get_children():
		(child as Button).disabled = true
	overcharge_box.disabled = true

func _refresh() -> void:
	_show(enemy, enemy_name, enemy_hp_bar, enemy_reserve_bar)
	_show(player, player_name, player_hp_bar, player_reserve_bar)
	player_state.text = "Jauge : %d   Marques : %d	Limite : %d" % [trainer.gauge, trainer.marks, trainer.mark_threshold()]

func _show(c: Combatant, label: Label, hp_bar: ProgressBar, reserve_bar: ProgressBar) -> void:
	label.text = "%s  Nv %d   PV %d/%d   Réserve %d/%d" % [
		c.data.species_name, c.level, c.hp, c.hp_max, c.pool.current, c.pool.maximum]
	hp_bar.max_value = c.hp_max
	hp_bar.value = c.hp
	reserve_bar.max_value = c.pool.maximum
	reserve_bar.value = c.pool.current

func _log(text: String) -> void:
	lines.append(text)
	if lines.size() > 5:
		lines.pop_front()
	journal.text = "\n".join(PackedStringArray(lines))
