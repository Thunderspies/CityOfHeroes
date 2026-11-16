-- Header file required to use ALL_PLAYERS
LoadLuaFile("Lua/Util.lua")
LoadLuaFile("Lua/SZE/SZETimer.lua")

function init()
	Message.DebugPrint("Ally Script Initialize")

-- This sets that the entity health the script is looking at is the variable, Actor, defined by the mission designer.
	boss = Encounter.ActorFromEncounterActorName(Encounter.MyEncounter(), Var.Get("Actor"))
	Message.DebugPrint(boss)

-- If no entity is specified, the script ends.
	if boss == nil or boss == "ent:none" then
		Message.DebugPrint("No Boss")
		EndScript()
		return
	end

	Message.PopHelp(ALL_PLAYERS, "POP_HELP_ALLIES")

-- This is the dialog that a designer specifies what the actor says when they're killed and when they get back up.
	DeadDialog = Var.Get( "ALLY_DEAD_DIALOG" )
	RecoverDialog = Var.Get("ALLY_RECOVER_DIALOG")

-- The designer specifies in the excel spreadsheet how long the timer, AllyKOTimer, is.
	TimetoRecover = Var.GetNumber("TIME_TO_RECOVER")

-- This defines the variables currenthealth has the health from whatever entity is been defined as boss.
	currentHealth = Entity.GetHealth(boss)

-- This creates a timer called AllyKOTimer. It's duration is the TimeToRecover variable, which a designer specifies in the mission.
	AllyKOTimer = Timer:new{ duration = TimetoRecover}

end

-- This is calling the function above.
init()

function tick()

-- This refreshes the variable currenthealth every tick to equal the entity's health who has been set as boss.
	currentHealth = Entity.GetHealth(boss)

-- If the entity's health is zero and the variable self.TimerStarted isn't set or is false, then it has the ally say the dialog specified in DeadDialog by the deisnger.
-- It then sets self.AllyDead to true. If this didn't happen, then everything in this function would happen every tick.
-- Finally, the timer that's specified above is set up to start. This starts the countdown to when the ally is revived.
	if currentHealth == 0 and not AllyKOTimer:running() then
		Message.DebugPrint("Ally has died.")
		Entity.Say(boss, DeadDialog	, 2)
		AllyKOTimer:start()
	end

-- When the timer is complete, then it sets the entity's health back to full. It then has the entity say the dialog specified in RecoverDialog by the designer.
-- The entity is then told to move to a player in the map. The timer is initialized so that if it starts again, it will start over. The timer is then told to stop running.
-- The variable self.TimerStarted is set to false so that the function above can be performed again if the entity's health is set to zero.
	if AllyKOTimer:complete() then
		Message.DebugPrint("Ally is alive.")
		Entity.SetHealth(boss, 1, false)
		Entity.Say(boss, RecoverDialog	, 2)
		AI.AddBehavior(boss, "MoveToPos(random:player)")
		AllyKOTimer:stop()
	end

end

