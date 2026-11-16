LoadLuaFile("Lua/SZE/SZEMain.lua")

edCall = function(player, victor)
	Entity.Say(player, "Aaaaguh", 2)
	DebugPrint(victor .. " defeated " .. player)
	
	return 0
end

onVolEnter = function(player, name)
	DebugPrint(player .. " entered " .. name)

	return 0
end

onVolExit = function(player, name)
	DebugPrint(player .. " exited " .. name)
	
	return 0
end

onEntCreate = function(player)
	DebugPrint(player)
	
	return 0
end

init = function()
	SummonGangChange = SZETeam:new{	name = "SummonGangChange", 
								layout = "SummonGangChange",
								spawnBehavior = "Say(\"<shout>Hello\")" }
								
	SummonDestroy = SZETeam:new{		name = "SummonDestroy", 
								layout = "SummonDestroy",
								spawnBehavior = "Say(\"<shout>Hello Twice\")" }
	
	fountTest = Fountain:new{	minRespawnTime = 0,
						maxRespawnTime = 5 }
	print("A")
	
	fountTest:addTeamSizeCount(2, 4, 10)
	
	SummonDestroy.fountain = fountTest
	print("B")
	
	SummonDestroy:activate()
	print("C")
	SummonGangChange:activate()
	print("D")
	
	Respawning = Timer:new{	name = "Respawning",
						duration = 5,
						reset = true }
	Respawning:start()
	
	iter2 = 50
	
	Callback.OnEntityDefeated("edCall")
	Callback.OnPlayerLeavesVolume("onVolExit")
	Callback.OnPlayerEntersVolume("onVolEnter")
	Callback.OnEntityCreated("onEntCreate")
	Callback.OnEntityFreed("onEntCreate")
	
end

init()

tick = function()
	szeTick()

	
	
	return
end