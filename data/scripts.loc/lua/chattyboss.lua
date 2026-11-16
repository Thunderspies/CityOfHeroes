init = function()
	Message.DebugPrint("Chatty Boss Init")

	boss = Encounter.ActorFromEncounterActorName(Encounter.MyEncounter(), Var.Get("Actor"))

	if boss == nil or boss == "ent:none" then
		EndScript()
	end

	healthLevel = {1.00, 0.75, 0.50, 0.25, 0}
	dialog = {Var.Get( "BOSS_1STBLOOD_DIALOG" ), Var.Get( "BOSS_75HEALTH_DIALOG" ), Var.Get( "BOSS_50HEALTH_DIALOG" ), Var.Get( "BOSS_25HEALTH_DIALOG" ), Var.Get( "BOSS_DEAD_DIALOG" )}
	objective = {nil, Var.Get( "BOSS_75HEALTH_Objective" ), Var.Get( "BOSS_50HEALTH_Objective" ), Var.Get( "BOSS_25HEALTH_Objective" ), Var.Get( "BOSS_DEAD_Objective" )}
	force = {nil, Var.IsTrue( "BOSS_75HEALTH_FORCE_HEALTH" ), Var.IsTrue( "BOSS_50HEALTH_FORCE_HEALTH" ), Var.IsTrue( "BOSS_25HEALTH_FORCE_HEALTH" ), nil}
	complete = {false, false, false, false, false}

	Team.SayOnKillHero(boss, Var.Get("BOSS_KILLEDHERO_DIALOG"))

	currentHealth = Entity.GetHealth(boss)
end

init()

tick = function()
	lastHealth = currentHealth
	currentHealth = Entity.GetHealth(boss)

	-- i= 1, 5 means that you're going to go through 1, 2, 3, 4, 5.  Every tick it'll do all 5.
	for i = 1, 5 do
		if (currentHealth < lastHealth or currentHealth <= 0) and currentHealth <= healthLevel[i] and not complete[i] then
			complete[i] = true
			if dialog[i] then
				Entity.Say(boss, dialog[i], 2)
			end
			if objective[i] then
				Mission.SetObjectiveComplete(1, objective[i])
			end
			if force[i] then
				Entity.SetHealth(boss, healthLevel[i])
			end
		end
	end
end
