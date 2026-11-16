init = function()
	Message.DebugPrint("Super Chatty Boss Init")

-- This specifies what actor in the encounter is being looked at for this script.
	boss = Encounter.ActorFromEncounterActorName(Encounter.MyEncounter(), Var.Get("Actor"))
	Message.DebugPrint(boss)

	if boss == nil or boss == "ent:none" then
		EndScript()
		return
	end

-- This specifies that the healthlevel is going to be at every 10%.
	healthLevel = {1.00, 0.90, 0.80, 0.70, 0.60, 0.50, 0.40, 0.30, 0.20, 0.10, 0}
	dialog = {Var.Get( "BOSS_1STBLOOD_DIALOG" ),
			Var.Get( "BOSS_90HEALTH_DIALOG" ),
			Var.Get( "BOSS_80HEALTH_DIALOG" ),
			Var.Get( "BOSS_70HEALTH_DIALOG" ),
			Var.Get( "BOSS_60HEALTH_DIALOG" ),
			Var.Get( "BOSS_50HEALTH_DIALOG" ),
			Var.Get( "BOSS_40HEALTH_DIALOG" ),
			Var.Get( "BOSS_30HEALTH_DIALOG" ),
			Var.Get( "BOSS_20HEALTH_DIALOG" ),
			Var.Get( "BOSS_10HEALTH_DIALOG" ),
			Var.Get( "BOSS_DEAD_DIALOG" )}
	objective = {nil,
			Var.Get( "BOSS_90HEALTH_Objective" ),
			Var.Get( "BOSS_80HEALTH_Objective" ),
			Var.Get( "BOSS_70HEALTH_Objective" ),
			Var.Get( "BOSS_60HEALTH_Objective" ),
			Var.Get( "BOSS_50HEALTH_Objective" ),
			Var.Get( "BOSS_40HEALTH_Objective" ),
			Var.Get( "BOSS_30HEALTH_Objective" ),
			Var.Get( "BOSS_20HEALTH_Objective" ),
			Var.Get( "BOSS_10HEALTH_Objective" ),
			Var.Get( "BOSS_DEAD_Objective" )}
	force = {nil,
			Var.IsTrue( "BOSS_90HEALTH_FORCE_HEALTH" ),
			Var.IsTrue( "BOSS_80HEALTH_FORCE_HEALTH" ),
			Var.IsTrue( "BOSS_70HEALTH_FORCE_HEALTH" ),
			Var.IsTrue( "BOSS_60HEALTH_FORCE_HEALTH" ),
			Var.IsTrue( "BOSS_50HEALTH_FORCE_HEALTH" ),
			Var.IsTrue( "BOSS_40HEALTH_FORCE_HEALTH" ),
			Var.IsTrue( "BOSS_30HEALTH_FORCE_HEALTH" ),
			Var.IsTrue( "BOSS_20HEALTH_FORCE_HEALTH" ),
			Var.IsTrue( "BOSS_10HEALTH_FORCE_HEALTH" ),
			nil}
	complete = {false, false, false, false, false, false, false, false, false, false, false}

	Team.SayOnKillHero(boss, Var.Get("BOSS_KILLEDHERO_DIALOG"))

	currentHealth = Entity.GetHealth(boss)
end

init()

tick = function()
	lastHealth = currentHealth
	currentHealth = Entity.GetHealth(boss)

	-- i= 1, 5 means that you're going to go through 1, 2, 3, 4, 5.  Every tick it'll do all 5.
	for i = 1, 11 do
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
