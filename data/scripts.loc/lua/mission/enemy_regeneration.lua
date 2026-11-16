-- Header file required to use ALL_PLAYERS
LoadLuaFile("Lua/Util.lua")

function init()

-- This sets that the entity health the script is looking at is the variable, Actor, defined by the mission designer.
	boss = Encounter.ActorFromEncounterActorName(Encounter.MyEncounter(), Var.Get("Actor"))

-- If no entity is specified, the script ends.
	if boss == nil or boss == "ent:none" then
		EndScript()
		return
	end

-- The designer specifies the health level that the entity recovers from, the floater text if the entity hasn't recovered, floater text if the entity is down, and the objective to stop it all.
	HealthLevel = Var.GetNumber( "HEALTH_RECOVER" )
	ObjectiveToStop = Var.Get("STOP_OBJECTIVE")
	FloaterTextRecover = Var.Get("FLOATER_TEXT_RECOVER")
	FloaterTextVictory = Var.Get("FLOATER_TEXT_VICTORY")
	currentHealth = Entity.GetHealth(boss)

end

init()

function tick()
	currentHealth = Entity.GetHealth(boss)

-- If the objective is complete, the victory floater text is displayed.
	if Mission.ObjectiveIsSucceeded(ObjectiveToStop) then
		Message.Floater(ALL_PLAYERS, FloaterTextVictory, 0x00ff00ff, "Attention")
		EndScript()
-- If the entity's health is lower than or equal to the variable specified by the designer, but the objective hasn't been completed, it's restored to full health with a floater text sent to all players.
	elseif currentHealth <= HealthLevel then
		Message.Floater(ALL_PLAYERS, FloaterTextRecover, 0x00ff00ff, "Attention")
		Entity.SetHealth(boss, 1, false)
	end
end
