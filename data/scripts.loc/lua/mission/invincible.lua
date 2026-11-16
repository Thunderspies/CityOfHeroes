-- Header file required to use ALL_PLAYERS
LoadLuaFile("Lua/Util.lua")

-- This is a script that will make an entity invincible on an objective being complete and send a floater text when that happens.
-- It will also make the entity not invincible when another objective is complete, sending another floater text.

-- Required to run Scripted Zone Event
function init()

	Message.DebugPrint("Full screen FX script is activated.")

-- This is specifying what actor is being used for the variable, boss.
	boss = Encounter.ActorFromEncounterActorName(Encounter.MyEncounter(), Var.Get("Actor"))
	Message.DebugPrint(boss)

-- This is setting up what objective activates the invincibility and the floater text to go along with it.
	ActivateOnObjectiveComplete = Var.Get("ACTIVATE_ON_OBJECTIVE_COMPLETE")
	FloaterTextInvincibleOn = Var.Get("FLOATER_TEXT_START")

-- This is setting up what objective deactivates the invincibility and the floater text to go along with it.
	DeactivateOnObjectiveComplete = Var.Get("DEACTIVATE_ON_OBJECTIVE_COMPLETE")
	FloaterTextInvincibleOff = Var.Get("FLOATER_TEXT_STOP")

-- This is setting InvincibleOn to zero to stop looping cases. When I tried doing self.VariableName = true, lua threw up errors.
	InvincibleOn = 0

end

init()

function tick()

-- When the objective is succeeded, it will trigger invincibility and send the floater text.
	if Mission.ObjectiveIsSucceeded(ActivateOnObjectiveComplete) and InvincibleOn == 0 then
		Message.Floater(ALL_PLAYERS, FloaterTextInvincibleOn, 0x00ff00ff, "Attention")
		InvincibleOn = InvincibleOn + 1
		Message.DebugPrint("Turning entity invincible.")
		AI.AddBehavior(boss, "Invincible(1)")
	end

-- If the designer has set the objective to start to be AutoStart, then this will happen automatically without floater text.
	if ActivateOnObjectiveComplete == "AutoStart" and InvincibleOn == 0 then
		InvincibleOn = InvincibleOn + 1
		Message.DebugPrint("Turning entity invincible by default.")
		AI.AddBehavior(boss, "Invincible(1)")
	end

-- When the designer sets the DeactiaveOnObjectiveComplete to be true, the entity will stop being invincible and the script will end.
	if Mission.ObjectiveIsSucceeded(DeactivateOnObjectiveComplete) and InvincibleOn == 1 then
		InvincibleOn = InvincibleOn + 1
		Message.DebugPrint("Turning Off Invincible.")
		Message.Floater(ALL_PLAYERS, FloaterTextInvincibleOff, 0x00ff00ff, "Attention")
		AI.AddBehavior(boss, "Invincible(0)")
		EndScript()
	end

end
